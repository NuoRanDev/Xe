#ifndef _XE_ERROR_OURPUT_HPP_
#define _XE_ERROR_OURPUT_HPP_

#include <exception>

namespace xe
{
	namespace XE_TYPE_NAME_OUTPUT
	{
		constexpr char LIB[] = "LIB";
		constexpr char APP[] = "APP";
	};

	namespace XE_CLR_COLOR
	{
		constexpr const char RESET[] = "\033[0m";
		constexpr const char RED[] = "\033[31m";
		constexpr const char GREEN[] = "\033[32m";
		constexpr const char YELLOW[] = "\033[33m";
		constexpr const char BLUE[] = "\033[34m";
		constexpr const char WHITE[] = "\033[37m";
	}

	void XE_CLR_OUTPUT(
		const char* color, const char* color_string, 
		const char* output_type,
		const char* addon_type, const char* output_text,
		const char* file, int line, const char* function);


#define XE_FATAL_OUTPUT(output_type, addon_type, output_text)\
		XE_CLR_OUTPUT(xe::XE_CLR_COLOR::RED, "FATAL", output_type, addon_type, output_text, __FILE__, __LINE__, __FUNCTION__);\
		std::terminate();

#define XE_ERROR_OUTPUT(output_type, addon_type, output_text)\
		XE_CLR_OUTPUT(xe::XE_CLR_COLOR::RED, "ERROR", output_type, addon_type, output_text, __FILE__, __LINE__, __FUNCTION__);

#define XE_WARNING_OUTPUT(output_type, addon_type, output_text)\
		XE_CLR_OUTPUT(xe::XE_CLR_COLOR::YELLOW, "WARNING", output_type, addon_type, output_text, __FILE__, __LINE__, __FUNCTION__);

#define XE_INFO_OUTPUT(output_type, addon_type, output_text)\
		XE_CLR_OUTPUT(xe::XE_CLR_COLOR::GREEN, "INFO", output_type, addon_type, output_text, __FILE__, __LINE__, __FUNCTION__);

#define XE_DEBUG_OUTPUT(output_type, addon_type, output_text)\
		XE_CLR_OUTPUT(xe::XE_CLR_COLOR::BLUE, "INFO", output_type, addon_type, output_text, __FILE__, __LINE__, __FUNCTION__);
} // namespace xe is end

#endif // _XE_ERROR_OURPUT_HPP_ IS EOF