#pragma once

#include "XeImageConstructor.g.h"

namespace winrt::XE::implementation
{
    struct XeImageConstructor : XeImageConstructorT<XeImageConstructor>
    {
        XeImageConstructor() = default;

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::XE::factory_implementation
{
    struct XeImageConstructor : XeImageConstructorT<XeImageConstructor, implementation::XeImageConstructor>
    {
    };
}
