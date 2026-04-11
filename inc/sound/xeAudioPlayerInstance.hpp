#ifndef _INC_XE_AUDIO_PLAYER_INSTANCE_HPP_
#define _INC_XE_AUDIO_PLAYER_INSTANCE_HPP_

namespace xe 
{
	class AudioPlayerInstance
	{
	public:
		AudioPlayerInstance()
		{
			device = nullptr;
			context = nullptr;
		}

		bool init() noexcept;



		~AudioPlayerInstance();

	private:
		void* device;
		void* context;
	};

} // namespace xe is end

#endif // _INC_XE_AUDIO_PLAYER_INSTANCE_HPP_ IS EOF