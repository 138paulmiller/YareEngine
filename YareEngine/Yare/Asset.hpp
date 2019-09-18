#pragma once
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Mesh.hpp"
#include "OS/FileSystem.hpp"

namespace yare {


	class Asset
	{
	public:
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

		bool _isLoaded;
		std::string  _name;
		std::string _filepath;
	};


	class TextureAsset : public Asset
	{
	public:

		bool isLoaded() override;
		void load() override;
		void unload()override;
		void* data()override;

	private:
		std::unique_ptr<graphics::TexturePixels> _pixels;
		std::unique_ptr<graphics::Texture > _texture;
	};

	class ShaderAsset : public Asset
	{
	public:

		bool isLoaded() override;
		void load() override;
		void unload()override;
		void* data()override;

	private:
		std::unique_ptr<graphics::Shader> _shader;
	};
}