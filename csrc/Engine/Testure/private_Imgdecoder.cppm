export module xe.Testure.decode.private_Imgdecoder;

import std;

import xe.Core.xeOrdinals;
import xe.Core.CoreClrOutput;

import xe.Testure.xeTestureCore;


namespace xe
{
	export std::unique_ptr<Testure> DecodePNG(xeByte* png_buffer, xeSize file_size);
#if defined(USE_AVIF)
	export std::unique_ptr<Testure> DecodeAVIF(xeByte* avif_buffer, xeSize file_size);
#endif // defined USE_AVIF
}