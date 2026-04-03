#include "window/xeWindowManager.hpp"
#include "Rfd.hpp"
#include "log/xeLogOutput.hpp"
#include "filesystem/xeFileMmapStream.hpp"
#include "xeFileDialog.hpp"
#include "file/audio/xeReadAudioFile.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <format>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace xe;
int main(int argc, char* argv[])
{
#if defined(_WIN32)
	system("chcp 65001");
	system("cls");
#endif

	String path = "C:\\Users\\wunuo\\Desktop\\In the HALL OF THE MOU - PHILHARMONIA ORCHESTRA of Hamburg.flac";
	auto c = Path::base_name(path);
	time_t create_time = static_cast<time_t>(Path::get_create_time(path));

	std::cout << "File : " << path << std::endl;
	// std::cout << "Create time : " << std::ctime(&create_time);
	std::cout << "Size : " << Path::get_size(path) / 1024 << " KiB " << std::endl;

	Mmapfstream flac_file = Mmapfstream();
	flac_file.open_file(path);

	AudioFile adf;
	size_t sz;
	auto flac_data = flac_file.data(sz);
	adf.load_file_not_copy_in_memory(flac_data, sz, "Flac test");

	PcmBlock pcm;
	read_memory_flac_audio_all_pcm(adf, pcm);
	flac_file.release();


	ALCdevice* device = NULL;
	ALCcontext* context = NULL;
	device = alcOpenDevice(NULL);
	if (!device) {
		fprintf(stderr, "fail to open device\n");
		return -1;
	}
	context = alcCreateContext(device, NULL);
	if (!context) {
		fprintf(stderr, "fail to create context.\n");
		return -1;
	}
	alcMakeContextCurrent(context);
	if (alGetError() != AL_NO_ERROR) {
		return -1;
	}
	//音频播放源
	ALuint source;
	//音频数据
	ALuint buffer;
	//音频格式 AL_FORMAT_STEREO
	ALenum audioFormat = AL_FORMAT_MONO_FLOAT32;
	//声道数目
	ALshort channels = pcm.header.channels;
	//是否循环播放
	ALboolean loop = 1;
	//播放源的位置
	ALfloat position[] = { 0.0f,0.0f,0.0f };
	//播放的速度
	ALfloat velocity[] = { 0.0f,0.0f,0.0f };
	alGenBuffers(1, &buffer);
	alGenSources(1, &source);
	alBufferData(buffer, audioFormat, pcm.pcm_data, pcm.data_size,  pcm.header.sample_rate);

	if (alGetError() != AL_NO_ERROR) 
	{
		return -1;
	}
	//为source绑定数据
	alSourcei(source, AL_BUFFER, buffer);
	//音高倍数
	alSourcef(source, AL_PITCH, 1.0f);
	//声音的增益
	alSourcef(source, AL_GAIN, 1.0f);
	//设置位置
	alSourcefv(source, AL_POSITION, position);
	//设置声音移动速度
	alSourcefv(source, AL_VELOCITY, velocity);
	//设置是否循环播放
	alSourcei(source, AL_LOOPING, loop);
	//播放音乐
	alSourcePlay(source);
	std::this_thread::sleep_for(std::chrono::minutes(1000));
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcDestroyContext(context);
	alcCloseDevice(device);
	return 0;

	return EXIT_SUCCESS; // Success
}