/*==================================================================================/
*                            Lazy ImgReader(2023-2024)                              /
*                   This is a library to load many teypes image                     /
*                                                                                   /
*                           supported types :jpeg png                               /
*                                                                                   /
*                                                                                   /
*==================================================================================*/
#ifndef _INC_LAZY_IMG_READER_H_
#define _INC_LAZY_IMG_READER_H_


#include "LazyMat.h"
#include "LazyImgio.h"
#include "LazyImgType.h"

/*===================================================/
*         THE LAZYIMG ALL MODULES LIST               /
* ===================================================/
*   SHOW IMAGE DATA                                  /
*   #define    NOT_ADD_WINDOW_SHOW_MODULES           /
* ===================================================/
*   READ OR WIRTE IMAGE BY SYSTEM IO PTR             /
*   #define    NOT_USE_C_STD_STREAM                  /
* ===================================================/
*   READ "*.TIFF OR "*.TIF" FILE                     /
*   #define    ADD_TIFF_MODULES                      /
* ===================================================/
*   ONLY NEED READ                                   /
*   #define    READ_ONLY                             /
*===================================================*/

#ifdef _DEBUG
#include "LazyWindow.h"
#endif // _DEBUG

#endif // _INC_LAZY_IMG_READER_H_ IS EOF