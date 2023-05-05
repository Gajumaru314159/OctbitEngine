//***********************************************************
//! @file
//! @brief		カメラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Camera/CameraImpl.h>
#include <Framework/Graphics/Camera/CameraManager.h>

namespace ob::graphics {

	Ref<Camera> Camera::Create() {
		return nullptr;// new Camera();
	}


	CameraImpl::CameraImpl() {
		m_fovY = 60.0f;
		CameraManager::Get().add(this);
	}
	CameraImpl::~CameraImpl() {
		CameraManager::Get().remove(this);
	}


	f32 CameraImpl::getFov()const {
		return m_fovY;
	}
	void CameraImpl::setFov(f32 value) {
		m_fovY = value;
	}

	Range CameraImpl::getClipRange()const {
		return { m_viewport.nearZ,m_viewport.farZ };
	}
	void CameraImpl::setClipRange(Range range) {
		m_viewport.nearZ = range.min;
		m_viewport.farZ = range.max;
	}

	CameraType CameraImpl::getCameraType()const {
		return m_type;
	}
	void CameraImpl::setCameraType(CameraType type) {
		m_type = type;
	}

	const Matrix& CameraImpl::getViewMatrix()const {
		//Matrix::Perspective(m_fovY, m_viewport.aspect(), m_viewport.nearZ, m_viewport.farZ)* m_transform.toMatrix().inverse();
		OB_NOTIMPLEMENTED();
		return m_viewMatrix;
	}
	const Matrix& CameraImpl::getProjectionMatrix()const {

		//Matrix::Perspective(m_fovY, m_viewport.aspect(), m_viewport.nearZ, m_viewport.farZ)* m_transform.toMatrix().inverse();
		OB_NOTIMPLEMENTED();
		return m_projMatrix;
	}
	const Matrix& CameraImpl::getViewProjectionMatrix()const {
		//Matrix::Perspective(m_fovY, m_viewport.aspect(), m_viewport.nearZ, m_viewport.farZ)* m_transform.toMatrix().inverse();
		OB_NOTIMPLEMENTED();
		return m_viewprojMatrix;
	}

	Vec3 CameraImpl::worldToScreen(const Vec3& position)const {
		OB_NOTIMPLEMENTED();
		return Vec3::Zero;
	}

	Vec3 CameraImpl::screenToWorld(const Vec3& position,f32 distance)const {
		OB_NOTIMPLEMENTED();
		return Vec3::Zero;
	}



	Rect CameraImpl::getVieportRect() {
		return m_rect;
	}
	void CameraImpl::setVieportRect(Rect rect) {
		m_rect = rect;
	}

	void CameraImpl::setRenderTarget(s32 displayNo) {
		m_display = displayNo;
	}
	void CameraImpl::setRenderTarget(const Ref<rhi::RenderTexture>& value) {
		m_renderTexture = value;
	}
	auto CameraImpl::getRenderTarget()const -> const Ref<rhi::RenderTexture>& {
		return m_renderTexture;
	}


	f32 CameraImpl::getAspect()const {
		return m_viewport.aspect();
	}

	CameraType CameraImpl::getType()const {
		return m_type;
	}




	//LayerMask CameraImpl::getLayerMask()const {
	//	return m_layerMask;
	//}
	//void CameraImpl::setLayerMask(LayerMask mask) {
	//	m_layerMask = mask;
	//}
	//auto CameraImpl::getPriority()const->CameraPriority {
	//	return m_priority;
	//}
	//void CameraImpl::setPriority(CameraPriority priority) {
	//	m_priority = priority;
	//}




}