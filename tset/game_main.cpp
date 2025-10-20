#include "window/xeWindowManager.hpp"
#include "log/xeLogOutput.hpp"
#include "filesystem/xeFileMmapStream.hpp"
#include "file/image/xeReadImage.hpp"
#include "testure/xeTesture.hpp"
#include <thread>
#include <iostream>


using namespace xe;
Texture *tex1;
Texture* tex2;

bool render1(Window *win)
{
	return tex1->draw(0.0f, 0.0f, static_cast<float>(win->get_width()), static_cast<float>(win->get_height()));
}

bool render2(Window* win)
{
	return tex2->draw(static_cast<float>(win->get_width()) / 2.0, static_cast<float>(win->get_height()) / 2.0,
		static_cast<float>(win->get_width()) / 3.0, static_cast<float>(win->get_height()) / 3.0);
}

int main()
{
	auto win_s = xe::WindowManager();
	win_s.create_window(1080, 720, "test", true);
	XE_DEBUG_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : SDL3", "text");
	auto str = xeString("test");
	auto win = win_s.get_window(str);

	Mmapfstream fs1;
	if(!fs1.open_file("F:\\BGS\\test.jpg"))
		return EXIT_FAILURE;
	ImageFile imgf1;
	uint64_t f_size = 0;
	byte_t* f_data1 = fs1.data(f_size);
	imgf1.load_file_not_copy_in_memory(f_data1, f_size, "ccccc");
	Image img1;
	if(!read_memory_jpg_image(imgf1, img1))
		return EXIT_FAILURE;
	tex1 = new Texture(win);
	//fs1.release();
	tex1->load_image(img1);

	

	Mmapfstream fs2;
	if (!fs2.open_file("F:\\BGS\\test.png"))
		return EXIT_FAILURE;
	ImageFile imgf2;
	f_size = 0;
	byte_t* f_data2 = fs2.data(f_size);
	imgf2.load_file_not_copy_in_memory(f_data2, f_size, "ccccc");
	Image img2;
	if (!read_memory_png_image(imgf2, img2))
		return EXIT_FAILURE;
	tex2 = new Texture(win);
	tex2->load_image(img2);

	
	win->submit_draw_command(&render1, false);
	win->submit_draw_command(&render2, false);
	

	win->draw_loop();
	delete tex1;
	//std::this_thread::sleep_for(std::chrono::seconds(5));
	return EXIT_SUCCESS;
}