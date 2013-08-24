#ifndef AERESOURCEMANAGER_H_
#define AERESOURCEMANAGER_H_


namespace aengine
{
	namespace AEResourceManager
	{
		extern std::string work_directory;

		void SetWorkDirectory(std::string work_dir);
		std::string GetWorkDirectory(void);

		int LoadImagePng(std::string res_name,AEImage &img);
		int LoadSceneCOLLADA(std::string res_name,AEScene &scene);
		int LoadMeshObj(std::string res_name,AEMesh &mesh);

		int LoadImage(std::string res_name,AEImage &img,std::string type);
		int LoadMesh(std::string res_name,AEMesh &mesh);

		int SaveSceneData(std::string res_name,AEScene &scene);
		int SaveSceneState(std::string res_name,AEScene &scene);
		int LoadScene(std::string res_name,AEScene &scene);
		int SaveScene(std::string res_name,AEScene &scene);

		int LoadConfig(std::string res_name,AEConfig &config);
		int SaveConfig(std::strign res_name,AEConfig &config);
	};
}

#endif /* AERESOURCEMANAGER_H_ */