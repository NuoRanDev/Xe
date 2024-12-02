#pragma once

namespace audio {

	enum class NodeType : int32_t {
		SOUNDWAVE,
		SOUNDOUTPUT,
	};

	class SoundNode {
	public:
		virtual ~SoundNode();
	protected:
		int32_t node_id;
	};
}