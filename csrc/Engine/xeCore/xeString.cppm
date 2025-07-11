export module xe.Core.xeString;


import std;

import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;
import xe.Core.CoreClrOutput;

namespace xe
{
	export constexpr xeInt64 SHORT_STRING_SIZE = 12;
	export class U8StringRef
	{
	public:
		// Init
		U8StringRef() = default;

		U8StringRef(const char* c_utf8_str);

		U8StringRef(const xeU8cstr* str, xeInt64 str_size);

		U8StringRef(const xeU8cstr* str, xeInt64 str_size, xeInt64 input_character_number);

		U8StringRef(const U8StringRef& temp_string);

		U8StringRef& operator=(const U8StringRef& temp_string);

		U8StringRef& operator=(const char* c_utf8_str);

		U8StringRef& operator=(const xeU8cstr* c_utf8_str);

		U8StringRef Slice(xeInt64 start, xeInt64 end) const;

		[[nodiscard]] xeInt64 CharacterNumber() const { return characters_number; }

		[[nodiscard]] xeInt64 CharacterDataSize() const { return characters_data_size; }

		void Append(xeUnicode character);

		void Append(U8StringRef append_string);

		// Find all

		[[nodiscard]] xeDynamicArray<xeInt64> FindAll(const xeUnicode separator);

		[[nodiscard]] xeDynamicArray<xeInt64> FindAll(const U8StringRef separator);

		[[nodiscard]] xeDynamicArray<xeInt64> FindAll(const xeU8cstr separator);

		[[nodiscard]] xeDynamicArray<xeInt64> FindAll(const xeU8cstr* separator, const xeInt64 size);

		// Split string by separator

		[[nodiscard]] xeDynamicArray<U8StringRef> Split(xeUnicode separator);

		[[nodiscard]] xeDynamicArray<U8StringRef> Split(U8StringRef separator);

		[[nodiscard]] xeUnicode At(xeInt64 offset);

		// U8StringRef Comparison Operators
		[[nodiscard]] xeUnicode operator[](xeInt64 offset) { return this->At(offset); }

		//
		[[nodiscard]] const char* c_str() const { return (const char*)characters_data; }

		// exclude end of '\0'
		[[nodiscard]] const xeInt64 length() const { return CharacterDataSize() - 1; }

		[[nodiscard]] const xeU8cstr* data()const { return characters_data; }

		[[nodiscard]] bool StringCmp(const U8StringRef cmp_str)
		{
			return StringCmp(cmp_str.data(), cmp_str.length());
		}

		[[nodiscard]] bool StringCmp(const char* cmp_str)
		{
			return StringCmp(reinterpret_cast<const xeU8cstr*>(cmp_str), std::strlen(cmp_str));
		}

		[[nodiscard]] bool StringCmp(const xeU8cstr* cmp_str)
		{
			return StringCmp(reinterpret_cast<const xeU8cstr*>(cmp_str), std::strlen(reinterpret_cast<const char*>(cmp_str)));
		}

		[[nodiscard]] bool StringCmp(const xeU8cstr* cmp_str, xeInt64 cmp_str_size);

		void Release()
		{
			xeFree(characters_data);
			characters_data_size = 0;
			characters_number = 0;
		}

		~U8StringRef() { Release(); }
		
	private:
		// data ptr
		xeU8cstr* characters_data;
		// string size
		xeInt64 characters_number;
		// alloc size
		xeInt64 characters_data_size;
		//
		void LoadData(const xeU8cstr* c_utf8_str, xeInt64 str_size, xeInt64 input_character_number);
		//
		void LoadData(const char* c_utf8_str);
		//
		void LoadData(const xeU8cstr* c_utf8_str);

		bool StringLongCmp(const U8StringRef cmp_str)
		{
			return StringLongCmp(cmp_str.data(), cmp_str.length());
		}

		bool StringLongCmp(const char* cmp_str)
		{
			return StringLongCmp(reinterpret_cast<const xeU8cstr*>(cmp_str), std::strlen(cmp_str));
		}

		bool StringLongCmp(const xeU8cstr* cmp_str)
		{
			return StringLongCmp(reinterpret_cast<const xeU8cstr*>(cmp_str), std::strlen(reinterpret_cast<const char*>(cmp_str)));
		}

		bool StringLongCmp(const xeU8cstr* cmp_str, xeInt64 cmp_str_size);

		bool StringShortCmp(const U8StringRef cmp_str)
		{
			return StringShortCmp(cmp_str.data(), cmp_str.length());
		}

		bool StringShortCmp(const char* cmp_str)
		{
			return StringShortCmp(reinterpret_cast<const xeU8cstr*>(cmp_str), std::strlen(cmp_str));
		}

		bool StringShortCmp(const xeU8cstr* cmp_str)
		{
			return StringShortCmp(reinterpret_cast<const xeU8cstr*>(cmp_str), std::strlen(reinterpret_cast<const char*>(cmp_str)));
		}

		bool StringShortCmp(const xeU8cstr* cmp_str, xeInt64 cmp_str_size);
		// 
		bool is_short_string;
	};

	export [[nodiscard]] inline U8StringRef& operator+(U8StringRef& buffer, U8StringRef string)
	{
		buffer.Append(string);
		return buffer;
	}

	export [[nodiscard]] inline U8StringRef& operator+(U8StringRef& buffer, const char* string)
	{
		buffer.Append(U8StringRef(string));
		return buffer;
	}

	// std::cout << <U8StringRef>
	export [[nodiscard]] inline std::ostream& operator<<(std::ostream& out, const U8StringRef& string)
	{
		out << string.c_str();
		return out;
	}

	export using xeString = U8StringRef;
}
