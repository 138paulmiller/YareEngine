
#include "Asset.hpp"
#include "Assets/TextureAsset.hpp"
namespace yare {


	Asset* Asset::Create(const std::string& name, const std::string& filepath, const std::type_info& info)
	{
		Asset* asset;
		if (info == typeid(graphics::Texture))
		{
			asset = (new assets::TextureAsset());
		}
		else if (info == typeid(graphics::Cubemap))
		{
			asset = (new assets::CubemapAsset());
		}
		else if (info == typeid(graphics::Shader))
		{
			asset = (new assets::ShaderAsset());
		}
		else
		{
			YARE_ASSERT(true, "Invalid Asset Type");
			return 0;
		}
		asset->setFilePath(filepath);
		asset->setName(name);
		return asset;
		//set uuid
	
	}

	
}