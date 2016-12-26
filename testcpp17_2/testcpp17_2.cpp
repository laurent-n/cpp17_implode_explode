#include <iostream>
#include <vector>
#include <string>
#include <string_view>


using namespace std;


template <class string_type>
class string_tokenizer {
public:
	string_tokenizer(string_type &_src, typename string_type::value_type _separator) :src(_src), separator(_separator)
	{
	}
	class iterator {
	public:
		iterator(string_tokenizer &_tok) :tok(_tok)
		{
			it = tok.src.begin();
			calc_next_it();
		}
		iterator& operator ++()
		{
			it = next_it;
			if (it != tok.src.end()) {
				it++;
				calc_next_it();
			}
			return *this;
		}
		string_type operator *()
		{
			//return string_type(it,next_it); // This one may not work with string_view as there's no constructor taking 2 string_view iterator.
			return tok.src.substr(it - tok.src.begin(), next_it - it);
		}
		bool operator !=(const iterator &other) const
		{
			return (it != other.it);
		}
	private:
		friend string_tokenizer;
		string_tokenizer &tok;
		typename string_type::iterator it;
		typename string_type::iterator next_it;
		void calc_next_it()
		{
			for (next_it = it; next_it != tok.src.end(); ++next_it)
				if (*next_it == tok.separator)
					break;
		}
	};

	typename string_tokenizer::iterator begin()
	{
		return string_tokenizer::iterator(*this);
	}
	typename string_tokenizer::iterator end()
	{
		string_tokenizer::iterator tmp(*this);
		tmp.it = tmp.next_it = src.end();
		return tmp;
	}
private:
	string_type &src;
	typename string_type::value_type separator;
};


int main()
{
	string str = "C++_is_fun";
	string_tokenizer<string> st(str, '_');
	for (auto it = st.begin(); it != st.end(); ++it)
		cout << *it << endl;

	string_view strsv = "C++17_is_fun_with_stringview";
	string_tokenizer<string_view> stsv(strsv, '_');
	for (auto it = stsv.begin(); it != stsv.end(); ++it)
		cout << *it << endl;

	cout << "Tests End" << endl;
	return 0;
}



