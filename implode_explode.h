#pragma once
#include <string>

/**
implode/join a set of string/string_view with Separator.
May be defined as static method of basic_string.
@param InputStringList iterable Container(vector,list,...) containing input string
@param Separator used to join strings
@return agregated string
**/
template<class T, class U>
auto string_implode(T &InputStringList, U Separator)
{
	std::basic_string <typename T::value_type::value_type > result_string;
	for (auto it = InputStringList.begin(); it != InputStringList.end(); it++)
	{
		if (it != InputStringList.begin())
			result_string += Separator;
		result_string += *it;

	}
	return result_string;
}

/**
explode/split a string/string_view with Separator.
May be defined as method of basic_string.
@param This input string (may not exists has class member, would be simply replaced by this
@param Separator used to split the string
@param Result Output container
**/
template<class T, class V>
void string_explode(const T &This, const  T Separator, V &Result)
{
	Result.clear();
	typename T::size_type p, start = 0;
	while (1)
	{
		p = This.find(Separator, start);
		if (p == T::npos)
		{
			Result.emplace_back(This, start, T::npos);
			return;
		}
		else
		{
			Result.emplace_back(This, start, p - start);
			// emplace_back doesn't works with string_view container, other version of string_explode use substr method
			//, may work if required construtors added on string_view class
			start = p + Separator.length();
		}
	}
}

/**
explode/split a string/string_view with Separator.
May be defined as method of basic_string.
@param This input string (may not exists has class member, would be simply replaced by this
@param Separator used to split the string
@return Output container
**/
template<class T, class V>
V string_explode(const T &This, const  T Separator)
{
	V Result; // Result Container V Parametred
	typename T::size_type p, start = 0;
	while (1)
	{
		p = This.find(Separator, start);
		if (p == T::npos)
		{
			Result.emplace_back(((typename V::value_type)(This)).substr(start, T::npos));
			return Result;
		}
		else
		{
			Result.emplace_back(((typename V::value_type)(This)).substr(start, p - start));
			start = p + Separator.length();
		}
	}
}
