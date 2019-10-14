//RenderTarget  - Abstracts Rendering to Texture(s) 
//	Can attach multiple render to render to.  

#include "Texture.hpp"

#include <vector>

//https://learnopengl.com/Advanced-Lighting/Deferred-Shading
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
namespace yare {
	namespace graphics {

	//What buffer will write to the target texture 
	enum class RenderTargetAttachment
	{
		Color=0, //Multiple Color Attachments are allowed Color are RGBA 32 bit
		Position,  //Uses RGB 16 floating bit point
		Normal,  //Uses RGB 16 floating bit point
		Count
	};
;	class RenderTarget
	{
	public:
		virtual ~RenderTarget()= default;
		virtual void use(RenderTargetAttachment attachment) = 0;
		//read attachment into texture
		virtual void read(RenderTargetAttachment attachment, TexturePixels pixel) = 0;
		virtual void setup() = 0; //must be called once before render

		virtual void bind() = 0;

		virtual void resize(int width, int height) { _width = width; _height = height; };
		inline int getWidth() { return _width; }
		inline int getHeight() { return _height; }

	private :
		int _width, _height;
};

}}