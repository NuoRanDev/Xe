# XE Visual Engine
support 3D and 2D

## Audio

### Marco List
#### $\quad$  1. NOT_USE_OGG
$\qquad$ Note:

$\qquad$ $\quad$ Not using libvorbis support

$\qquad$ Disable Function:

$\qquad$ $\quad$ bool OpenOGGData(AudioEncodedData* ogg_data, xeAnyType& dec_typpe, PcmBlock& pcm_block)

$\qquad$ $\quad$ PlayState GetOGGPcm(xeAnyType dec_typpe, PcmBlock& pcm_block)

$\qquad$ $\quad$ bool OGGSeek(xeAnyType dec_typpe, xeSize pos)

$\qquad$ $\quad$ void CloseOGGData(xeAnyType dec_typpe)

#### $\quad$  2. USE_FLAC
$\qquad$ Note:

$\qquad$ $\quad$ Using libflac support

#### $\quad$ 3. USE_MP3
$\qquad$ Note:

$\qquad$ $\quad$ Using mpg123 support

$\qquad$ Enable Function:

$\qquad$ $\quad$ bool OpenMP3Data(AudioEncodedData* mp3_data, xeAnyTypePtr& dec_typpe, PcmBlock& pcm_block);

$\qquad$ $\quad$ PlayState GetMP3Pcm(xeAnyTypePtr dec_typpe, PcmBlock& pcm_block);

$\qquad$ $\quad$ bool MP3Seek(xeAnyTypePtr dec_typpe, xeSize pos);

$\qquad$ $\quad$ void CloseMP3Data(AudioEncodedData* mp3_data, xeAnyTypePtr dec_typpe);

## Tesure
### Marco List
#### $\quad$  1. USE_AVIF
$\qquad$ Note:

$\qquad$ $\quad$ Using libavif support

$\qquad$ Enable Function:

$\qquad$ $\quad$ std::unique_ptr\<Testure\> DecodeAVIF(xeByte* avif_buffer, xeSize file_size)

## Core
### Marco List
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

## Graphical
### Marco List
#### $\quad$  NOT_USE_VULKAN
$\qquad$ Note:

$\qquad$ $\quad$ Disable vulkan API.

$\qquad$ Action scope:

$\qquad$ $\quad$ class