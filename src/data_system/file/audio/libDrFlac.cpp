#include "log/xeLogOutput.hpp"
#include "type/xeOrdinals.hpp"

#include "file/audio/xeReadAudioFile.hpp"

#include <type_traits>
#include <format>

#define DRFLAC_IMPLEMENTATION
extern"C" 
{
#include "dr_flac.h"
}

namespace xe
{
	static void* dr_malloc(size_t sz, void* _none) noexcept
	{
		return xe_malloc<byte_t>(sz);
	}

	static void dr_free(void* p, void* _none) noexcept
	{
		xe_free(p);
	}

	static void* dr_realloc(void* p, size_t sz, void* _none) noexcept
	{
		return xe_realloc<byte_t>(reinterpret_cast<byte_t*>(p), sz);
	}

	static drflac_allocation_callbacks dr_flac_alloc_cb_struct =
	{
		.pUserData = nullptr,
		.onMalloc = dr_malloc,
		.onRealloc = dr_realloc,
		.onFree = dr_free,
	};

	static void cb_dr_free(void* data)
	{
		drflac_free(data, &dr_flac_alloc_cb_struct);
	}

	template<typename T> 
	byte_t* type_drflac_full_read(drflac* flac_context, uint64_t& size,
		drflac_uint64(*drflac_read_pcm_frames_type_extension)(drflac*, drflac_uint64, T*))
	{
		uint64_t block_pcm_frame_number =  ((flac_context->maxBlockSizeInPCMFrames > 0) ? flac_context->maxBlockSizeInPCMFrames : 4096);
		//uint64_t block_size = block_pcm_frame_number * sizeof(T);
		uint64_t need_byte_size = flac_context->totalPCMFrameCount * sizeof(T);

		XE_INFO_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeAudio :DecFlac", std::format("size: {0} MB", need_byte_size / 1024 / 1024 ).c_str());
		
		T* out_data = xe_malloc<T>(need_byte_size);
		T* cur_data = out_data;

		for (uint64_t i = 0; i < flac_context->totalPCMFrameCount;)
		{
			auto read_size = drflac_read_pcm_frames_type_extension(flac_context, block_pcm_frame_number, out_data);
			if (read_size < 0)
			{
				XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeAudio :DecFlac", "File is broken!");
				drflac_close(flac_context);
				return reinterpret_cast<byte_t*>(out_data);
			}
			cur_data += read_size / sizeof(T);
			i += read_size;
		}

		drflac_close(flac_context);
		return reinterpret_cast<byte_t*>(out_data);
	}

	drflac* read_memory_flac_audio_info(size_t flac_size, const byte_t* flac_data, PcmHeader& pcmh)
	{
		drflac* flac_context = drflac_open_memory(flac_data, flac_size, &dr_flac_alloc_cb_struct);
		if (flac_context == nullptr)
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeAudio :DecFlac", "Can't read memory file!");
			return nullptr;
		}
		pcmh.channels = flac_context->channels;
		pcmh.sample_rate = flac_context->sampleRate;
		pcmh.sample_type = U;
		return flac_context;
	}

	bool read_memory_flac_audio_all_pcm(const AudioFile& file, PcmBlock& pcm_out) noexcept
	{
		PcmHeader pcmh;
		drflac* flac_context;
		size_t flac_size = 0;
		const byte_t* flac_data = file.get_file_data(flac_size);

		if (flac_size == 0 || flac_data == nullptr)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeAudio: DecFlac", "Can't read memory file!");
			return false;
		};

		flac_context = read_memory_flac_audio_info(flac_size, flac_data, pcmh);
		if (flac_context == nullptr)
			return false;

		switch(flac_context->bitsPerSample)
		{
		case 8:
		case 16:
			pcm_out.header.sample_bit_size = PCM_SAMPLE_BIT_SIZE::L16;
			goto UINT16_PCM_DEC;
			break;

		case 24:
		case 32:
			pcm_out.header.sample_bit_size = PCM_SAMPLE_BIT_SIZE::L32;
			goto UINT32_PCM_DEC;
			break;

		default:
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeAudio :DecFlac", "format not support!");
			return false;
		}

		drflac_open_and_read_pcm_frames_f32;

	UINT16_PCM_DEC:
		//drflac_open_and_read_pcm_frames_f32();
		pcm_out.pcm_data = type_drflac_full_read<drflac_int16>(flac_context, pcm_out.data_size, &(drflac_read_pcm_frames_s16));
		if (pcm_out.pcm_data == nullptr)
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeAudio :DecFlac", "Decode is failed!");
			return false;
		}
		return true;
	UINT32_PCM_DEC:

		pcm_out.pcm_data = type_drflac_full_read<drflac_int32>(flac_context, pcm_out.data_size, &(drflac_read_pcm_frames_s32));
		if (pcm_out.pcm_data == nullptr)
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeAudio :DecFlac", "Decode is failed!");
			return false;
		}
		return true;
	}
} // namespace xe is end