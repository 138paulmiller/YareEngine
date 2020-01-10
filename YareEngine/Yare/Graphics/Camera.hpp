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
		) :_up(up), _position({ 0,0,0 }
		) {

			setForward(forward);
		}
		virtual ~Camera(){

		}


		virtual void setUp(const glm::vec3& up)
		{
			_up = up;
		}

		virtual void setForward(const glm::vec3& forward)
		{
			_forward= forward;
			_right = glm::cross(_forward, _up);
			_view = glm::lookAt(_position, _position + _forward, _up);
		}

		virtual void setPosition(const glm::vec3& position)
		{
			_position = position;
			_view = glm::lookAt(_position, _position + _forward, _up);

		}

		virtual void setProjection(const glm::mat4& projection)
		{
			_projection = projection;
		}

		virtual const glm::vec3& getPosition()const { return _position; }

		virtual const glm::mat4& getProjection() const { return _projection; }
		virtual const glm::mat4& getView() const { return _view; }

		virtual void unloadUniforms(UniformBlock& uniforms) const;

	private:
		glm::vec3 _position;
		glm::vec3 _up;
		glm::vec3 _right;
		glm::vec3 _forward;
		glm::mat4 _view;
		glm::mat4 _projection;
	};

	class PerspectiveCamera :public Camera
	{
	public:
		PerspectiveCamera(
			float fovy, float aspect,
			float near = 0.1, float far = 1000,
			const glm::vec3& up = { 0,1,0 },
			const glm::vec3& forward = { 0,0,-1 }) :
			Camera(up, forward)
		{
			Camera::setProjection(glm::perspective(fovy, aspect, near, far));
		}

		void unloadUniforms(UniformBlock& uniforms) const override {
			Camera::unloadUniforms(uniforms);
			uniforms.setUniform("near", near);
			uniforms.setUniform("far", far);
		}
	private:
		float near, far;

	};
	class OrthographicCamera :public Camera
	{
	public:
		OrthographicCamera(
			float left = -10, float right  =10, float top = 10, float bottom = -10,
			float near = 0.1, float far = 1000,
			const glm::vec3& up = { 0,1,0 },
			const glm::vec3& forward = { 0,0,-1 }) :
			Camera(up, forward)
		{
			Camera::setProjection(glm::ortho(left, right, top, bottom, near, far));

		}
		void unloadUniforms(UniformBlock& uniforms) const override {
			Camera::unloadUniforms(uniforms);
			uniforms.setUniform("near", near);
			uniforms.setUniform("far", far);
		}
	private:
		float near, far;

	};

} }