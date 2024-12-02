#pragma once
#include <stdint.h>
#include <SoundNode.h>
#include <unordered_map>

namespace audio {
	class SoundNodePool {
	public:
		//添加声音节点到池
		int32_t AddSoundNode(SoundNode* sound_node);
		//得到声音节点的指针
		SoundNode* GetSoundNodePtr(int32_t sound_node_id);
	private:
		std::unordered_map<int32_t, SoundNode*> sound_node_pool;
	};
}