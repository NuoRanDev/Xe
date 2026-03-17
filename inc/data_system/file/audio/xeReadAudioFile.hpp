#ifndef _XE_RWAD_AUDIO_HPP_
#define _XE_RWAD_AUDIO_HPP_

#include "type/xeOrdinals.hpp"
#include "file/audio/xeAudioFile.hpp"

namespace xe
{
#if defined(USE_FLAC)
	bool read_memory_flac_audio(const AudioFile& file, int& img_out) noexcept;
#endif // USE_AVIF
#if defined(USE_OGG)
	bool read_memory_ogg_audio(const AudioFile& file, int& img_out) noexcept;
#endif // USE_AVIF
	bool read_memory_pcm_audio(const AudioFile& file, int& img_out) noexcept
	{
	}

} // namespace xe is end

#endif // _XE_RWAD_AUDIO_HPP_ IS EOF