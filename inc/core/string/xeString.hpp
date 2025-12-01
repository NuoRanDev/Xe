#ifndef _XE_STRING_HPP_
#define _XE_STRING_HPP_


#include <cstdint>
#include <vector>
#include <ostream>

#include "memory/xeAlloc.hpp"
#include "type/xeOrdinals.hpp"

namespace xe
{
	constexpr int64_t SHORT_STRING_SIZE = 12;
	class U8StringRef
	{
	public:
		// Init
		U8StringRef() = default;

		U8StringRef(const char* c_utf8_str) noexcept;

		U8StringRef(const utf8_t* str, int64_t str_size) noexcept;

		U8StringRef(const utf8_t* str, int64_t str_size, int64_t input_character_number) noexcept;

		U8StringRef(const U8StringRef& temp_string) noexcept;

		U8StringRef& operator=(const U8StringRef& temp_string) noexcept;

		U8StringRef& operator=(const char* c_utf8_str) noexcept;

		U8StringRef& operator=(const utf8_t* c_utf8_str) noexcept;

		U8StringRef Slice(int64_t start, int64_t end) const noexcept;

		[[nodiscard]] int64_t character_number() const noexcept { return characters_number; }

		[[nodiscard]] int64_t character_data_size() const noexcept { return characters_data_size; }

		void append(unicode_t character) noexcept;

		void append(U8StringRef append_string) noexcept;

		// Find all

		[[nodiscard]] std::vector<int64_t> find_all(const unicode_t pattern) const noexcept;

		[[nodiscard]] std::vector<int64_t> find_all(const U8StringRef pattern) const noexcept;

		[[nodiscard]] std::vector<int64_t> find_all(const utf8_t pattern) const noexcept;

		[[nodiscard]] std::vector<int64_t> find_all(const utf8_t* pattern, const int64_t size) const noexcept;

		// Find
		int64_t find_start(const utf8_t* pattern, int64_t pattern_size) const noexcept;

		int64_t find_last(const utf8_t* pattern, int64_t pattern_size) const noexcept;

		// Split string by separator

		[[nodiscard]] std::vector<U8StringRef> split(unicode_t separator) noexcept;

		[[nodiscard]] std::vector<U8StringRef> split(U8StringRef separator) noexcept;

		[[nodiscard]] unicode_t at(int64_t offset) noexcept;

		// U8StringRef Comparison Operators
		[[nodiscard]] unicode_t operator[](int64_t offset) noexcept { return this->at(offset); }

		// const c type string
		[[nodiscard]] const char* c_str() const noexcept { return (const char*)characters_data; }

		// exclude end of '\0'
		[[nodiscard]] const int64_t length() const noexcept { return character_data_size() - 1; }

		[[nodiscard]] const utf8_t* data()const noexcept { return characters_data; }

		[[nodiscard]] bool stringcmp(const U8StringRef& cmp_str) const noexcept
		{
			return stringcmp(cmp_str.data(), cmp_str.length());
		}

		[[nodiscard]] bool stringcmp(const char* cmp_str) const noexcept
		{
			return stringcmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(cmp_str));
		}

		[[nodiscard]] bool stringcmp(const utf8_t* cmp_str) const noexcept
		{
			return stringcmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(reinterpret_cast<const char*>(cmp_str)));
		}

		[[nodiscard]] bool stringcmp(const utf8_t* cmp_str, const int64_t cmp_str_size) const noexcept;

		bool operator<(const U8StringRef& src) const noexcept
		{
			return this->characters_data_size < src.characters_data_size;
		}

		bool operator==(const U8StringRef& src) const noexcept
		{
			return this->stringcmp(src.data(), src.characters_data_size);
		}

		template<IntegralType T> void integral_to_string(T number) noexcept
		{
			utf8_t characters[sizeof(T)] = { 0 };
			int64_t str_size = 0;

			integral_to_string_ptr(static_cast<int64_t>(number), characters, str_size);
			load_data(characters, str_size, str_size);
		}

		void release() noexcept
		{
			xe_free(characters_data);
			characters_data = nullptr;
			characters_data_size = 0;
			characters_number = 0;
		}

		~U8StringRef() { release(); }

	private:
		// data ptr
		utf8_t* characters_data;
		// string size
		int64_t characters_number;
		// alloc size
		int64_t characters_data_size;
		//
		void load_data(const utf8_t* c_utf8_str, int64_t str_size, int64_t input_character_number) noexcept;
		//
		void load_data(const char* c_utf8_str) noexcept;
		//
		void load_data(const utf8_t* c_utf8_str) noexcept;

		bool string_long_cmp(const U8StringRef cmp_str) const noexcept
		{
			return string_long_cmp(cmp_str.data(), cmp_str.length());
		}

		bool string_long_cmp(const char* cmp_str) const noexcept
		{
			return string_long_cmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(cmp_str));
		}

		bool string_long_cmp(const utf8_t* cmp_str) const noexcept
		{
			return string_long_cmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(reinterpret_cast<const char*>(cmp_str)));
		}

		bool string_long_cmp(const utf8_t* cmp_str, int64_t cmp_str_size) const noexcept;

		bool string_short_cmp(const U8StringRef cmp_str) noexcept
		{
			return string_short_cmp(cmp_str.data(), cmp_str.length());
		}

		bool string_short_cmp(const char* cmp_str) noexcept
		{
			return string_short_cmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(cmp_str));
		}

		bool string_short_cmp(const utf8_t* cmp_str) noexcept
		{
			return string_short_cmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(reinterpret_cast<const char*>(cmp_str)));
		}

		bool string_short_cmp(const utf8_t* cmp_str, int64_t cmp_str_size) const noexcept;

		void integral_to_string_ptr(int64_t number, utf8_t* str, int64_t& str_size) noexcept;
		// 
		bool is_short_string;
	};

	[[nodiscard]] inline U8StringRef& operator+(U8StringRef& buffer, U8StringRef string)
	{
		buffer.append(string);
		return buffer;
	}

	[[nodiscard]] inline U8StringRef& operator+(U8StringRef& buffer, const char* string)
	{
		buffer.append(U8StringRef(string));
		return buffer;
	}

	// std::cout << <U8StringRef>
	[[nodiscard]] inline std::ostream& operator<<(std::ostream& out, const U8StringRef& string)
	{
		out << string.c_str();
		return out;
	}

	using xeString = U8StringRef;
} // namespace xe is end



#endif // _XE_STRING_HPP_ IS EOF