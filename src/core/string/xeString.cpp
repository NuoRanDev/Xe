#include "string/xeString.h"

#include <algorithm>

#include "log/xeLogOutput.h"
#include <devboost/xeMemCmp.h>

namespace xe
{
	constexpr utf8_t PREFIX[] = { 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
	constexpr unicode_t CODE_UP[] =
	{
		0x80,           // U+00000000 - U+0000007F  
		0x800,          // U+00000080 - U+000007FF  
		0x10000,        // U+00000800 - U+0000FFFF  
		0x200000,       // U+00010000 - U+001FFFFF  
		0x4000000,      // U+00200000 - U+03FFFFFF  
		0x80000000      // U+04000000 - U+7FFFFFFF  
	};

	inline bool compare_memory(const utf8_t* Lhs, const utf8_t* Rhs, int64_t Length) noexcept
	{
		if (Length == 0)
			return 0;
		return std::memcmp(Lhs, Rhs, Length) == 0;
	}

	static int utf8_byte_type(utf8_t c)  noexcept
	{
		if (c < 0x80)
			return 1;
		else if (c < 0xC0)
			return 0;
		else if (c >= 0xF5 || (c & 0xFE) == 0xC0)
		{
			// "octet values c0, c1, f5 to ff never appear"
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "octet values c0, c1, f5 to ff never appear");
			return -1;
		}
		else
		{
			int value = (((0xe5 << 24) >> ((unsigned)c >> 4 << 1)) & 3) + 1;
			// assert(value >= 2 && value <=4);
			return value;
		}
	}

	static bool utf8_bype_is_valid_leading_byte(int type) noexcept { return type > 0; }

	static bool utf8_byte_is_continuation(const utf8_t c) noexcept { return utf8_byte_type(c) == 0; }

	static int utf32_to_utf8(unicode_t utf32_str, utf8_t* utf8_str) noexcept
	{
		if (utf32_str == 0) return 0;
		int i, len = 6;
		for (i = 0; i < len; ++i)
			if (utf32_str < CODE_UP[i]) break;
		if (i == len)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "The utf32 string is invalid");
			return 0;
		}
		len = i + 1;
		while (i > 0)
		{
			utf8_str[i] = static_cast<utf8_t>((utf32_str & 0x3F) | 0x80);
			utf32_str = utf32_str >> 6;
			i--;
		}
		utf8_str[0] = static_cast<utf8_t>(utf32_str | PREFIX[len - 1]);
		return len;
	}

	static void utf8_to_utf32(utf8_t* src, unicode_t& des) noexcept
	{
		if (src == nullptr || (*src) == 0)
		{
			des = 0;
			return;
		}

		utf8_t b = src[0];

		if (b < 0x80)
		{
			des = b;
			return;
		}

		if (b < 0xC0 || b > 0xFD)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "The src is invalid");
			return;
		} // the src is invalid  

		size_t len;

		if (b < 0xE0)
		{
			des = b & 0x1F;
			len = 2;
		}
		else if (b < 0xF0)
		{
			des = b & 0x0F;
			len = 3;
		}
		else if (b < 0xF8)
		{
			des = b & 0x07;
			len = 4;
		}
		else if (b < 0xFC)
		{
			des = b & 0x03;
			len = 5;
		}
		else
		{
			des = b & 0x01;
			len = 6;
		}

		for (int i = 1; i < len; i++)
		{
			b = src[i];
			if (b < 0x80 || b > 0xBF)
			{
				XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "the src is invalid");
				return; // the src is invalid  
			}
			des = (des << 6) + (b & 0x3F);
		}
	}

	int64_t count_utf8(const utf8_t* utf8, int64_t alloc_size) noexcept
	{
		if (!utf8 && alloc_size)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Utf8 text not coherent");
			return 0;
		}
		int count = 0;
		const utf8_t* stop = utf8 + alloc_size;
		while (utf8 < stop)
		{
			int type = utf8_byte_type(*utf8);
			if (!utf8_bype_is_valid_leading_byte(type) || utf8 + type > stop)
			{
				XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Utf8 text not coherent");
				return 0;  // Sequence extends beyond end.
			}
			while ((type--) > 1) {
				++utf8;
				if (!utf8_byte_is_continuation(*utf8))
				{
					XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Utf8 text not coherent");
					return 0;
				}
			}
			++utf8;
			++count;
		}
		return count;
	}

	U8StringRef::U8StringRef(const char* c_utf8_str) noexcept
	{
		// inculde '/0'
		int64_t src_str_size = std::strlen((const char*)c_utf8_str);
		// c style string end with 0
		characters_data_size = src_str_size + 1;
		characters_data = xe_malloc<utf8_t>(characters_data_size);
		std::memcpy(characters_data, c_utf8_str, characters_data_size);

		is_short_string = (characters_data_size < SHORT_STRING_SIZE);
		characters_number = count_utf8(characters_data, src_str_size);
	}

	U8StringRef::U8StringRef(const utf8_t* str, int64_t str_size) noexcept :U8StringRef(str, str_size, count_utf8(str, str_size)) {}

	U8StringRef::U8StringRef(const utf8_t* str, int64_t str_size, int64_t input_character_number) noexcept
	{
		characters_data_size = str_size + 1;
		characters_data = xe_malloc<utf8_t>(characters_data_size);
		std::memcpy(characters_data, str, str_size);
		characters_data[str_size] = '\0';
		characters_number = input_character_number;
		is_short_string = (characters_data_size < SHORT_STRING_SIZE);
	}

	U8StringRef::U8StringRef(const U8StringRef& temp_string) noexcept
	{
		characters_number = temp_string.character_number();
		characters_data_size = temp_string.character_data_size();
		characters_data = xe_malloc<utf8_t>(characters_data_size);
		std::memcpy(characters_data, temp_string.data(), characters_data_size);
		is_short_string = (characters_data_size < SHORT_STRING_SIZE);
	}

	U8StringRef& U8StringRef::operator=(const U8StringRef& temp_string) noexcept
	{
		release();
		this->load_data(temp_string.data(), temp_string.character_data_size(), temp_string.character_number());
		return *this;
	}

	U8StringRef& U8StringRef::operator=(const char*& c_utf8_str) noexcept
	{
		release();
		this->load_data(c_utf8_str);
		return *this;
	}

	U8StringRef& U8StringRef::operator=(const utf8_t*& c_utf8_str) noexcept
	{
		release();
		this->load_data(c_utf8_str);
		return *this;
	}

	U8StringRef U8StringRef::Slice(int64_t start, int64_t end) const noexcept
	{
		int64_t _cur_number = 0;
		utf8_t* start_str_ptr = nullptr;
		utf8_t* cur_str_ptr = characters_data;
		int64_t need_copy_offset = 0;

		start = std::min(start, characters_number);
		end = std::clamp(end, start, characters_number);

		while (_cur_number != start)
		{
			int type = utf8_byte_type(*cur_str_ptr);
			while ((type--) > 1) {
				cur_str_ptr++;
				if (!utf8_bype_is_valid_leading_byte(*cur_str_ptr))
				{
					XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Utf8 text not coherent");
					return 0;
				}
			}
			cur_str_ptr++;
			_cur_number++;
		}
		start_str_ptr = cur_str_ptr;

		while (_cur_number != end)
		{
			int type = utf8_byte_type(*cur_str_ptr);
			while ((type--) > 1) {
				cur_str_ptr++;
				need_copy_offset++;
				if (!utf8_bype_is_valid_leading_byte(*cur_str_ptr))
				{
					XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Utf8 text not coherent");
					return 0;
				}
			}
			need_copy_offset++;
			cur_str_ptr++;
			_cur_number++;
		}
		return U8StringRef(start_str_ptr, need_copy_offset);
	}

	void U8StringRef::append(unicode_t character) noexcept
	{
		utf8_t output_utf8_str[4] = { 0 };
		int64_t jump_str_offset = characters_data_size - 1;
		int output_utf8_str_size = utf32_to_utf8(character, output_utf8_str);
		utf8_t* append_ptr_start = nullptr;

		// 
		characters_data_size = characters_data_size + output_utf8_str_size;

		// alloc
		characters_data = xe_realloc<utf8_t>(characters_data, characters_data_size);

		// end is '\0'
		characters_data[characters_data_size - 1] = 0;

		characters_number = characters_number + 1;

		append_ptr_start = characters_data + jump_str_offset;
		std::memcpy(append_ptr_start, output_utf8_str, output_utf8_str_size);
		is_short_string = (characters_data_size < SHORT_STRING_SIZE);
	}

	void U8StringRef::append(U8StringRef append_string) noexcept
	{
		int64_t append_string_data_size = append_string.character_data_size();
		int64_t append_string_characters_number = append_string.character_number();
		int64_t jump_str_offset = characters_data_size - 1;
		utf8_t* append_ptr_start = nullptr;

		characters_number = characters_number + append_string_characters_number;

		// exclude append string 's '\0'
		characters_data_size = characters_data_size + append_string_data_size - 1;

		// alloc
		characters_data = xe_realloc<utf8_t>(characters_data, characters_data_size);
		append_ptr_start = characters_data + jump_str_offset;
		//
		std::memcpy(append_ptr_start, append_string.characters_data, append_string_data_size);
		is_short_string = (characters_data_size < SHORT_STRING_SIZE);

	}

	std::vector<int64_t> U8StringRef::find_all(const unicode_t separator) noexcept
	{
		utf8_t utf8_separator[4] = { 0 };
		auto out_size = utf32_to_utf8(separator, utf8_separator);
		if (out_size == 0)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Not input unicode");
			return std::vector<int64_t>();
		}
		if (out_size == 1)return find_all(utf8_separator[0]);
		else
		{
			return find_all(utf8_separator, out_size);
		}
	}

	std::vector<int64_t> U8StringRef::find_all(const U8StringRef separator) noexcept
	{
		if (separator.length() == 0)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Found string is null");
			return std::vector<int64_t>();
		}
		if (separator.length() == 1)
			return find_all((separator.data())[0]);
		else
		{
			return find_all(separator.data(), separator.length());
		}
	}

	std::vector<int64_t> U8StringRef::find_all(const utf8_t separator) noexcept
	{
		return find_byte_all_memory_int64size(reinterpret_cast<const byte_t*>(characters_data), length(), separator);
	}

	std::vector<int64_t> U8StringRef::find_all(const utf8_t* separator, const int64_t size) noexcept
	{
		auto found_array = std::vector<int64_t>();
		const char* cur;
		while (true)
		{
			cur = std::strstr(c_str(), reinterpret_cast<const char*>(separator));
			if (cur == nullptr)
				break;
			auto size_offset = reinterpret_cast<size_t>(cur) - reinterpret_cast<size_t>(separator);
			found_array.push_back(static_cast<int>(size_offset));
		}
		return found_array;
	}

	std::vector<U8StringRef> U8StringRef::split(const unicode_t separator) noexcept
	{
		std::vector<U8StringRef> output;
		return output;
	}

	std::vector<U8StringRef> U8StringRef::split(U8StringRef separator) noexcept
	{
		return std::vector<U8StringRef>();
	}

	unicode_t U8StringRef::at(int64_t offset) noexcept
	{
		int64_t _cur_number = 0;
		utf8_t* cur_str_ptr = characters_data;
		unicode_t output_character;
		if (characters_number < 0 || characters_number>characters_data_size)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Out of string");
			return U'\0';
		}
		while (_cur_number != characters_number)
		{
			int type = utf8_byte_type(*cur_str_ptr);
			while ((type--) > 1) {
				cur_str_ptr++;
				if (!utf8_bype_is_valid_leading_byte(*cur_str_ptr))
				{
					XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Utf8 text not coherent");
					return U'\0';
				}
			}
			cur_str_ptr++;
			_cur_number++;
		}
		utf8_to_utf32(cur_str_ptr, output_character);
		return output_character;
	}

	bool U8StringRef::stringcmp(const utf8_t* cmp_str, const int64_t cmp_str_size) const noexcept
	{
		if (is_short_string)
		{
			return string_short_cmp(cmp_str, cmp_str_size);
		}
		else
		{
			return string_long_cmp(cmp_str, cmp_str_size);
		}
	}	

	void U8StringRef::load_data(const utf8_t* c_utf8_str, int64_t str_size, int64_t input_character_number) noexcept
	{
		characters_data_size = str_size + 1;
		characters_data = xe_malloc<utf8_t>(characters_data_size);
		std::memcpy(characters_data, c_utf8_str, str_size);
		characters_data[str_size] = '\0';
		characters_number = input_character_number;
		is_short_string = (characters_data_size < SHORT_STRING_SIZE);
	}

	void U8StringRef::load_data(const char* c_utf8_str) noexcept
	{
		load_data(reinterpret_cast<const utf8_t*>(c_utf8_str));
	}

	void U8StringRef::load_data(const utf8_t* c_utf8_str) noexcept
	{
		// inculde '/0'
		int64_t src_str_size = std::strlen(reinterpret_cast<const char*>(c_utf8_str));
		// c style string end with 0
		load_data(c_utf8_str, src_str_size, count_utf8(characters_data, src_str_size));
	}

	bool U8StringRef::string_long_cmp(const utf8_t* cmp_str, const int64_t cmp_str_size) const noexcept
	{
		if (cmp_str_size != characters_data_size - 1)
			return false;
		return long_memory_cmp(reinterpret_cast<byte_t*>(characters_data), reinterpret_cast<const byte_t*>(cmp_str), cmp_str_size);
	}

	bool U8StringRef::string_short_cmp(const utf8_t* cmp_str, const int64_t cmp_str_size) const noexcept
	{
		if (cmp_str_size != characters_data_size - 1)
			return false;
		return short_memory_cmp(reinterpret_cast<byte_t*>(characters_data), reinterpret_cast<const byte_t*>(cmp_str), cmp_str_size);
	}
} // namespace xe is end