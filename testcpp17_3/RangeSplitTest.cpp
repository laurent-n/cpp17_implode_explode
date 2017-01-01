#include <iostream>
#include <regex>
#include <experimental/string_view>
#include <range/v3/all.hpp>

using namespace std;
//#define std::basic_string_view  std::experimental::basic_string_view
//using std::basic_string_view = std::experimental::basic_string_view;
namespace std {
template<class CharT, class Traits>
using basic_string_view = std::experimental::basic_string_view<CharT, Traits>;
using string_view = std::experimental::string_view;
//using basic_string_view = std::experimental::basic_string_view;
};


template<typename CharT, typename Traits = std::char_traits<CharT>>
class split_range_regex
{
public:
    using value_type = std::basic_string_view<CharT, Traits>;
    using traits_type = Traits;
    using size_type = typename value_type::size_type;
    using difference_type = typename value_type::difference_type;
    using reference = value_type;
    using const_reference = reference;
    using pointer = const value_type *;
    using const_pointer = pointer;
    using regex_type = std::basic_regex<CharT>;

    class iterator : public std::forward_iterator_tag
    {
    public:
        iterator(value_type sv, const regex_type &re)
            :it_(sv.begin(), sv.end(), re, -1)
        {}

        iterator() = default;

        reference operator*() const {get_curr(); return curr_;}
        pointer operator->() const {get_curr(); return &curr_;}

        iterator &operator++() {++it_; return *this;}
        iterator operator++(int)
        {
            auto temp = *this;
            ++it_;
            return temp;
        }

        bool operator==(const iterator &rhs) { return it_ == rhs.it_; }
        bool operator!=(const iterator &rhs) { return it_ != rhs.it_; }

    private:
        using token_iterator = std::regex_token_iterator<typename value_type::iterator, CharT>;
        token_iterator it_;
        mutable value_type curr_; //Here just because -> requires a value to get a pointer to.

        void get_curr() const
        {
            auto match = *it_;
            curr_ = value_type(&*match.first, match.length());
        }
    };

    using const_iterator = iterator;

    split_range_regex(value_type sv, const regex_type &re)
        : first_(sv, re)
    {}

    split_range_regex() = delete;
    ~split_range_regex() { cout<<"destructor split_range_regex"<<endl; };


    iterator begin() const noexcept                        {return iterator(first_);}
    const_iterator cbegin() const noexcept                {return const_iterator(first_);}

    iterator end() const noexcept                        {return iterator(past_last_);}
    const_iterator cend() const noexcept                {return const_iterator(past_last_);}

    bool empty() const noexcept
    {
        return past_last_ == first_;
    }

private:
    iterator first_;
    iterator past_last_;
};

template<typename CharT, typename Traits = std::char_traits<CharT>>
split_range_regex<CharT, Traits> split_string(std::basic_string_view<CharT, Traits> sv,
    const typename split_range_regex<CharT, Traits>::regex_type &re)
    {
        return split_range_regex<CharT, Traits>(sv, re);
    }
//*****************************************************************************************

template<typename CharT, typename Traits = std::char_traits<CharT>>
class split_range_string
{
public:
    using value_type = std::basic_string_view<CharT, Traits>;
    using traits_type = Traits;
    using size_type = typename value_type::size_type;
    using difference_type = typename value_type::difference_type;
    using reference = value_type;
    using const_reference = reference;
    using pointer = const value_type *;
    using const_pointer = pointer;

    using regex_type = std::basic_regex<CharT>;

    class iterator : public std::forward_iterator_tag
    {
    public:
        iterator(value_type sv, value_type sep)
            //: it_(sv.begin(), sv.end(), re, -1)
            :sep_(sep),str_(sv),cur_it(sv.begin()),next_it(sv.begin())
        {
            calc_next_it();
        }
        iterator(typename value_type::const_iterator _init_it)
        :cur_it(_init_it),next_it(_init_it) {} // Constructor for end()

        iterator() = default;

        reference operator*() const {get_curr(); return curr_;}
        pointer operator->() const {get_curr(); return &curr_;}

        iterator &operator++() {
            //++it_;
            cur_it=next_it;
            if(cur_it!=str_.end()) {
                ++cur_it;
                calc_next_it();
            }
            return *this;}
        iterator operator++(int)
        {
            auto temp = *this;
//            ++it_;
            cur_it=next_it;
            if(cur_it!=str_.end()) {
                ++cur_it;
                calc_next_it();
            }

            return temp;
        }

        bool operator==(const iterator &rhs)
        {
            return cur_it == rhs.cur_it;
        }
        bool operator!=(const iterator &rhs)
        {
            return cur_it != rhs.cur_it;
        }

    private:
        //using token_iterator = std::regex_token_iterator<typename value_type::iterator, CharT>;
        value_type sep_;
        value_type str_;
        typename value_type::const_iterator cur_it,next_it;

        void calc_next_it()
        {
            //cout<<"calc_next_it : "<<((unsigned long long)(void*)this)<<" "<<src<<endl;
            for(next_it=cur_it;next_it!=str_.cend();++next_it) {
                //cout<<"*next_it==separator "<<*next_it<<"=='"<<separator<<"'"<<endl;
                if(*next_it==sep_[0])
                    break;
                }

        }

        /*void calc_next_it()
        {
            for(next_pos=cur_pos;next_pos<str_.size();++next_pos) {
                //cout<<"*next_it==separator "<<*next_it<<"=='"<<separator<<"'"<<endl;
                if(str_[next_pos]==sep_[0])
                    break;
                }
        }*/

        //token_iterator it_;
        mutable value_type curr_; //Here just because -> requires a value to get a pointer to.

        void get_curr() const
        {
            curr_ = value_type(&*cur_it, next_it-cur_it);
        }
    };

    using const_iterator = iterator;

    split_range_string(value_type sv, value_type  sep)
        : first_(sv, sep),past_last_(sv.end())
    {}

    split_range_string() = delete;
    ~split_range_string() { cout<<"destructor split_range_string"<<endl; };


    iterator begin() const noexcept                        {return iterator(first_);}
    const_iterator cbegin() const noexcept                {return const_iterator(first_);}

    iterator end() const noexcept                        {return iterator(past_last_);}
    const_iterator cend() const noexcept                {return const_iterator(past_last_);}

    bool empty() const noexcept
    {
        return past_last_ == first_;
    }

private:
    iterator first_;
    iterator past_last_;
};

template<typename CharT, typename Traits = std::char_traits<CharT>>
split_range_string<CharT, Traits> split_string(std::basic_string_view<CharT, Traits> sv,
    std::basic_string_view<CharT, Traits> sep)
    {
        return split_range_string<CharT, Traits>(sv, sep);
    }

//*****************************************************************************************
string string_toLower(string Str)
{
    std::transform(Str.begin(), Str.end(), Str.begin(), ::tolower);    
    return Str;
}

int main()
{

    std::vector<int> v{1,5,9,2,4,3,8,7,6};
    ranges::sort( v );
    string str1="C++ Is Fun !!!";
    string_view vstr1=str1;
    auto r1=regex("\\s");
//    for(auto x : split_string(vstr1,regex("\\s")))  // Fail du to temporary regex
    for(auto x : split_string(vstr1,r1))
        cout << x << endl;

    for(auto x : split_string(string_view(string_toLower("C++ Is Fun!!!")),r1))
        cout << x << endl;
    
    for(auto x : split_string(string_view(string_toLower("C++ Is Fun!!!")),r1))
        cout << x << endl;
    cout << "----------- Test of string sep -------------" << endl;
    string_view sep(" ");
    for(auto x : split_string(vstr1,sep))
        cout << x << endl;
    for(auto x : split_string(string_view(string_toLower("C++ Is Very Fun!!!")),sep))
        cout << x << endl;
    cout << "Hello world!" << endl;
    return 0;
}
