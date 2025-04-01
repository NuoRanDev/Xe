export module xe.Audio.Instance.private_Audiodecoder;

import std;

import xe.Core.xeBaseDataType;
import xe.Core.xeOrdinals;

import xe.Audio.AudioCore;

namespace xe
{
	export enum class PlayState :xeUint32 { _PLAY = 0, _END, _ERROR };

	export xeSize OpenMP3Data(xeByte* mp3_data, xeSize size, xeAnyType& dec_typpe, PcmFormat& pcm_format);

	export PlayState GetMP3Pcm(xeAnyType dec_typpe, PcmBlock& pcmdata);

	export bool MP3Seek(xeAnyType dec_typpe, xeSize point);

	export void CloseMP3Data(xeAnyType dec_typpe);

	export bool OpenOGGData(AudioEncodedData* ogg_data, xeAnyType& dec_typpe, PcmBlock& pcm_block);

	export PlayState GetOGGPcm(xeAnyType dec_typpe, PcmBlock& pcmdata);

	export bool OGGSeek(xeAnyType dec_typpe, xeSize point);

	export void CloseOGGData(xeAnyType dec_typpe);
}