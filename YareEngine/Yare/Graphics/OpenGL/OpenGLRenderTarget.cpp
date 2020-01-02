//	Can attach multiple render to render to.  

#include "OpenGLRenderTarget.hpp"
#include "OpenGLTexture.hpp"


//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/#using-the-depth
namespace yare {
	namespace graphics {



		OpenGLRenderTarget::OpenGLRenderTarget()
		{

			glGenFramebuffers(1, &_framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
			glGenRenderbuffers(1, &_depthStencilbuffer);

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			OpenGLCheckError();


			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				_buffers[i].used = false;
			}
		}

		OpenGLRenderTarget::~OpenGLRenderTarget()
		{
			unbind();
			
			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				glDeleteTextures(1, &_buffers[i].texture);
			}
			glDeleteRenderbuffers(1, &_depthStencilbuffer);
			glDeleteFramebuffers(1, &_framebuffer);
		}
		namespace 
		{
			unsigned int GetOpenGLAttachment(RenderTargetAttachment attachment)
			{
				switch (attachment)
				{
				case RenderTargetAttachment::Diffuse:
				case RenderTargetAttachment::Specular:
				case RenderTargetAttachment::Emissive:
				case RenderTargetAttachment::Position:
				case RenderTargetAttachment::Normal:
					return GL_COLOR_ATTACHMENT0 + (int)attachment;
				case RenderTargetAttachment::Depth:
					return GL_DEPTH_ATTACHMENT;
				}
			}
			unsigned int GetOpenGLAttachmentFormat(RenderTargetAttachment attachment)
			{
				switch (attachment)
				{
				case RenderTargetAttachment::Diffuse:
				case RenderTargetAttachment::Specular:
				case RenderTargetAttachment::Emissive:
					return GL_RGBA;
				case RenderTargetAttachment::Position:
				case RenderTargetAttachment::Normal:
					return GL_RGB;
				case RenderTargetAttachment::Depth:
					return GL_DEPTH_COMPONENT;
				}
			}
			unsigned int GetOpenGLAttachmentInternalFormat(RenderTargetAttachment attachment)
			{
				switch (attachment)
				{
				case RenderTargetAttachment::Diffuse:
				case RenderTargetAttachment::Specular:
				case RenderTargetAttachment::Emissive:
					return GL_RGBA;
				case RenderTargetAttachment::Position:
				case RenderTargetAttachment::Normal:
					return GL_RGB;
				case RenderTargetAttachment::Depth:
					return GL_DEPTH_COMPONENT24;

				}
			}
			const std::string RenderTargetAttachmentUniformName(RenderTargetAttachment attachment)
			{
				switch (attachment)
				{
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

				}
			}
		}
		int OpenGLRenderTarget::getNumberOfAttachments()
		{
			_numUsed = 0;
			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				if (_buffers[i].used)
				{
					_numUsed++;
				}
			}
			return _numUsed;
		}
		void OpenGLRenderTarget::resize(int width, int height)
		{
			RenderTarget::resize(width, height);
			bind();
			//count number of used buffers to allocat
			unsigned int* attachments = new unsigned int[_numUsed];
			int j = 0;

			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				RenderTargetAttachment attachment = RenderTargetAttachment(i);
				unsigned int internalFormat = GetOpenGLAttachmentInternalFormat(attachment);
				unsigned int format = GetOpenGLAttachmentFormat(attachment);
				unsigned int target = GetOpenGLAttachment(attachment);
				if (_buffers[i].used)
				{
					if (attachment != RenderTargetAttachment::Depth) {
						attachments[j++] = target;
					}
					glBindTexture(GL_TEXTURE_2D, _buffers[i].texture);

					glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, NULL);
					glBindTexture(GL_TEXTURE_2D, 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, _buffers[i].texture, 0);

					OpenGLCheckError();
					

				}
			}

			glBindRenderbuffer(GL_RENDERBUFFER, _depthStencilbuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilbuffer);
			OpenGLCheckError();

			glDrawBuffers(_numUsed, attachments);
			OpenGLCheckError();
			//attach it to the framebuffer


			// finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Setup Failed ! Incomplete Framebuffer !" << std::endl;
	

			unbind();
		}

		void OpenGLRenderTarget::setup(const std::vector<RenderTargetAttachment>& attachments)
		{
			bind();
			for (const RenderTargetAttachment & attachment : attachments)
			{
				//Initialize the buffer object
				OpenGLRenderBuffer& buffer = _buffers[(int)attachment];
				if (buffer.used)
				{
					std::cout << "Error: Buffer Already Used!";
					return;
				}
				//create texture
				buffer.used = true;

				glGenTextures(1, &buffer.texture);
				glBindTexture(GL_TEXTURE_2D, buffer.texture);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				OpenGLCheckError();

			}
			unbind();
			getNumberOfAttachments();

		}
		void OpenGLRenderTarget::bind(RenderTargetMode mode )
		{
			switch (mode)
			{
			case RenderTargetMode::ReadWrite:
				glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
				return;
			case RenderTargetMode::Read:
				glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);
				return;
			case RenderTargetMode::Draw:
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
				return;
			}
		}
		void OpenGLRenderTarget::unbind(RenderTargetMode mode)
		{
			switch (mode)
			{
			case RenderTargetMode::ReadWrite:
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				return;
			case RenderTargetMode::Read:
				glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
				return;
			case RenderTargetMode::Draw:
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				return;
			}
		}
		void OpenGLRenderTarget::bindTextures(int offset)
		{
			int unit = offset;
			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				if (_buffers[i].used)
				{
					glActiveTexture(GL_TEXTURE0+unit++);
					glBindTexture(GL_TEXTURE_2D, _buffers[i].texture);
					OpenGLCheckError();
				}
			}
		}

		void OpenGLRenderTarget::unloadUniforms(UniformBlock& uniforms)
		{
			int j = 0;
			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				RenderTargetAttachment attachment = RenderTargetAttachment(i);
				if (_buffers[i].used)
				{
					uniforms.setUniform(RenderTargetAttachmentUniformName(attachment), j++);

				}
			}
		}
		
		void OpenGLRenderTarget::unloadAttachment(RenderTarget* target, RenderTargetAttachment source, RenderTargetAttachment destination, int xoff, int yoff, int width, int height)
		{
			int filter = source == RenderTargetAttachment::Depth ? GL_NEAREST: GL_LINEAR;
			int bufferMask = source == RenderTargetAttachment::Depth ? GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT;
			bind(RenderTargetMode::Read);
			glReadBuffer(GetOpenGLAttachment(source));
			if (target) {
				target->bind(RenderTargetMode::Draw);
				if(source != RenderTargetAttachment::Depth)
					glDrawBuffer(GetOpenGLAttachment(destination));
				glBlitFramebuffer(0, 0, this->getWidth(), this->getHeight(), xoff, yoff, xoff+ width, yoff+height,
					bufferMask, filter);
			}
			else
			{
				//bind to default
				unbind(RenderTargetMode::Draw);
				if (source != RenderTargetAttachment::Depth)
					glDrawBuffer(GetOpenGLAttachment(destination));

				glBlitFramebuffer(0, 0, this->getWidth(), this->getHeight(), xoff, yoff, xoff + width, yoff + height,
					bufferMask, filter);
			}

		}
	}


}