
#include "TextureAsset.hpp"

namespace yare {
	namespace assets {

		////////////////////////////////////// Texture Asset ///////////////////////

		bool TextureAsset::isLoaded()
		{
			return _texture != nullptr;
		}
		void TextureAsset::load()
		{
			_texture.reset(graphics::Texture::Create());
			graphics::Texture::ReadFile(Asset::getFilePath(), _pixels);
			_texture->load(_pixels);
			_texture->generateMipMaps();
			//_texture->update();

		}
		void TextureAsset::unload()
		{
			_texture.reset();

		}

		void* TextureAsset::data()
		{
			return _texture.get();
		}

		////////////////////////////////////// Cubemap Asset ///////////////////////

		bool CubemapAsset::isLoaded()
		{
			return _texture != nullptr;
		}
		void CubemapAsset::load()
		{
			_texture.reset(graphics::Cubemap::CreateFromFile(Asset::getFilePath()));


		}
		void CubemapAsset::unload()
		{
			_texture.reset();

		}

		void* CubemapAsset::data()
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
}