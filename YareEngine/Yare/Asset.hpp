#pragma once
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Cubemap.hpp"
#include "Graphics/Mesh.hpp"
#include "OS/FileSystem.hpp"

namespace yare {

	/*
		These are editor level access to modifying Various Engine Resources. 
	*/
	class Asset
	{
	public:
		//Create Corresponding asset class based on asset type
		static Asset * Create(const std::string& name, const std::string& filepath, const std::type_info& info);
		
		virtual ~Asset() = default;

		virtual bool isLoaded() = 0;
		virtual void load() = 0;
		virtual void unload() = 0;
		virtual void* data() = 0;


		inline const std::string& getName() const { return _name; }
		
		//Must Notify AssetManager if name changes
		inline void setName(const std::string& name) { _name = name; }

		inline void setFilePath(const std::string& filepath) { _filepath = filepath; }
		inline const std::string& getFilePath() const { return _filepath; }

		
	protected:

		std::string  _name;
		std::string _filepath;
	};


}