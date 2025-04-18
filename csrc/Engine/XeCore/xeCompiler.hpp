#ifndef _INC_XE_COMPILER_HPP_
#define _INC_XE_COMPILER_HPP_

// Export function data
#if defined(_MSC_VER) // MSVC
#define EXPORT_HEAD __declspec(dllexport)
#elif defined(__GNUC__) //GCC
#define EXPORT_HEAD __attribute__((visibility("default")))
#endif // API EXPORT IS END

#define EXTERN_C_STMT				extern"C"

#ifndef XE_CALL
#if defined(WIN32) || defined(_WIN32)
#define XE_CALL __stdcall
#else
#define XE_CALL __cdecl
#endif
#endif


#if defined(EXPORT_C_SHARP_API)

#define XE_EXPORT_C_SHARP_API(ret_type) EXTERN_C_STMT EXPORT_HEAD ret_type XE_CALL
#define XE_EXPORT_C_PLUS_PLUS_API

#elif defined(EXPORT_C_PLUS_PLUS_API)

#define XE_EXPORT_C_SHARP_API
#define XE_EXPORT_C_PLUS_PLUS_API	EXPORT_HEAD

#endif

#endif // _INC_XE_COMPILER_HPP_ IS EOF