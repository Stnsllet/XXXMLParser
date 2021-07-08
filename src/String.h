#pragma once
#include <string>
#include <vector>
#include <format>
#include <stdarg.h>
#include <sstream>
#include <ranges>

class String : public std::string
{
	typedef std::vector<String> StringVector;
public:
	String() {};
	String(const char* src) { *this = src; };
	String(std::string src) { assign(src); };
	String& operator=(const char* newstr) {
		if (newstr == NULL) erase();
		else assign(newstr);
		return *this;
	}
	operator const char* () const { return c_str(); }
	operator const int() const { return (empty()) ? (0) : (std::stoi(*this)); }
	operator const long() const { return (empty()) ? (0) : (std::stol(*this)); }
	operator const float() const { return (empty()) ? (0) : ((float)std::atof(*this)); }
	operator const unsigned int() const { 
		auto trans = [](unsigned char c) {
			if ('0' <= c && '9' >= c) return (c - 0x30);//0x30は'0'の文字コード
			if ('A' <= c && 'F' >= c) return (c + 0x0A - 0x41);//0x41は'A'の文字コード
			if ('a' <= c && 'f' >= c) return (c + 0x0A - 0x61);//0x61は'a'の文字コード
			return 0;
		};
		if (!starts_with("0x")) return 0;
		unsigned int i, j = 0;
		const char* str = *this;
		const char* str_ptr = str + strlen(str) - 1;
		for (i = 0; i < strlen(str)-2; i++) {
			j += trans(*str_ptr--) * (unsigned int)pow(16, i);
		}
		return j;
	}
	template <class... Args>
	String& format(const char* message, Args... args) {
		assign(std::format(message, args...));
		return *this;
	}
	bool operator==(const String& diff) const { return length() == diff.length(); }
	bool operator==(const char* diff) const { return (diff == NULL) ? (empty()) : (0 == compare(diff)); }

	bool equal(const String& diff) const { return std::char_traits<char>::compare(*this, diff, length()) == 0; }
	bool lengthLow(const char* diff) const { return (diff == NULL) ? (false) : (0 > compare(diff)); }
	bool lengthHigh(const char* diff) const { return (diff == NULL) ? (!empty()) : (0 < compare(diff)); }

	StringVector split(char sep) const{
		StringVector ret;
		for (const auto& word : *this | std::views::split(std::views::single(sep)) | std::views::transform([](auto v)
			{
				auto cv = v | std::views::common;
				return std::string{ cv.begin(), cv.end() };
			})) {
			ret.push_back(String(word));
		}
		return ret;
	}
	String& join(StringVector str_list, const char* sep) {
		erase();
		for (auto& str : str_list) {
			if (!empty())*this += sep;
			*this += str;
		}
		return *this;
	}
};

typedef std::vector<String> StringVector;