import xe.Audio.AudioContest;

import xe.Core.xeString;
import xe.Core.CoreClrOutput;

#include "AL/al.h"
#include "AL/alc.h"

namespace xe
{
	bool AudioContest::OpenAudioDevice()
	{
		alcGetString(nullptr, ALC_DEVICE_SPECIFIER);

		// OpenAL Soft on 耳机 (WH-CH720N)
		// Soft on 扬声器 (Realtek(R) Audio)
		cur_audio_device_name = alcGetString(nullptr, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
		cur_audio_device = alcOpenDevice(nullptr);
		if (!cur_audio_device)
		{
			XE_ERROR_OUTPUT("<LIB: OPENAL> Failed to open OpenAL device");
			return false;
		}

		audio_context = alcCreateContext(cur_audio_device, nullptr);
		if (!alcMakeContextCurrent(audio_context))
		{
			XE_ERROR_OUTPUT("<LIB: OPENAL> Failed to make OpenAL context current");
			alcCloseDevice(cur_audio_device);
			return false;
		}
		return true;
	}

	bool AudioContest::OpenAudioDevice(xeString name)
	{
		return true;
	}

	void AudioContest::Release()
	{
		alcDestroyContext(audio_context);
		alcCloseDevice(cur_audio_device);
	}

	AudioContest::~AudioContest()
	{
		Release();
	}
}