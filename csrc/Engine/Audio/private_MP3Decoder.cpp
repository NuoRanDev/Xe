#if defined(USE_MP3)
import xe.Audio.Instance.private_Audiodecoder;

import std;

import xe.Core.xeBaseDataType;
import xe.Core.CoreClrOutput;
import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;

import xe.Audio.AudioCore;

#include "mpg123.h"
#include <cstdio>

namespace xe
{
	using MP3Type = mpg123_handle;

	xeInt32 MP3ReadDecodedData(xeAnyTypePtr src, xeAnyTypePtr dst, size_t size, size_t* got_size)
	{
		xeSize read_size = ReadAudioDecodedData(dst, size, 1, src);
		if (read_size < 0) return -1;
		*got_size = read_size;
		return 0;
	}

	xeInt64 MP3SeekDecodedData(xeAnyTypePtr src, int64_t offset, int whence)
	{
		return static_cast<xeInt64>(SeekAudioDecodedData(src, offset, whence));
	}

	void MP3CloseDecodedData(xeAnyTypePtr src)
	{
		CloseAudioDecodedData(src);
	}

	bool OpenMP3Data(AudioEncodedData* mp3_data, xeAnyTypePtr* pdec_typpe, PcmBlock* pcm_block)
	{
		int state = 0;
		int channels = 0, encoding = 0;
		long rate = 0;

		MP3Type* dec_mp3_type = mpg123_new(nullptr, &state);

		if (state != MPG123_OK)
		{
			XE_ERROR_OUTPUT("<LIB: MPG123> Init decoder failed");
			return false;
		}
		state = mpg123_reader64(dec_mp3_type, MP3ReadDecodedData, MP3SeekDecodedData, MP3CloseDecodedData);
		if (state != MPG123_OK)
		{
			XE_ERROR_OUTPUT("<LIB: MPG123> Init callback failed");
			return false;
		}
		mpg123_open_handle(dec_mp3_type, reinterpret_cast<xeAnyTypePtr>(mp3_data));
		if (state != MPG123_OK)
		{
			XE_ERROR_OUTPUT("<LIB: MPG123> Load file failed");
			return false;
		}
		//state = mpg123_param(dec_mp3_type, MPG123_VERBOSE, param.verbose, 0.);

		state = mpg123_getformat(dec_mp3_type, &rate, &channels, &encoding);
		if (state != MPG123_OK)
		{
			XE_ERROR_OUTPUT("<LIB: MPG123> File is broken");
			return false;
		}
		if ((encoding & MPG123_ENC_16) != 0)
		{
			if (channels == 1) pcm_block->format = PcmFormat::FORMAT_STEREO16;
			else { pcm_block->format = PcmFormat::FORMAT_MONO16; }
		}
		if ((encoding & MPG123_ENC_8) != 0)
		{
			if (channels == 1) pcm_block->format = PcmFormat::FORMAT_STEREO16;
			else { pcm_block->format = PcmFormat::FORMAT_MONO16; }
		}
		pcm_block->channel = channels;
		pcm_block->freq = rate;

		mpg123_format_none(dec_mp3_type);
		mpg123_format(dec_mp3_type, rate, channels, encoding);

		pcm_block->size = mpg123_outblock(dec_mp3_type);
		pcm_block->data = xeMalloc<xeByte>(pcm_block->size);

		pdec_typpe = reinterpret_cast<xeAnyTypePtr*>(&dec_mp3_type);
		return true;
	}

	PlayState GetMP3Pcm(xeAnyTypePtr dec_typpe, PcmBlock* pcmdata)
	{
		MP3Type* dec_mp3_type = reinterpret_cast<MP3Type*>(dec_typpe);
		size_t done = 0;
		if (mpg123_read(dec_mp3_type, pcmdata->data, pcmdata->size, &done) != MPG123_OK)
		{
			return PlayState::_ERROR;
		}
		if (done == 0)
		{
			return PlayState::_END;
		}
		else
		{
			return PlayState::_PLAY;
		}
	}

	bool MP3Seek(xeAnyTypePtr dec_typpe, xeSize pos)
	{
		MP3Type* dec_mp3_type = reinterpret_cast<MP3Type*>(dec_typpe);
		if (mpg123_seek(dec_mp3_type, pos, SEEK_SET) < 0)
		{
			return false;
		}
		return true;
	}

	void CloseMP3Data(AudioEncodedData* mp3_data, xeAnyTypePtr dec_typpe)
	{
		MP3Type* dec_mp3_type = reinterpret_cast<MP3Type*>(dec_typpe);
		mpg123_close(dec_mp3_type);
		mpg123_delete(dec_mp3_type);
		CloseAudioDecodedData(mp3_data);
		mpg123_exit();
	}
}
#endif // defined(USE_MP3)