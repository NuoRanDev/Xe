#include "xeIOCsharpAPI.hpp"

#ifdef EXPORT_C_SHARP_API
xe::ioMmapfstream* XE_CALL GetMmapClassObj()
{
	return new xe::ioMmapfstream();
}

bool XE_CALL OpenFile(xe::ioMmapfstream* class_obj, const char* file_path)
{
	printf("csharp-api is opening %s\n", file_path);
	return class_obj->GetFilePtr(file_path);
}

bool XE_CALL MemcpyOut(xe::ioMmapfstream* class_obj, xe::byte_t* dst, int32_t start, int32_t offset)
{
	return class_obj->FstraemMemcpyOut<xe::byte_t>(dst, start, offset);
}

void XE_CALL DeleteMmapClassObj(xe::ioMmapfstream* class_obj)
{
	if (class_obj != nullptr)
	{
		delete class_obj;
	}
}
#endif // EXPORT_C_SHARP_API IS END