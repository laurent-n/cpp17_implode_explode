#include "implode_explode.h"
#include <iostream>
#include <string>
#include <vector>
#include <string_view>

using namespace std;



int main()
{
	vector<string> Vector1{ "C++","is","fun!" };
	string res1 = string_implode(Vector1, " ");
	cout << "string_implode(Vector1,' ') => " << res1 << endl;
	//string_implode(Vector1,' ') => C++ is fun!

	vector<wstring> Vectorw1{ L"C++",L"is",L"widely",L"fun!" };
	wstring wres1 = string_implode(Vectorw1, L" ");
	wcout << L"string_implode(Vectorw1,' ') => " << wres1 << endl;
	//string_implode(Vectorw1, ' ') = > C++ is widely fun!

	vector<string> Vector2;
	string_explode(string("C++ 17 will be fun!"), string(" "), Vector2);
	cout << "string_explode  =>" << string_implode(Vector2, "_") << endl;
	//string_explode = >C++_17_will_be_fun!

	auto Vector3 = string_explode<string, vector<string> >("C++ 17 will be fun! by return", " ");
	cout << "string_explode  =>" << string_implode(Vector3, "_") << endl;
	//string_explode = >C++_17_will_be_fun!_by_return

	// ======================= Using String View
	vector<string_view> Vector1sv{ "C++","string_view","are","fun!" };
	string res2 = string_implode(Vector1sv, " ");
	cout << "string_implode(Vector1sv,' ') => " << res2 << endl;
	//string_implode(Vector1sv,' ') => C++ string_view are fun!

	string str1("C++ 17 will be fun! with string_view by return");
	auto Vector4 = string_explode<string, vector<string_view> >(str1, " ");
	cout << "string_explode  =>" << string_implode(Vector4, "/") << endl;
	//string_explode2 = >C++/17/will/be/fun!/with/string_view/by/return

	auto Vector5 = string_explode<string_view, vector<string_view> >("C++ 17 will be fun! with string_view by return", " ");
	cout << "string_explode  =>" << string_implode(Vector5, "/") << endl;
	//string_explode2 = >C++/17/will/be/fun!/with/string_view/by/return


}