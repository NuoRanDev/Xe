#ifndef _INC_XE_STRING_HPP_
#define _INC_XE_STRING_HPP_

#include "xeAlloc.hpp"
#include "xeCoreClrOutput.hpp"
#include "xeCoreConfig.hpp"

namespace xe
{
	class XE_CORE_EXPORT_C_PLUS_PLUS_API U8StringRef
	{
	public:
		// Init
		U8StringRef() = default;

		U8StringRef(const char* c_utf8_str);

		U8StringRef(const char8_t* str, int64_t str_size);

		U8StringRef(const char8_t* str, int64_t str_size, int64_t input_character_number);

		U8StringRef(const U8StringRef& temp_string);

		U8StringRef& operator=(const U8StringRef& temp_string);

		U8StringRef& operator=(const char* c_utf8_str);

		U8StringRef Slice(int64_t start, int64_t end) const;

		[[nodiscard]] int64_t CharacterNumber() const { return characters_number; }

		[[nodiscard]] int64_t CharacterDataSize() const { return characters_data_size; }

		void Append(char32_t character);

		void Append(U8StringRef append_string);

		[[nodiscard]] std::vector<U8StringRef> Split(char32_t separator);

		[[nodiscard]] std::vector<U8StringRef> Split(U8StringRef separator);

		[[nodiscard]] char32_t At(int64_t offset);

		// U8StringRef Comparison Operators
		[[nodiscard]] char32_t operator[](int64_t offset) { return this->At(offset); }

		//
		[[nodiscard]] const char* c_str() const { return (const char*)characters_data; }

		// exclude end of '\0'
		[[nodiscard]] const int64_t length() const { return CharacterDataSize() - 1; }

		[[nodiscard]] const char8_t* data()const { return characters_data; }

		void Release();

		~U8StringRef() { Release(); }

	private:
		// data ptr
		char8_t* characters_data;
		// string size
		int64_t characters_number;
		// alloc size
		int64_t characters_data_size;
		//
		void LoadData(const char8_t* str, int64_t str_size, int64_t input_character_number);
		//
		void LoadData(const char* c_utf8_str);
	};

	[[nodiscard]] XE_CORE_EXPORT_C_PLUS_PLUS_API inline U8StringRef& operator+(U8StringRef& buffer, U8StringRef string)
	{
		buffer.Append(string);
		return buffer;
	}

	[[nodiscard]] XE_CORE_EXPORT_C_PLUS_PLUS_API inline U8StringRef& operator+(U8StringRef& buffer, const char *string)
	{
		buffer.Append(U8StringRef(string));
		return buffer;
	}

	// std::cout << <U8StringRef>
	XE_CORE_EXPORT_C_PLUS_PLUS_API std::ostream& operator<<(std::ostream& out, const U8StringRef& string);

	using xeString = U8StringRef;
}

#endif // _INC_XE_STRING_HPP_ IS EOF