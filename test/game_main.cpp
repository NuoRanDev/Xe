#include "window/xeWindowManager.hpp"
#include "Rfd.hpp"
#include "log/xeLogOutput.hpp"
#include "filesystem/xeFileMmapStream.hpp"
#include "xeFileDialog.hpp"

#include <format>
#include <iostream>
#include <fstream>

using namespace xe;


int main(int argc, char* argv[])
{
#if defined(_WIN32)
	system("chcp 65001");
	system("cls");
#endif
	String f_s = u8"C:\\Users\\wunuo\\Desktop\\unzip.py";
	Mmapfstream fs;
	fs.open_file(f_s);
	auto p = fs.get_mmap_ptr();
	char py_code[3];
	py_code[0] = p[0];
	py_code[1] = p[1];
	py_code[2] = 0;
	std::cout << py_code << std::endl;

	WindowManager wmg = WindowManager(argv[0]);
	String window_title = nullptr;
	auto win = wmg.create_window(1280, 720, window_title, true);
	win->show();

	return EXIT_SUCCESS; // Success
}