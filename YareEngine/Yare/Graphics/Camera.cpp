
#include "Camera.hpp"

namespace yare {
	namespace graphics {

	void Camera::unloadUniforms(UniformBlock& uniforms) const
	{
		//If this is a deferred render. instead of adding lights to the pass. Specify which buffers are going to be written to. 
		//e.g. For flat, it should only be color
		//use UBOs and render views for this
		uniforms.setUniform("view", getView());
		uniforms.setUniform("projection", getProjection());
		uniforms.setUniform("view_pos", getPosition());

	}
	}
}