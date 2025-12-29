#ifndef _XE_STRING_HPP_
#define _XE_STRING_HPP_


#include <cstdint>
#include <vector>
#include <ostream>

#include "type/xeOrdinals.hpp"


struct RustString
{
	int64_t size;
	const xe::utf8_t* data;
};

namespace xe
{
	constexpr int64_t SHORT_STRING_SIZE = 12;

	// kmp is O(m+n)
	// nomal is O(m*n)
	// 
	constexpr int64_t USE_KMP_STRING_SIZE = 32 * 32 + 1;

	class U8StringRef
	{
	public:
		// Init
		U8StringRef() { load_default_str(); }

		U8StringRef(const char* _c_str) noexcept { load_c_str_add0(_c_str); }

		U8StringRef(const utf8_t* str) noexcept { load_cpp_u8_str_add0(str); }

		U8StringRef(const utf8_t* str, int64_t str_size) noexcept { load_rust_str_add0(str, str_size); }

		U8StringRef(const utf8_t* str, int64_t str_size, int64_t input_character_number) noexcept
		{
			load_xe_str_include0(str, str_size, input_character_number);
		}

		U8StringRef(RustString rust_str) noexcept
		{
			if (rust_str.data == nullptr)
			{
				load_default_str();
				return;
			}
			load_rust_str_add0(rust_str.data, rust_str.size);
		}

		U8StringRef(const U8StringRef& temp_string) noexcept
		{
			if(temp_string.is_empty())
			{
				load_default_str();
				return;
			}
			load_xe_str_include0(temp_string.data(), temp_string.get_characters_data_size(), temp_string.get_characters_number());
		}

		U8StringRef& operator=(const U8StringRef& temp_string) noexcept 
		{
			if (temp_string.is_empty())
			{
				this->load_default_str();
				return *this;
			}
			this->load_xe_str_include0(temp_string.data(), temp_string.get_characters_data_size(), temp_string.get_characters_number());
			return *this;
		}

		U8StringRef& operator=(const char* _c_str) noexcept
		{
			if (_c_str == nullptr)
			{
				this->load_default_str();
				return *this;
			}
			this->load_cpp_u8_str_add0(reinterpret_cast<const utf8_t*>(_c_str));
			return *this;
		}

		U8StringRef& operator=(const utf8_t* c_utf8_str) noexcept
		{
			if (c_utf8_str == nullptr)
			{
				this->load_default_str();
				return *this;
			}
			this->load_cpp_u8_str_add0(c_utf8_str);
			return *this;
		}

		U8StringRef& operator=(const RustString& rust_str) noexcept
		{
			if (rust_str.data == nullptr)
			{
				this->load_default_str();
				return *this;
			}
			this->load_rust_str_add0(rust_str.data, rust_str.size);
			return *this;
		}

		// range: include start exclude end
		[[nodiscard]] U8StringRef slice(int64_t start, int64_t end) const noexcept;

		bool is_empty() const noexcept;

		[[nodiscard]] int64_t get_characters_number() const noexcept { return characters_number; }

		[[nodiscard]] int64_t get_characters_data_size() const noexcept { return characters_data_size; }

		void append(unicode_t character) noexcept;

		void append(U8StringRef append_string) noexcept;

		// Find all

		[[nodiscard]] std::vector<int64_t> find_all(const unicode_t pattern) const noexcept;

		[[nodiscard]] std::vector<int64_t> find_all(const U8StringRef pattern) const noexcept;

		[[nodiscard]] std::vector<int64_t> find_all(const utf8_t pattern) const noexcept;

		[[nodiscard]] std::vector<int64_t> find_all(const utf8_t* pattern, const int64_t size) const noexcept;

		// Find
		int64_t find_start(const U8StringRef pattern_str) const noexcept { return find_start(pattern_str.data(), pattern_str.get_characters_data_size()); }

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
		[[nodiscard]] const int64_t length() const noexcept { return characters_data_size - 1; }

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
			if ((this->is_empty()) ^ (src.is_empty()))
				return false;
			return this->stringcmp(src.data(), src.characters_data_size);
		}

		template<IntegralType T> void integral_to_string(T number) noexcept
		{
			utf8_t characters[sizeof(T)] = { 0 };
			int64_t str_size = 0;

			integral_to_string_ptr(static_cast<int64_t>(number), characters, str_size);
			load_data(characters, str_size, str_size);
		}

		void release() noexcept;

		~U8StringRef() { release(); }

	private:
		// data ptr
		utf8_t* characters_data;
		// string size
		int64_t characters_number;
		// alloc size
		int64_t characters_data_size;
		
		// default load data
		void load_default_str() noexcept;
		// load other xe string
		void load_xe_str_include0(const utf8_t* xe_utf8_str, int64_t xe_str_size, int64_t input_character_number) noexcept;
		// load cpp char8_t style utf8 string
		void load_cpp_u8_str_add0(const utf8_t* c_utf8_str) noexcept;
		// load rust string
		void load_rust_str_add0(const utf8_t* xe_rust_str, int64_t rust_str_size);
		// load c style string
		void load_c_str_add0(const char* _c_str) noexcept
		{
			load_cpp_u8_str_add0(reinterpret_cast<const utf8_t*>(_c_str));
		}

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
		auto str = string.c_str();
		if (str == nullptr)
			out << "";
		else
		{
			out << str;
		}
		return out;
	}

	using String = U8StringRef;
} // namespace xe is end



#endif // _XE_STRING_HPP_ IS EOF