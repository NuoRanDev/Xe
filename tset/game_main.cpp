#include "window/xeWindowManager.hpp"
#include "Rfd.hpp"
#include "log/xeLogOutput.hpp"
#include "filesystem/xeFileMmapStream.hpp"
#include "Rfd.hpp"

#include <format>
#include <iostream>
#include <fstream>

using namespace xe;


int main(int argc, char* argv[])
{
	system("chcp 65001");
	std::vector<const utf8_t *> str_v;
	str_v.push_back(u8"*.txt");
	str_v.push_back(u8"*.doc");
	auto rstr = save(u8"C:\\", u8"文本文件test", str_v.data(), str_v.size());
	String str = rstr;
	free_string(rstr.data);
	std::cout << str << "\n";

	return EXIT_SUCCESS; // Success
}