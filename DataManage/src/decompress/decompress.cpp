#include "decompress/decompress.h"

#include "decompress/private/private_FileType.h"
#include "decompress/private/private_ImageFileReader.h"
#include "decompress/private/private_Decompress.h"

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
		file_map = new xeOtherCompressFileHeaderFormat[1];
		if (!fs->GetFilePtr(file_path))
		{
#ifdef _DEBUG
			std::cout << std::format("ERROR CAN NOT FIND FILE: {0}\n", file_path);
#endif // _DEBUG
			return false;
		}
		// Read File Header
		fs = new iMmapfstream();
		fs->FstraemStartMemcpy(file_map, sizeof(xeOtherCompressFileHeaderFormat));
		// Read file tree
		file_tree = new xeCompressFileBlockInfo[file_map->file_number];
		fs->FstraemByteMemcpy((byte_t*)file_tree, sizeof(xeOtherCompressFileHeaderFormat), sizeof(xeCompressFileBlockInfo) * file_map->file_number);
		return true;
	}

	// Free Data
	void GameData::Release()
	{
		delete[]file_map;
		delete[]file_tree;
		fs->Release();
	}

	GameData::~GameData()
	{
		Release();
		delete fs;
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
		Testure* poutput_testures = new Testure[number];
		byte_t* compressed_data = nullptr;
		byte_t* not_compressed_data = nullptr;

		uint64_t compressed_size = 0;
		uint64_t not_compressed_size = 0;
		uint64_t block_start = 0;

		for (uint64_t i = 0; i < number; i++)
		{
			// find data by name
			for (uint64_t j = 0; j < file_map->file_number; j++)
			{
				if (strcmp((char*)(file_tree[j].file_name), (char*)(file_name_list[i].data)) == 0)
				{
					compressed_size = file_tree->_compress_size;
					not_compressed_size = file_tree->_not_compress_size;
					block_start = file_tree->_block_start;
					goto IS_FIND_FILE;
				}
			}
#ifdef _DEBUG
			std::cout << std::format("Not find file NAME:{0}\n", (char*)(file_name_list[i].data));
#endif // _DEBUG
			poutput_testures[i] = {0};
			continue;
			// if found ,get file type
		IS_FIND_FILE:
			compressed_data = fs->GetFstreamPtr<byte_t>(block_start);
			if (compressed_data == nullptr)
			{
#ifdef _DEBUG
				std::cout << std::format("Read failed :{0}\n", file_name_list[i]);
#endif // _DEBUG
				poutput_testures[i] = { 0 };
				continue;
			}
			not_compressed_data = new byte_t[not_compressed_size];
			DecompressLZMA(compressed_data, not_compressed_data, compressed_size, not_compressed_size);
			if (GetNameExtension((char*)(file_name_list[i].data), file_name_list[i]._size, ".jpeg", 6) ||
				GetNameExtension((char*)(file_name_list[i].data), file_name_list[i]._size, ".jpg", 5))
			{
				DecompressJPEG(&(poutput_testures[i]), not_compressed_data);
			}
			if (GetNameExtension((char*)(file_name_list[i].data), file_name_list[i]._size, ".png", 5))
			{
			}
			else
			{
#ifdef _DEBUG
				std::cout << std::format("Can't Supproted :{0}\n", file_name_list[i]);
#endif // _DEBUG
				poutput_testures[i] = { 0 };
			}
			delete[]not_compressed_data;
			not_compressed_data = nullptr;
			continue;
		}
		return poutput_testures;
	}
#endif // defined C_SHARP_API IS END

#if defined(EXPORT_C_PLUS_PLUS_API)
	std::vector<Testure> TestureData::GetTestureData(std::vector<std::string>& file_name_list)
	{
		std::vector<Testure> output_testures;
		byte_t* compressed_data = nullptr;
		byte_t* not_compressed_data = nullptr;

		uint64_t compressed_size = 0;
		uint64_t not_compressed_size = 0;
		uint64_t block_start = 0;

		for (auto& file_name : file_name_list)
		{
			// find data by name
			for (uint64_t j = 0; j < file_map->file_number; j++)
			{
				if (file_name == (char*)(file_tree[j].file_name))
				{
					compressed_size = file_tree->_compress_size;
					not_compressed_size = file_tree->_not_compress_size;
					block_start = file_tree->_block_start;
					goto IS_FIND_FILE;
				}
			}
#ifdef _DEBUG
			std::cout << std::format("Not find file NAME:{0}\n", file_name);
#endif // _DEBUG
			output_testures.push_back({ 0 });
			continue;

			// if found ,get file type
		IS_FIND_FILE:
			Testure img;

			compressed_data = fs->GetFstreamPtr<byte_t>(block_start);
			if (compressed_data == nullptr)
			{
#ifdef _DEBUG
				std::cout << std::format("Read failed :{0}\n", file_name);
#endif // _DEBUG
				output_testures.push_back({ 0 });
				continue;
			}

			not_compressed_data = new byte_t[not_compressed_size];
			DecompressLZMA(compressed_data, not_compressed_data, compressed_size, not_compressed_size);
			if (GetNameExtension(file_name.c_str(), file_name.size(), ".jpeg", 6) ||
				GetNameExtension(file_name.c_str(), file_name.size(), ".jpg", 5))
			{
				DecompressJPEG(&img, not_compressed_data);
				output_testures.push_back(img);
			}
			if (GetNameExtension(file_name.c_str(), file_name.size(), ".png", 5))
			{
			}
			else
			{
#ifdef _DEBUG
				std::cout << std::format("Can't Supproted :{0}\n", file_name);
#endif // _DEBUG
				img = { 0 };
				output_testures.push_back(img);
			}
			delete[]not_compressed_data;
			not_compressed_data = nullptr;
			continue;
		}
		return output_testures;
	}
#endif // defined EXPORT_C_PLUS_PLUS_API IS END

	TestureData::~TestureData()
	{
		std::cout << std::format("TestureData File IS RELEASE\n");
	}

	AudioData::AudioData(char* file_path):GameData(file_path)
	{
		if (xeAssetFileType::AUDIO_ASSET_FILE_HEADER != file_map->header)
		{
#ifdef _DEBUG
			std::cout << std::format("NOT AUDIO FILE: {0} \n", file_path);
#endif // _DEBUG IS END
			throw std::format("NOT AUDIO FILE: {0} \n", file_path);
		}
		if (xeCompressSolution::NONE != file_map->compress_solution)
		{
#ifdef _DEBUG
			std::cout << std::format("AUDIO FILE NOT NONE COMPRESSING FILE: {0} \n", file_path);
#endif // _DEBUG IS END
			throw std::format("AUDIO FILE NOT LZMA COMPRESSING FILE: {0} \n", file_path);
		}
	}

	Audio* AudioData::GetTestureDataCsharp(CsharpString* file_name_list, uint64_t number)
	{
		Audio* poutput_audios = nullptr;
		// PS: NOT FREE !!!!!
		byte_t* not_compressed_data = nullptr;

		uint64_t not_compressed_size = 0;
		uint64_t block_start = 0;
		
		for (uint64_t i = 0; i < number; i++)
		{
			// find data by name
			for (uint64_t j = 0; j < file_map->file_number; j++)
			{
				if (strcmp((char*)(file_tree[j].file_name), (char*)(file_name_list[i].data)) == 0)
				{
					not_compressed_size = file_tree->_not_compress_size;
					block_start = file_tree->_block_start;
					goto IS_FIND_FILE;
				}
#ifdef _DEBUG
				std::cout << std::format("Not find file NAME:{0}\n", (char*)(file_name_list[i].data));
#endif // _DEBUG
				poutput_audios[i].solution = xeAudioCompressSolution::ERROR;
				continue;
				// if found ,get file type
			IS_FIND_FILE:
				not_compressed_data = fs->GetFstreamPtr<byte_t>(block_start);
				if (not_compressed_data == nullptr)
				{
#ifdef _DEBUG
					std::cout << std::format("Read failed :{0}\n", (char*)(file_name_list[i].data));
#endif // _DEBUG
					poutput_audios[i].solution = xeAudioCompressSolution::ERROR;
					continue;
				}
				if (GetNameExtension((char*)(file_name_list[i].data), file_name_list[i]._size, ".ogg", 5))
				{
					poutput_audios->solution = xeAudioCompressSolution::OGG;
					poutput_audios->_size = not_compressed_size;
					poutput_audios->data = new byte_t[not_compressed_size];
					memcpy(poutput_audios->data, not_compressed_data, not_compressed_size);
				}
				else
				{
#ifdef _DEBUG
					std::cout << std::format("Can't Supproted :{0}\n", (char*)(file_name_list[i].data));
#endif // _DEBUG
					poutput_audios[i].solution = xeAudioCompressSolution::ERROR;
				}
			}
		}
		return poutput_audios;
	}

	std::vector<Audio> AudioData::GetTestureData(std::vector<std::string>& file_name_list)
	{
		std::vector<Audio>output_testures;
	}
}