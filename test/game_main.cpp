#include "window/xeWindowManager.hpp"
#include "Rfd.hpp"
#include "log/xeLogOutput.hpp"
#include "filesystem/xeFileMmapStream.hpp"
#include "xeFileDialog.hpp"

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

	String path = "C:\\Users\\wunuo\\Desktop\\class - 副本.cpp";
	auto c = Path::base_name(path);
	time_t create_time = static_cast<time_t>(Path::get_create_time(path));

	std::cout << "File : " << path << std::endl;
	std::cout << "Create time : " << std::ctime(&create_time);
	std::cout << "Size : " << Path::get_size(path) / 1024 << " KiB " << std::endl;

	return EXIT_SUCCESS; // Success
}