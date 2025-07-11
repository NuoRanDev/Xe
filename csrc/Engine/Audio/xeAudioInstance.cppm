export module xe.Audio.AudioInstance;

import std;

import xe.Core.xeBaseDataType;
import xe.Core.CoreClrOutput;
import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;

import xe.Audio.AudioCore;

namespace xe
{
	export class AudioInstance
	{
	public:
		AudioInstance();

		bool LoadData(std::unique_ptr<AudioEncodedData> data);

		void Play();

		void Stop();

		void Seek(xeSize point);

		void ReStart() { Seek(0); }

		~AudioInstance();

	private:
		// pcm
		PcmBlock pcm_block;

		//
		xeAnyTypePtr dec_contest;

		// file data
		AudioEncodedData* audio_data;

		std::function<bool(AudioEncodedData*, xeAnyTypePtr*, PcmBlock*)> CB_OpenEncodedData;

		std::function<PlayState(xeAnyTypePtr, PcmBlock*)> CB_GetPcm;

		std::function<bool(xeAnyTypePtr, xeSize)> CB_Seek;

		std::function<void(AudioEncodedData*, xeAnyTypePtr)> CB_Close;
	};
}