#include "XeCore.hpp"
#include "xeAssets.h"
#include "xeWindow.h"

int main(int argc, char* argv[])
{
	xe::Application::LaodApplication(argc, argv, "test.log");
	xe::ioMmapfstream file = xe::ioMmapfstream();
	file.GetFilePtr(L"C:\\Users\\Wunuo\\Desktop\\main.cpp");
	std::cout << file.GetFstreamPtr<char>(0) << "\n";
	return xe::Application::DestroyApplication();
}