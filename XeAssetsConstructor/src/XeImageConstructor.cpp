#include "pch.h"
#include "XeImageConstructor.h"
#if __has_include("XeImageConstructor.g.cpp")
#include "XeImageConstructor.g.cpp"
#endif

namespace winrt::XE::implementation
{
    int32_t XeImageConstructor::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void XeImageConstructor::MyProperty(int32_t /*value*/)
    {
        throw hresult_not_implemented();
    }
}
