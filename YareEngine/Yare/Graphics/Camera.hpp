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
		virtual void setAspect(float aspect)
		{
		
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
			float near = 1, float far = 100,
			const glm::vec3& up = { 0,1,0 },
			const glm::vec3& forward = { 0,0,-1 }) :
			Camera(up, forward)
		{
			_fovy = fovy;
			_near = near;
			_far = far;
			Camera::setProjection(glm::perspective(fovy, aspect, near, far));
		}
		void setAspect(float aspect)
		{
			Camera::setProjection(glm::perspective(_fovy, aspect, _near, _far));

		}

		void unloadUniforms(UniformBlock& uniforms) const override {
			Camera::unloadUniforms(uniforms);
			uniforms.setUniform("near", _near);
			uniforms.setUniform("far", _far);
		}
	private:
		float _near, _far, _fovy;

	};
	class OrthographicCamera :public Camera
	{
	public:
		OrthographicCamera(
			float width  = 10, float height =10, 
			float near = 1, float far = 100,
			const glm::vec3& up = { 0,1,0 },
			const glm::vec3& forward = { 0,0,-1 }) :
			Camera(up, forward)
		{
			_near = near;
			_far = far;
			_height = height;
			_width = width;
			setAspect(1.f);
		}

		void setAspect(float aspect)
		{
			float boundX = _width * aspect;
			float boundY = _height;
		
			Camera::setProjection(glm::ortho(-1.f * boundX, boundX, -1.f * boundY, boundY, _near, _far));
		}

		void unloadUniforms(UniformBlock& uniforms) const override {
			Camera::unloadUniforms(uniforms);
			uniforms.setUniform("near", _near);
			uniforms.setUniform("far", _far);
		}
	private:
		float _near, _far;
		float _width, _height;
	};

} }