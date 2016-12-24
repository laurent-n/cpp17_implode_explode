# c++ join/split code snippet
Here are 3 codes snippet implementing string join and string split using C++ 11/14/17.

To use it, just include `split_join.h` or copy paste the function you want, there's no external dependencies, you can even rename it as your convenience.

You can also adapt it to work with basic_string only or force to return a vector.
### string_join
Will join/merge/aggregate `string`/`string_view` with a separator and return the result into a `string`
Exemple :
```
  vector<string> Vector1{ "C++","is","fun!" };
  string res1 = string_join(Vector1, " ");
  cout << "string_join(Vector1,' ') => " << res1 << endl;
```
Result : "C++ is fun!"

Works also with `wstring`
```
  vector<wstring> Vectorw1{ L"C++",L"is",L"widely",L"fun!" };
  wstring wres1 = string_join(Vectorw1, L" ");
  wcout << L"string_join(Vectorw1,' ') => " << wres1 << endl;
```

### string_split
Will split/tokenize a `string`/`string_view` in multiple `string`/`string_view` in a collection/container
```
  vector<string> Vector2;
  string_split(string("C++ 17 will be fun!"), string(" "), Vector2);
  cout << "string_split  =>" << string_join(Vector2, "_") << endl;
```
Result : "C++_17_will_be_fun!"  `// I use string_join to display easily`
  
Another syntax using return, specify input string type and output a template parameter on call
```
  auto Vector3 = string_split<string, vector<string> >("C++ 17 will be fun! by return", " ");
  cout << "string_split  =>" << string_join(Vector3, "_") << endl;
```  
  Result : C++_17_will_be_fun!_by_return

**See main_demo.cpp for more examples.**

### Tested Compiler
Code running on VS2013/2015/2017 
In order to have GCC working with `string_view` on C++ 17 Beta phase, it will require this extra code
Else it works fine on GCC 4.9
```  
using namespace std::experimental;
// Quick fix required to run on GCC , standard seems to cover it
// basic_string& operator+=( std::basic_string_view<CharT, Traits> sv);
string& operator+=(string &str,string_view sv){
    str+=string(sv);
    return str;
}
```  

