//	Can attach multiple render to render to.  

#include "OpenGLRenderTarget.hpp"
#include "OpenGLTexture.hpp"


//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/#using-the-depth
namespace yare {
	namespace graphics {
		namespace
		{

		unsigned int GetOpenGLAttachment(RenderTargetAttachment attachment, int unit)
		{
			switch (attachment)
			{
			case RenderTargetAttachment::Scene:
			case RenderTargetAttachment::Diffuse:
			case RenderTargetAttachment::Specular:
			case RenderTargetAttachment::Emissive:
			case RenderTargetAttachment::Position:
			case RenderTargetAttachment::Normal:
			case RenderTargetAttachment::Shadow:
			case RenderTargetAttachment::Depth:
				return GL_COLOR_ATTACHMENT0 + unit;
			}
		}

		TextureFormat GetTextureFormat(RenderTargetAttachment attachment)
		{
			switch (attachment)
			{

			case RenderTargetAttachment::Scene:
			case RenderTargetAttachment::Diffuse:
			case RenderTargetAttachment::Specular:
			case RenderTargetAttachment::Emissive:
			case RenderTargetAttachment::Shadow:
				return TextureFormat::RGB8;
			case RenderTargetAttachment::Depth:
			case RenderTargetAttachment::Position:
			case RenderTargetAttachment::Normal:
				return TextureFormat::XYZ16;


			}
		}
		TextureType GetTextureType(RenderTargetAttachment attachment)
		{
			switch (attachment)
			{

			case RenderTargetAttachment::Scene:
			case RenderTargetAttachment::Diffuse:
			case RenderTargetAttachment::Specular:
			case RenderTargetAttachment::Emissive:
			case RenderTargetAttachment::Position:
			case RenderTargetAttachment::Normal:
			case RenderTargetAttachment::Depth:
			case RenderTargetAttachment::Shadow:
				return TextureType::Image;

			}
		}
		const std::string RenderTargetAttachmentUniformName(RenderTargetAttachment attachment)
		{
			switch (attachment)
			{

			case RenderTargetAttachment::Scene:
				return "scene";
			case RenderTargetAttachment::Diffuse:
				return "diffuse";
			case RenderTargetAttachment::Specular:
				return "specular";
			case RenderTargetAttachment::Emissive:
				return "emissive";
			case RenderTargetAttachment::Position:
				return "position";
			case RenderTargetAttachment::Normal:
				return "normal";
			case RenderTargetAttachment::Depth:
				return "depth";
			case RenderTargetAttachment::Shadow:
				return "shadow";

			}
		}
	}


		OpenGLRenderTarget::OpenGLRenderTarget()
		{

			glGenFramebuffers(1, &_framebuffer);
			OpenGLCheckError();

			glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
			glGenRenderbuffers(1, &_depthStencilbuffer);
			OpenGLCheckError();

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				_buffers[i].used = false;
				_buffers[i].texture = 0;
			}
		}

		OpenGLRenderTarget::~OpenGLRenderTarget()
		{
			unbind();
			
			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				if (_buffers[i].used)
				{
					delete _buffers[i].texture;
				}
			}
			glDeleteRenderbuffers(1, &_depthStencilbuffer);
			glDeleteFramebuffers(1, &_framebuffer);
		}

			
		int OpenGLRenderTarget::getNumberOfAttachments()const
		{

			return _numUsed;
		}

		void OpenGLRenderTarget::getAttachments(std::vector<RenderTargetAttachment>& attachments)const
		{
			attachments.clear();

			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				if (_buffers[i].used)
				{
					attachments.push_back(RenderTargetAttachment(i));
				}
			}
		}

		void OpenGLRenderTarget::resize(int width, int height)
		{
			RenderTarget::resize(width, height);
			bind();


			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				RenderTargetAttachment attachment = RenderTargetAttachment(i);

				if (_buffers[i].used)
				{

					unsigned int target = GetOpenGLAttachment(attachment, _buffers[i].unit);
					TexturePixels pixels;
					pixels.data = 0;
					pixels.height = height; pixels.width = width;
					pixels.format = GetTextureFormat(attachment);
					_buffers[i].texture->load(pixels);

					glBindTexture(GL_TEXTURE_2D, 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, _buffers[i].texture->getHandle(), 0);

					OpenGLCheckError();
					

				}
			}

			glBindRenderbuffer(GL_RENDERBUFFER, _depthStencilbuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilbuffer);
			OpenGLCheckError();



			// finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Setup Failed ! Incomplete Framebuffer !" << std::endl;
			OpenGLCheckError();


			unbind();
		}
		void OpenGLRenderTarget::setup(const std::vector<RenderTargetAttachment>& attachments)
		{
			_numUsed = attachments.size();
			std::vector<unsigned int> layout;
			int unit = 0;
			bind();
			for (const RenderTargetAttachment & attachment : attachments)
			{
				//Initialize the buffer object
				OpenGLRenderBuffer& buffer = _buffers[(int)attachment];
				if (buffer.used)
				{
					std::cout << "Error: Buffer Already Used! Failed to add this buffer";
					_numUsed--; //remove from numused
					if (_numUsed < 0)_numUsed = 0;
					return;
				}
				unsigned int target = GetOpenGLAttachment(attachment, unit);
				layout.push_back(target) ;
		
				//create texture
				buffer.used = true;
				buffer.unit = unit;
				unit++;
				buffer.texture = new OpenGLTexture(GetTextureType(attachment), GetTextureFormat(attachment));
				buffer.texture->update();
				OpenGLCheckError();

			}
			//setup draw buffer
			//count number of used buffers to allocat
			glDrawBuffers(getNumberOfAttachments(), layout.data());
			OpenGLCheckError();


			unbind();


		}
		void OpenGLRenderTarget::bind(RenderTargetMode mode )
		{
			switch (mode)
			{
			case RenderTargetMode::ReadDraw:
				glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
				return;
			case RenderTargetMode::Read:
				glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);
				return;
			case RenderTargetMode::Draw:
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
				return;
			}
			OpenGLCheckError();

		}
		void OpenGLRenderTarget::unbind(RenderTargetMode mode)
		{
			switch (mode)
			{
			case RenderTargetMode::ReadDraw:
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				return;
			case RenderTargetMode::Read:
				glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
				return;
			case RenderTargetMode::Draw:
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				return;
			}
			OpenGLCheckError();

		}
		void OpenGLRenderTarget::bindTextures(int offset)
		{
			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				if (_buffers[i].used)
				{
					_buffers[i].texture->bind(_buffers[i].unit + offset);
					OpenGLCheckError();
				}
			}
		}
		
		Texture* OpenGLRenderTarget::getTexture(RenderTargetAttachment attachment)
		{ 
			return _buffers[(int)attachment].texture; 
		}

		void OpenGLRenderTarget::unloadUniforms(UniformBlock& uniforms)
		{
			int j = 0;
			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				RenderTargetAttachment attachment = RenderTargetAttachment(i);
				if (_buffers[i].used)
				{
					uniforms.setUniform(RenderTargetAttachmentUniformName(attachment), _buffers[i].texture);

				}
			}
		}
		int OpenGLRenderTarget::getAttachmentUnit(RenderTargetAttachment attachment)
		{
			return GetOpenGLAttachment(attachment, _buffers[(int)attachment].unit);

		}
		void OpenGLRenderTarget::blit(
			RenderTarget* target, RenderTargetAttachment source, RenderTargetAttachment destination, 
			int xoff, int yoff, int width, int height)
		{
			int srcUnit = _buffers[(int)source].unit;
			int destUnit = 0;
			OpenGLRenderTarget* openglTarget = dynamic_cast<OpenGLRenderTarget*>(target);
			if (openglTarget )
			{
				bool isValid = true;
				if (!openglTarget->_buffers[(int)destination].used) {
					std::cout << "OpenGL RenderTarget : Blit : Destination Buffer " << RenderTargetAttachmentUniformName(destination) << "Not Used";
					isValid = false;
				}
				if (!openglTarget->_buffers[(int)source].used) {
					std::cout << "OpenGL RenderTarget : Blit : Source Buffer " << RenderTargetAttachmentUniformName(destination) << "Not Used";
					isValid = false;
				}
				if (isValid)
				{
					destUnit = openglTarget->_buffers[(int)destination].unit;
				}
			}


			if (destination == RenderTargetAttachment::Depth) 
			{
				bind(RenderTargetMode::Read);

				if (target) {
					target->bind(RenderTargetMode::Draw);
				}
				else {
					//bind to default
					unbind(RenderTargetMode::Draw);
				}
				glBlitFramebuffer(0, 0, this->getWidth(), this->getHeight(), xoff, yoff, xoff + width, yoff + height,
					GL_DEPTH_BUFFER_BIT, GL_NEAREST);
				//TODO  - should also allow to load and read from depth, perhaps fbo must be cleared? 
				OpenGLCheckError();

				return;
			}
			
			bind(RenderTargetMode::Read);
			glReadBuffer(GetOpenGLAttachment(source, srcUnit));
			OpenGLCheckError();

			if (target) {
				target->bind(RenderTargetMode::Draw);
				glDrawBuffer(GetOpenGLAttachment(destination, destUnit));
				OpenGLCheckError();
			}
			else {
				//bind to default, draws to default
				unbind(RenderTargetMode::Draw);
			}

			glBlitFramebuffer(0, 0, this->getWidth(), this->getHeight(), xoff, yoff, xoff + width, yoff + height,
				GL_COLOR_BUFFER_BIT, GL_LINEAR);
			OpenGLCheckError();

			
		

		}
	}


}