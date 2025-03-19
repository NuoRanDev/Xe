export module xe.xeTesture.decode.private_Imgdecoder;

import std;

import xe.xeCore.xeOrdinals;
import xe.xeCore.xeCoreClrOutput;

import xe.xeTesture.xeTestureCore;


namespace xe
{
	export bool DecodePNG(std::unique_ptr<Testure> img, xeByte* png_buffer, xeSize file_size);
	export bool DecodeAVIF(std::unique_ptr<Testure> img, xeByte* avif_buffer, xeSize file_size);
}