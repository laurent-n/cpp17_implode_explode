#include <iostream>
#include "string17.h"

using namespace std;

// Used to test on temporary string
string string_toLower(string Str)
{
    std::transform(Str.begin(), Str.end(), Str.begin(), ::tolower);
    return Str;
}


void VerbosingTest()
{
string17 str("C++ is fun");
    cout << str << endl;
    //vector<string> vector1=str.splits(" "); // string => vector<string>
    auto vector1=str.splits(" "); // string => vector<string>
    for(auto s:vector1) {
        cout << s << endl;
    }

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
	cout << "---- test split on regex -----" << endl;
	regex r1("\\s");
	strsv.splitf(r1, [&](const string_view &s)
	{
		cout << s <<"   ,Pos="<<(s.data() -strsv.data())<<" ,Len="<<s.length()<< endl;
	}); // string_view => functor
	cout <<"splits (regex)="<<string_join( strsv.splits(r1),"_")<<endl;
	cout <<"splitsv(regex)="<<string_join( strsv.splitsv(r1),"-")<<endl;

}

// TESTS from N3593 prototype
using Vec_sv = std::vector<string_view>;

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
        auto vec = Vec_sv(string_view17().splitsv(","));
        EXPECT_EQUAL(vec.size(), 0U);
        //EXPECT_EQUAL(vec[0], "");
    }
    {
        auto vec = Vec_sv(string_view17("").splitsv(","));
        EXPECT_EQUAL(vec.size(), 0U);
        //EXPECT_EQUAL(vec[0], "");
    }
    {
        auto vec = Vec_sv(string_view17().splitsv(','));
        EXPECT_EQUAL(vec.size(), 0U);
        //EXPECT_EQUAL(vec[0], "");
    }
    {
        auto vec = Vec_sv(string_view17().splitsv(regex("\\s")));
        EXPECT_EQUAL(vec.size(), 0U);
        //EXPECT_EQUAL(vec[0], "");
    }
}

static void test_LeadingDelimiters()
{
    {
        auto vec = string_view17(",").splits(',');
        EXPECT_EQUAL(2u, vec.size());
        EXPECT_EQUAL("", vec[0]);
        EXPECT_EQUAL("", vec[1]);
    }
    {
        auto vec = Vec_sv(string_view17(", ").splitsv( ','));
        EXPECT_EQUAL(2u, vec.size());
        EXPECT_EQUAL("", vec[0]);
        EXPECT_EQUAL(" ", vec[1]);
    }
}

static void test_SimpleLiteralTests()
{
    {
        auto vec = Vec_sv(string_view17("a").splitsv( ","));
        EXPECT_EQUAL(1u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
    }
    {
        auto vec = Vec_sv(string_view17("a,").splitsv( ","));
        EXPECT_EQUAL(2u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
        EXPECT_EQUAL("", vec[1]);
    }
    {
        auto vec = Vec_sv(string_view17("a,b").splitsv(regex(",")));
        EXPECT_EQUAL(2u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
        EXPECT_EQUAL("b", vec[1]);
    }
    {
        auto vec = Vec_sv(string_view17("a,").splitsv(regex(",")));
        EXPECT_EQUAL(2u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
        EXPECT_EQUAL("", vec[1]);
    }
    {
        auto vec = Vec_sv(string_view17("a,b").splitsv( ","));
        EXPECT_EQUAL(2u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
        EXPECT_EQUAL("b", vec[1]);
    }
    {
        auto vec = Vec_sv(string_view17("-a-b-c----d").splitsv( "-"));
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
        auto vec = Vec_sv(string_view17("-a-b-c----d").splitsv( "--"));

        EXPECT_EQUAL(3u, vec.size());
        EXPECT_EQUAL("-a-b-c", vec[0]);
        EXPECT_EQUAL("", vec[1]);
        EXPECT_EQUAL("d", vec[2]);
    }
}


static void test_AnyOfDelimiter()
{
    {
        auto vec = Vec_sv(string_view17("a.b-c,. d, e .f-").splitsv(regex("[.,-]")));
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
    { // with temporary object
		string s("ss");
//        auto vec= string_view17(string_toLower( "A.b-C,. d, e .f-")).splits(regex("[\\.,\\-]"));
		auto vec = string_view17(string_toLower("A.b-C,. d, e .f-").c_str()).splits(regex("[\\.,\\-]"));
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
    {  // With multisize regex added + after []
        auto vec = Vec_sv(string_view17("a.b-c,. d, e .f-").splitsv(regex("[.,-]+")));
        EXPECT_EQUAL(7u, vec.size());
        EXPECT_EQUAL("a", vec[0]);
        EXPECT_EQUAL("b", vec[1]);
        EXPECT_EQUAL("c", vec[2]);
        EXPECT_EQUAL(" d", vec[3]);
        EXPECT_EQUAL(" e ", vec[4]);
        EXPECT_EQUAL("f", vec[5]);
        EXPECT_EQUAL("", vec[6]);
    }

}


static void test_EmptyDelimiters()
{
    {
        auto vec = Vec_sv(string_view17().splitsv(""));
        EXPECT_EQUAL(vec.size(), 0U);
    }
    {
        auto vec = Vec_sv(string_view17().splitsv('\0'));
        EXPECT_EQUAL(vec.size(), 0U);
    }
    {
        auto vec = Vec_sv(string_view17("").splitsv( ""));
        EXPECT_EQUAL(vec.size(), 0U);
    }
    {
        auto vec = Vec_sv(string_view17("").splitsv('\0'));
        EXPECT_EQUAL(vec.size(), 0U);
    }
    {
        auto vec = Vec_sv(string_view17("x").splitsv( ""));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "x");
    }
    {
        auto vec = Vec_sv(string_view17("x").splitsv('\0'));
        EXPECT_EQUAL(vec.size(), 1U);
        EXPECT_EQUAL(vec[0], "x");
    }
    {
        auto vec = Vec_sv(string_view17("abc").splitsv(""));
        EXPECT_EQUAL(vec.size(), 3U);
        EXPECT_EQUAL(vec[0], "a");
        EXPECT_EQUAL(vec[1], "b");
        EXPECT_EQUAL(vec[2], "c");
    }
    {
        auto vec = Vec_sv(string_view17("abc").splitsv('\0'));
        EXPECT_EQUAL(vec.size(), 3U);
        EXPECT_EQUAL(vec[0], "a");
        EXPECT_EQUAL(vec[1], "b");
        EXPECT_EQUAL(vec[2], "c");
    }
    {
        Vec_sv vec ;
        string_view17("abc").splitf("",[&](auto const &s) {vec.push_back(s);});
        EXPECT_EQUAL(vec.size(), 3U);
        EXPECT_EQUAL(vec[0], "a");
        EXPECT_EQUAL(vec[1], "b");
        EXPECT_EQUAL(vec[2], "c");
    }
    {
        Vec_sv vec ;
        string_view17("a,b,c").splitf(",",[&](auto const &s) {vec.push_back(s);});
        EXPECT_EQUAL(vec.size(), 3U);
        EXPECT_EQUAL(vec[0], "a");
        EXPECT_EQUAL(vec[1], "b");
        EXPECT_EQUAL(vec[2], "c");
    }
{
        Vec_sv vec ;
        string_view17("a,,,,c").splitf(",,",[&](auto const &s) {vec.push_back(s);});
        EXPECT_EQUAL(vec.size(), 3U);
        EXPECT_EQUAL(vec[0], "a");
        EXPECT_EQUAL(vec[1], "");
        EXPECT_EQUAL(vec[2], "c");
    }

}

void test_Join()
{
    Vec_sv vsv{"C++","is","fun"};
    string res;
    res=string17::join(vsv,"-");
    EXPECT_EQUAL(res, "C++-is-fun");
    //Pyhton Style
    //res="."s.join(vsv); // Will work when it will be in the standard
    res=string17(".").join(vsv);
    EXPECT_EQUAL(res, "C++.is.fun");

    vector<const char*> vchar{"C++","is","fun"};
    res=string17::join(vsv,"*");
    EXPECT_EQUAL(res, "C++*is*fun");

}

int main()
{
    test_EmptyStrings();
    test_LeadingDelimiters();
    test_SimpleLiteralTests();
    test_AnyOfDelimiter();
    test_EmptyDelimiters();
    test_Join();
//    VerbosingTest();
    cout << "---- End of tests ----" << endl;

    return 0;
}

