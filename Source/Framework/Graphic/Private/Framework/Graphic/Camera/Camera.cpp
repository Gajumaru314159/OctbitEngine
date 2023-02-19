//***********************************************************
//! @file
//! @brief		カメラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Camera.h>

namespace ob::graphic {

	Ref<Camera> Camera::Create() {
		return new Camera();
	}

	Ref<Camera> Camera::GetMain() {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}
	Array<Ref<Camera>> Camera::GetAll() {
		OB_NOTIMPLEMENTED();
		return {};
	}

	Camera::Camera() {
		m_fovY = 60.0f;
	}


	f32 Camera::getFov()const {
		return m_fovY;
	}
	void Camera::setFov(f32 value) {
		m_fovY = value;
	}

	Range Camera::getClipRange()const {
		return { m_viewport.nearZ,m_viewport.farZ };
	}
	void Camera::setClipRange(Range range) {
		m_viewport.nearZ = range.min;
		m_viewport.farZ = range.max;
	}

	CameraType Camera::getCameraType()const {
		return m_type;
	}
	void Camera::setCameraType(CameraType type) {
		m_type = type;
	}

	const Matrix& Camera::getViewMatrix()const {
		//Matrix::Perspective(m_fovY, m_viewport.aspect(), m_viewport.nearZ, m_viewport.farZ)* m_transform.toMatrix().inverse();
		OB_NOTIMPLEMENTED();
		return m_viewMatrix;
	}
	const Matrix& Camera::getProjectionMatrix()const {

		//Matrix::Perspective(m_fovY, m_viewport.aspect(), m_viewport.nearZ, m_viewport.farZ)* m_transform.toMatrix().inverse();
		OB_NOTIMPLEMENTED();
		return m_projMatrix;
	}
	const Matrix& Camera::getViewProjectionMatrix()const {
		//Matrix::Perspective(m_fovY, m_viewport.aspect(), m_viewport.nearZ, m_viewport.farZ)* m_transform.toMatrix().inverse();
		OB_NOTIMPLEMENTED();
		return m_viewprojMatrix;
	}

	Vec3 Camera::worldToScreen(const Vec3& position)const {
		OB_NOTIMPLEMENTED();
		return Vec3::Zero;
	}

	Vec3 Camera::screenToWorld(const Vec3& position)const {
		OB_NOTIMPLEMENTED();
		return Vec3::Zero;
	}





	LayerMask Camera::getLayerMask()const {
		return m_layerMask;
	}
	void Camera::setLayerMask(LayerMask mask) {
		m_layerMask = mask;
	}

	Rect Camera::getVieportRect() {
		return m_rect;
	}
	void Camera::setVieportRect(Rect rect) {
		m_rect = rect;
	}

	void Camera::setRenderTarget(s32 displayNo) {
		m_display = displayNo;
	}
	void Camera::setRenderTarget(const Ref<rhi::RenderTexture>& value) {
		m_renderTexture = value;
	}
	auto Camera::getRenderTarget()const -> const Ref<rhi::RenderTexture>& {
		return m_renderTexture;
	}


	f32 Camera::getAspect()const {
		return m_viewport.aspect();
	}

	CameraType Camera::getType()const {
		return m_type;
	}

	auto Camera::getPriority()const->CameraPriority {
		return m_priority;
	}
	void Camera::setPriority(CameraPriority priority) {
		m_priority = priority;
	}




}// namespcae ob