#include "window/xeWindowManager.h"
#include "log/xeLogOutput.h"
#include <thread>

using namespace xe;

int main()
{
	auto win_s = xe::WindowManager();
	win_s.create_window(1080, 720, "test", true);
	XE_DEBUG_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : SDL3", "text");
	auto str = xeString("test");
	auto win = win_s.get_window(str);
	win->main_loop();
	//std::this_thread::sleep_for(std::chrono::seconds(5));
	return EXIT_SUCCESS;
}