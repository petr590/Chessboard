#ifndef CHESSBOARD_UTIL_CPP
#define CHESSBOARD_UTIL_CPP

#include <cstring>

namespace chessboard {

	template<typename T, size_t N>
	static constexpr size_t size(T(&)[N]) {
		return N;
	}

	static char* repeat_char(char ch, unsigned int times) {
		char* str = new char[times + 1];

		for(unsigned int i = 0; i < times; ++i)
			str[i] = ch;

		str[times] = '\0';

		return str;
	}

	static char* repeat_str(const char* str, unsigned int times) {
		size_t str_length = strlen(str),
			length = str_length * times;
		char* result = new char[length + 1];

		for(const char* end = result + length; result < end; ) {
			for(size_t i = 0; i < str_length; ++i)
				*(result++) = str[i];
		}

		result[0] = '\0';

		return result - length;
	}
}

#endif
