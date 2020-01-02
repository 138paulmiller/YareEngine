//RenderTarget  - Abstracts Rendering to Texture(s) 
//	Can attach multiple render to render to.  

#include "../RenderTarget.hpp"
#include "OpenGLTexture.hpp"
#include "../Shader.hpp"
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
		void setup(const std::vector<RenderTargetAttachment>& attachments) override;
		void resize(int width, int height) override;
		void bind(RenderTargetMode mode = RenderTargetMode::ReadDraw) override;
		void bindTextures(int offset = 0) override;
		void unbind(RenderTargetMode mode = RenderTargetMode::ReadDraw) override;
		void unloadUniforms(UniformBlock& uniforms);
		int getNumberOfAttachments() ;
		void unloadAttachment(RenderTarget* target, RenderTargetAttachment source, RenderTargetAttachment destination, int xoff, int yoff, int width, int height);

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
		unsigned int _depthStencilbuffer;
	//unsigned int _renderbuffer; 
		unsigned int _framebuffer;
		char _numUsed; //cached, intialized on setup
	};

}}