#if !defined(NOT_USE_OGG)
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

	bool OpenOGGData(AudioEncodedData* ogg_data, xeAnyTypePtr* pdec_typpe, PcmBlock* pcm_block)
	{
		ov_callbacks ogg_callbacks =
		{
			.read_func = ReadAudioDecodedData,
			.seek_func = SeekAudioDecodedData,
			.close_func = CloseAudioDecodedData,
			.tell_func = TellAudioDecodedData
		};

		OGGType* dec_ogg_type = xeMalloc<OGGType>(1);

		if (ov_open_callbacks(reinterpret_cast<void*>(ogg_data), dec_ogg_type, nullptr, -1, ogg_callbacks) < 0)
		{
			XE_ERROR_OUTPUT("<LIB: VORBIS> Open OGG file is failed, file could be broken!");
			return false;
		}

		vorbis_info* ogg_info = ov_info(dec_ogg_type, -1);

		pcm_block->freq = ogg_info->rate;
		pcm_block->size = 4 * Storage::KiB;
		pcm_block->data = xeMalloc<xeByte>(pcm_block->size);
		pcm_block->channel = ogg_info->channels;
		if (ogg_info->channels > 1) pcm_block->format = PcmFormat::FORMAT_STEREO16;
		else { pcm_block->format = PcmFormat::FORMAT_MONO16; }

		*pdec_typpe = reinterpret_cast<xeAnyTypePtr>(dec_ogg_type);

		return true;
	}

	PlayState GetOGGPcm(xeAnyTypePtr dec_typpe, PcmBlock* pcm_block)
	{
		OGGType* dec_ogg_type = reinterpret_cast<OGGType*>(dec_typpe);
		int current_section;
		auto state = ov_read(dec_ogg_type, reinterpret_cast<char*>(pcm_block->data), (xeUint32)(pcm_block->size), 0, 2, 1, &current_section);
		if (state == 0) return PlayState::_END;
		if (state > 0)
		{
			pcm_block->buffer_in = state;
			return PlayState::_PLAY;
		}
		else
		{
			XE_ERROR_OUTPUT("<LIB: VORBIS> Read OGG file's stream is failed, file could be broken!");
			return PlayState::_ERROR;
		}
	}

	bool OGGSeek(xeAnyTypePtr dec_typpe, xeSize pos)
	{
		OGGType* dec_ogg_type = reinterpret_cast<OGGType*>(dec_typpe);
		if (ov_pcm_seek(dec_ogg_type, pos))
		{
			XE_WARNING_OUTPUT("<LIB: VORBIS> Seek OGG file is failed");
			return false;
		}
		return true;
	}

	void CloseOGGData(AudioEncodedData* ogg_data, xeAnyTypePtr dec_typpe)
	{
		OGGType* dec_ogg_type = reinterpret_cast<OGGType*>(dec_typpe);
		ov_clear(dec_ogg_type);
		xeFree(dec_ogg_type);
		CloseAudioDecodedData(ogg_data);
	}
}
#endif // !defined(NOT_USE_OGG)