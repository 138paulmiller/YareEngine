
#include "Asset.hpp"

namespace yare {


	////////////////////////////////////// Texture Asset ///////////////////////

	bool TextureAsset::isLoaded()
	{
		return _texture != nullptr;
	}
	void TextureAsset::load()
	{
		_texture.reset(graphics::Texture::Create());
		_pixels.reset(new graphics::TexturePixels);
		graphics::Texture::ReadFile(Asset::getFilePath(), *_pixels);
		_texture->load(*_pixels);
		_texture->generateMipMaps();

	}
	void TextureAsset::unload()
	{
		_pixels = nullptr;
		_texture = nullptr;
		
	}

	void* TextureAsset::data()
	{
		return _texture.get();
	}

	////////////////////////////////////// Shader Asset ///////////////////////

	bool ShaderAsset::isLoaded()
	{
		return _shader != nullptr;
	}
	void ShaderAsset::load()
	{
		_shader.reset(graphics::Shader::Create());
		std::string vertSource, fragSource;

		os::FileSystem::ReadFile(Asset::getFilePath() + ".vert", vertSource);
		os::FileSystem::ReadFile(Asset::getFilePath() + ".frag", fragSource);

		_shader.reset(graphics::Shader::Create());
		_shader->compile(vertSource, fragSource);

	}
	void ShaderAsset::unload()
	{
		_shader = nullptr;
	}

	void* ShaderAsset::data()
	{
		return _shader.get();
	}
	/////////////////////////////////// //////////////////////////////
}