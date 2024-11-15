#pragma once
#include <stdint.h>
#include <string>

namespace audio {

	class SoundWave {
	public:
		SoundWave(const std::string& wave_path);

		~SoundWave();
		//������ݴ�С
		[[nodiscard]] uint64_t GetDataSize() const;
		//��û���id
		[[nodiscard]] uint32_t GetBufferID() const;
		//������Ƶ·��
		std::string SetSoundWavePath(const std::string& wave_path);
		//�����Ƶ·��
		[[nodiscard]] std::string GetSoundWavePath() const;
		//�����Ƶͨ������
		[[nodiscard]] uint32_t GetNumChannels() const;
		//��ò�������
		[[nodiscard]] uint32_t GetSampleRate() const;
	public:
		enum struct SampleRateEnum : int8_t;
		//��ƵԴ·��
		const std::string wave_path;
		//��Ƶ���ݵ�ָ��
		void* pData;
		//ѹ����
		float compression = 1.0f;
		//��������
		float volume_multiplier = 1.0f;
		//��������
		float pitch_multiplier = 1.0f;
		//�Ƿ�ѭ������
		bool is_looping = false;
	private:
		//��ʼ����Դ
		bool Init();
		//�ͷ���Դ
		bool Release();
		//���ɻ���id
		void GenBufferID();
		//�������ļ����ص�������
		bool LoadWaveFromPathToBuffer();
		//�ͷŻ�����Դ
		void ReleaseBuffer() const;
		//���ݴ�С����λbit
		uint64_t data_size;
		//����id
		uint32_t buffer_id;
		//��ƵԴ·��
		std::string wave_path;
		//ͨ������
		uint32_t num_channels;
		//�������� ��λ��Hz
		uint32_t sample_rate;
	};


}