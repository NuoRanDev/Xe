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
	system("chcp 65001");
	std::vector<String> str_v;
	str_v.push_back(String(u8"txt"));
	str_v.push_back(String(u8"doc"));

	Path dir = String(u8"C:\\");

	FileDialog fd = FileDialog();
	fd.set_directory(dir);
	fd.add_filter(String(u8"文本系统 , text list"), str_v);

	String str = fd.save_file().path_str;
	std::cout << str << "\n";

	return EXIT_SUCCESS; // Success
}