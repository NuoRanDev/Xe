#include "xeTestureDecode.hpp"

#include "decode/private_ImageFileReader.hpp"

namespace xe
{
	Testure* DeCodeFile(TestureFile* File)
	{
		Testure* output = nullptr;
		switch (File->encodesolution)
		{
		case xeColorChannel::RGB:
			if(DecompressJPEG(output, File->data))return nullptr;
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