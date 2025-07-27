import std;

import xe.AssetIO.xeCompressFileStruct;
import xe.AssetIO.xeAssetType;


import xe.Core.xeString;
import xe.Core.xeBaseDataType;
import xe.Core.xeApplication;
import xe.Core.xeAlloc;
import xe.Core.xeOrdinals;
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

	xe::oMmapfstream fs;
	fs.OpenFile("C:/Users/root/Desktop/test.flac");

	static xe::PcmBlock pcb;
	static xe::xeAnyTypePtr ptr;

	xe::AudioEncodedData ad = xe::AudioEncodedData(fs.GetFstreamPtr<xe::xeByte>(0), fs.file_size, xe::xeAudioCompressSolution::FLAC, u8"tttt", 5llu);
	xe::OpenFLACData(&ad, &ptr, &pcb);

	while(xe::GetFLACPcm(ptr, &pcb) == xe::PlayState::_PLAY);

	return xe::Application::DestroyApplication();
}
#endif // !defined(EXPORT_C_SHARP_API)