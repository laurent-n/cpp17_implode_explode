// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non - commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>

#pragma once

#include <experimental/string_view>
using std__string_view = std::experimental::string_view;

#include <cassert>
#include <cstddef>
#include <type_traits>

#define STRING_SPLIT_STD 1 // https://isocpp.org/files/papers/n3593.html

namespace str {

// Return type for delimiters.
//
// +-----+-----+------------+
// ^ tok ^     ^    rest    ^
//       f     f+c
//
// Either FIRST or COUNT must be non-zero.
struct DelimiterResult {
    size_t first;
    size_t count;
};

// Return type for tokenizers.
//
// +-----+-----+------------+
// ^     ^ tok ^    rest    ^
//       f     f+c
//
// Either FIRST or COUNT must be non-zero.
struct TokenizerResult {
    size_t first;
    size_t count;
};

//
// N3593:
//
// A string delimiter. This is the default delimiter used if a string is given
// as the delimiter argument to std::split().
//
// The delimiter representing the empty string (std::literal_delimiter("")) will
// be defined to return each individual character in the input string. This
// matches the behavior of splitting on the empty string "" in perl.
//
class StringLiteral
{
    const std::string delim_; // Need a copy... Maybe reuse SplitStringType??

public:
    explicit StringLiteral(std__string_view delim) : delim_(delim) {}

    DelimiterResult operator()(std__string_view str) const
    {
        if (delim_.empty())
        {
#if STRING_SPLIT_STD
            if (str.size() <= 1)
                return { std__string_view::npos, 0 };
            return { 1, 0 };
#else
            // Return the whole string as a token.
            // Makes StringLiteral("") behave exactly as AnyOf("").
            return { std__string_view::npos, 0 };
#endif
        }

        return { str.find(delim_), delim_.size() };
    }
};

// A character delimiter.
// This is faster than StringLiteral if you want to break the string at a
// specific character.
class CharLiteral
{
    const char ch_;

public:
    explicit CharLiteral(char ch) : ch_(ch) {}

    DelimiterResult operator()(std__string_view str) const {
        return { str.find(ch_), 1 };
    }
};

//
// N3593:
//
// Each character in the given string is a delimiter. A std::any_of_delimiter
// with string of length 1 behaves the same as a std::literal_delimiter with the
// same string of length 1.
//
class AnyOf
{
    const std::string chars_; // Need a copy... Maybe reuse SplitStringType??

public:
    explicit AnyOf(std__string_view chars) : chars_(chars) {}

    DelimiterResult operator()(std__string_view str) const
    {
#if STRING_SPLIT_STD
        if (chars_.empty())
        {
            if (str.size() <= 1)
                return { std__string_view::npos, 0 };
            return { 1, 0 };
        }
#endif
        return { str.find_first_of(chars_), 1 };
    }
};

namespace impl {

struct DoSplitResult {
    std__string_view tok; // The current token.
    std__string_view str; // The rest of the string.
};

inline bool DoSplit(DoSplitResult& res, std__string_view str, DelimiterResult del)
{
    //
    // +-----+-----+------------+
    // ^ tok ^     ^    rest    ^
    //       f     f+c
    //

    assert(del.first > 0 || del.count > 0);

    if (del.first == std__string_view::npos) {
        res.tok = str;
        res.str = {};
        return true;
    }

    assert(del.first + del.count >= del.first);
    assert(del.first + del.count <= str.size());

    res.tok = str.substr(0, del.first);
    res.str = str.substr(del.first + del.count);
    return true;
}

inline bool DoSplit(DoSplitResult& res, std__string_view str, TokenizerResult tok)
{
    //
    // +-----+-----+------------+
    // ^     ^ tok ^    rest    ^
    //       f     f+c
    //

    assert(tok.first > 0 || tok.count > 0);

    if (tok.first == std__string_view::npos) {
        res.tok = {};
        res.str = {};
        return false;
    }

    assert(tok.first + tok.count >= tok.first);
    assert(tok.first + tok.count <= str.size());

    res.tok = str.substr(tok.first, tok.count);
    res.str = str.substr(tok.first + tok.count);
    return true;
}

template <typename Range>
class SplitIterator
{
    //
    // XXX:
    //
    // The iterators should probably do all the hard work and SplitRange should
    // only be thin wrapper around two of them...
    //
    //  - Probably copies strings...
    //  - Non-default constructible delimiters/tokenizers?!?!
    //

public:
    using iterator_category = std::input_iterator_tag;
    using value_type        = std__string_view const;
    using reference         = std__string_view const&;
    using pointer           = std__string_view const*;
    using difference_type   = ptrdiff_t;

private:
    Range* range_;

public:
    explicit SplitIterator(Range* range)
        : range_(range)
    {
    }

    reference operator*()
    {
        assert(range_ != nullptr && "dereferencing past-the-end iterator");
        return range_->curr_.tok;
    }

    pointer operator->()
    {
        assert(range_ != nullptr && "dereferencing past-the-end iterator");
        return &range_->curr_.tok;
    }

    SplitIterator& operator++()
    {
        assert(range_ != nullptr && "dereferencing past-the-end iterator");
        if (!range_->Advance())
            range_ = nullptr;
        return *this;
    }

    bool operator==(SplitIterator rhs) const { return range_ == rhs.range_; }
    bool operator!=(SplitIterator rhs) const { return !(*this == rhs); }
};

template <typename String, typename Splitter>
class SplitRange
{
    static_assert(std::is_constructible<std__string_view, String>::value,
        "String type must be explicitly convertible to std__string_view");

    friend class SplitIterator<SplitRange>;

    // The string to split
    String str_;
    // The delimiter
    Splitter split_;
    // The current token and the rest of the string
    DoSplitResult curr_;
    // Is the range empty?
    bool is_empty_;

    bool Advance0()
    {
        is_empty_ = !DoSplit(curr_, curr_.str, split_(curr_.str));
        return !is_empty_;
    }

    bool Advance()
    {
        if (curr_.str.data() == nullptr)
            return false;
        return Advance0();
    }

public:
    using iterator       = SplitIterator<SplitRange>;
    using const_iterator = SplitIterator<SplitRange>;

    template <typename StrT, typename SplitT>
    explicit SplitRange(StrT&& str, SplitT&& split)
        : str_(std::forward<StrT>(str))
        , split_(std::forward<SplitT>(split))
    {
        Reset();
        Advance0();
    }

    void Reset()
    {
        curr_.tok = {};
        curr_.str = std__string_view(str_);
    }

//    template <typename Predicate>
//    bool ForEach(Predicate pred)
//    {
//        for (auto I = begin(), E = end(); I != E; ++I)
//        {
//            if (!pred(*I))
//                return false;
//        }
//
//        return true;
//    }
//
//    template <typename Predicate>
//    bool operator ()(Predicate pred)
//    {
//        return ForEach(pred);
//    }

    iterator begin() { return is_empty_ ? iterator(nullptr) : iterator(this); }
    iterator end() { return iterator(nullptr); }

    template <
        typename R,
        typename = std::enable_if_t<
            std::is_constructible<R, iterator, iterator>::value
        >
    >
    explicit operator R() { return R(begin(), end()); }
};

template <typename StrT, typename SplitT, typename PredicateT>
inline bool ForEach(StrT&& str, SplitT&& split, PredicateT&& pred)
{
    impl::DoSplitResult curr;

    curr.tok = {};
    curr.str = std__string_view(str);

    for (;;)
    {
        if (!impl::DoSplit(curr, curr.str, split(curr.str)))
            return true;
        if (!pred(curr.tok))
            return false;
        if (curr.str.data() == nullptr)
            return true;
    }
}

} // namespace impl

//
// N3593:
//
// Rvalue support
//
// As described so far, std::split() may not work correctly if splitting a
// std::string_view that refers to a temporary string. In particular, the
// following will not work:
//
//      for (std::string_view s : std::split(GetTemporaryString(), "-")) {
//          s now refers to a temporary string that is no longer valid.
//      }
//
// To address this, std::split() will move ownership of rvalues into the Range
// object that is returned from std::split().
//
template <typename T>
struct SplitStringType
{
    using V = std::conditional_t<
        //
        // If T decays into a char [const]*, return a pointer.
        //
        // XXX:
        //
        // This is wrong for character arrays which are not null-terminated...
        // std::string_view calls strlen on them...
        //
        std::is_convertible< T, char const* >::value,
            char const*,
            std::conditional_t<
                //
                // If T is an rvalue-reference, remove the reference.
                // Otherwise just return T.
                //
                std::is_rvalue_reference<T>::value,
                    std::remove_reference_t<T>,
                    T
            >
    >;

    // Add const. SplitRange will never modify the string.
    using type = std::add_const_t<V>;
};

//
// N3593:
//
// The default delimiter when not explicitly specified is
// std::literal_delimiter.
//
template <typename T, typename U = std::decay_t<T>>
struct SplitterType
{
    using type = std::conditional_t<
        std::is_constructible< CharLiteral, T >::value,
        CharLiteral,
        std::conditional_t<
            std::is_constructible< StringLiteral, T >::value,
            StringLiteral,
            U
            >
        >;
};

//
// N3593:
//
// The std::split() algorithm takes a std::string_view and a Delimiter as
// arguments, and it returns a range of std::string_view objects as output. The
// std::string_view objects in the returned range will refer to substrings of
// the input text. The Delimiter object defines the boundaries between the
// returned substrings.
//
template <typename StrT, typename SplitT>
inline auto Split(StrT&& str, SplitT&& split)
{
    using R = impl::SplitRange<
        typename SplitStringType< StrT >::type,
        typename SplitterType< SplitT >::type
        >;

    return R(std::forward<StrT>(str), std::forward<SplitT>(split));
}

template <typename StrT, typename SplitT, typename PredicateT>
inline bool ForEach(StrT&& str, SplitT&& split, PredicateT&& pred)
{
    using S = typename SplitterType< SplitT, SplitT&& >::type;

    return impl::ForEach<StrT, S>(
        std::forward<StrT>(str),
            static_cast<S>(split), // forward or construct
                std::forward<PredicateT>(pred));
}

} // namespace str
