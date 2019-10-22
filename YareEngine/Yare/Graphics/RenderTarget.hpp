//RenderTarget  - Abstracts Rendering to Texture(s) 
//	Can attach multiple render to render to.  
#pragma once 
#include "Texture.hpp"

#include <vector>

//https://learnopengl.com/Advanced-Lighting/Deferred-Shading
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
namespace yare {
	namespace graphics {

	//What buffer will write to the target texture 
	enum class RenderTargetAttachment
	{
		//Depth = 0//Not Implemented yet
		Color=0, //Multiple Color Attachments are allowed Color are RGBA 32 bit
		Position,  //Uses RGB 16 floating bit point
		Normal,  //Uses RGB 16 floating bit point
		Count
	};
;	class RenderTarget
	{
	public:

		static RenderTarget* Create();

		virtual ~RenderTarget()= default;
		virtual void use(RenderTargetAttachment attachment) = 0;
		//read attachment into texture
		virtual void read(RenderTargetAttachment attachment, TexturePixels & pixel) = 0;
		virtual void setup() = 0; //must be called once before render

		///if isRead is true, then binds textures. Else bind just framebuffer
		virtual void bind(bool isRead=false) = 0;

		virtual void resize(int width, int height) { _width = width; _height = height; };
		inline int getWidth() { return _width; }
		inline int getHeight() { return _height; }

	private :
		int _width, _height;
};

}}