export module xe.Audio.Instance.private_Audiodecoder;

import std;

import xe.Core.xeBaseDataType;
import xe.Core.xeOrdinals;

import xe.Audio.AudioCore;

namespace xe
{
	export xeSize OpenMP3Data(std::unique_ptr<AudioEncodedData> mp3_data, xeAnyType dec_typpe);

	export bool GetMP3Pcm(xeAnyType dec_typpe, std::unique_ptr<PcmBlock> pcmdata);

	export bool MP3Seek(xeAnyType dec_typpe, xeSize point);

	export void CloseMP3Data(xeAnyType dec_typpe);
}