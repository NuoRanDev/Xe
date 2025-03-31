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
		std::unique_ptr<PcmBlock> pcm_block;

		// file data
		std::unique_ptr<AudioEncodedData> audio_data;

		std::function<bool(std::unique_ptr<AudioEncodedData>, xeAnyType&, xeUint32&)> CB_OpenEncodedData;

		std::function<bool(xeAnyType, PcmBlock&)> CB_GetPcm;

		std::function<bool(xeAnyType, xeSize)> CB_Seek;

		std::function<void(xeAnyType)> CB_Close;
	};
}