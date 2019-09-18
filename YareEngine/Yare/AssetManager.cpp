#include "AssetManager.hpp"
namespace yare
{
	AssetManager& AssetManager::GetInstance()
	{
		static AssetManager assetManager;
		return assetManager;
	}

	//Defulat engine assets. Should load from config file
	void AssetManager::loadDefaultAssets()
	{
		add<graphics::Shader>("Shader_SkyBox", YARE_ASSET("Shaders/skybox"));
		add<graphics::Shader>("Shader_Phong", YARE_ASSET("Shaders/phong"));

		add<graphics::Texture>("Image_Container_SkyBox", YARE_ASSET("Images/skybox.png"));
		add<graphics::Texture>("Image_Container_Diffuse", YARE_ASSET("Images/container_diffuse.png"));
		add<graphics::Texture>("Image_Container_Specular", YARE_ASSET("Images/container_specular.png"));

	}

}
