#ifndef _CSHARP_API_XE_ASSET_CSHARP_H_
#define _CSHARP_API_XE_ASSET_CSHARP_H_
/*===========================================
*                   Note
* THIS IS NOT C PLUS PLUS API HEADER
* DO NOT INCLUDE THIS FILE
* THIS FILE IS EXPORT DLL FUNCTION TO CSHRAP
============================================*/

#include "xeIO.h"
#include "xeIOConfing.h"

#ifdef EXPORT_C_SHARP_API
XE_EXPORT_C_SHARP_API(xe::ioMmapfstream*) GetMmapClassObj();

XE_EXPORT_C_SHARP_API(bool) OpenFile(xe::ioMmapfstream* class_obj, const char* file_path);

XE_EXPORT_C_SHARP_API(bool) MemcpyOut(xe::ioMmapfstream* class_obj, xe::byte_t* dst, int32_t start, int32_t offset);

XE_EXPORT_C_SHARP_API(void) DeleteMmapClassObj(xe::ioMmapfstream* class_obj);
#endif // EXPORT_C_SHARP_API IS END

#endif // _CSHARP_API_XE_ASSET_CSHARP_H_ IS END