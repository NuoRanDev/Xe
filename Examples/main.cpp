import std;

import xe.AssetIO.xeCompressFileStruct;

int main()
{
	std::cout << "IMAGE_ASSET_FILE_HEADER=" << xe::xeAssetFileType::IMAGE_ASSET_FILE_HEADER << ",\n";
	std::cout << "AUDIO_ASSET_FILE_HEADER=" << xe::xeAssetFileType::AUDIO_ASSET_FILE_HEADER << ",\n";
	std::cout << "VIDEO_ASSET_FILE_HEADER=" << xe::xeAssetFileType::VIDEO_ASSET_FILE_HEADER << ",\n";
	std::cout << "TEXT_ASSET_FILE_HEADER=" << xe::xeAssetFileType::TEXT_ASSET_FILE_HEADER << ",\n";
	std::cout << "MODEL_ASSET_FILE_HEADER=" << xe::xeAssetFileType::MODEL_ASSET_FILE_HEADER << ",\n";
	std::cout << "SHADER_ASSET_FILE_HEADER=" << xe::xeAssetFileType::SHADER_ASSET_FILE_HEADER << ",\n";
	return 0;
}


/*
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



	xe::oMmapfstream file;
	file.OpenFile("C:\\Users\\root\\Desktop\\zero.mp3");
	xe::AudioEncodedData encoded_audio_data;
	encoded_audio_data.size = file.file_size;
	encoded_audio_data.solution = xe::xeAudioCompressSolution::OGG;
	encoded_audio_data.data = xe::xeMalloc<xe::xeByte>(file.file_size);
	encoded_audio_data.cur_ptr = encoded_audio_data.data;

	file.FstraemStartMemcpyOut<xe::xeUint8>(encoded_audio_data.data, encoded_audio_data.size);

	xe::xeAnyTypePtr type_audio = nullptr;

	xe::PcmBlock pcm_block;
	//xe::OpenOGGData(&encoded_audio_data, type_audio, pcm_block);
	xe::OpenMP3Data(&encoded_audio_data, &type_audio, &pcm_block);

	std::vector<std::int8_t> audio_pcm_data;

	while (xe::GetMP3Pcm(type_audio, &pcm_block) != xe::PlayState::_END)
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

	xe::CloseMP3Data(&encoded_audio_data, type_audio);

	return xe::Application::DestroyApplication();
}
*/