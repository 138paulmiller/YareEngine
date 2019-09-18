#pragma once 
#include "Asset.hpp"

namespace yare {



class AssetManager
{

	struct AssetDeleter
	{
		void operator()(Asset* asset)
		{
			if (asset)
			{
				if (asset->isLoaded()) {
					asset->unload();
				}
				delete asset;
			}
		}
	};
public:
	static AssetManager& GetInstance();

	void loadDefaultAssets();

	template<typename DataType >
	DataType* get(const std::string& name)
	{
		Asset* asset = _assets[name].get();
		if (!asset->isLoaded())
		{
			asset->load();
		}
		return (DataType*)(asset->data());
	}
	
	template<typename DataType >
	void add(const std::string& name, const std::string& filepath)
	{
		Asset * asset;
		if (typeid(DataType) == typeid(graphics::Texture))
		{
			asset = ( new TextureAsset());
		}
		else if (typeid(DataType) == typeid(graphics::Shader))
		{
			asset = (new ShaderAsset());
		}

		asset->setFilePath(filepath);
		asset->setName(name);
		
		//set uuid
		_assets.emplace(std::make_pair(name, asset ));
	}

private:


	std::unordered_map<std::string, std::unique_ptr<Asset, AssetDeleter>> _assets;
};


	
}