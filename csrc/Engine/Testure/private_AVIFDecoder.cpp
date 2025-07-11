#if defined(USE_AVIF)
import xe.Testure.decode.private_Imgdecoder;

import std;

import xe.Core.xeOrdinals;
import xe.Core.xeBaseDataType;
import xe.Core.CoreClrOutput;

import xe.Testure.xeTestureCore;

#include "avif/avif.h"

namespace xe
{
	std::unique_ptr<Testure> DecodeAVIF(xeByte* avif_buffer, xeSize file_size)
	{
		std::unique_ptr<Testure> img;
		avifRGBImage avif_rgb = { 0 };

		avifDecoder* decoder = avifDecoderCreate();
		if (decoder == NULL) {
			XE_WARNING_OUTPUT("<LIB: AVIF> DecodeAVIFing Memory allocation failure");
			return nullptr;
		}

		avifResult result = avifDecoderSetIOMemory(decoder, avif_buffer, file_size);
		if (result != AVIF_RESULT_OK) {
			XE_WARNING_OUTPUT("<LIB: AVIF> DecodeAVIFing Cannot set IO on avifDecoder:");
			goto cleanup;
		}

		result = avifDecoderParse(decoder);
		if (result != AVIF_RESULT_OK) {
			XE_WARNING_OUTPUT(std::format("<LIB: AVIF> DecodeAVIFing Failed to decode image: {}", avifResultToString(result)).c_str());
			goto cleanup;
		}

		avifRGBImageSetDefaults(&avif_rgb, decoder->image);
		result = avifRGBImageAllocatePixels(&avif_rgb);
		if (result != AVIF_RESULT_OK) {
			XE_WARNING_OUTPUT(std::format("<LIB: AVIF> DecodeAVIFing allocation of RGB samples failed: {}\n", avifResultToString(result)).c_str());
			goto cleanup;
		}

		img->LoadData(avif_rgb.width, avif_rgb.height, xeColorChannel::RGB);

		std::memcpy(img->GetData<void*>(), avif_rgb.pixels, img->GetTestureSize());
	cleanup:
		avifRGBImageFreePixels(&avif_rgb);
		avifDecoderDestroy(decoder);
		return nullptr;
	}
}
#endif // defined USE_AVIF