import xe.Audio.Instance.private_Audiodecoder;

import std;

import xe.Core.xeBaseDataType;
import xe.Core.CoreClrOutput;
import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;

import xe.Audio.AudioCore;

#include "FLAC/stream_decoder.h"

namespace xe
{
	using FLACType = FLAC__StreamDecoder;

	FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder* decoder, const FLAC__Frame* frame, const FLAC__int32* const buffer[], void* client_data)
	{
		std::puts("Got write callback");
		return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
	}

	void metadata_callback(const FLAC__StreamDecoder* decoder, const FLAC__StreamMetadata* metadata, void* client_data)
	{
		if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO) {
			/* save for later */
			xeUint64 total_samples = metadata->data.stream_info.total_samples;
			xeUint32 sample_rate = metadata->data.stream_info.sample_rate;
			xeUint32 channels = metadata->data.stream_info.channels;
			xeUint32 bps = metadata->data.stream_info.bits_per_sample;

			printf( "sample rate    : %u Hz\n", sample_rate);
			printf( "channels       : %u\n", channels);
			printf( "bits per sample: %u\n", bps);
			printf( "total samples  : %lld \n", total_samples);
		}
	}

	void error_callback(const FLAC__StreamDecoder* decoder, FLAC__StreamDecoderErrorStatus status, void* client_data)
	{
		(void)decoder, (void)client_data;

		printf( "Got error callback: %s\n", FLAC__StreamDecoderErrorStatusString[status]);
	}

	FLAC__StreamDecoderReadStatus ReadFLACDecodedData(const FLAC__StreamDecoder* decoder, xeByte* buffer, size_t* byte_size, xeAnyTypePtr block_data)
	{
		if (*byte_size > 0)
		{
			*byte_size = ReadAudioDecodedData(buffer, *byte_size, 1, block_data);
			if (*byte_size == 0)
				return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
			else
				return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
		}
		else
			return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
	}

	FLAC__StreamDecoderSeekStatus SeekFLACDecodedData(const FLAC__StreamDecoder* decoder, xeUint64 absolute_byte_offset, xeAnyTypePtr block_data)
	{
		if (SeekAudioDecodedData(block_data, absolute_byte_offset, SEEK_SET) < 0)
			return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
		else
			return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
	}

	FLAC__StreamDecoderTellStatus TellFLACDecodeData(const FLAC__StreamDecoder* decoder, FLAC__uint64* absolute_byte_offset, xeAnyTypePtr block_data)
	{
		long pos = 0;
		if ((pos = TellAudioDecodedData(block_data)) < 0)
			return FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
		else 
		{
			*absolute_byte_offset = static_cast<FLAC__uint64>(pos);
			return FLAC__STREAM_DECODER_TELL_STATUS_OK;
		}
	}

	FLAC__StreamDecoderLengthStatus LengthFLACDecodeData(const FLAC__StreamDecoder* decoder, FLAC__uint64* stream_length, xeAnyTypePtr block_data) { return FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR; }

	FLAC__bool EOFFLACDecodeData(const FLAC__StreamDecoder* decoder, xeAnyTypePtr block_data)
	{
		return IsAudioDecodedDataEOF(block_data);
	}

	bool OpenFLACData(AudioEncodedData* flac_data, xeAnyTypePtr& dec_typpe, PcmBlock& pcm_block)
	{
		FLAC__StreamDecoderInitStatus init_status;
		FLACType* dec_flac_type = FLAC__stream_decoder_new();

		if (dec_flac_type == nullptr)
		{
			XE_ERROR_OUTPUT("<LIB :FLAC> allocating decoder");
			return false;
		}
		FLAC__stream_decoder_set_md5_checking(dec_flac_type, true);

		init_status = FLAC__stream_decoder_init_stream(dec_flac_type,
			ReadFLACDecodedData,
			SeekFLACDecodedData,
			TellFLACDecodeData,
			LengthFLACDecodeData,
			EOFFLACDecodeData, write_callback, metadata_callback, error_callback, flac_data);
		
		if (init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK) 
		{
			XE_ERROR_OUTPUT(std::format("<LIB :FLAC> initializing decoder: {0}", FLAC__StreamDecoderInitStatusString[init_status]).c_str());
			return false;
		}
		bool ok = FLAC__stream_decoder_process_until_end_of_stream(dec_flac_type);
		return true;
	}

	PlayState GetFLACPcm(xeAnyTypePtr dec_typpe, PcmBlock& pcm_block)
	{
		return PlayState();
	}

	bool FLACSeek(xeAnyTypePtr dec_typpe, xeSize pos)
	{
		return true;
	}

	void CloseFLACata(AudioEncodedData* flac_data, xeAnyTypePtr dec_typpe)
	{
		auto dec_flac_type = reinterpret_cast<FLACType*>(dec_typpe);

		FLAC__stream_decoder_delete(dec_flac_type);
		CloseAudioDecodedData(flac_data);
	}
}