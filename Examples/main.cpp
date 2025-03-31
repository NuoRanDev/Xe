import std;

import xe.Core.xeString;
import xe.Core.xeBaseDataType;
import xe.Core.xeApplication;
import xe.Core.xeAlloc;

import xe.IO.xeOMmapfstream;

import xe.Testure.xeTestureCore;
import xe.Testure.xeTestureDecode;

import xe.Graphical.Widget;

import xe.Audio.Instance.private_Audiodecoder;
import xe.Audio.AudioCore;

extern"C"
{
#include "AL/al.h"
#include "AL/alc.h"
}


int main(int argc, char* argv[])
{
	if (!(xe::Application::LaodApplication(argc, argv, "test.log")))
		return xe::Application::DestroyApplication();
	std::system("chcp 65001");

	ALCdevice* device = alcOpenDevice(nullptr);
	if (!device) {
		std::cerr << "Failed to open OpenAL device" << std::endl;
		return xe::Application::DestroyApplication();
	}

	ALCcontext* context = alcCreateContext(device, nullptr);
	if (!alcMakeContextCurrent(context)) {
		std::cerr << "Failed to make OpenAL context current" << std::endl;
		alcCloseDevice(device);
		return xe::Application::DestroyApplication();
	}

	//auto window = xe::xeWindow();

	//window.CreatWindow(800, 600, "aaaa");
	//window.WindowRendering();
	//window.StartWindowEnvet();

	xe::oMmapfstream file;
	file.OpenFile("C:\\Users\\root\\Desktop\\zero.mp3");
	xe::AudioEncodedData mp3_f;
	mp3_f._size = file.file_size;
	mp3_f.solution = xe::xeAudioCompressSolution::OGG;
	mp3_f.data = xe::xeMalloc<xe::xeByte>(file.file_size);
	file.FstraemStartMemcpyOut<xe::xeUint8>(mp3_f.data, mp3_f._size);

	xe::xeAnyType type_mp3 = nullptr;

	xe::PcmBlock pcm;
	pcm.size = xe::OpenMP3Data(mp3_f.data, mp3_f._size, type_mp3, pcm.format);
	pcm.data = xe::xeMalloc<xe::xeByte>(pcm.size);

	std::vector<std::int8_t> mp3_data;
	

	while (xe::GetMP3Pcm(type_mp3, pcm) == xe::PlayState::_END)
	{
		mp3_data.insert(mp3_data.end(), pcm.data, pcm.data + pcm.size);
	}

	ALuint buffer, source;

	alGenBuffers(1, &buffer);
	alBufferData(buffer, static_cast<ALenum>(pcm.format), mp3_data.data(), mp3_data.size(), 44100);
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);
	// 等待直到播放完毕
	ALint state;
	do {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		alGetSourcei(source, AL_SOURCE_STATE, &state);
	} while (state == AL_PLAYING);

	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcDestroyContext(context);
	alcCloseDevice(device);

	return xe::Application::DestroyApplication();
}
/*
	ALuint buffer, source;
	alGenBuffers(1, &buffer);
	//auto channels = (info.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	alBufferData(buffer, AL_FORMAT_MONO16, audioData.data(), audioData.size(), 44100);
	alGenSources(2, &source);
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);

	// 清理资源
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcDestroyContext(context);
	alcCloseDevice(device);
*/