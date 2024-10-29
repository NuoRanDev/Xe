#ifndef _INC_DECOMPRESS_H_
#define _INC_DECOMPRESS_H_

#include "xeDataManageConfig.h"
#include "xeCompressFileStruct.h"

namespace xe
{
	class TestureData;
	class AudioData;

	class GameData
	{
	public:
		friend TestureData;

		GameData(char* file_path);

		bool DynamicLoad(char* file_path);

		void Release();

		~GameData();
	private:

		// Header Files
		uint64_t file_tree_header_size;

		// File Data
		xeOtherCompressFileHeaderFormat* file_map;

		// File tree
		xeCompressFileBlockInfo* file_tree;

		// File Stream
		std::ifstream fs;
	};

	class TestureData : public GameData
	{
	public:
		explicit TestureData(char* file_path);

		// Testure IO
#if defined(EXPORT_C_SHARP_API)
		Testure* GetTestureDataCsharp(CsharpString* file_name_list, uint64_t number);
#elif defined(EXPORT_C_PLUS_PLUS_API)
		std::vector<Testure> GetTestureData(std::vector<std::string>& file_name_list);
#endif // defined(C_SHARP_API) IS END

		~TestureData();
	};

	class AudioData : public GameData
	{
	public:
		AudioData(char* file_path);

		// Testure IO
#if defined(EXPORT_C_SHARP_API)
		Testure* GetTestureDataCsharp(char** file_name_list, int number);
#elif defined(EXPORT_C_PLUS_PLUS_API)
		std::vector<Testure> GetTestureData(std::vector<std::string>& file_name_list);
#endif // defined(C_SHARP_API) IS END

		~AudioData();
	private:
		std::mutex* IO_LOCK;
	};
}

#endif // _INC_COMPRESS_H_ IS EOF