import xe.Core.xeApplication;

import std;

import xe.Core.CoreClrOutput;

namespace xe
{
	namespace Application
	{
		static std::u8string application_path;
		static std::u8string application_name;
		static std::fstream* log_file;

		bool LaodApplication(int argc, char** argv, const char* out_log_name)
		{
			std::filesystem::path file_name_with_path = argv[0];
			application_path = file_name_with_path.remove_filename().u8string();
			application_name = file_name_with_path.filename().u8string();
			auto& log_file_path = file_name_with_path.replace_filename(out_log_name);
			log_file = new std::fstream(log_file_path, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
			return true;
		}

		const char8_t* GetApplicationPath()
		{
			return application_path.c_str();
		}

		const char8_t* GetApplicationName()
		{
			return application_name.c_str();
		}


		std::fstream* GetLogFile()
		{
			return log_file;
		}

		int DestroyApplication()
		{
			log_file->close();
			delete log_file;
			return 0;
		}
	}
}