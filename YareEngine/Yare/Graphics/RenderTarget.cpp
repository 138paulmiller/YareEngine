#include "Graphics.hpp"
#include "RenderTarget.hpp"
#include "OpenGL/OpenGLRenderTarget.hpp"
namespace yare {
	namespace graphics{
		RenderTarget* RenderTarget::Create()
		{
			switch (Graphics::GetRenderAPI())
			{
			case RenderAPI::OpenGL:
				return new OpenGLRenderTarget();
			default:
				YARE_ASSERT(false, "No Render platform selected! ");
				return 0;
			}
		}
	}
}