#include "window/xeWindowManager.hpp"
#include "Rfd.hpp"
#include "log/xeLogOutput.hpp"
#include "filesystem/xeFileMmapStream.hpp"
#include "xeFileDialog.hpp"
#include "file/audio/xeReadAudioFile.hpp"

#include "FAudio.h"

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

	puts("bbbbb");
	flac_file.release();

	return EXIT_SUCCESS; // Success
}