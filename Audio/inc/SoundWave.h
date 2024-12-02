#pragma once
#include <stdint.h>
#include <string>

namespace audio {

	class SoundWave {
	public:
		SoundWave(const std::string& wave_path);

		~SoundWave();
		//获得数据大小
		[[nodiscard]] uint64_t GetDataSize() const;
		//获得缓存id
		[[nodiscard]] uint32_t GetBufferID() const;
		//设置音频路径
		std::string SetSoundWavePath(const std::string& wave_path);
		//获得音频路径
		[[nodiscard]] std::string GetSoundWavePath() const;
		//获得音频通道数量
		[[nodiscard]] uint32_t GetNumChannels() const;
		//获得采样数量
		[[nodiscard]] uint32_t GetSampleRate() const;
	public:
		enum struct SampleRateEnum : int8_t;
		//音频源路径
		const std::string wave_path;
		//音频数据的指针
		void* pData;
		//压缩比
		float compression = 1.0f;
		//音量乘数
		float volume_multiplier = 1.0f;
		//音调乘数
		float pitch_multiplier = 1.0f;
		//是否循环播放
		bool is_looping = false;
	private:
		//初始化资源
		bool Init();
		//释放资源
		bool Release();
		//生成缓存id
		void GenBufferID();
		//将音波文件加载到缓存中
		bool LoadWaveFromPathToBuffer();
		//释放缓存资源
		void ReleaseBuffer() const;
		//数据大小，单位bit
		uint64_t data_size;
		//缓存id
		uint32_t buffer_id;
		//音频源路径
		std::string wave_path;
		//通道数量
		uint32_t num_channels;
		//采样速率 单位：Hz
		uint32_t sample_rate;
	};


}