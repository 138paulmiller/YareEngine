//RenderTarget  - Abstracts Rendering to Texture(s) 
//	Can attach multiple render to render to.  

#include "../RenderTarget.hpp"
#include "OpenGLTexture.hpp"
#include <vector>

//https://learnopengl.com/Advanced-Lighting/Deferred-Shading
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
namespace yare {
	namespace graphics {
		
;	class OpenGLRenderTarget : public RenderTarget
	{
	public:
		OpenGLRenderTarget();
		~OpenGLRenderTarget() ;		
		void use(RenderTargetAttachment attachment) override;
		void resize(int width, int height) override;
		void setup() override;
		void bind(bool isRead = false) override;
		void read(RenderTargetAttachment attachment, TexturePixels& pixels) override { YARE_ASSERT(false, "Not Implemented"); }

	private:
		//
		struct OpenGLRenderBuffer
		{
			bool used;
			unsigned int texture; //handle
		};
		OpenGLRenderBuffer _buffers[(const int)RenderTargetAttachment::Count]; //corresponding texture handle
		
		//buffer for frame to write depth and stencil data to. 
		unsigned int _renderbuffer; 
		unsigned int _framebuffer;
		char _numUsed; //cached, intialized on setup
	};

}}