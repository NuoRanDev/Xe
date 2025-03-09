export module xeTesture.xeTestureDecode;

import std;

import xeCore.xeBaseDataType;

import xeTesture.xeTestureCore;
import xeTesture.decode.private_ImageFileReader;

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
			if (DecompressPNG(output, File->data, File->_size))return nullptr;
			return output;
		}
	}
}