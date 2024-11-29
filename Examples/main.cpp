#include "XeCore.hpp"
#include "xeAssets.h"
#include "xeWindow.h"

int main(int argc, char* argv[])
{
	xe::Application::LaodApplication(argc, argv, "test.log");
	xe::oMmapfstream file = xe::oMmapfstream();
	return xe::Application::DestroyApplication();
}