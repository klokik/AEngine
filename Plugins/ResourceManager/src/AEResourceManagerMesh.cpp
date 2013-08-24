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
		LoadObjFile(mesh,res_name.c_str());

		return AE_OK;
	}
}