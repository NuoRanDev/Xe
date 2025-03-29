export module xe.Audio.Instance;

import std;

import xe.Core.xeBaseDataType;
import xe.Core.CoreClrOutput;
import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;

namespace xe
{
	export class AudioInstance
	{
	public:
		AudioInstance();

		bool LoadData(std::unique_ptr<AudioFile> data);

		void Play();

		void Stop();

		void ReStart();

		~AudioInstance();

	private:

		//std::function<>
	};
}