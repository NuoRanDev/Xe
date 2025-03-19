import xe.xeTesture.xeTestureDecode;

import std;

import xe.xeCore.xeBaseDataType;

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
			//if (DecompressJPEG(output, File->data))return nullptr;
			return output;
		case xeColorChannel::BOOL:
			// BOOL IS SAME ENCODE RGBA
		case xeColorChannel::GRAY16:
			// BOOL IS SAME ENCODE GRAY16
		case xeColorChannel::RGBA:
			if (!DecodePNG(std::move(output), File->data, File->_size))return nullptr;
			return output;
		}
	}
}