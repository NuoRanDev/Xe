import xe.Testure.xeTestureDecode;

import std;

import xe.Core.xeBaseDataType;
import xe.Core.CoreClrOutput;

import xe.Testure.xeTestureCore;
import xe.Testure.decode.private_Imgdecoder;

namespace xe
{
	std::unique_ptr<Testure> DeCodeFile(std::unique_ptr<TestureEncodedData> file)
	{
		std::unique_ptr<Testure> output;
		switch (file->encodesolution)
		{
#if defined(USE_AVIF)
		case xeColorChannel::RGB:
			output = DecodeAVIF(file->data, file->size);
			if (output == nullptr)
			{
				XE_WARNING_OUTPUT("<Function: DecodeAVIF> running Faild");
				goto READING_IMG_FALIED;
			}
			return output;
#endif // deinfed(USE_AVIF)
		case xeColorChannel::BOOL:
			// BOOL IS SAME ENCODE RGBA
		case xeColorChannel::GRAY16:
			// BOOL IS SAME ENCODE GRAY16
		case xeColorChannel::RGBA:
			output = DecodePNG(file->data, file->size);
			if (output == nullptr)
			{
				XE_WARNING_OUTPUT("<Function: DecodePNG> running Faild");
				goto READING_IMG_FALIED;
			}
			return output;
		}
		READING_IMG_FALIED:
		return nullptr;
	}
}