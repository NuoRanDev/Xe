#include <SoundWave.h>
#include <stdexcept>

#include "al.h"

audio::SoundWave::SoundWave(const std::string& wave_path) :
	wave_path(wave_path)
{
	if (!Init) throw std::runtime_error("SoundWave Init fail !");
}

audio::SoundWave::~SoundWave(){}

bool audio::SoundWave::Init()
{
	GenBufferID();
	if (!LoadWaveFromPathToBuffer()) return false;
	return true;
}

bool audio::SoundWave::Release()
{
	ReleaseBuffer();
	return true;
}

void audio::SoundWave::GenBufferID()
{
	alGenBuffers(1, &buffer_id);
}

bool audio::SoundWave::LoadWaveFromPathToBuffer()
{
	return true;
}

void audio::SoundWave::ReleaseBuffer() const
{
	alDeleteBuffers(1, &buffer_id);
}

uint64_t audio::SoundWave::GetDataSize() const
{
	return data_size;
}

[[nodiscard]] uint32_t audio::SoundWave::GetBufferID() const
{
	return buffer_id;
}

std::string audio::SoundWave::SetSoundWavePath(const std::string& wave_path)
{
	Release();
	this->wave_path = wave_path;
	GenBufferID();
	LoadWaveFromPathToBuffer();
	return wave_path;
}

std::string audio::SoundWave::GetSoundWavePath() const
{
	return wave_path;
}

uint32_t audio::SoundWave::GetNumChannels() const
{
	return num_channels;
}

uint32_t audio::SoundWave::GetSampleRate() const
{
	return sample_rate;
}