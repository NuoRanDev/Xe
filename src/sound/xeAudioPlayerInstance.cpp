#include "xeAudioPlayerInstance.hpp"

#include "log/xeLogOutput.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace xe
{
	bool AudioPlayerInstance::init() noexcept
	{
		// device  : ALCdevice*
		// context : ALCcontext*
		device = alcOpenDevice(nullptr);
		if (!device) 
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeSound : OpenAL", "Fail to open device");
			return false;
		}
		context = alcCreateContext(reinterpret_cast<ALCdevice*>(device), nullptr);
		if (!context) 
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeSound : OpenAL", "Fail to create context");
			return false;
		}
		alcMakeContextCurrent(reinterpret_cast<ALCcontext*>(context));
		if (alGetError() != AL_NO_ERROR) 
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeSound : OpenAL", "Fail to make current context");
			return false;
		}
		return true;
	}

	AudioPlayerInstance::~AudioPlayerInstance()
	{
		alcDestroyContext(reinterpret_cast<ALCcontext*>(context));
		alcCloseDevice(reinterpret_cast<ALCdevice*>(device));
	}
} // namespace xe is end