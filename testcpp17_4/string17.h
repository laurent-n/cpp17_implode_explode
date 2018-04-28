#include <vector>
#include <string>
#include <regex>
#include <type_traits>
#ifdef __GNUC__
#include <experimental/string_view>

template<class CharT, class Traits>
using basic_string_view = std::experimental::basic_string_view<CharT, Traits>;
using string_view = std::experimental::string_view;
#else

#include <string_view>
using std::basic_string_view;
#endif // __GNUC__

using std::basic_string;
using std::vector;
using std::basic_regex;

template<
	class CharT,
	class Traits
>
class basic_string_view17;


template<
	class CharT,
	class Traits = std::char_traits<CharT>,
	class Allocator = std::allocator<CharT>
>
class basic_string17 :public basic_string<CharT, Traits, Allocator>
{
public:
	using basic_string<CharT, Traits, Allocator>::basic_string;


	//******************************************************************************
	// SPLIT Version replacing splits & splitsv for any separator
	template<class StringType = basic_string_view<CharT, Traits>, class SeparatorType >
	vector<StringType > split(const SeparatorType &Separator) const
	{
		basic_string_view17<CharT, Traits> sv(this->c_str(),this->size());
		return sv.split<StringType>(Separator);
	}

	// SPLIT Version replacing splitf & splitc for any separator
	template<class SeparatorType, class TargetType>
	void split(const SeparatorType &Separator, TargetType &Target) const
	{
		basic_string_view17<CharT, Traits> sv(this->c_str(), this->size());
		return sv.split(Separator, Target);
	}
	//******************************************************************************
	// Version returning a String View as caller is a lvalue
	template<class SeparatorType >
	vector<basic_string_view<CharT, Traits> > split2(const SeparatorType &Separator)  const &
	{
		basic_string_view17<CharT, Traits> sv(this->c_str(), this->size());
		return sv.split<basic_string_view<CharT, Traits>>(Separator);
	}
	
	//******************************************************************************
	// Version returning a String  as caller is a rvalue / temporary object
	template<class SeparatorType >
	vector<basic_string<CharT, Traits> > split2(const SeparatorType &Separator)  const &&
	{
		basic_string_view17<CharT, Traits> sv(this->c_str(), this->size());
		return sv.split<basic_string<CharT, Traits>>(Separator);
	}
	

	template<class T, class U>
	static basic_string<CharT, Traits, Allocator> join(T &InputStringList, U Separator)
	{
		basic_string<CharT, Traits, Allocator> result_string;
		typename basic_string<CharT, Traits, Allocator>::size_type StrLen = 0;
		auto it = InputStringList.begin();
		for (it = InputStringList.begin(); it != InputStringList.end(); it++)
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

	void toupper_inplace()
	{
		std::transform(begin(), end(), begin(), ::toupper);
	}

};
typedef basic_string17<char> string17;

template<class T>
basic_string<typename T::value_type::value_type, typename T::value_type::traits_type>
string_join(const T &InputStringList, const basic_string_view<typename T::value_type::value_type, typename T::value_type::traits_type> Separator)
{
	basic_string<typename T::value_type::value_type, typename T::value_type::traits_type> result_string;
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



//**************************************************************************************************************************
//**************************************************************************************************************************
//**************************************************************************************************************************
template<
	class CharT,
	class Traits = std::char_traits<CharT>
>
class basic_string_view17 :public basic_string_view<CharT, Traits>
{

	template <typename T>
	struct HasclearMethod
	{
		template <class, class> class checker;

		template <typename C>
		static std::true_type test(checker<C, decltype(&C::clear)> *);

		template <typename C>
		static std::false_type test(...);

		typedef decltype(test<T>(nullptr)) type;
		static const bool value = std::is_same<std::true_type, decltype(test<T>(nullptr))>::value;
	};


public:
	using basic_string_view<CharT, Traits>::basic_string_view;


	//******************************************************************************
	// Split by string
	template <class F
		, typename std::enable_if<!HasclearMethod<F>::value>::type* = nullptr
	>
		void split(const basic_string_view<CharT, Traits> &Separator, F functor) const
	{

		if (Separator.size() <= 1) {
			if (Separator.size() == 0)
				this->split(0, functor);
			else
				this->split(Separator[0], functor);
			return;
		}

		if (this->empty()) return;
		typename basic_string_view<CharT, Traits>::size_type p, start = 0;
		while (1)
		{
			p = basic_string_view<CharT, Traits>::find(Separator, start);
			if (p == basic_string_view<CharT, Traits>::npos)
			{
				functor(basic_string_view<CharT, Traits>::substr(start, basic_string_view<CharT, Traits>::npos));
				return;
			}
			else
			{
				functor(basic_string_view<CharT, Traits>::substr(start, p - start));
				start = p + Separator.length();
			}
		}

	}
	// Overload with separator as simple char to allow optimization.
	template <class F
		, typename std::enable_if<!HasclearMethod<F>::value>::type* = nullptr
	>
	void split(const typename basic_string_view<CharT, Traits>::value_type Separator, F functor) const
	{

		if (this->empty()) return;
		typename basic_string_view<CharT, Traits>::size_type p = 0, start = 0;
		if (Separator == 0) {
			for (p = 0; p<this->size(); ++p)
				functor(basic_string_view<CharT, Traits>::substr(p, 1));
			return;
		}
		while (1)
		{
			if (p >= this->size())
			{
				functor(basic_string_view<CharT, Traits>::substr(start));
				return;
			}
			if ((*this)[p] == Separator)
			{
				functor(basic_string_view<CharT, Traits>::substr(start, p - start));
				++p;
				start = p;
			}
			else
			{
				++p;
			}
		}
	}
	//******************************************************************************
	template <class F
		, typename std::enable_if<!HasclearMethod<F>::value>::type* = nullptr
	>
	void split(const basic_regex<CharT> &Separator, F functor) const
	{
		if (this->empty()) return;
		using sregex_iterator = std::regex_iterator<typename basic_string_view17<CharT, Traits>::iterator, CharT>;
		sregex_iterator it_(this->begin(), this->end(), Separator);
		typename basic_string_view17<CharT, Traits>::size_type prev = 0;
		for (; it_ != sregex_iterator(); ++it_)
		{
			auto m = *it_;
			//std::cout << m.str() << " at position " << m.position()<< " len= " << m.length() << '\n';
			functor(this->substr(prev, m.position() - prev));
			prev = m.position() + m.length();

		}
		functor(this->substr(prev));
		//std::cout << "End with  " << this->substr(prev) << '\n';
	}
	//******************************************************************************
	// Split on container aka splitc
	template <class SeparatorType,class T
		, typename std::enable_if<HasclearMethod<T>::value>::type* = nullptr
	>
		void split(const SeparatorType &Separator, T &Result) const
	{
		Result.clear();
		split(Separator, [&](const string_view &s) {Result.emplace_back(s);}); 
	}

	//******************************************************************************
	// SPLIT Version for char replacing splits & splitsv for any separator
	//	template<class StringType=basic_string_view<CharT, Traits> >
	//	vector<StringType > split(const typename basic_string_view<CharT, Traits>::value_type Separator) const
	template<class StringType = basic_string_view<CharT, Traits>, class SeparatorType >
	vector<StringType > split(const SeparatorType &Separator) const
	{
		vector<StringType >  Result;
		split(Separator, [&](const basic_string_view<CharT, Traits> &s) {Result.emplace_back(s); });
		return Result;
	}

	// Convert a string
	basic_string<CharT, Traits> tolower() const
	{
		basic_string<CharT, Traits> res; 
		res.reserve(size()); // Allows reserve the space		
		std::transform(begin(), end(), std::back_inserter(res), ::tolower);
		return res;
	}

	void toupper_inplace()
	{
		std::transform(begin(), end(), begin(), ::toupper);
	}


};
typedef basic_string_view17<char, std::char_traits<char> > string_view17;
typedef basic_string_view17<wchar_t, std::char_traits<wchar_t> >  wstring_view17;

// Split by string as non-member function
// Unable to make this option works with char*
/*template <
	class CharT,
	class Traits = std::char_traits<CharT>,
	class StringType = basic_string_view<CharT, Traits> >
vector<StringType > string_split(const basic_string_view17<CharT, Traits> &InputStr, const basic_string_view<CharT, Traits> &Separator) 
{
	return InputStr.split(Separator);
}
*/
// Can works with char*
template <class StringType = string_view >
vector<StringType > string_split(const string_view17 &InputStr, const std::string_view &Separator)
{
	return InputStr.split(Separator);
}
