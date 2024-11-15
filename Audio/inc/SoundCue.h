#pragma once
#include <unordered_map>
#include <functional>
#include <stdint.h>

#include "SoundWave.h"

namespace audio {
	class SoundCue {
	public:
		~SoundCue();
		//播放
		void Play();
		//暂停播放
		void Stop();
		//设置触发条件
		void SetPlayCondition();
		//添加wave到SoundCue，反回一个id
		int32_t AddWave(SoundWave* wave_ptr);
		//从SoundCue中移除wave
		bool RemoveWave(int32_t wave_id);
	public:
		//是否烘培
		bool is_hooking = true;
		//是否循环播放
		bool is_looping = false;
	private:
		std::unordered_map<int32_t, SoundWave*> wave_arr;
	};
}