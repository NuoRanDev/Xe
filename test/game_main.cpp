#include "window/xeWindowManager.hpp"
#include "Rfd.hpp"
#include "log/xeLogOutput.hpp"
#include "filesystem/xeFileMmapStream.hpp"
#include "xeAudioPlayerInstance.hpp"
#include "xeSoundSource.hpp"
#include "file/audio/xeReadAudioFile.hpp"

#include <format>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

#define  _CRT_SECURE_NO_WARNINGS

using namespace xe;
int main(int argc, char** argv)
{
#if defined(_WIN32)
	system("chcp 65001");
	system("cls");
#endif

	String spath = u8"C:\\Users\\wunuo\\Desktop\\audio.flac";
	//String path = "C:\\Users\\wunuo\\Desktop\\In the HALL OF THE MOU - PHILHARMONIA ORCHESTRA of Hamburg.flac";
	Path path = Path(spath);
	time_t create_time = path.get_create_time();

	std::cout << "File : " << spath << std::endl;
	std::cout << "Create time : " << std::ctime(&create_time);
	std::cout << "Size : " << path.get_size() / 1024 << " KiB " << std::endl;

	Mmapfstream flac_file = Mmapfstream();
	flac_file.open_file(path);

	AudioFile adf;
	size_t sz;
	String u8_s = u8"Flac test";
	auto flac_data = flac_file.data(sz);
	adf.load_file_not_copy_in_memory(flac_data, sz, u8_s);

	PcmBlock pcm;
	read_memory_flac_audio_all_pcm(adf, pcm);
	flac_file.release();

	AudioPlayerInstance a_instc;
	a_instc.init();
	
	SoundSource sscrc = SoundSource();
	sscrc.lood_audio(pcm);
	sscrc.set_gain(1.0f);
	sscrc.set_is_loop(true);
	sscrc.set_pitch(1.0f);
	
	vec3f v;
	vec3f pos;
	float pi = 3.1415926f;
	float w = pi / 4;
	float r = 10;

	auto start = std::chrono::system_clock::now();
	std::chrono::duration<double>diff;
	
	sscrc.play();
	using std::cout, std::endl;
	using std::chrono::duration_cast, std::chrono::system_clock;
	using std::chrono::milliseconds, std::chrono::seconds;

	while(true)
	{
		auto ms_cpp =
			duration_cast<milliseconds>(system_clock::now().time_since_epoch())
			.count();
		double ms = (double)ms_cpp / 1000.0 ;
		double st = (w * ms) - ((long long)((w * ms) / (2 * pi)) * (2 * pi));
		v = { w * r * cosf((float)(st)-pi / 2) ,
			w * r * sinf((float)(st)-pi / 2),
			0
		};
		sscrc.set_velocity(v);
		pos = { cosf((float)st) * r ,
			 sinf((float)st) * r,
			0
		};
		std::cout << "v Vx: " << v.x << " Vy: " << v.y << " Vz: " << v.z << "\n";
		std::cout << "pos x: " << pos.x << " y: " << pos.y << " z: " << pos.z << "\n";
		// windos
		system("cls");
		// linux
		//system("clear");
		sscrc.set_position(pos);
	}
	/*
	xe::WindowManager wmsg = xe::WindowManager(argv);
	auto name = xe::String("sasas");
	auto win = wmsg.create_window(600, 800, name);
	win->show();*/

	
	return EXIT_SUCCESS; // Success
}