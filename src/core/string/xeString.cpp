#include "string/xeString.hpp"

#include <algorithm>

#include "log/xeLogOutput.hpp"
#include "memory/xeAlloc.hpp"
#include "devboost/xeMemCmp.hpp"
#include "devboost/xeMemFind.hpp"


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

	inline bool compare_memory(const utf8_t* lhs, const utf8_t* rhs, int64_t length) noexcept
	{
		if (length == 0)
			return 0;
		return std::memcmp(lhs, rhs, length) == 0;
	}

	static int utf8_byte_type(utf8_t c) noexcept
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

	static void utf8_to_utf32(const utf8_t* src, unicode_t& des) noexcept
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

	static bool utf8_to_utf16le(const utf8_t* u8_src, int64_t utf8_str_size, utf16le_t** u16le_dst, int64_t& utf16_str_size) noexcept
	{
		int64_t len = utf8_str_size;
		bool is_ok = true;
		dynamic_array<utf16le_t> u16str;
		uint32_t c1;
		uint32_t c2;
		uint32_t c3;
		uint32_t c4;
		uint32_t code_point;
		u16str.reserve(utf8_str_size);

		// is having BOM
		if (len > 3 && u8_src[0] == 0xEF && u8_src[1] == 0xBB && u8_src[2] == 0xBF)
		{
			u8_src += 3;
			len -= 3;
		}

		for (int64_t i = 0; i < len; ++i)
		{
			c1 = u8_src[i];	// Get UTF8
			if ((c1 & 0x80) == 0)
			{
				u16str.push_back(static_cast<utf16le_t>(c1));
				continue;
			}
			switch (c1 & 0xF0)
			{
			case 0xF0: // 4 Byte, 0x10000 TO 0x10FFFF
				c2 = u8_src[++i];
				c3 = u8_src[++i];
				c4 = u8_src[++i];
				code_point = ((c1 & 0x07U) << 18) | ((c2 & 0x3FU) << 12) | ((c3 & 0x3FU) << 6) | (c4 & 0x3FU);
				if (code_point >= 0x10000)
				{
					code_point -= 0x10000;
					u16str.push_back(static_cast<utf16le_t>((code_point >> 10) | 0xD800U));
					u16str.push_back(static_cast<utf16le_t>((code_point & 0x03FFU) | 0xDC00U));
				}
				else
				{
					u16str.push_back(static_cast<utf16le_t>(code_point));
				}
				break;

			case 0xE0:
				c2 = u8_src[++i];
				c3 = u8_src[++i];
				code_point = ((c1 & 0x0FU) << 12) | ((c2 & 0x3FU) << 6) | (c3 & 0x3FU);
				u16str.push_back(static_cast<utf16le_t>(code_point));
				break;

			case 0xD0:
			case 0xC0:
				c2 = u8_src[++i];
				code_point = ((c1 & 0x1FU) << 12) | ((c2 & 0x3FU) << 6);
				u16str.push_back(static_cast<utf16le_t>(code_point));
				break;
			default:
				is_ok = false;
				break;
			}
		}
		if(is_ok)
		{
			*u16le_dst = xe_malloc<utf16le_t>(u16str.size() + 1);
			std::memcpy(*u16le_dst, u16str.data(), u16str.size() * sizeof(utf16le_t));
			u16le_dst[u16str.size()] = 0;
		}
		return is_ok;
	}

	static dynamic_array<int64_t> compute_prefix(const utf8_t* str, const int64_t start_size, const int64_t end_size, const int64_t segment_size) noexcept
	{
		dynamic_array<int64_t> pat = dynamic_array<int64_t>((uint64_t)(end_size - start_size), 0);
		int64_t index_char = 0;
		for (int64_t i = start_size; i < end_size; i += segment_size)
		{
			while (index_char > 0 && str[i] != str[index_char])
				index_char = pat[index_char - 1];
			if (str[i] == str[index_char])
				index_char++;
			pat[i] = index_char;
		}
		return pat;
	}

	static dynamic_array<int64_t> u8kmp_find_start_all(const utf8_t* main_str, const int64_t main_str_size, const utf8_t* children_str, const int64_t children_str_size) noexcept
	{
		dynamic_array<int64_t> matches;
		dynamic_array<int64_t> next = compute_prefix(children_str, 0, children_str_size, 1ll);

		
		uint64_t mark_index = 0;
		for (int64_t i = 0; i < main_str_size; i++) 
		{
			while (mark_index > 0 && main_str[i] != children_str[mark_index])
				mark_index = next[mark_index - 1];
			if (main_str[i] == children_str[mark_index])
				mark_index++;
			if (mark_index == children_str_size)
			{
				matches.push_back(i - children_str_size + 1);
				mark_index = next[mark_index - 1];
			}
		}
		return matches;
	}

	static dynamic_array<int64_t> u8_find_simd_start_all(const utf8_t* main_str, const int64_t main_str_size, const utf8_t* children_str, const int64_t children_str_size) noexcept
	{
		dynamic_array<int64_t> matches;
		int64_t cur = 0;
		do
		{
			cur = memfind(reinterpret_cast<const byte_t*>(children_str), reinterpret_cast<const byte_t*>(main_str + cur), children_str_size, main_str_size);
			if (cur == -1)
				return matches;
			matches.push_back(cur);
			cur++;
		} while (cur < (main_str_size - children_str_size));
		return matches;
	}

	static int64_t count_utf8(const utf8_t* utf8, int64_t alloc_size) noexcept
	{
		int count = 0;
		if ((utf8 == nullptr) || (alloc_size == 0) || (utf8[0] == '\0'))
		{
			return count;
		}
		
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

	U8StringRef U8StringRef::slice(int64_t start, int64_t end) const noexcept
	{
		int64_t _cur_number = 0;
		utf8_t* start_str_ptr = nullptr;
		utf8_t* cur_str_ptr = characters_data;
		int64_t need_copy_offset = 0;

		start = std::min(start, characters_number);
		if (end != -1)
			end = std::clamp(end, start, characters_number);
		else
		{
			end = characters_number;
		}

		while (_cur_number != start)
		{
			int type = utf8_byte_type(*cur_str_ptr);
			while ((type--) > 1) 
			{
				cur_str_ptr++;
				if (!utf8_bype_is_valid_leading_byte(*cur_str_ptr))
				{
					XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Utf8 text not coherent");
					return "";
				}
			}
			cur_str_ptr++;
			_cur_number++;
		}
		start_str_ptr = cur_str_ptr;

		while (_cur_number != end)
		{
			int type = utf8_byte_type(*cur_str_ptr);
			while ((type--) > 1) 
			{
				cur_str_ptr++;
				need_copy_offset++;
				if (!utf8_bype_is_valid_leading_byte(*cur_str_ptr))
				{
					XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Utf8 text not coherent");
					return "";
				}
			}
			need_copy_offset++;
			cur_str_ptr++;
			_cur_number++;
		}
		return U8StringRef(start_str_ptr, need_copy_offset + 1, end - start);
	}

	bool U8StringRef::is_empty() const noexcept
	{
		return characters_number == 0;
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
		int64_t append_string_data_size = append_string.get_characters_data_size();
		int64_t append_string_characters_number = append_string.get_characters_number();
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

	dynamic_array<int64_t> U8StringRef::find_all(const unicode_t pattern) const noexcept
	{
		utf8_t utf8_separator[4] = { 0 };
		auto out_size = utf32_to_utf8(pattern, utf8_separator);
		if (out_size == 0)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Not input unicode");
			return dynamic_array<int64_t>();
		}
		if (out_size == 1)return find_all(utf8_separator[0]);
		else
		{
			return find_all(utf8_separator, out_size);
		}
	}

	dynamic_array<int64_t> U8StringRef::find_all(const U8StringRef pattern) const noexcept
	{
		if (pattern.length() == 0)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Found string is null");
			return dynamic_array<int64_t>();
		}
		if (pattern.length() == 1)
			return find_all((pattern.data())[0]);
		else
		{
			return find_all(pattern.data(), pattern.length());
		}
	}

	dynamic_array<int64_t> U8StringRef::find_all(const utf8_t pattern) const noexcept
	{
		return find_byte_all_memory_int64size(reinterpret_cast<const byte_t*>(characters_data), length(), pattern);
	}

	dynamic_array<int64_t> U8StringRef::find_all(const utf8_t* pattern, const int64_t size) const noexcept
	{
		if (size < 32 || (characters_data_size < USE_KMP_STRING_SIZE))
			u8kmp_find_start_all(characters_data, characters_data_size, pattern, size);
		return u8_find_simd_start_all(characters_data, characters_data_size, pattern, size);
	}

	dynamic_array<int64_t> U8StringRef::find_kmp_all(const utf8_t* pattern, const int64_t size) const noexcept
	{
		return u8kmp_find_start_all(characters_data, characters_data_size, pattern, size);
	}

	dynamic_array<int64_t> U8StringRef::find_simd_all(const utf8_t* pattern, const int64_t size) const noexcept
	{
		return u8_find_simd_start_all(characters_data, characters_data_size, pattern, size);
	}

	int64_t U8StringRef::find_start(const utf8_t* pattern, int64_t pattern_size) const noexcept
	{
		if (pattern_size > characters_data_size)
			return -1;
		return memfind(reinterpret_cast<const byte_t*>(pattern), reinterpret_cast<const byte_t*>(characters_data), pattern_size - 1, characters_data_size);
	}

	int64_t U8StringRef::find_last(const utf8_t* pattern, int64_t pattern_size) const noexcept
	{
		return -1;
	}

	dynamic_array<U8StringRef> U8StringRef::split(const unicode_t separator) noexcept
	{
		dynamic_array<U8StringRef> output;
		return output;
	}

	dynamic_array<U8StringRef> U8StringRef::split(U8StringRef separator) noexcept
	{
		return dynamic_array<U8StringRef>();
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

	void U8StringRef::release() noexcept
	{
		xe_free(characters_data);
		characters_data = nullptr;
		characters_data_size = 0;
		characters_number = 0;
	}

	void U8StringRef::load_xe_str_include0(const utf8_t* xe_utf8_str, int64_t xe_str_size, int64_t input_character_number) noexcept
	{
		characters_data_size = xe_str_size;
		characters_data = xe_malloc<utf8_t>(characters_data_size);
		std::memcpy(characters_data, xe_utf8_str, xe_str_size - 1);
		characters_data[xe_str_size - 1] = '\0';
		characters_number = input_character_number;
		is_short_string = (characters_data_size < SHORT_STRING_SIZE);
	}

	void U8StringRef::load_cpp_u8_str_add0(const utf8_t* cpp_utf8_str) noexcept
	{
		if (cpp_utf8_str == nullptr)
		{
			load_default_str();
			return;
		}
		// include '/0'
		int64_t src_str_size = std::strlen(reinterpret_cast<const char*>(cpp_utf8_str));
		// c style string end with 0
		load_rust_str_add0(cpp_utf8_str, src_str_size);
	}

	void U8StringRef::load_rust_str_add0(const utf8_t* xe_rust_str, int64_t rust_str_size)
	{
		int64_t found_character_number = count_utf8(xe_rust_str, rust_str_size);
		if (found_character_number == 0)
		{
			load_default_str();
			return;
		}
		load_xe_str_include0(xe_rust_str, rust_str_size + 1, found_character_number);
	}

	void U8StringRef::load_default_str() noexcept
	{
		characters_data = nullptr;
		characters_data_size = 0;
		characters_number = 0;
		is_short_string = true;
	}

	bool U8StringRef::string_long_cmp(const utf8_t* cmp_str, const int64_t cmp_str_size) const noexcept
	{
		if (cmp_str_size != characters_data_size)
			return false;
		return long_memory_cmp(reinterpret_cast<byte_t*>(characters_data), reinterpret_cast<const byte_t*>(cmp_str), cmp_str_size);
	}

	bool U8StringRef::string_short_cmp(const utf8_t* cmp_str, const int64_t cmp_str_size) const noexcept
	{
		if (cmp_str_size != characters_data_size)
			return false;
		return short_memory_cmp(reinterpret_cast<byte_t*>(characters_data), reinterpret_cast<const byte_t*>(cmp_str), cmp_str_size);
	}

	void U8StringRef::integral_to_string_ptr(int64_t number, utf8_t* str, int64_t& str_size) noexcept
	{
		str_size = 0;
		while(number)
		{
			str[str_size++] = static_cast<utf8_t>((number % 10) + '0');
			number = number / 10;
		}
	}

	bool U16StringRef::load_utf8(const U8StringRef& u8_str) noexcept
	{
		bool state = utf8_to_utf16le(u8_str.data(), u8_str.get_characters_data_size(), &str_data, size);
		if (!state)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeCore", "Utf8 to utf16 failed");
			str_data = nullptr;
			size = 0;
		}
		return state;
	}

	void U16StringRef::release() noexcept
	{
		xe_free(str_data);
	}
} // namespace xe is end