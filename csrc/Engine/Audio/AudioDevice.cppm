export module xe.Audio.AudioContest;

import std;

import xe.Core.xeString;
import xe.Core.xeBaseDataType;

import xe.Audio.AudioInstance;

extern"C"
{
import "AL/alc.h";
}

namespace xe
{
	export class AudioContest
	{
	public:
		AudioContest() = default;

		bool OpenAudioDevice();

		void Release();

		~AudioContest();

	private:

		xeString cur_audio_device_name;
		ALCdevice* cur_audio_device;

		ALCcontext* audio_context;
		std::map<bool, AudioInstance> audio_instance_map;
	};
}