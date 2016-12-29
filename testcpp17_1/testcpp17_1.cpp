#include <iostream>
#include <vector>
#include <string>
#include <string_view>


using namespace std;


template<
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
>
class basic_string17:public basic_string<CharT,Traits,Allocator>
{
public:
    using basic_string<CharT,Traits,Allocator>::basic_string;

	
    vector<basic_string<CharT,Traits,Allocator> > splits(const basic_string<CharT,Traits,Allocator> Separator) const
    {
        vector<basic_string<CharT,Traits,Allocator> > Result;
        typename basic_string<CharT,Traits,Allocator>::size_type p, start = 0;
        while (1)
        {
            p = basic_string<CharT,Traits,Allocator>::find(Separator, start);
            if (p == basic_string<CharT,Traits,Allocator>::npos)
            {
                Result.emplace_back(basic_string<CharT,Traits,Allocator>::substr(start, basic_string<CharT,Traits,Allocator>::npos));
                return Result;
            }
            else
            {
                Result.emplace_back(basic_string<CharT,Traits,Allocator>::substr(start, p - start));
                start = p + Separator.length();
            }
        }
    }
	// Overload with separator as simple char to allow optimization.
	vector<basic_string<CharT, Traits, Allocator> > splits(const typename basic_string<CharT, Traits, Allocator>::value_type Separator) const
	{
		vector<basic_string<CharT, Traits, Allocator> > Result;
		typename basic_string<CharT, Traits, Allocator>::size_type p=0, start = 0;
		while (1)
		{
			if(data()[p]== Separator)
			{
				Result.emplace_back(basic_string<CharT, Traits, Allocator>::substr(start, p - start));
				++p;
				start = p;
			}
			else if (data()[p] == 0)
			{
				Result.emplace_back(basic_string<CharT, Traits, Allocator>::substr(start, basic_string<CharT, Traits, Allocator>::npos));
				return Result;
			}
			else
			{
				++p;
			}
		}
	}


	vector<basic_string_view<CharT, Traits> > splitsv(const basic_string_view<CharT, Traits> &Separator) const
	{
		basic_string_view17<CharT, Traits> tmp(this->data(), this->length()); // Should work with tmp(*this)
		return tmp.splitsv(Separator);
	}

	vector<basic_string_view<CharT, Traits> > splitsv(const typename basic_string<CharT, Traits, Allocator>::value_type Separator) const
	{
		basic_string_view17<CharT, Traits> tmp(this->data(), this->length()); // Should work with tmp(*this)
		return tmp.splitsv(Separator);
	}

	template<class T,class U>
	static basic_string<CharT, Traits, Allocator> join(T &InputStringList, U Separator)
	{
		basic_string<CharT, Traits, Allocator> result_string;
		for (auto it = InputStringList.begin(); it != InputStringList.end(); it++)
		{
			if (it != InputStringList.begin())
				result_string += Separator;
			result_string += *it;

		}
		return result_string;
	}
	template<class T>
	basic_string<CharT, Traits, Allocator> join(T &InputStringList)
	{
		basic_string<CharT, Traits, Allocator> result_string;
		for (auto it = InputStringList.begin(); it != InputStringList.end(); it++)
		{
			if (it != InputStringList.begin())
				result_string += *this;
			result_string += *it;

		}
		return result_string;
	}


};
typedef basic_string17<char> string17;

/*
template<class T,class U>
basic_string<typename T::value_type::value_type, typename T::value_type::traits_type> string_join(T &InputStringList, U Separator)
{
	basic_string<T::value_type::value_type, T::value_type::traits_type> result_string;
	for (auto it = InputStringList.begin(); it != InputStringList.end(); it++)
	{
	if (it != InputStringList.begin())
	result_string += Separator;
	result_string += *it;

	}
	return result_string;
}
*/
template<class T>
basic_string<typename T::value_type::value_type, typename T::value_type::traits_type> string_join(const T &InputStringList, const basic_string_view<typename T::value_type::value_type,typename T::value_type::traits_type> Separator)
{
	basic_string<T::value_type::value_type, T::value_type::traits_type> result_string;
	size_t StrLen = 0;	
	if (InputStringList.empty())
		return result_string;
	auto it = InputStringList.begin();
	for (; it != InputStringList.end(); ++it)
		StrLen += it->size() + Separator.size();
	result_string.reserve(StrLen);
	result_string += *InputStringList.begin();
	for (it = ++InputStringList.begin(); it != InputStringList.end(); ++it)
	{
		result_string += Separator;
		result_string += *it;
	}
	return result_string;
}

// doesn't works with char*
/*template<class T, class U>
auto string_join2(T &InputStringList, U Separator)
{	
	if (InputStringList.empty())
		return T::value_type();
	auto result_string = *InputStringList.begin();
	for (auto it = ++InputStringList.begin(); it != InputStringList.end(); it++)
	{
		result_string += Separator;
		result_string += *it;
	}
	return result_string;
}
*/


template<
	class CharT,
	class Traits = std::char_traits<CharT>
>
class basic_string_view17 :public basic_string_view<CharT, Traits>
{
public:
	using basic_string_view<CharT, Traits>::basic_string_view;

	vector<basic_string<CharT, Traits> > splits(const basic_string_view<CharT, Traits> &Separator) const
	{
		vector<basic_string<CharT, Traits> > Result;
		typename basic_string_view<CharT, Traits>::size_type p, start = 0;
		while (1)
		{
			p = basic_string_view<CharT, Traits>::find(Separator, start);
			if (p == basic_string_view<CharT, Traits>::npos)
			{
				Result.emplace_back(basic_string_view<CharT, Traits>::substr(start));
				return Result;
			}
			else
			{
				Result.emplace_back(basic_string_view<CharT, Traits>::substr(start, p - start));
				start = p + Separator.length();
			}
		}
	}
	// Overload with separator as simple char to allow optimization.
	vector<basic_string<CharT, Traits> > splits(const typename basic_string_view<CharT, Traits>::value_type Separator) const
	{
		vector<basic_string<CharT, Traits> >  Result;
		typename basic_string_view<CharT, Traits>::size_type p = 0, start = 0;
		while (1)
		{
			if (data()[p] == Separator)
			{
				Result.emplace_back(basic_string_view<CharT, Traits>::substr(start, p - start));
				++p;
				start = p;
			}
			else if (data()[p] == 0)
			{
				Result.emplace_back(basic_string_view<CharT, Traits>::substr(start));
				return Result;
			}
			else
			{
				++p;
			}
		}
	}
	vector<basic_string_view<CharT, Traits> > splitsv(const basic_string_view<CharT, Traits> &Separator) const
	{
		vector<basic_string_view<CharT, Traits> > Result;
		typename basic_string_view<CharT, Traits>::size_type p, start = 0;
		while (1)
		{
			p = basic_string_view<CharT, Traits>::find(Separator, start);
			if (p == basic_string_view<CharT, Traits>::npos)
			{
				Result.emplace_back(basic_string_view<CharT, Traits>::substr(start, basic_string_view<CharT, Traits>::npos));
				return Result;
			}
			else
			{
				Result.emplace_back(basic_string_view<CharT, Traits>::substr(start, p - start));
				start = p + Separator.length();
			}
		}
	}
	// Overload with separator as simple char to allow optimization.
	vector<basic_string_view<CharT, Traits> > splitsv(const typename basic_string_view<CharT, Traits>::value_type Separator) const
	{
		vector<basic_string_view<CharT, Traits> >  Result;
		typename basic_string_view<CharT, Traits>::size_type p = 0, start = 0;
		while (1)
		{
			if (data()[p] == Separator)
			{
				Result.emplace_back(basic_string_view<CharT, Traits>::substr(start, p - start));
				++p;
				start = p;
			}
			else if (data()[p] == 0)
			{
				Result.emplace_back(basic_string_view<CharT, Traits>::substr(start));
				return Result;
			}
			else
			{
				++p;
			}
		}
	}
	template <class F>
	void splitf(const basic_string_view<CharT, Traits> &Separator,F functor) const
	{
		typename basic_string_view<CharT, Traits>::size_type p, start = 0;
		while (1)
		{
			p = basic_string_view<CharT, Traits>::find(Separator, start);
			if (p == basic_string_view<CharT, Traits>::npos)
			{
				functor(basic_string_view<CharT, Traits>::substr(start, basic_string_view<CharT, Traits>::npos));
				return ;
			}
			else
			{
				functor(basic_string_view<CharT, Traits>::substr(start, p - start));
				start = p + Separator.length();
			}
		}

	}
	template <class T>
	void splitc(const basic_string_view<CharT, Traits> &Separator,T &Result) const
	{		
		typename basic_string_view<CharT, Traits>::size_type p, start = 0;
		Result.clear();
		while (1)
		{
			p = basic_string_view<CharT, Traits>::find(Separator, start);
			if (p == basic_string_view<CharT, Traits>::npos)
			{
				Result.emplace_back(basic_string_view<CharT, Traits>::substr(start, basic_string_view<CharT, Traits>::npos));
				return ;
			}
			else
			{
				Result.emplace_back(basic_string_view<CharT, Traits>::substr(start, p - start));
				start = p + Separator.length();
			}
		}
	}

};
typedef basic_string_view17<char> string_view17;


int main()
{
    string17 str("C++ is fun");
    cout << str << endl;
    vector<string> vector1=str.splits(" "); // string => vector<string>
    for(auto s:vector1)
        cout << s << endl;

	vector1 = str.splits(' '); // string => vector<string> with char separator
	for (auto s : vector1)
		cout << s << endl;


	string_view17 strsv("C++17 is fun with string_view");
	cout << strsv << endl;
	vector<string> vector2 = strsv.splits(" "); // string_view => vector<string>
	for (auto s : vector2)
		cout << s << endl;
	vector2 = strsv.splits(' '); // string_view => vector<string> with char separator
	for (auto s : vector2)
		cout << s << endl;

	cout << "---- test splitsv -----" << endl;
	vector<string_view> vector3 = strsv.splitsv(" "); // string_view => vector<string_view>
	for (auto s : vector3)
		cout << s << endl;
	vector3 = strsv.splitsv(' '); // string_view => vector<string_view> with char separator
	for (auto s : vector3)
		cout << s << endl;

	vector<string_view> vector4 = str.splitsv(" "); // string => vector<string_view>
	for (auto s : vector4)
		cout << s << endl;
	vector4 = str.splitsv(' '); // string => vector<string_view> with char separator
	for (auto s : vector4)
		cout << s << endl;
	cout << "---- test splitf -----" << endl;
	strsv.splitf(" ", [&](const string_view &s)
	{
		cout << s <<"   ,Pos="<<(s.data() -strsv.data())<<" ,Len="<<s.length()<< endl;
	}); // string_view => functor
	cout << "---- test splitc -----" << endl;
	vector<string_view> vector5;
	strsv.splitc(" ", vector5); // splitc in vector<string_view>
	for (auto s : vector5)
		cout << s << endl; 
	vector<string> vector6;
	strsv.splitc(" ", vector6); // splitc in vector<string>
	for (auto s : vector6)
		cout << s << endl;

	cout << "Join of string vector=" << string17::join(vector6, "_") << endl;
	cout << "Join of string_view vector=" << string17::join(vector5, "_") << endl;
	cout << "Join of string_view vector with string_join=" << string_join(vector5, "_") << endl;
	//cout << "pythonic Join of string =" << "-"s.join(vector6) << endl;
	cout << "pythonic Join of string =" << string17("-").join(vector6) << endl;
	// doesn't works with char*
	//vector<char*> vector7{ "C++","is","fun","even with char*" };
	//cout << "Join of char* vector =" << string_join2(vector7, "_") << endl;

    cout << "---- End of tests ----" << endl;

    return 0;
}

