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
					return GL_COLOR_ATTACHMENT0 + unit;
				case RenderTargetAttachment::Depth:
					return GL_DEPTH_STENCIL_ATTACHMENT;
				}
			}
			unsigned int GetOpenGLAttachmentFormat(RenderTargetAttachment attachment)
			{
				switch (attachment)
				{

				case RenderTargetAttachment::Scene:
				case RenderTargetAttachment::Diffuse:
				case RenderTargetAttachment::Specular:
				case RenderTargetAttachment::Emissive:
					return GL_RGBA;
				case RenderTargetAttachment::Position:
				case RenderTargetAttachment::Normal:
					return GL_RGB;
				case RenderTargetAttachment::Depth:
					return GL_DEPTH24_STENCIL8;
				}
			}
			unsigned int GetOpenGLAttachmentInternalFormat(RenderTargetAttachment attachment)
			{
				switch (attachment)
				{

				case RenderTargetAttachment::Scene:
				case RenderTargetAttachment::Diffuse:
				case RenderTargetAttachment::Specular:
				case RenderTargetAttachment::Emissive:
					return GL_RGBA;
				case RenderTargetAttachment::Position:
				case RenderTargetAttachment::Normal:
					return GL_RGB;
				case RenderTargetAttachment::Depth:
					return GL_DEPTH24_STENCIL8;

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


			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				RenderTargetAttachment attachment = RenderTargetAttachment(i);
				unsigned int internalFormat = GetOpenGLAttachmentInternalFormat(attachment);
				unsigned int format = GetOpenGLAttachmentFormat(attachment);
				if (_buffers[i].used)
				{

					unsigned int target = GetOpenGLAttachment(attachment, _buffers[i].unit);
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

			OpenGLCheckError();
			//attach it to the framebuffer


			// finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Setup Failed ! Incomplete Framebuffer !" << std::endl;
	

			unbind();
		}
		//https://stackoverflow.com/questions/51030120/concept-what-is-the-use-of-gldrawbuffer-and-gldrawbuffers
		void OpenGLRenderTarget::setup(const std::vector<RenderTargetAttachment>& attachments)
		{

			unsigned int* layout = new unsigned int[attachments.size()];
			int unit = 0;
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
				if (attachment != RenderTargetAttachment::Depth) {
					unsigned int target = GetOpenGLAttachment(attachment, unit);
					layout[unit] = target;
				}
				//create texture
				buffer.used = true;
				buffer.unit = unit;
				unit++;
				glGenTextures(1, &buffer.texture);
				glBindTexture(GL_TEXTURE_2D, buffer.texture);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				OpenGLCheckError();

			}
			//setup draw buffer
			//count number of used buffers to allocat
			glDrawBuffers(getNumberOfAttachments(), layout);
			delete[]layout;

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
		}
		void OpenGLRenderTarget::bindTextures(int offset)
		{
			int unit = offset;
			for (int i = 0; i < (const int)RenderTargetAttachment::Count; i++)
			{
				if (_buffers[i].used)
				{
					glActiveTexture(GL_TEXTURE0+ _buffers[i].unit+offset);
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
					uniforms.setUniform(RenderTargetAttachmentUniformName(attachment), _buffers[i].unit);

				}
			}
		}
		int OpenGLRenderTarget::getAttachmentUnit(RenderTargetAttachment attachment)
		{
			return GetOpenGLAttachment(attachment, _buffers[(int)attachment].unit);

		}
		void OpenGLRenderTarget::unloadAttachment(RenderTarget* target, RenderTargetAttachment source, RenderTargetAttachment destination, int xoff, int yoff, int width, int height)
		{
			int srcUnit = _buffers[(int)source].unit;
			int destUnit = 0;
			OpenGLRenderTarget* openglTarget = dynamic_cast<OpenGLRenderTarget*>(target);
			if (openglTarget )
			{
				destUnit = openglTarget->_buffers[(int)destination].unit;
			}

			int filter = source == RenderTargetAttachment::Depth ? GL_NEAREST: GL_LINEAR;
			int bufferMask = source == RenderTargetAttachment::Depth ? GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT;
			bind(RenderTargetMode::Read);
			glReadBuffer(GetOpenGLAttachment(source, srcUnit));
			if (target) {
				target->bind(RenderTargetMode::Draw);
				if(source != RenderTargetAttachment::Depth)
					glDrawBuffer(GetOpenGLAttachment(destination, destUnit));
				glBlitFramebuffer(0, 0, this->getWidth(), this->getHeight(), xoff, yoff, xoff+ width, yoff+height,
					bufferMask, filter);
			}
			else
			{
				//bind to default
				unbind(RenderTargetMode::Draw);
				if (source != RenderTargetAttachment::Depth)
					glDrawBuffer(GetOpenGLAttachment(destination, destUnit));

				glBlitFramebuffer(0, 0, this->getWidth(), this->getHeight(), xoff, yoff, xoff + width, yoff + height,
					bufferMask, filter);
			}

		}
	}


}