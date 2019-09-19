#pragma once 
#include "Asset.hpp"
#include <typeinfo>

namespace yare {



class AssetManager
{

protected:
	void* getImpl(const std::string name);
	void addImpl(const std::string& name, const std::string& filepath, const std::type_info& info);


public:
	static AssetManager& GetInstance();


	AssetManager();
	~AssetManager();

	void loadEngineContent();
	const std::string& GetContentDir(bool isProjectDir = false);
	

	//Get and Add should run asynchrounously and should take in callbacks after asset has been loaded!
	
	template<typename DataType >
	DataType* get(const std::string& name)
	{
		return (DataType*)getImpl(name);
	}
	
	void remove(const std::string& name);

	template<typename DataType >
	void add(const std::string& name, const std::string& filepath)
	{
		addImpl(name, filepath, typeid(DataType));
	}

private:
	//Default engine content directory
	std::string _engineContentDir; 
	//Project specific content directory
	std::string _projectContentDir;
	
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
	std::unordered_map<std::string, std::unique_ptr<Asset, AssetDeleter> > _assets;
};


	
}