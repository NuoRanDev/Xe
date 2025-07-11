export module  xe.Testure.xeTestureCsharpAPI;
#if defined(EXPORT_C_SHARP_API)
import std;

import xe.Core.xeOrdinals;
import xe.Core.xeBaseDataType;

import xe.Testure.xeTestureCore;
import xe.Testure.xeTestureDecode;


#include "xeCompiler.hpp"

// xe.Testure.xeTestureCore

XE_EXPORT_C_SHARP_API(xe::Testure*) CreateTesture()
{
	return new xe::Testure();
}

XE_EXPORT_C_SHARP_API(void) LoadData(xe::Testure* self, xe::xeUint32 i_x, xe::xeUint32 i_y, xe::xeColorChannel i_channel)
{
	self->LoadData(i_x, i_y, i_channel);
}

XE_EXPORT_C_SHARP_API(xe::xeByte*) GetData(xe::Testure* self)
{
	return self->GetData<xe::xeByte>();
}

XE_EXPORT_C_SHARP_API(xe::xeUint64) GetLineSize(xe::Testure* self)
{
	return self->GetLineSize();
}

XE_EXPORT_C_SHARP_API(xe::xeUint64) GetTestureSize(xe::Testure* self)
{
	return self->GetTestureSize();
}

XE_EXPORT_C_SHARP_API(xe::xeUint32) x(xe::Testure* self)
{
	return self->x;
}

XE_EXPORT_C_SHARP_API(xe::xeUint32) y(xe::Testure* self)
{
	return self->y;
}

XE_EXPORT_C_SHARP_API(xe::xeUint32) bit_depth(xe::Testure* self)
{
	return self->bit_depth;
}

XE_EXPORT_C_SHARP_API(xe::xeColorChannel) channel(xe::Testure* self)
{
	return self->channel;
}

XE_EXPORT_C_SHARP_API(void) DestroyTesture(xe::Testure* self)
{
	delete self;
}

// end xe.Testure.xeTestureCore

// xe.Testure.xeTestureDecode
XE_EXPORT_C_SHARP_API(xe::Testure*) DeCodeFile(xe::TestureEncodedData* file)
{
	return xe::DeCodeFile(std::move(std::unique_ptr<xe::TestureEncodedData>(file))).get();
}
// end xe.Testure.xeTestureDecode

#endif // !defined(EXPORT_C_SHARP_API)