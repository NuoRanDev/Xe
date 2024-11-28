#ifndef _INC_COMPRESS_H_
#define _INC_COMPRESS_H_

#include "xeAssetsConfig.h"
#include "xeCompressFileStruct.h"

namespace xe
{
	class GameDataFactory
	{
	public:
		GameDataFactory();
		bool CreateDataFactory(const char* dir_path);
		~GameDataFactory();
	private:
		xeVideoCompressFileHeaderFormat		video_data;
		xeImageCompressFileHeaderFormat		image_data;
		xeAudioCompressFileHeaderFormat		audio_data;
		xeModelCompressFileHeaderFormat		model_data;
		xeTextCompressFileHeaderFormat		text_data;
		xeShaderCompressFileHeaderFormat	shader_data;
		xeDataBaseCompressFileHeaderFormat	db_data;

		xeOtherCompressFileHeaderFormat* adder;
	};
}

#endif // _INC_COMPRESS_H_ IS EOF