#include "window/xeWindowManager.hpp"
#include "log/xeLogOutput.hpp"
#include "filesystem/xeFileMmapStream.hpp"
#include "file/image/xeReadImage.hpp"
#include "testure/xeTesture.hpp"
#include <thread>
#include <iostream>


using namespace xe;
Texture *tex;

bool render(Window *win)
{
	return tex->draw(0, 0, win->get_width(), win->get_height());
}

int main()
{
	auto win_s = xe::WindowManager();
	win_s.create_window(1080, 720, "test", true);
	XE_DEBUG_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : SDL3", "text");
	auto str = xeString("test");
	auto win = win_s.get_window(str);

	Mmapfstream fs;
	if(!fs.open_file("F:\\BGS\\1758303716609.jpg"))
		return EXIT_FAILURE;

	ImageFile imgf;
	uint64_t f_size = 0;

	byte_t* f_data = fs.data(f_size);

	imgf.load_file_not_copy_in_memory(f_data, f_size, "ccccc");

	Image img;

	if(!read_memory_jpg_image(imgf, img))
		return EXIT_FAILURE;

	tex = new Texture(*win);
	std::cout << "aaaa\n";
	std::cout << (uint32_t)(img.data()[21]);
	tex->load_image(img);

	win->submit_draw_command(render, false);

	win->draw_loop();
	delete tex;
	//std::this_thread::sleep_for(std::chrono::seconds(5));
	return EXIT_SUCCESS;
}