#pragma once

#include <Yare/Graphics/Shader.hpp>
#include <Yare/Graphics/Texture.hpp>
#include <Yare/Graphics/Cubemap.hpp>
#include <Yare/Graphics/Mesh.hpp>
#include <Yare/OS/FileSystem.hpp>
#include <Yare/Asset.hpp>

namespace yare {

	namespace assets {

		class TextureAsset : public Asset
		{
		public:

			bool isLoaded() override;
			void load() override;
			void unload()override;
			void* data()override;

		private:
			graphics::TexturePixels _pixels;
			std::unique_ptr<graphics::Texture > _texture;
		};

		class CubemapAsset : public Asset
		{
		public:

			bool isLoaded() override;
			void load() override;
			void unload()override;
			void* data()override;

		private:
			graphics::TexturePixels _pixels;
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
}