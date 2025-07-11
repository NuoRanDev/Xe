export module ExportCsharpApi;

import xe.AssetsPacking;

import std;

import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;
import xe.Core.CoreClrOutput;

import xe.AssetIO.xeCompressFileStruct;

#include "xeCompiler.hpp"

using namespace xe;

XE_EXPORT_C_SHARP_API(AssetsPacking*) CreateAssetsPacking(xeUint32 cmp_solution, xeUint64 AssetFileType)
{
	return new AssetsPacking(static_cast<xeCompressSolution>(cmp_solution), AssetFileType);
}

XE_EXPORT_C_SHARP_API(bool) AssetsPackingWrite(AssetsPacking* self, bool(XE_CALL *open_cb)(xeSize), bool(XE_CALL *write_cb)(const xeByte*, const xeSize), void(XE_CALL *close_cb)(void))
{
	return self->Write(open_cb, write_cb, close_cb);
}

XE_EXPORT_C_SHARP_API(bool) AssetsPackingAddAsset(AssetsPacking* self, xeU8cstr* block_name, xeByte* input_data, xeSize data_size)
{
	return self->AddAsset(input_data, block_name, data_size);
}

XE_EXPORT_C_SHARP_API(void)AssetsPackingReleaseAsset(AssetsPacking* self)
{
	delete[] self;
	self = nullptr;
}