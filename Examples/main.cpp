import xe.xeCore.xeString;
import xe.xeCore.xeBaseDataType;
import xe.xeCore.xeApplication;

import xe.xeIO.xeOMmapfstream;

import xe.xeTesture.xeTestureCore;
import xe.xeTesture.xeTestureDecode;

#include "opencv4/opencv2/opencv.hpp"
#include <cstring>

int main(int argc, char* argv[])
{
	xe::Application::LaodApplication(argc, argv, "test.log");
	std::system("chcp 65001");
	xe::xeString str1 = "C:/Users/root/Desktop/k.png";
	xe::oMmapfstream file;
	file.OpenFile(str1);

	size_t size_img = file.file_size;
	
	auto f_data = std::unique_ptr<xe::TestureFile>(new xe::TestureFile);
	f_data->data  = new xe::xeByte[size_img];
	f_data->encodesolution = xe::xeColorChannel::RGBA;
	f_data->_size = size_img;
	std::memcpy(f_data->data, file.GetFstreamPtr<xe::xeByte>(0), size_img);

	std::unique_ptr<xe::Testure> tex = xe::DeCodeFile(std::move(f_data));
	cv::Mat img = cv::Mat(tex->y, tex->x, CV_8UC4, tex->pixel_data);

	cv::imshow("test", img);
	while (1)
	{
		if (cv::waitKey(1) == 'e')
		{
			break;
		}
	}
	//cv::Mat img = cv::Mat();
	delete[]f_data->data;
	return xe::Application::DestroyApplication();
}