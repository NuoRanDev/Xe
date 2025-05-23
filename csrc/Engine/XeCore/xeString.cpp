﻿import std;

import xe.Core.xeString;

import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;
import xe.Core.CoreClrOutput;

namespace xe
{
	constexpr xeU8cstr PREFIX[] = { 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
	constexpr xeUint32 CODE_UP[] =
	{
		0x80,           // U+00000000 - U+0000007F  
		0x800,          // U+00000080 - U+000007FF  
		0x10000,        // U+00000800 - U+0000FFFF  
		0x200000,       // U+00010000 - U+001FFFFF  
		0x4000000,      // U+00200000 - U+03FFFFFF  
		0x80000000      // U+04000000 - U+7FFFFFFF  
	};

	inline bool CompareMemory(const xeU8cstr* Lhs, const xeU8cstr* Rhs, xeInt64 Length)
	{
		if (Length == 0)
			return 0;
		return std::memcmp(Lhs, Rhs, Length) == 0;
	}

	int Utf8ByteType(xeByte c)
	{
		if (c < 0x80)
			return 1;
		else if (c < 0xC0)
			return 0;
		else if (c >= 0xF5 || (c & 0xFE) == 0xC0)
		{
			// "octet values c0, c1, f5 to ff never appear"
			XE_WARNING_OUTPUT("<FUNCTION: XESTRING> octet values c0, c1, f5 to ff never appear");
			return -1;
		}
		else
		{
			int value = (((0xe5 << 24) >> ((unsigned)c >> 4 << 1)) & 3) + 1;
			// assert(value >= 2 && value <=4);
			return value;
		}
	}

	bool Utf8BypeIsValidLeadingByte(int type) { return type > 0; }

	bool Utf8ByteIsContinuation(const xeU8cstr c) { return Utf8ByteType(c) == 0; }

	int Utf32ToUtf8(char32_t utf32_str, xeU8cstr* utf8_str)
	{
		if (utf32_str == 0) return 0;
		int i, len = 6;
		for (i = 0; i < len; ++i)
			if (utf32_str < CODE_UP[i]) break;
		if (i == len)
		{
			XE_WARNING_OUTPUT("<FUNCTION: XESTRING> The utf32 string is invalid");
			return 0;
		}
		len = i + 1;
		while (i > 0)
		{
			utf8_str[i] = static_cast<xeU8cstr>((utf32_str & 0x3F) | 0x80);
			utf32_str = utf32_str >> 6;
			i--;
		}
		utf8_str[0] = static_cast<xeU8cstr>(utf32_str | PREFIX[len - 1]);
		return len;
	}

	void Utf8ToUtf32(xeU8cstr* src, char32_t& des)
	{
		if (src == nullptr || (*src) == 0)
		{
			des = 0;
			return;
		}

		xeU8cstr b = src[0];

		if (b < 0x80)
		{
			des = b;
			return;
		}

		if (b < 0xC0 || b > 0xFD)
		{
			XE_ERROR_OUTPUT("<FUNCTION: XESTRING> The src is invalid");
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
				XE_ERROR_OUTPUT("<FUNCTION: XESTRING> the src is invalid");
				return; // the src is invalid  
			}
			des = (des << 6) + (b & 0x3F);
		}
	}

	xeInt64 CountUTF8(const xeU8cstr* utf8, xeInt64 alloc_size)
	{
		if (!utf8 && alloc_size)
		{
			XE_WARNING_OUTPUT("<FUNCTION: XESTRING> Utf8 text not coherent");
			return 0;
		}
		int count = 0;
		const xeU8cstr* stop = utf8 + alloc_size;
		while (utf8 < stop)
		{
			int type = Utf8ByteType(*utf8);
			if (!Utf8BypeIsValidLeadingByte(type) || utf8 + type > stop)
			{
				XE_WARNING_OUTPUT("<FUNCTION: XESTRING> Utf8 text not coherent");
				return 0;  // Sequence extends beyond end.
			}
			while ((type--) > 1) {
				++utf8;
				if (!Utf8ByteIsContinuation(*utf8))
				{
					XE_WARNING_OUTPUT("<FUNCTION: XESTRING> Utf8 text not coherent");
					return 0;
				}
			}
			++utf8;
			++count;
		}
		return count;
	}

	U8StringRef::U8StringRef(const char* c_utf8_str)
	{
		// inculde '/0'
		int64_t src_str_size = std::strlen((const char*)c_utf8_str);
		// c style string end with 0
		characters_data_size = src_str_size + 1;
		characters_data = xeMalloc<xeU8cstr>(characters_data_size);
		std::memcpy(characters_data, c_utf8_str, characters_data_size);

		characters_number = CountUTF8(characters_data, src_str_size);
	}

	U8StringRef::U8StringRef(const xeU8cstr* str, xeInt64 str_size) :U8StringRef(str, str_size, CountUTF8(str, str_size)) {}

	U8StringRef::U8StringRef(const xeU8cstr* str, xeInt64 str_size, xeInt64 input_character_number)
	{
		characters_data_size = str_size + 1;
		characters_data = xeMalloc<xeU8cstr>(characters_data_size);
		std::memcpy(characters_data, str, str_size);
		characters_data[str_size] = '\0';
		characters_number = input_character_number;
	}

	U8StringRef::U8StringRef(const U8StringRef& temp_string)
	{
		characters_number = temp_string.CharacterNumber();
		characters_data_size = temp_string.CharacterDataSize();
		characters_data = xeMalloc<xeU8cstr>(characters_data_size);
		std::memcpy(characters_data, temp_string.data(), characters_data_size);
	}

	U8StringRef& U8StringRef::operator=(const U8StringRef& temp_string)
	{
		Release();
		this->LoadData(temp_string.data(), temp_string.CharacterDataSize(), temp_string.CharacterNumber());
		return *this;
	}

	U8StringRef& U8StringRef::operator=(const char* c_utf8_str)
	{
		Release();
		this->LoadData(c_utf8_str);
		return *this;
	}

	U8StringRef& U8StringRef::operator=(const xeU8cstr* c_utf8_str)
	{
		Release();
		this->LoadData(c_utf8_str);
		return *this;
	}

	U8StringRef U8StringRef::Slice(xeInt64 start, xeInt64 end) const
	{
		int64_t _cur_number = 0;
		xeU8cstr* start_str_ptr = nullptr;
		xeU8cstr* cur_str_ptr = characters_data;
		int64_t need_copy_offset = 0;

		start = std::min(start, characters_number);
		end = std::clamp(end, start, characters_number);

		while (_cur_number != start)
		{
			int type = Utf8ByteType(*cur_str_ptr);
			while ((type--) > 1) {
				cur_str_ptr++;
				if (!Utf8ByteIsContinuation(*cur_str_ptr))
				{
					XE_WARNING_OUTPUT("<FUNCTION: XESTRING> Utf8 text not coherent");
					return 0;
				}
			}
			cur_str_ptr++;
			_cur_number++;
		}
		start_str_ptr = cur_str_ptr;

		while (_cur_number != end)
		{
			int type = Utf8ByteType(*cur_str_ptr);
			while ((type--) > 1) {
				cur_str_ptr++;
				need_copy_offset++;
				if (!Utf8ByteIsContinuation(*cur_str_ptr))
				{
					XE_WARNING_OUTPUT("<FUNCTION: XESTRING> Utf8 text not coherent");
					return 0;
				}
			}
			need_copy_offset++;
			cur_str_ptr++;
			_cur_number++;
		}
		return U8StringRef(start_str_ptr, need_copy_offset);
	}

	void U8StringRef::Append(xeUnicode character)
	{
		xeU8cstr output_utf8_str[4] = { 0 };
		int64_t jump_str_offset = characters_data_size - 1;
		int output_utf8_str_size = Utf32ToUtf8(character, output_utf8_str);
		xeU8cstr* append_ptr_start = nullptr;

		// 
		characters_data_size = characters_data_size + output_utf8_str_size;

		// alloc
		characters_data = xeRealloc<xeU8cstr>(characters_data, characters_data_size);

		// end is '\0'
		characters_data[characters_data_size - 1] = 0;

		characters_number = characters_number + 1;

		append_ptr_start = characters_data + jump_str_offset;
		std::memcpy(append_ptr_start, output_utf8_str, output_utf8_str_size);
	}

	void U8StringRef::Append(U8StringRef append_string)
	{
		int64_t append_string_data_size = append_string.CharacterDataSize();
		int64_t append_string_characters_number = append_string.CharacterNumber();
		int64_t jump_str_offset = characters_data_size - 1;
		xeU8cstr* append_ptr_start = nullptr;

		characters_number = characters_number + append_string_characters_number;

		// exclude append string 's '\0'
		characters_data_size = characters_data_size + append_string_data_size - 1;

		// alloc
		characters_data = xeRealloc<xeU8cstr>(characters_data, characters_data_size);
		append_ptr_start = characters_data + jump_str_offset;
		//
		std::memcpy(append_ptr_start, append_string.characters_data, append_string_data_size);

	}

	xeUnicode U8StringRef::At(xeInt64 offset)
	{
		int64_t _cur_number = 0;
		xeU8cstr* cur_str_ptr = characters_data;
		char32_t output_character;
		if (characters_number < 0 || characters_number>characters_data_size)
		{
			XE_WARNING_OUTPUT("<FUNCTION: XESTRING> Out of string");
			return U'\0';
		}
		while (_cur_number != characters_number)
		{
			int type = Utf8ByteType(*cur_str_ptr);
			while ((type--) > 1) {
				cur_str_ptr++;
				if (!Utf8ByteIsContinuation(*cur_str_ptr))
				{
					XE_WARNING_OUTPUT("<FUNCTION: XESTRING> Utf8 text not coherent");
					return U'\0';
				}
			}
			cur_str_ptr++;
			_cur_number++;
		}
		Utf8ToUtf32(cur_str_ptr, output_character);
		return output_character;
	}

	void U8StringRef::LoadData(const xeU8cstr* c_utf8_str, xeInt64 str_size, xeInt64 input_character_number)
	{
		characters_data_size = str_size + 1;
		characters_data = xeMalloc<xeU8cstr>(characters_data_size);
		std::memcpy(characters_data, c_utf8_str, str_size);
		characters_data[str_size] = '\0';
		characters_number = input_character_number;
	}

	void U8StringRef::LoadData(const char* c_utf8_str)
	{
		LoadData(reinterpret_cast<const xeU8cstr*>(c_utf8_str));
	}

	void U8StringRef::LoadData(const xeU8cstr* c_utf8_str)
	{
		// inculde '/0'
		int64_t src_str_size = std::strlen(reinterpret_cast<const char*>(c_utf8_str));
		// c style string end with 0
		LoadData(c_utf8_str, src_str_size, CountUTF8(characters_data, src_str_size));
	}
}