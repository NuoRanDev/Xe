export module xe.xeTesture.decode.private_Imgdecoder;

import std;

import xe.xeCore.xeOrdinals;
import xe.xeCore.xeCoreClrOutput;

import xe.xeTesture.xeTestureCore;


namespace xe
{
	export std::unique_ptr<Testure> DecodePNG(xeByte* png_buffer, xeSize file_size);
	export std::unique_ptr<Testure> DecodeAVIF(xeByte* avif_buffer, xeSize file_size);
}