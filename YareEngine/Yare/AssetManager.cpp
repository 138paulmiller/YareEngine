#include "AssetManager.hpp"
namespace yare
{
	AssetManager& AssetManager::GetInstance()
	{
		static AssetManager assetManager;
		return assetManager;
	}


	AssetManager::AssetManager():
	_engineContentDir(YARE_PROJECT_DIR"/Content/")
	{
	}
	AssetManager::~AssetManager()
	{
		_assets.clear();
	}

	const std::string& AssetManager::GetContentDir(bool isProjectDir)
	{
		if (isProjectDir)
		{
			return _projectContentDir;
		}
		else
		{
			return _engineContentDir;
		}
	}

	void AssetManager::remove(const std::string& name)
	{

		Asset* asset = _assets[name].get();
		if (asset->isLoaded())
		{
			asset->unload();
		}
	}
	void* AssetManager::getImpl(const std::string name)
	{
		Asset* asset = _assets[name].get();
		if (asset == 0)
		{
			std::cout << "Could not find asset" << name;
			return 0;
		}
		if (!asset->isLoaded())
		{
			asset->load();
		}
		return asset->data();
	}
	void AssetManager::addImpl(const std::string& name, const std::string& filepath, const std::type_info& info)
	{
		Asset * asset = Asset::Create(name, filepath, info);
		_assets.emplace(std::make_pair(name, asset));
	}


	//Defulat engine assets. Should load from config file
	void AssetManager::loadEngineContent()
	{
		//By default . Should be one for each lighting model
		add<graphics::Shader>("skybox"              , _engineContentDir + "Shaders/skybox"                );
		add<graphics::Shader>("mesh_flat"           , _engineContentDir + "Shaders/mesh_flat"             );
		add<graphics::Shader>("mesh_phong"          , _engineContentDir + "Shaders/mesh_phong"            );
		add<graphics::Shader>("quad_textured"       , _engineContentDir + "Shaders/quad_textured"         );
		add<graphics::Texture>("container_diffuse"  , _engineContentDir + "Textures/container_diffuse.png"  );
		add<graphics::Texture>("container_specular" , _engineContentDir + "Textures/container_specular.png" );
		add<graphics::Cubemap>("skybox_cubemap"     , _engineContentDir + "Textures/skybox_cubemap.png"             );
	}

}
