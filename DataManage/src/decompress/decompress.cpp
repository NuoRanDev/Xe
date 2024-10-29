#include "decompress/decompress.h"

#include "decompress/private/private_FileType.h"
#include "decompress/private/private_ImageFileReader.h"

namespace xe
{
	GameData::GameData(char* file_path)
	{
		// Init
		if(!DynamicLoad(file_path))
		{
			// CAN NOT FIND FILE
			throw std::format("ERROR CAN NOT FIND FILE: {0}", file_path);
		}
	}

	bool GameData::DynamicLoad(char* file_path)
	{
		// Free
		if (nullptr != file_map)
		{
			Release();
		}
		// Get File
		fs = std::ifstream(file_path);
		file_map = new xeOtherCompressFileHeaderFormat[1];
		if (!fs.is_open())
		{
#ifdef _DEBUG
			std::cout << std::format("ERROR CAN NOT FIND FILE: {0}\n", file_path);
#endif // _DEBUG
			return false;
		}
		// Read File Header
		fs.read((char*)file_map, sizeof(xeOtherCompressFileHeaderFormat));
		// Get header size
		file_tree_header_size = sizeof(xeCompressFileBlockInfo) * file_map->file_number;
		// Read file tree
		file_tree = new xeCompressFileBlockInfo[file_map->file_number];
		fs.read((char*)file_tree, sizeof(xeCompressFileBlockInfo) * file_map->file_number);
		return true;
	}

	// Free Data
	void GameData::Release()
	{
		delete[]file_map;
		delete[]file_tree;
		fs.close();
	}

	GameData::~GameData()
	{
	}

	TestureData::TestureData(char* file_path) :GameData(file_path)
	{
		if (xeAssetFileType::IMAGE_ASSET_FILE_HEADER != file_map->header)
		{
#ifdef _DEBUG
			std::cout << std::format("NOT IMAGE FILE: {0} \n", file_path);
#endif // _DEBUG IS END
			throw std::format("NOT IMAGE FILE: {0} \n", file_path);
		}
		if (xeCompressSolution::LZMA != file_map->compress_solution)
		{
#ifdef _DEBUG
			std::cout << std::format("IMAGE FILE NOT LZMA COMPRESSING FILE: {0} \n", file_path);
#endif // _DEBUG IS END
			throw std::format("IMAGE FILE NOT LZMA COMPRESSING FILE: {0} \n", file_path);
		}
	}
#if defined(EXPORT_C_SHARP_API)
	Testure* TestureData::GetTestureDataCsharp(CsharpString* file_name_list, uint64_t number)
	{
		Testure* poutput_testure = new Testure[number];
		byte_t* compressed_data = nullptr;
		for (uint64_t i = 0; i < number; i++)
		{
			if (GetNameExtension((char*)(file_name_list->data), file_name_list->_size, ".jpeg", 6) ||
				GetNameExtension((char*)(file_name_list->data), file_name_list->_size, ".jpg", 5))
			{
				//DecompressJPEG(&(poutput_testure[i]));
			}
		}
		return poutput_testure;
	}
#elif defined(EXPORT_C_PLUS_PLUS_API)
	std::vector<Testure> TestureData::GetTestureData(std::vector<std::string>& file_name_list)
	{
	}
#endif // defined(C_SHARP_API) IS END

	TestureData::~TestureData()
	{
	}
}