export module xe.Audio;

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
		xeAnyType dec_contest;

		// file data
		std::unique_ptr<AudioEncodedData> audio_data;

		std::function<bool(AudioEncodedData*, xeAnyType&, PcmBlock&)> CB_OpenEncodedData;

		std::function<PlayState(xeAnyType, PcmBlock&)> CB_GetPcm;

		std::function<bool(xeAnyType, xeSize)> CB_Seek;

		std::function<void(xeAnyType)> CB_Close;
	};
}