#include "AEResourceManager.h"

void LoadObjFile(AEMesh &mesh, const char *path);

namespace aengine
{
	int AEResourceManager::LoadMesh(std::string res_name,AEMesh &mesh)
	{
		return LoadMeshObj(res_name,mesh);
	}

	int AEResourceManager::LoadMeshObj(std::string res_name,AEMesh &mesh)
	{
		std::string file_name=work_directory+res_name;
		LoadObjFile(mesh,file_name.c_str());

		return AE_OK;
	}
}