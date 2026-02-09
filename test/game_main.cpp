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
	WindowManager wmg = WindowManager(argv[0]);
	String window_title = u8"xe测试";
	auto win = wmg.create_window(1280, 720, window_title);
	win->show();

	return EXIT_SUCCESS; // Success
}