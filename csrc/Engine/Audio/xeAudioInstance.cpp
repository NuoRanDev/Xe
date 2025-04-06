﻿import xe.Audio;

import xe.Core.xeBaseDataType;
import xe.Core.CoreClrOutput;
import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;

import xe.Audio.AudioCore;
import xe.Audio.Instance.private_Audiodecoder;
namespace xe
{
	AudioInstance::AudioInstance()
	{
		audio_data = nullptr;
		CB_OpenEncodedData = nullptr;
		CB_GetPcm = nullptr;
		CB_Seek = nullptr;
		CB_Close = nullptr;
	}

	bool AudioInstance::LoadData(std::unique_ptr<AudioEncodedData> data)
	{
		switch (data->solution)
		{
#if defined(USE_OGG)
		case xeAudioCompressSolution::OGG:
			audio_data = std::move(data);
			CB_OpenEncodedData = OpenOGGData;
			CB_GetPcm = GetOGGPcm;
			CB_Seek = OGGSeek;
			CB_Close = CloseOGGData;
			if (!CB_OpenEncodedData(audio_data.get(), dec_contest, pcm_block))
			{
				CB_Close(dec_contest);
				return false;
			}
#endif // USE_OGG
		default:
			break;
		}
		return true;
	}

	void AudioInstance::Play()
	{
	}

	void AudioInstance::Stop()
	{
	}

	void AudioInstance::Seek(xeSize point)
	{
	}

	AudioInstance::~AudioInstance()
	{
	}
}