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



    vector<basic_string<CharT,Traits,Allocator> > splits(const  basic_string<CharT,Traits,Allocator> Separator) const
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

	vector<basic_string_view<CharT, Traits> > splitsv(const  basic_string_view<CharT, Traits> &Separator) const
	{
		basic_string_view17<CharT, Traits> tmp(this->data(),this->length()); // Should work with tmp(*this)
		return tmp.splitsv(Separator);  
	}

};
typedef basic_string17<char> string17;


template<
	class CharT,
	class Traits = std::char_traits<CharT>
>
class basic_string_view17 :public basic_string_view<CharT, Traits>
{
public:
	using basic_string_view<CharT, Traits>::basic_string_view;

	vector<basic_string<CharT, Traits> > splits(const  basic_string_view<CharT, Traits> &Separator) const
	{
		vector<basic_string<CharT, Traits> > Result;
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

	vector<basic_string_view<CharT, Traits> > splitsv(const  basic_string_view<CharT, Traits> &Separator) const
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

};
typedef basic_string_view17<char> string_view17;


int main()
{
    string17 str("C++ is fun");
    cout << str << endl;
    vector<string> vector1=str.splits(" "); // string => vector<string>
    for(auto s:vector1)
        cout << s << endl;

	string_view17 strsv("C++17 is fun with string_view");
	cout << strsv << endl;
	vector<string> vector2 = strsv.splits(" "); // string_view => vector<string>
	for (auto s : vector2)
		cout << s << endl;

	vector<string_view> vector3 = strsv.splitsv(" "); // string_view => vector<string_view>
	for (auto s : vector3)
		cout << s << endl;

	vector<string_view> vector4 = str.splitsv(" "); // string => vector<string_view>
	for (auto s : vector4)
		cout << s << endl;

    cout << "---- End of tests ----" << endl;

    return 0;
}

