import xe.Audio.AudioCore;

import std;

import xe.Core.xeOrdinals;
import xe.Core.xeBaseDataType;

#include <cstdio>

namespace xe
{
	xeSize ReadAudio(void* dst, xeSize size1, xeSize size2, void* class_data)
	{
		auto ogg_data = reinterpret_cast<AudioEncodedData*>(class_data);
		xeSize len = size1 * size2;
		if (ogg_data->cur_ptr + len > ogg_data->data + ogg_data->_size)
		{
			len = ogg_data->data + ogg_data->_size - ogg_data->cur_ptr;
		}
		std::memcpy(dst, ogg_data->cur_ptr, len);
		ogg_data->cur_ptr += len;
		return len;
	}

	int SeekAudio(void* class_data, xeInt64 to, int type)
	{
		auto ogg_data = reinterpret_cast<AudioEncodedData*>(class_data);
		switch (type)
		{
		case SEEK_CUR:
			ogg_data->cur_ptr += to;
			break;
		case SEEK_END:
			ogg_data->cur_ptr = ogg_data->data + ogg_data->_size - to;
			break;
		case SEEK_SET:
			ogg_data->cur_ptr = ogg_data->data + to;
			break;
		default:
			return -1;
		}
		if (ogg_data->cur_ptr < ogg_data->data)
		{
			ogg_data->cur_ptr = ogg_data->data;
			return -1;
		}
		if (ogg_data->cur_ptr > ogg_data->data + ogg_data->_size)
		{
			ogg_data->cur_ptr = ogg_data->data + ogg_data->_size;
			return -1;
		}
		return 0;
	}

	int CloseAudio(void* class_data)
	{
		auto ogg_data = reinterpret_cast<AudioEncodedData*>(class_data);
		ogg_data->cur_ptr = ogg_data->data;
		return 0;
	}

	long TellAudio(void* class_data)
	{
		auto ogg_data = reinterpret_cast<AudioEncodedData*>(class_data);
		return (ogg_data->cur_ptr - ogg_data->data);
	}
}