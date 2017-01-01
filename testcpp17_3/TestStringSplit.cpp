// N3593 prototype Test
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

#include "StringSplit.h"

#include <regex>
#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
string string_toLower(string Str)
{
    std::transform(Str.begin(), Str.end(), Str.begin(), ::tolower);
    return Str;
}



using Vec_sv = std::vector<std__string_view>;

// template <typename T> struct Incomplete;

template <typename In, typename Out>
using SameSST
    = std::is_same< Out, typename str::SplitStringType<In>::type >;

static_assert( SameSST< char*,                   char const*      const >::value, "");
static_assert( SameSST< char const*,             char const*      const >::value, "");
static_assert( SameSST< char const[3],           char const*      const >::value, "");
static_assert( SameSST< char const(&)[3],        char const*      const >::value, "");
static_assert( SameSST< char const(&&)[3],       char const*      const >::value, "");
static_assert( SameSST< char[3],                 char const*      const >::value, "");
static_assert( SameSST< char(&)[3],              char const*      const >::value, "");
static_assert( SameSST< char(&&)[3],             char const*      const >::value, "");
static_assert( SameSST< std__string_view,        std__string_view const >::value, "");
static_assert( SameSST< std__string_view const,  std__string_view const >::value, "");
static_assert( SameSST< std__string_view const&, std__string_view const&>::value, "");
static_assert( SameSST< std__string_view&&,      std__string_view const >::value, "");
static_assert( SameSST< std::string const&,      std::string      const&>::value, "");
static_assert( SameSST< std::string&&,           std::string      const >::value, "");

#include <iostream>

template <typename T, typename U>
static void expect_equal(T const& lhs, U const& rhs, char const* strlhs, char const* strrhs, int line)
{
    if (lhs == rhs)
        return;
    std::cout << "line(" << line << ") assertion failed: " << strlhs << " == " << strrhs << "\n";
    std::cout << "  actual values: " << lhs << " and " << rhs << "\n";
}

template <typename T, typename U>
static void expect_not_equal(T const& lhs, U const& rhs, char const* strlhs, char const* strrhs, int line)
{
    if (lhs != rhs)
        return;
    std::cout << "line(" << line << ") assertion failed: " << strlhs << " != " << strrhs << "\n";
    std::cout << "  actual values: " << lhs << " and " << rhs << "\n";
}

#define EXPECT_EQUAL(X, Y) expect_equal(X, Y, #X, #Y, __LINE__)
#define EXPECT_NOT_EQUAL(X, Y) expect_not_equal(X, Y, #X, #Y, __LINE__)


static void test_EmptyStrings()
{
    {
        auto vec = Vec_sv(str::Split(std__string_view(), ","));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "");
    }
    {
        auto vec = Vec_sv(str::Split("", ","));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "");
    }
    {
        auto vec = Vec_sv(str::Split(std__string_view(), str::AnyOf(",")));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "");
    }
    {
        auto vec = Vec_sv(str::Split("", str::AnyOf(",")));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "");
    }
}

static void test_EmptyDelimiters()
{
    {
        auto vec = Vec_sv(str::Split(std__string_view(), ""));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "");
    }
    {
        auto vec = Vec_sv(str::Split(std__string_view(), str::AnyOf("")));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "");
    }
    {
        auto vec = Vec_sv(str::Split("", ""));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "");
    }
    {
        auto vec = Vec_sv(str::Split("", str::AnyOf("")));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "");
    }
    {
        auto vec = Vec_sv(str::Split("x", ""));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "x");
    }
    {
        auto vec = Vec_sv(str::Split("x", str::AnyOf("")));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "x");
    }
    {
        auto vec = Vec_sv(str::Split("abc", ""));
#if STRING_SPLIT_STD
        EXPECT_EQUAL(vec.size(), 3U);
        EXPECT_EQUAL(vec[0], "a");
        EXPECT_EQUAL(vec[1], "b");
        EXPECT_EQUAL(vec[2], "c");
#else
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "abc");
#endif
    }
    {
        auto vec = Vec_sv(str::Split("abc", str::AnyOf("")));
#if STRING_SPLIT_STD
        EXPECT_EQUAL(vec.size(), 3U);
        EXPECT_EQUAL(vec[0], "a");
        EXPECT_EQUAL(vec[1], "b");
        EXPECT_EQUAL(vec[2], "c");
#else
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "abc");
#endif
    }
}

static void test_LeadingDelimiters()
{
    {
        auto vec = Vec_sv(str::Split(",", ','));
        EXPECT_EQUAL(2u, vec.size());
        EXPECT_EQUAL("", vec[0]);
        EXPECT_EQUAL("", vec[1]);
    }
    {
        auto vec = Vec_sv(str::Split(", ", ','));
        EXPECT_EQUAL(2u, vec.size());
        EXPECT_EQUAL("", vec[0]);
        EXPECT_EQUAL(" ", vec[1]);
    }
}

static void test_SimpleLiteralTests()
{
    {
        auto vec = Vec_sv(str::Split("a", ","));
        EXPECT_EQUAL(1u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
    }
    {
        auto vec = Vec_sv(str::Split("a,", ","));
        EXPECT_EQUAL(2u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
        EXPECT_EQUAL("", vec[1]);
    }
    {
        auto vec = Vec_sv(str::Split("a,b", ","));
        EXPECT_EQUAL(2u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
        EXPECT_EQUAL("b", vec[1]);
    }
    {
        auto vec = Vec_sv(str::Split("-a-b-c----d", "-"));
        EXPECT_EQUAL(8u, vec.size());
        EXPECT_EQUAL("", vec[0]);
        EXPECT_EQUAL("a", vec[1]);
        EXPECT_EQUAL("b", vec[2]);
        EXPECT_EQUAL("c", vec[3]);
        EXPECT_EQUAL("", vec[4]);
        EXPECT_EQUAL("", vec[5]);
        EXPECT_EQUAL("", vec[6]);
        EXPECT_EQUAL("d", vec[7]);
    }
    {
        auto vec = Vec_sv(str::Split("-a-b-c----d", "--"));

        EXPECT_EQUAL(3u, vec.size());
        EXPECT_EQUAL("-a-b-c", vec[0]);
        EXPECT_EQUAL("", vec[1]);
        EXPECT_EQUAL("d", vec[2]);
    }
}

static void test_AnyOfDelimiter()
{
    {
        auto vec = Vec_sv(str::Split("a.b-c,. d, e .f-", str::AnyOf(".,-")));
        EXPECT_EQUAL(8u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
        EXPECT_EQUAL("b", vec[1]);
        EXPECT_EQUAL("c", vec[2]);
        EXPECT_EQUAL("", vec[3]);
        EXPECT_EQUAL(" d", vec[4]);
        EXPECT_EQUAL(" e ", vec[5]);
        EXPECT_EQUAL("f", vec[6]);
        EXPECT_EQUAL("", vec[7]);
    }
    { // Doesn't works with temporary object
        auto vec = Vec_sv(str::Split(string_toLower( "A.b-C,. d, e .f-"), str::AnyOf(".,-")));
        EXPECT_EQUAL(8u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
        EXPECT_EQUAL("b", vec[1]);
        EXPECT_EQUAL("c", vec[2]);
        EXPECT_EQUAL("", vec[3]);
        EXPECT_EQUAL(" d", vec[4]);
        EXPECT_EQUAL(" e ", vec[5]);
        EXPECT_EQUAL("f", vec[6]);
        EXPECT_EQUAL("", vec[7]);
    }

}

static void test_Iterators()
{
    {
        auto R = str::Split("a,b,c,d", ",");
        Vec_sv vec;
        for (auto I = R.begin(), E = R.end(); I != E; ++I)
        {
            vec.push_back(*I);
        }
        EXPECT_EQUAL(4u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
        EXPECT_EQUAL("b", vec[1]);
        EXPECT_EQUAL("c", vec[2]);
        EXPECT_EQUAL("d", vec[3]);
    }
    {
        auto R = str::Split("a,b,c,d", ",");
        EXPECT_EQUAL("a", *R.begin());
        EXPECT_EQUAL("a", *R.begin());
        EXPECT_EQUAL("a", *R.begin());
        auto I = R.begin();
        ++I;
        EXPECT_EQUAL("b", *R.begin());
        ++I;
        EXPECT_EQUAL("c", *R.begin());
    }
    {
        auto R = str::Split("a,b,c,d", ",");
        auto I = R.begin();
        auto J = R.begin();
        ++I;
        assert(I == I);
        assert(*I == *I);
        assert(J == I);
        assert(*J == *I);
        assert(J == J);
        assert(*J == *J);
        I = ++J;
        ++J;
        assert(J == I);
        assert(*J == *I);
    }
}

static void test_Tokenizer()
{
    auto splitter = [](std__string_view str) -> str::TokenizerResult {
        return { str.find('a'), 1 };
    };

    {
        auto vec = Vec_sv(str::Split(" a   a \t  a ", splitter));
        EXPECT_EQUAL(vec.size(), 3U);
        EXPECT_EQUAL(vec[0], "a");
        EXPECT_EQUAL(vec[1], "a");
        EXPECT_EQUAL(vec[2], "a");
    }
    {
        auto vec = Vec_sv(str::Split(" b   b \t  b ", splitter));
        EXPECT_EQUAL(vec.size(), 0U);
    }
};

namespace str {

struct Regex
{
    template <typename T>
    using ItsMe = std::is_same<Regex, std::decay_t<T>>;

    const std::regex regex;
    std::cmatch match;
    // XXX: match_flags?!

    template <
        typename Arg, typename ...Args,
        typename = std::enable_if_t<
            !std::is_same< std::decay_t<Arg>, Regex >::value
        >
    >
    explicit Regex(Arg&& arg, Args&&... args)
        : regex(std::forward<Arg>(arg), std::forward<Args>(args)...)
    {
    }

    DelimiterResult operator ()(std__string_view str) /*const*/
    {
        char const* I = str.data();
        char const* E = str.data() + str.size();

        if (!std::regex_search(I, E, match, regex))
            return { std__string_view::npos, 0 };

        return { static_cast<size_t>(match.position()),
                 static_cast<size_t>(match.length()) };
    }
};

struct RegexToken
{
    template <typename T>
    using ItsMe = std::is_same<RegexToken, std::decay_t<T>>;

    const std::regex regex;
    std::cmatch match;
    // XXX: match_flags?!

    template <
        typename Arg, typename ...Args,
        typename = std::enable_if_t<
            !std::is_same< std::decay_t<Arg>, RegexToken >::value
        >
    >
    explicit RegexToken(Arg&& arg, Args&&... args)
        : regex(std::forward<Arg>(arg), std::forward<Args>(args)...)
    {
    }

    TokenizerResult operator ()(std__string_view str) /*const*/
    {
        char const* I = str.data();
        char const* E = str.data() + str.size();

        if (!std::regex_search(I, E, match, regex))
            return { std__string_view::npos, 0 };

        return { static_cast<size_t>(match[0].first - I),
                 static_cast<size_t>(match[0].length()) };
    }
};

} // namespace str

template <typename T> struct Incomplete;

static void test_Regex()
{
    {
        auto vec = Vec_sv(str::Split("a b \t   c \td\n  \t e", str::Regex("\\s+")));
        EXPECT_EQUAL(vec.size(), 5u);
        EXPECT_EQUAL(vec[0], "a");
        EXPECT_EQUAL(vec[1], "b");
        EXPECT_EQUAL(vec[2], "c");
        EXPECT_EQUAL(vec[3], "d");
        EXPECT_EQUAL(vec[4], "e");
    }
    {
        auto vec = Vec_sv(str::Split("1   2  \t 3 \n\t4    ", str::RegexToken("\\d+")));
        EXPECT_EQUAL(vec.size(), 4u);
        EXPECT_EQUAL(vec[0], "1");
        EXPECT_EQUAL(vec[1], "2");
        EXPECT_EQUAL(vec[2], "3");
        EXPECT_EQUAL(vec[3], "4");
    }
    {
        auto vec = Vec_sv(str::Split("a   b  \t c \n\td    ", str::RegexToken("\\d+")));
        EXPECT_EQUAL(vec.size(), 0u);
    }
}

static void test_ForEach()
{
    {
        int count = 0;
        str::ForEach("", '-', [&](std__string_view tok) {
            ++count;
            return true;
        });
        EXPECT_EQUAL(count, 1);
    }
    {
        int count = 0;
        str::ForEach("a-a-a-", '-', [&](std__string_view tok) {
            ++count;
            return true;
        });
        EXPECT_EQUAL(count, 4);
    }
    {
        struct CharToken {
            const char ch;

            CharToken(char c) : ch(c) {}
            CharToken() = delete;
            CharToken(CharToken const&) = delete;
            CharToken& operator =(CharToken const&) = delete;

            str::TokenizerResult operator ()(std__string_view str) const {
                return { str.find('a'), 1 };
            }
        };

        int count = 0;
        str::ForEach("a-a-a-", CharToken('a'), [&](std__string_view tok) {
            ++count;
            return true;
        });
        EXPECT_EQUAL(count, 3);
    }
}


int main()
{
    string str1="C++ Is Fun!!!";
    cout<<string_toLower(str1)<<endl;
    cout<<str1<<endl;
    test_EmptyStrings();
    test_EmptyDelimiters();
    test_LeadingDelimiters();
    test_SimpleLiteralTests();
    test_AnyOfDelimiter();
    test_Iterators();
    test_Tokenizer();
    test_Regex();
    test_ForEach();
}
