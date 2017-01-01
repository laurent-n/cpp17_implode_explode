#include <iostream>
#include <vector>
#include <iterator>
#include <range/v3/all.hpp>
#include <cassert>

using namespace std;
using namespace ranges;


template <class string_type>
class string_tokenizer_by_char;

//--------------------------------------------------------------------------------------------
template <class string_type>
class string_tokenizer_iterator_by_char : public std::iterator<std::forward_iterator_tag, string_type> {
public:
    string_tokenizer_iterator_by_char(const string_type &_src,typename string_type::value_type _separator):src(_src),separator(_separator)
    {
        //cout<<"Construteur : src="<<((unsigned long long)(void*)&src)<<" src.str()="<<((unsigned long long)(void*)src.c_str())<<" "<<src<<endl;
        it=src.cbegin();
        calc_next_it();
    }
    string_tokenizer_iterator_by_char& operator ++()
    {
        //cout<<"operator ++ : "<<tok.src<<endl;
        it=next_it;
        if(it!=src.cend()) {
            it++;
            calc_next_it();
        }
        return *this;
    }
    string_type operator *()
    {
        //cout<<"operator * "<<((unsigned long long)(void*)this)<<" "<<((unsigned long long)(void*)&src)<<" "<<src<<endl;
        //return string_type(it,next_it); // This one may not work with string_view as there's no constructor taking 2 string_view iterator.
        return src.substr(it-src.cbegin(),next_it-it);
    }
    bool operator !=(const string_tokenizer_iterator_by_char &other) const
    {
        return (it!=other.it);
    }
protected:
    const string_type &src;

    typename string_type::value_type separator;
    typename string_type::const_iterator it;
    typename string_type::const_iterator next_it;

    string_tokenizer_iterator_by_char(const string_type &_src):src(_src)
    {} // Just to initialize end()
    friend string_tokenizer_by_char<string_type>;

    void calc_next_it()
    {
        //cout<<"calc_next_it : "<<((unsigned long long)(void*)this)<<" "<<src<<endl;
        for(next_it=it;next_it!=src.cend();++next_it) {
            //cout<<"*next_it==separator "<<*next_it<<"=='"<<separator<<"'"<<endl;
            if(*next_it==separator)
                break;
            }
    }
};

//--------------------------------------------------------------------------------------------
template <class string_type>
class string_tokenizer_by_char{
public:
    string_tokenizer_by_char(const string_type &_src,typename string_type::value_type _separator)
        :src(_src),separator(_separator)
        ,TheBegin(_src,_separator)
        ,TheEnd(_src)
    {
        TheEnd.it=TheEnd.next_it=src.cend();
    }
    typedef string_tokenizer_iterator_by_char<string_type> iterator;

    string_tokenizer_by_char::iterator begin()
    {
        //cout<<"string_tokenizer::begin "<<((unsigned long long)(void*)&TheBegin)<<" "<<endl;
        return TheBegin;
    }
    string_tokenizer_by_char::iterator end()
    {
        return TheEnd;
    }
protected:
    const string_type &src;
    typename string_type::value_type separator;
    string_tokenizer_iterator_by_char<string_type>  TheBegin;
    string_tokenizer_iterator_by_char<string_type>  TheEnd;

};


//***********************************************************************************
template <class string_type>
class string_tokenizer_range
    : protected string_tokenizer_by_char<string_type>
    , public iterator_range<typename string_tokenizer_by_char<string_type>::iterator,typename string_tokenizer_by_char<string_type>::iterator>
{
public:
    string_tokenizer_range(const string_type &str,typename string_type::value_type sep)
        :string_tokenizer_by_char<string_type>(str,sep)
        ,iterator_range<typename string_tokenizer_by_char<string_type>::iterator,typename string_tokenizer_by_char<string_type>::iterator>(string_tokenizer_by_char<string_type>::begin(),string_tokenizer_by_char<string_type>::end())
        {}
    using iterator_range<typename string_tokenizer_by_char<string_type>::iterator,typename string_tokenizer_by_char<string_type>::iterator>::begin;
    using iterator_range<typename string_tokenizer_by_char<string_type>::iterator,typename string_tokenizer_by_char<string_type>::iterator>::end;
};


//***********************************************************************************
 template <class string_type>
  string_tokenizer_range<string_type> string_split(const string_type &str,typename string_type::value_type sep)
 {
    return string_tokenizer_range<string_type>(str,sep);
 }



int main()
{
    string str="C++_is_fun";
    string_tokenizer_by_char<string> st(str,'_');
    for(auto it=st.begin();it!=st.end();++it)
        cout<<*it<<endl;

    cout << "Tests on Vector" << endl;
    vector<string> vect1(st.begin(),st.end());
    for(auto &x:vect1)
        cout<<x<<endl;

    cout<<" ----- Range Test 1 -------"<<endl;
    std::vector<int> vi{1,6,5,2,4,3};
    std::vector<int> vi2 = std::move(vi) | action::sort | action::unique;
    for(auto i:vi)
        cout<<i<<endl;
    for(auto i:vi2)
        cout<<"x  "<<i<<endl;

    cout<<" ----- Range Test 2 -------"<<endl;
 iterator_range<vector<int>::iterator,vector<int>::iterator> ir(vi2.begin(),vi2.end());
    for(auto i:ir)
        cout<<"y  "<<i<<endl;
    cout<<" ----- Range Test 3 -------"<<endl;
 iterator_range<string_tokenizer_by_char<string>::iterator,string_tokenizer_by_char<string>::iterator> irst(st.begin(),st.end());
    for(auto i:irst)
        cout<<"z  "<<i<<endl;
    cout<<" ----- Range Test 4 -------"<<endl;
    string str2("C++ 17 is fun");
//  for(auto i:string_split("C++ 17 is fun",' '))    // Strange loose of string reference on non generic version, doesnt works on Generic version
    for(auto i:string_split(str2,' '))
        cout<<i<<endl;
/*    cout<<" ----- Range Test 5 -------"<<endl;
    std::vector<string> vi3=string_split(str2,' '); // Doesn't works
    for(auto i:vi3)
        cout<<i<<endl;
        */

    cout << "Tests End" << endl;
    return 0;
}

