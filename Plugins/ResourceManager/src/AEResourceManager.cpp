#include <iostream>
#include <fstream>

#include "AEResourceManager.h"


namespace aengine
{
	void AEResourceManager::SetWorkDirectory(std::string work_dir)
	{
		work_directory=work_dir;
	}

	std::string AEResourceManager::GetWorkDirectory(void)
	{
		return work_directory;
	}

	std::string AEResourceManager::LoadString(std::string res_name)
	{
		std::ifstream if_str;
		if_str.open(work_directory+res_name,std::ifstream::in);

		std::string result((std::istreambuf_iterator<char>(if_str)),std::istreambuf_iterator<char>());

		if_str.close();

		return result;
	}
}