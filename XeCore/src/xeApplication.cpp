#include "xeApplication.h"

namespace xe
{
	namespace Application
	{
		static std::string application_path;
		static std::string application_name;

		void LaodApplication(int argc, char* argv[])
		{
			std::filesystem::path file_name_with_path = argv[0];
			application_path = file_name_with_path.remove_filename().string();
			application_name = file_name_with_path.filename().string();
		}
	}
}