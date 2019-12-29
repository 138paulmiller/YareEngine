//RenderTarget  - Abstracts Rendering to Texture(s) 
//	Can attach multiple render to render to.  
#pragma once 
#include "Texture.hpp"
#include "Shader.hpp"
#include <vector>

//https://learnopengl.com/Advanced-Lighting/Deferred-Shading
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
namespace yare {
	namespace graphics {

	//What buffer will write to the target texture 
	enum class RenderTargetAttachment
	{
		//Depth = 0//Not Implemented yet
		Position=0,  //Uses RGB floating bit point
		Normal,  //Uses RGB 
		Color, //RGBA floating
		Depth, //
		Stencil, //
		Count
	};
;	class RenderTarget
	{
	public:

		static RenderTarget* Create();

		virtual ~RenderTarget()= default;
		virtual void use(const std::vector<RenderTargetAttachment>& attachments) = 0;
		//read attachment into texture
		virtual void read(RenderTargetAttachment attachment, TexturePixels & pixel) = 0;
		///if isRead is true, then binds textures. Else bind just framebuffer
		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual void bindTextures() = 0;
		virtual void loadUniforms(Shader * shader)= 0 ;
		virtual void resize(int width, int height) { _width = width; _height = height; };
		inline int getWidth() { return _width; }
		inline int getHeight() { return _height; }

	private :
		int _width, _height;
};

}}