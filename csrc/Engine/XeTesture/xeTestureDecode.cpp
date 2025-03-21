import xe.xeTesture.xeTestureDecode;

import std;

import xe.xeCore.xeBaseDataType;
import xe.xeCore.xeCoreClrOutput;

import xe.xeTesture.xeTestureCore;
import xe.xeTesture.decode.private_Imgdecoder;

namespace xe
{
	std::unique_ptr<Testure> DeCodeFile(std::unique_ptr<TestureFile> File)
	{
		std::unique_ptr<Testure> output;
		switch (File->encodesolution)
		{
		case xeColorChannel::RGB:
			output = DecodeAVIF(File->data, File->_size);
			if (output == nullptr)
			{
				XE_WARNING_OUTPUT("<Function> DecodeAVIF running Faild");
				goto READING_IMG_FALIED;
			}
			return output;
		case xeColorChannel::BOOL:
			// BOOL IS SAME ENCODE RGBA
		case xeColorChannel::GRAY16:
			// BOOL IS SAME ENCODE GRAY16
		case xeColorChannel::RGBA:
			output = DecodePNG(File->data, File->_size);
			if (output == nullptr)
			{
				XE_WARNING_OUTPUT("<Function> DecodePNG running Faild");
				goto READING_IMG_FALIED;
			}
			return output;
		}
		READING_IMG_FALIED:
		return nullptr;
	}
}