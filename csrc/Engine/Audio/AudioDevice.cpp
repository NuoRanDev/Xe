import xe.Audio.Device;
import xe.Core.xeString;

import "AL/al.h";
import "AL/alc.h";

namespace xe
{
	AudioContest::AudioContest()
	{
	}

	bool AudioContest::FindAudioDevice()
	{
		alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
		return true;
	}

	bool AudioContest::OpenAudioDevice(xeString name)
	{
		return true;
	}

	AudioContest::~AudioContest()
	{
	}
}