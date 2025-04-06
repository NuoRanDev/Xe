import std;

import xe.Core.xeString;
import xe.Core.xeBaseDataType;
import xe.Core.xeApplication;
import xe.Core.xeAlloc;
import xe.Core.CoreClrOutput;

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
	if (!(xe::Application::LaodApplication(argc, argv)))
		return xe::Application::DestroyApplication();

	const ALCchar* devices = alcGetString(nullptr, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);

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

	
	xe::oMmapfstream file;
	file.OpenFile("C:\\Users\\root\\Desktop\\Destination.ogg");
	xe::AudioEncodedData encoded_audio_data;
	encoded_audio_data._size = file.file_size;
	encoded_audio_data.solution = xe::xeAudioCompressSolution::OGG;
	encoded_audio_data.data = xe::xeMalloc<xe::xeByte>(file.file_size);
	encoded_audio_data.cur_ptr = encoded_audio_data.data;

	file.FstraemStartMemcpyOut<xe::xeUint8>(encoded_audio_data.data, encoded_audio_data._size);

	xe::xeAnyType type_audio = nullptr;

	xe::PcmBlock pcm_block;
	xe::OpenOGGData(&encoded_audio_data, type_audio, pcm_block);

	std::vector<std::int8_t> audio_pcm_data;
	

	while (xe::GetOGGPcm(type_audio, pcm_block) != xe::PlayState::_END)
	{
		audio_pcm_data.insert(audio_pcm_data.end(), pcm_block.data, pcm_block.data + pcm_block.buffer_in);
	}

	ALuint buffer, source;

	alGenBuffers(1, &buffer);
	alBufferData(buffer, static_cast<ALenum>(pcm_block.format), audio_pcm_data.data(), audio_pcm_data.size(), pcm_block.freq);
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	xe::XE_INFO_OUTPUT("Play audio");
	alSourcePlay(source);

	ALint state;

	auto window = xe::xeWindow();

	window.CreatWindow(800, 600, "aaaa");
	window.WindowRendering();
	window.StartWindowEvent();

	//std::this_thread::sleep_for(std::chrono::microseconds(100));
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	xe::XE_INFO_OUTPUT("Play audio end");

	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcDestroyContext(context);
	alcCloseDevice(device);

	xe::xeFree(encoded_audio_data.data);

	return xe::Application::DestroyApplication();
}