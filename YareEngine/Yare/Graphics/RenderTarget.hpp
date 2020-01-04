//RenderTarget  - Abstracts Rendering to Texture(s) 
//	Can attach multiple render to render to.  
#pragma once 
#include "Texture.hpp"
#include "Shader.hpp"
#include "UniformBlock.hpp"
#include <vector>

//https://learnopengl.com/Advanced-Lighting/Deferred-Shading
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
namespace yare {
	namespace graphics {

	//What buffer will write to the target texture 
	enum class RenderTargetAttachment
	{
		Scene =0, //Scene Color 
		Position,  //Uses RGB floating bit point
		Normal,  //Uses RGB 
		Diffuse, //RGBA floating
		Specular, //RGBA floating
		Emissive, //RGBA floating
		Depth, // TODO - Do not use internal depth buffer write to depth on geom pass. 
		//use depth pass to write this texture to depth instead of blitting to it
		Stencil, //
		Count
	};

	enum class RenderTargetMode
	{
		ReadDraw =0, 
		Read,//read from target 
		Draw //draw to target
	};

	//TODO - Copy into Attachment  given another rend	ertarget
;	class RenderTarget
	{
	public:

		static RenderTarget* Create();

		virtual ~RenderTarget()= default;
		virtual void setup(const std::vector<RenderTargetAttachment>& attachments) = 0;
		//copy targets depth buffer into the targets. if null, copy to default depth buffer
		virtual void unloadAttachment(RenderTarget* target, RenderTargetAttachment source, RenderTargetAttachment destination, int xoff, int yoff, int width, int height ) = 0;
		//read attachment into texture
		virtual void read(RenderTargetAttachment attachment, TexturePixels & pixel) = 0;
		virtual void bind(RenderTargetMode mode = RenderTargetMode::ReadDraw) = 0;
		virtual void unbind(RenderTargetMode mode = RenderTargetMode::ReadDraw) = 0;
		virtual void bindTextures(int offset=0) = 0; //int is the texture unit offset
		virtual void unloadUniforms(UniformBlock& uniforms)= 0 ;
		virtual int getNumberOfAttachments() const = 0;
		virtual void getAttachments(std::vector<RenderTargetAttachment>& attachments) const =0;

		virtual void resize(int width, int height) { _width = width; _height = height; };
		inline int getWidth() { return _width; }
		inline int getHeight() { return _height; }

	private :
		int _width, _height;
};

}}