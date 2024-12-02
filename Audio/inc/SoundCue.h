#pragma once
#include <unordered_map>
#include <functional>
#include <stdint.h>

#include "SoundWave.h"

namespace audio {
	class SoundCue {
	public:
		~SoundCue();
		//����
		void Play();
		//��ͣ����
		void Stop();
		//���ô�������
		void SetPlayCondition();
		//���wave��SoundCue������һ��id
		int32_t AddWave(SoundWave* wave_ptr);
		//��SoundCue���Ƴ�wave
		bool RemoveWave(int32_t wave_id);
	public:
		//�Ƿ����
		bool is_hooking = true;
		//�Ƿ�ѭ������
		bool is_looping = false;
	private:
		std::unordered_map<int32_t, SoundWave*> wave_arr;
	};
}