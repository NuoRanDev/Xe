export module xe.Audio.Instance.private_Audiodecoder;

import std;

import xe.Core.xeBaseDataType;
import xe.Core.xeOrdinals;

import xe.Audio.AudioCore;

namespace xe
{
	// mp3
#if defined(USE_MP3)
	export bool OpenMP3Data(AudioEncodedData* mp3_data, xeAnyTypePtr* pdec_typpe, PcmBlock* pcm_block);

	export PlayState GetMP3Pcm(xeAnyTypePtr dec_typpe, PcmBlock* pcm_block);

	export bool MP3Seek(xeAnyTypePtr dec_typpe, xeSize pos);

	export void CloseMP3Data(AudioEncodedData* mp3_data, xeAnyTypePtr dec_typpe);
#endif // defined(USE_MP3)

	// ogg
#if !defined(NOT_USE_OGG)
	export bool OpenOGGData(AudioEncodedData* ogg_data, xeAnyTypePtr* pdec_typpe, PcmBlock* pcm_block);

	export PlayState GetOGGPcm(xeAnyTypePtr dec_typpe, PcmBlock* pcm_block);

	export bool OGGSeek(xeAnyTypePtr dec_typpe, xeSize pos);

	export void CloseOGGData(AudioEncodedData* ogg_data, xeAnyTypePtr dec_typpe);
#endif // !defined(NOT_USE_OGG)

	// flac
	export bool OpenFLACData(AudioEncodedData* flac_data, xeAnyTypePtr& dec_typpe, PcmBlock& pcm_block);

	export PlayState GetFLACPcm(xeAnyTypePtr dec_typpe, PcmBlock& pcm_block);

	export bool FLACSeek(xeAnyTypePtr dec_typpe, xeSize pos);

	export void CloseFLACata(AudioEncodedData* flac_data, xeAnyTypePtr dec_typpe);
}