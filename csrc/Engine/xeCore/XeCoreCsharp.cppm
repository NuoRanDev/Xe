export module xe.XeCore.csharp;

#if defined(EXPORT_C_SHARP_API)
import xe.Core.CoreClrOutput;
import xe.Core.xeApplication;
import xe.Core.xeBaseDataType;
import xe.Core.xeOrdinals;
import xe.Core.Clock;
import xe.Core.xeApplication;

#include "xeCompiler.hpp"

// xe.Core.CoreClrOutput

XE_EXPORT_C_SHARP_API(void) XE_ERROR_OUTPUT(const char* output_text)
{
	xe::XE_ERROR_OUTPUT(output_text);
}

XE_EXPORT_C_SHARP_API(void) XE_WARNING_OUTPUT(const char* output_text)
{
	xe::XE_WARNING_OUTPUT(output_text);
}

XE_EXPORT_C_SHARP_API(void) XE_INFO_OUTPUT(const char* output_text)
{
	xe::XE_INFO_OUTPUT(output_text);
}

// end xe.Core.CoreClrOutput

// xe.Core.xeApplication;

XE_EXPORT_C_SHARP_API(bool) LaodApplication(int argc, char** argv)
{
	return xe::Application::LaodApplication();
}

XE_EXPORT_C_SHARP_API(int) DestroyApplication()
{
	return xe::Application::DestroyApplication();
}
// end xe.Core.xeApplication;

#endif // !defined(EXPORT_C_SHARP_API)