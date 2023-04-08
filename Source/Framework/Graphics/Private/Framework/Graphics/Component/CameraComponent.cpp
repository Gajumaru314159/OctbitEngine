//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Component/CameraComponent.h>

namespace ob::graphics {


	CameraComponent::CameraComponent() {

		m_type = CameraType::Perspective;
		m_rect = Rect(0,0,1,1);
		m_fovY = 60.f;
		m_clipRange = Range(0.001f,10000.0f);
		m_display = 0;
		m_clearColor = Color::Black;
		m_renderTexture = nullptr;

	}

	// Field of View
	f32 CameraComponent::getFov()const {
		return m_fovY;
	}
	void CameraComponent::setFov(f32 value) {
		setProperty(m_fovY, value, TC("FovY"));
	}

	// クリップ範囲
	Range CameraComponent::getClipRange()const {
		return m_clipRange;
	}
	void CameraComponent::setClipRange(Range value) {
		setProperty(m_clipRange, value, TC("ClipRange"));
	}

	// カメラ形式
	CameraType CameraComponent::getCameraType()const {
		return m_type;
	}
	void CameraComponent::setCameraType(CameraType value) {
		setProperty(m_type, value, TC("CameraType"));
	}

	// Viewport Rect
	Rect CameraComponent::getVieportRect() {
		return m_rect;
	}
	void CameraComponent::setVieportRect(Rect value) {
		setProperty(m_rect, value, TC("ViewportRect"));
	}


	Color CameraComponent::getClearColor()const {
		return m_clearColor;
	}
	void CameraComponent::setClearColor(const Color& value) {
		setProperty(m_clearColor, value, TC("ClearColor"));
	}

	// 描画先(ディスプレイ)
	s32 CameraComponent::getDisplay()const {
		return m_display;
	}
	void CameraComponent::setDisplay(s32 displayNo) {
		setProperty(m_display, displayNo, TC("Display"));
	}

	// 描画先(RenderTexture)
	auto CameraComponent::getRenderTarget()const -> const Ref<rhi::RenderTexture>& {
		return m_renderTexture;
	}
	void CameraComponent::setRenderTarget(const Ref<rhi::RenderTexture>& value) {
		m_renderTexture = value;
	}


	const Matrix& CameraComponent::getViewMatrix()const {
		OB_NOTIMPLEMENTED();
		return Matrix::Identity;
	}
	const Matrix& CameraComponent::getProjectionMatrix()const {
		OB_NOTIMPLEMENTED();
		return Matrix::Identity;
	}
	const Matrix& CameraComponent::getViewProjectionMatrix()const {
		OB_NOTIMPLEMENTED();
		return Matrix::Identity;
	}

	Vec3 CameraComponent::worldToScreen(const Vec3& position)const {
		OB_NOTIMPLEMENTED();
		return Vec3::Zero;
	}
	Vec3 CameraComponent::screenToWorld(const Vec3& position)const {

		OB_NOTIMPLEMENTED();
		return Vec3::Zero;
	}


}// namespcae ob