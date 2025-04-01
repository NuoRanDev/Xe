import xe.Audio.Instance.private_Audiodecoder;

import std;

import xe.Core.xeBaseDataType;
import xe.Core.CoreClrOutput;
import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;
import xe.Core.Unit;

import xe.Audio.AudioCore;

#include "vorbis/vorbisfile.h"
#include <cstdio>

namespace xe
{
	using OGGType = OggVorbis_File;

	xeSize ReadOgg(void* dst, xeSize size1, xeSize size2, void* class_data)
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

	int SeekOgg(void* class_data, ogg_int64_t to, int type) 
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

	int CloseOgg(void* class_data)
	{
		auto ogg_data = reinterpret_cast<AudioEncodedData*>(class_data);
		ogg_data->cur_ptr = ogg_data->data;
		return 0;
	}

	long TellOgg(void* class_data)
	{
		auto ogg_data = reinterpret_cast<AudioEncodedData*>(class_data);
		return (ogg_data->cur_ptr - ogg_data->data);
	}

	bool OpenOGGData(AudioEncodedData* ogg_data, xeAnyType& dec_typpe, PcmBlock& pcm_block)
	{
		ov_callbacks ogg_callbacks =
		{
			.read_func = ReadOgg,
			.seek_func = SeekOgg,
			.close_func = CloseOgg,
			.tell_func = TellOgg
		};

		OGGType* dec_ogg_type = xeMalloc<OGGType>(1);

		if (ov_open_callbacks(reinterpret_cast<void*>(ogg_data), dec_ogg_type, nullptr, -1, ogg_callbacks) < 0)
		{
			XE_ERROR_OUTPUT("<LIB: VORBIS> Open OGG file is failed, file could be broken!");
			return false;
		}

		dec_typpe = reinterpret_cast<xeAnyType>(dec_ogg_type);

		vorbis_info* ogg_info = ov_info(dec_ogg_type, -1);

		pcm_block.freq = ogg_info->rate;
		pcm_block.format = PcmFormat::FORMAT_STEREO16;
		pcm_block.size = 4 * Storage::KiB;
		pcm_block.data = xeMalloc<xeByte>(pcm_block.size);
		pcm_block.channel = ogg_info->channels;

		return true;
	}

	PlayState GetOGGPcm(xeAnyType dec_typpe, PcmBlock& pcmdata)
	{
		OGGType* dec_ogg_type = reinterpret_cast<OGGType*>(dec_typpe);
		int current_section;
		auto state = ov_read(dec_ogg_type, reinterpret_cast<char*>(pcmdata.data), (xeUint32)(pcmdata.size), 0, 2, 1, &current_section);
		if (state == 0) return PlayState::_END;
		if (state > 0)
		{
			pcmdata.buffer_in = state;
			return PlayState::_PLAY;
		}
		else 
		{
			XE_ERROR_OUTPUT("<LIB: VORBIS> Read OGG file's stream is failed, file could be broken!");
			return PlayState::_ERROR;
		}
	}

	bool OGGSeek(xeAnyType dec_typpe, xeSize pos)
	{
		OGGType* dec_ogg_type = reinterpret_cast<OGGType*>(dec_typpe);
		if (ov_pcm_seek(dec_ogg_type, pos))
		{
			XE_WARNING_OUTPUT("<LIB: VORBIS> Seek OGG file is failed");
			return false;
		}
		return true;
	}

	void CloseOGGData(xeAnyType dec_typpe)
	{
		OGGType* dec_ogg_type = reinterpret_cast<OGGType*>(dec_typpe);
		ov_clear(dec_ogg_type);
		xeFree(dec_ogg_type);
	}
}