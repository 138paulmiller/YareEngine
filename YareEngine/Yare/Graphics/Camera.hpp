#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>
#include "UniformBlock.hpp"
namespace yare { namespace graphics {

	class Camera
	{
	public:
		Camera(
			const glm::vec3& up = { 0,1,0 },
			const glm::vec3& forward = { 0,0,-1 }
		) :_up(up), _position({0,0,0}) {

			setForward(forward);
		}

		void setForward(const glm::vec3& forward) 
		{
			_forward= forward;
			_right = glm::cross(_forward, _up);
			_view = glm::lookAt(_position, _position + _forward, _up);
		}

		void setPosition(const glm::vec3& position)
		{
			_position = position;
			_view = glm::lookAt(_position, _position + _forward, _up);

		}

		void setProjection(const glm::mat4& projection)
		{
			_projection = projection;
		}

		const glm::vec3& getPosition()const { return _position; }

		const glm::mat4& getProjection() const { return _projection; }
		const glm::mat4& getView() const { return _view; }

		void loadUniforms(UniformBlock& uniforms) const;
		
	private:
		glm::vec3 _position;
		glm::vec3 _up;
		glm::vec3 _right;
		glm::vec3 _forward;
		glm::mat4 _view;
		glm::mat4 _projection;
	};

} }