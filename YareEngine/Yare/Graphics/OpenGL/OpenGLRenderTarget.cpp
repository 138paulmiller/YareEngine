//	Can attach multiple render to render to.  

#include "OpenGLRenderTarget.hpp"
#include "OpenGLTexture.hpp"

namespace yare {
	namespace graphics {
		OpenGLRenderTarget::OpenGLRenderTarget()
		{

			glGenFramebuffers(1, &_framebuffer);
			bind();
			OpenGLCheckError();

			glGenRenderbuffers(1, &_renderbuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
			OpenGLCheckError();
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 0,0); // use a single renderbuffer object for both a depth AND stencil buffer.
			OpenGLCheckError();
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderbuffer); // now actually attach it
			OpenGLCheckError();

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
			bind();
			glDeleteRenderbuffers(1, &_renderbuffer);
			glDeleteFramebuffers(1, &_framebuffer);
		}
		
		void OpenGLRenderTarget::resize(int width, int height)
		{
			RenderTarget::resize(width, height);

			bind();
			OpenGLCheckError();
			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				if (_buffers[i].used)
				{
					glBindTexture(GL_TEXTURE_2D, _buffers[i].texture);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, width, 0, GL_RGB, GL_FLOAT, NULL);

				}
			}
			glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
			OpenGLCheckError();
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, getWidth(), getHeight()); // use a single renderbuffer object for both a depth AND stencil buffer.
			OpenGLCheckError();
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderbuffer); // now actually attach it
			OpenGLCheckError();

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			OpenGLCheckError();
			unbind();
		}

		void OpenGLRenderTarget::use(RenderTargetAttachment attachment) 
		{
			bind();
			//Initialize the buffer object
			OpenGLRenderBuffer & buffer  = _buffers[(int)attachment];
			if (buffer.used)
			{
				std::cout << "Error: Buffer Already Used!";
				return;
			}
			//create texture
			buffer.used = true;

			glGenTextures(1, &buffer.texture);
			glBindTexture(GL_TEXTURE_2D, buffer.texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			OpenGLCheckError();

			
			switch (attachment)
			{
			case RenderTargetAttachment::Color:
			case RenderTargetAttachment::Position:
			case RenderTargetAttachment::Normal:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (int)attachment, GL_TEXTURE_2D, buffer.texture, 0);
				break;
			}
			OpenGLCheckError();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		}
		void OpenGLRenderTarget::setup()
		{
			//count number of used buffers to allocat
			_numUsed = 0;
			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				if (_buffers[i].used)
				{
					_numUsed++;
				}
			}
			unsigned int *attachments = new unsigned int[_numUsed];
			int j = 0;
			int colorAttachmentCount = 0;

			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				if (_buffers[i].used)
				{
					switch (RenderTargetAttachment(i))
					{
						//All use color buffer for data
						case RenderTargetAttachment::Color:
						case RenderTargetAttachment::Normal:
						case RenderTargetAttachment::Position:
							//See on creation, 
							attachments[j] = GL_COLOR_ATTACHMENT0 + i;

						break;
					}

					j++;
				}
			}
			bind();
			glDrawBuffers(_numUsed, attachments);
			OpenGLCheckError();

			// finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Setup Failed ! Incomplete Framebuffer !" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			OpenGLCheckError();

		}
		void OpenGLRenderTarget::bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		}
		void OpenGLRenderTarget::unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		void OpenGLRenderTarget::bindTextures()
		{
			int unit = 0;
			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				unit = i; //should this match attachment point?
				if (_buffers[i].used)
				{
					glActiveTexture(GL_TEXTURE0+unit);
					glBindTexture(GL_TEXTURE_2D, _buffers[i].texture);
				}
			}
		}

	}
}