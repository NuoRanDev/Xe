import xe.Core.xeString;
import xe.Core.xeBaseDataType;
import xe.Core.xeApplication;
import xe.Core.xeAlloc;

import xe.IO.xeOMmapfstream;

import xe.Testure.xeTestureCore;
import xe.Testure.xeTestureDecode;

import xe.Graphical.Widget.Window;

import std;


int main(int argc, char* argv[])
{
	if(!(xe::Application::LaodApplication(argc, argv, "test.log")))
		return xe::Application::DestroyApplication();
	std::system("chcp 65001");
	xe::xeString str1 = "C:/Users/root/Desktop/k.png";
	xe::oMmapfstream file;
	file.OpenFile(str1);

	size_t size_img = file.file_size;
	
	auto f_data = std::unique_ptr<xe::TestureFile>(new xe::TestureFile);
	f_data->data  = xe::xeMalloc<xe::xeByte>(size_img);
	f_data->encodesolution = xe::xeColorChannel::RGBA;
	f_data->_size = size_img;
	std::memcpy(f_data->data, file.GetFstreamPtr<xe::xeByte>(0), size_img);

	std::unique_ptr<xe::Testure> tex = xe::DeCodeFile(std::move(f_data));

	auto window = xe::xeWindow();

	window.CreatWindow(800, 600, "aaaa");
	window.InitWindowVulkan();

	return xe::Application::DestroyApplication();
}