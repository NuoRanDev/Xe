#ifndef _XE_RWAD_AUDIO_HPP_
#define _XE_RWAD_AUDIO_HPP_

#include "type/xeOrdinals.hpp"
#include "file/audio/xeAudioFile.hpp"
#include "audio/xeAudio.hpp"

namespace xe
{
#if defined(USE_FLAC)
#endif // USE_FLAC
	bool read_memory_flac_audio_all_pcm(const AudioFile& file, PcmBlock& pcm_out) noexcept;
#if defined(USE_OGG)
	bool read_memory_ogg_audio_pcm(const AudioFile& file, PcmBlock& audio_out) noexcept;
#endif // USE_OGG
	//bool read_memory_pcm_audio_pcm(const AudioFile& file, AudioPcmAllBlock& audio_out) noexcept { return true; }

} // namespace xe is end

#endif // _XE_RWAD_AUDIO_HPP_ IS EOF