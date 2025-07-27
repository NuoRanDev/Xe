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
	static int ooo = 0;
	static std::mutex oool;

	struct FlacDataIO
	{
		PcmBlock* pcm_data;
		AudioEncodedData* audio_data;
	};

	struct FLACType 
	{
		FLAC__StreamDecoder *stream;
		FlacDataIO *data;
	};

	FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder* decoder, const FLAC__Frame* frame, const FLAC__int32* const buffer[], void* block_data)
	{
		auto io_ptr_data = reinterpret_cast<FlacDataIO*>(block_data);
		oool.lock();
		std::printf("%d: Got write callback\n", ooo);
		oool.unlock();
		if(frame->header.number.sample_number == 0)
		{
		}
		return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
	}

	void MetaData(const FLAC__StreamDecoder* decoder, const FLAC__StreamMetadata* metadata, void* block_data)
	{
		auto io_ptr_data = reinterpret_cast<FlacDataIO*>(block_data);
		if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO) {
			/* save for later */
			io_ptr_data->pcm_data->size = metadata->data.stream_info.total_samples;
			io_ptr_data->pcm_data->freq = metadata->data.stream_info.sample_rate;
			io_ptr_data->pcm_data->channel = metadata->data.stream_info.channels;

			if (io_ptr_data->pcm_data->channel > 1) io_ptr_data->pcm_data->format = PcmFormat::FORMAT_STEREO16;
			else { io_ptr_data->pcm_data->format = PcmFormat::FORMAT_MONO16; }

			io_ptr_data->pcm_data->buffer_in = metadata->data.stream_info.bits_per_sample;

			printf( "sample rate    : %u Hz\n", io_ptr_data->pcm_data->freq);
			printf( "channels       : %llu\n", io_ptr_data->pcm_data->channel);
			//printf( "bits per sample: %u\n", bps);
			printf( "total samples  : %lld \n", io_ptr_data->pcm_data->size);
		}
	}

	void error_callback(const FLAC__StreamDecoder* decoder, FLAC__StreamDecoderErrorStatus status, void* client_data)
	{
		(void)decoder, (void)client_data;

		printf( "Got error callback: %s\n", FLAC__StreamDecoderErrorStatusString[status]);
	}

	FLAC__StreamDecoderReadStatus ReadFLACDecodedData(const FLAC__StreamDecoder* decoder, xeByte* buffer, size_t* byte_size, xeAnyTypePtr block_data)
	{
		auto io_ptr_data = reinterpret_cast<FlacDataIO*>(block_data);
		if (*byte_size > 0)
		{
			*byte_size = ReadAudioDecodedData(buffer, *byte_size, 1, io_ptr_data->audio_data);
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
		auto io_ptr_data = reinterpret_cast<FlacDataIO*>(block_data);
		if (SeekAudioDecodedData(io_ptr_data->audio_data, absolute_byte_offset, SEEK_SET) < 0)
			return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
		else
			return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
	}

	FLAC__StreamDecoderTellStatus TellFLACDecodeData(const FLAC__StreamDecoder* decoder, FLAC__uint64* absolute_byte_offset, xeAnyTypePtr block_data)
	{
		long pos = 0;
		auto io_ptr_data = reinterpret_cast<FlacDataIO*>(block_data);
		if ((pos = TellAudioDecodedData(io_ptr_data->audio_data)) < 0)
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
		auto io_ptr_data = reinterpret_cast<FlacDataIO*>(block_data);
		return IsAudioDecodedDataEOF(io_ptr_data->audio_data);
	}

	bool OpenFLACData(AudioEncodedData* flac_data, xeAnyTypePtr* pdec_typpe, PcmBlock* pcm_block)
	{
		FLAC__StreamDecoderInitStatus init_status;
		FLACType* dec_flac_type = xeMalloc<FLACType>(1);

		dec_flac_type->stream =	FLAC__stream_decoder_new();
		dec_flac_type->data = xeMalloc<FlacDataIO>(1);
		dec_flac_type->data->audio_data = flac_data;
		dec_flac_type->data->pcm_data = pcm_block;

		if (dec_flac_type->stream == nullptr)
		{
			XE_ERROR_OUTPUT("<LIB :FLAC> allocating decoder");
			return false;
		}
		FLAC__stream_decoder_set_md5_checking(dec_flac_type->stream, true);

		init_status = FLAC__stream_decoder_init_stream(dec_flac_type->stream,
			ReadFLACDecodedData,
			SeekFLACDecodedData,
			TellFLACDecodeData,
			LengthFLACDecodeData,
			EOFFLACDecodeData, write_callback, MetaData, error_callback, dec_flac_type->data);
		
		if (init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK) 
		{
			XE_ERROR_OUTPUT(std::format("<LIB :FLAC> initializing decoder: {0}", FLAC__StreamDecoderInitStatusString[init_status]).c_str());
			return false;
		}
		//bool ok = FLAC__stream_decoder_process_until_end_of_stream(dec_flac_type->stream);
		*pdec_typpe = reinterpret_cast<xeAnyTypePtr>(dec_flac_type);
		return true;
	}

	PlayState GetFLACPcm(xeAnyTypePtr dec_typpe, PcmBlock* pcm_block)
	{
		auto dec_flac_type = reinterpret_cast<FLACType*>(dec_typpe);
		auto state = 0;
		FLAC__stream_decoder_process_single(dec_flac_type->stream);
		if (state)
		{
			return PlayState::_END;
		}
		FLAC__stream_decoder_process_single(dec_flac_type->stream);
		//std::this_thread::sleep_for(std::chrono::seconds(5));
		oool.lock();
		ooo++;
		oool.unlock();
		return PlayState::_PLAY;
	}

	bool FLACSeek(xeAnyTypePtr dec_typpe, xeSize pos)
	{
		return true;
	}

	void CloseFLACata(AudioEncodedData* flac_data, xeAnyTypePtr dec_typpe)
	{
		auto dec_flac_type = reinterpret_cast<FLACType*>(dec_typpe);

		FLAC__stream_decoder_delete(dec_flac_type->stream);
		xeFree(dec_flac_type->data);
		xeFree(dec_flac_type);
		CloseAudioDecodedData(flac_data);
	}
}