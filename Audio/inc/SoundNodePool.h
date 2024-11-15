#pragma once
#include <stdint.h>
#include <SoundNode.h>
#include <unordered_map>

namespace audio {
	class SoundNodePool {
	public:
		//��������ڵ㵽��
		int32_t AddSoundNode(SoundNode* sound_node);
		//�õ������ڵ��ָ��
		SoundNode* GetSoundNodePtr(int32_t sound_node_id);
	private:
		std::unordered_map<int32_t, SoundNode*> sound_node_pool;
	};
}