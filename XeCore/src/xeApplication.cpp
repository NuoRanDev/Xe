#include "xeApplication.h"

namespace xe
{
	namespace Application
	{
		static std::string application_path;
		static std::string application_name;
		static std::fstream* log_file;

		void LaodApplication(int argc, char** argv, const char* out_log_name)
		{
			std::filesystem::path file_name_with_path = argv[0];
			application_path = file_name_with_path.remove_filename().string();
			application_name = file_name_with_path.filename().string();
			auto& log_file_path = file_name_with_path.replace_filename(out_log_name);
			log_file = new std::fstream(log_file_path, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
		}

		std::string GetApplicationPath()
		{
			return application_path;
		}

		std::string GetApplicationName()
		{
			return application_name;
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