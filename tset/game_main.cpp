#include "window/xeWindowManager.hpp"
#include "xeRfd.hpp"
#include "log/xeLogOutput.hpp"
#include "filesystem/xeFileMmapStream.hpp"

#include <format>
#include <iostream>
#include <fstream>

using namespace xe;

int main(int argc, char* argv[])
{
	system("chcp 65001");
	//WindowManager win_msg = WindowManager(argv[0]);
	//win_msg.create_window(800, 600, u8"Test Window", true);

	//auto win = win_msg.get_window(u8"Test Window");
	//win->draw_loop();

	RustString save_path = save_file();
	xeString rust_path_str = save_path;
	free_string(save_path.data);

	Path path = rust_path_str;

	std::cout << rust_path_str << std::endl;

	Mmapfstream fs = Mmapfstream();
	fs.open_file(path);
	char str_c[6] = { 0 };
	fs.read(0, 5, str_c);

	std::cout << str_c << std::endl;


	return EXIT_SUCCESS; // Success
}