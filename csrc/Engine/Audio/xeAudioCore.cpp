import xe.Audio.AudioCore;

import std;

import xe.Core.xeOrdinals;
import xe.Core.xeBaseDataType;

#include <cstdio>

namespace xe
{
	xeSize ReadAudioDecodedData(xeAnyTypePtr dst, xeSize type_size, xeSize type_number, xeAnyTypePtr src)
	{
		auto decoded_data = reinterpret_cast<AudioEncodedData*>(src);
		xeSize len = type_size * type_number;
		if (decoded_data->cur_ptr + len > decoded_data->data + decoded_data->size)
		{
			len = decoded_data->data + decoded_data->size - decoded_data->cur_ptr;
		}
		std::memcpy(dst, decoded_data->cur_ptr, len);
		decoded_data->cur_ptr += len;
		return len;
	}

	int SeekAudioDecodedData(xeAnyTypePtr src, xeInt64 offset, int type)
	{
		auto decoded_data = reinterpret_cast<AudioEncodedData*>(src);
		switch (type)
		{
		case SEEK_CUR:
			decoded_data->cur_ptr += offset;
			break;
		case SEEK_END:
			decoded_data->cur_ptr = decoded_data->data + decoded_data->size - offset;
			break;
		case SEEK_SET:
			decoded_data->cur_ptr = decoded_data->data + offset;
			break;
		default:
			return -1;
		}
		if (decoded_data->cur_ptr < decoded_data->data)
		{
			decoded_data->cur_ptr = decoded_data->data;
			return -1;
		}
		if (decoded_data->cur_ptr > decoded_data->data + decoded_data->size)
		{
			decoded_data->cur_ptr = decoded_data->data + decoded_data->size;
			return -1;
		}
		return 0;
	}

	int CloseAudioDecodedData(xeAnyTypePtr src)
	{
		auto decoded_data = reinterpret_cast<AudioEncodedData*>(src);
		decoded_data->cur_ptr = decoded_data->data;
		return 0;
	}

	long TellAudioDecodedData(xeAnyTypePtr src)
	{
		auto decoded_data = reinterpret_cast<AudioEncodedData*>(src);
		return (long)(decoded_data->cur_ptr - decoded_data->data);
	}

	bool IsAudioDecodedDataEOF(xeAnyTypePtr src)
	{
		auto decoded_data = reinterpret_cast<AudioEncodedData*>(src);
		return decoded_data->cur_ptr == decoded_data->data + decoded_data->size;
	}
}