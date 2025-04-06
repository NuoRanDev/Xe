# XE Visual Engine
support 3D and 2D

## Audio

### Marco List
#### $\quad$  1. USE_OGG
$\qquad$ Note:

$\qquad$ $\quad$ Using libvorbis support

#### $\quad$  2. USE_FLAC
$\qquad$ Note:

$\qquad$ $\quad$ Using libflac support

#### $\quad$ 3. USE_MP3
$\qquad$ Note:

$\qquad$ $\quad$ Using libminimp3 support

## Tesure

## Core
#### $\quad$  OPEN_CLR
$\qquad$ Note:

$\qquad$ $\quad$ The file outputing functions will be closed when this marco be defined.

$\qquad$ Action scope:

$\qquad$ $\quad$ void XE_ERROR_OUTPUT(const char* output_text)

$\qquad$ $\quad$ void XE_WARNING_OUTPUT(const char* output_text)

$\qquad$ $\quad$ void XE_INFO_OUTPUT(const char* output_text)

#### $\quad$ USE_MIMALLOC
$\qquad$ Note:

$\qquad$ $\quad$ The outputing functions will be closed when this marco be defined.

$\qquad$ affected scope:

$\qquad$ $\quad$ template\<typename T\> T* xeMalloc(xeSize alloc_number)

$\qquad$ $\quad$ template\<typename T\>T* xeRealloc(T* src_ptr, xeSize alloc_number)

$\qquad$ $\quad$ void xeFree(void* src_ptr)