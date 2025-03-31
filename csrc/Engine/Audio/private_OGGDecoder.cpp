import xe.Audio.Instance.private_Audiodecoder;

import std;

import xe.Core.xeBaseDataType;
import xe.Core.CoreClrOutput;
import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;

import xe.Audio.AudioCore;

#include "vorbis/vorbisfile.h"

namespace xe
{
	using OGGType = OggVorbis_File;
	void OpenOGGData(xeByte* ogg_data, xeSize size, xeAnyType& dec_typpe, PcmFormat& pcm_format)
	{
		OGGType* dec_ogg_type = xeMalloc<OGGType>(1);
		ov_open_callbacks(ogg_data, dec_ogg_type, nullptr, 0, OV_CALLBACKS_NOCLOSE);
		dec_typpe = reinterpret_cast<xeAnyType>(dec_ogg_type);
		pcm_format = PcmFormat::AL_FORMAT_STEREO16;
	}
	PlayState GetOGGPcm(xeAnyType dec_typpe, PcmBlock& pcmdata)
	{
		OGGType* dec_ogg_type = reinterpret_cast<OGGType*>(dec_typpe);
		xeInt32 current_section;
		xeInt32 readed = ov_read(dec_ogg_type, reinterpret_cast<char*>(pcmdata.data), pcmdata.size, 0, 2, 1, &current_section);
		if (readed == 0)
		{
			return PlayState::_END;
		}
		else if (readed < 0)
		{
			XE_WARNING_OUTPUT("<LIB: VORBIS> Read OGG file is failed");
			return PlayState::_ERROR;
		}
		return PlayState::_PLAY;
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