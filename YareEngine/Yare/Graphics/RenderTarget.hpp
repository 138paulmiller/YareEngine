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
		Color=0, //Multiple Color Attachments are allowed 
		Depth,
		Stencil,
		Custom
	};
;	class RenderTarget
	{
	public:
		virtual ~RenderTarget()= default;
		virtual void addTarget(RenderTargetAttachment attachment, Texture * texture) = 0;
		virtual void bind() = 0;

	private:
		//Nonowning. 
		std::vector <Texture*> _textures;
	};

}}