import std;

import xe.Core.xeString;
import xe.Core.xeBaseDataType;
import xe.Core.xeApplication;
import xe.Core.xeAlloc;

import xe.IO.xeOMmapfstream;

import xe.Testure.xeTestureCore;
import xe.Testure.xeTestureDecode;

import xe.Graphical.Widget.Window;

int main(int argc, char* argv[])
{
	if(!(xe::Application::LaodApplication(argc, argv, "test.log")))
		return xe::Application::DestroyApplication();
	std::system("chcp 65001");

	auto window = xe::xeWindow();

	window.CreatWindow(800, 600, "aaaa");
	window.WindowRendering();
	//window.StartWindowEnvet();

	return xe::Application::DestroyApplication();
}