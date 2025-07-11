import std;

import xe.AssetIO.xeCompressFileStruct;
import xe.AssetIO.xeAssetType;


import xe.Core.xeString;
import xe.Core.xeBaseDataType;
import xe.Core.xeApplication;
import xe.Core.xeAlloc;
import xe.Core.CoreClrOutput;

import xe.IO.xeOMmapfstream;

import xe.Testure.xeTestureCore;
import xe.Testure.xeTestureDecode;

import xe.Graphical.Widget;

import xe.Audio.Instance.private_Audiodecoder;
import xe.Audio.AudioCore;

#if !defined(EXPORT_C_SHARP_API)
int main(int argc, char* argv[])
{
	if (!(xe::Application::LaodApplication(argc, argv)))
		return xe::Application::DestroyApplication();

	xe::xeWindow win = xe::xeWindow();

	win.CreatWindow(400, 400, "xeTesture", false);
	win.WindowRendering();

	

	return xe::Application::DestroyApplication();
}
#endif // !defined(EXPORT_C_SHARP_API)