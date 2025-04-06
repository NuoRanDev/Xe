export module xe.Audio.Device;

import std;

import xe.Core.xeString;

import "AL/al.h";
import "AL/alc.h";

namespace xe
{
	export class AudioContest
	{
	public:
		AudioContest();

		bool FindAudioDevice();

		bool OpenAudioDevice(xeString name);
		
		~AudioContest();

	private:
		//std::map<xeString, ALCdevice> audio_device_list;
	};
}