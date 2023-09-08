//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component/CameraComponent.h>
#include <Framework/Graphics/Camera.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	CameraComponent::CameraComponent() {

		m_type = graphics::CameraType::Perspective;
		m_rect = Rect(0,0,1,1);
		m_fovY = 60.f;
		m_clipRange = Range(0.001f,10000.0f);
		m_display = 0;
		m_clearColor = Color::Black;
		m_renderTexture = nullptr;

	}

	//@―---------------------------------------------------------------------------
	//! @brief  RenderViewを設定
	//@―---------------------------------------------------------------------------
	void CameraComponent::setRenderView(UPtr<graphics::RenderView> renderView) {
		m_camera = graphics::Camera::Create(std::move(renderView));
	}

	//@―---------------------------------------------------------------------------
	//! @brief  Field of View
	//@―---------------------------------------------------------------------------
	f32 CameraComponent::getFov()const {
		return m_fovY;
	}
	void CameraComponent::setFov(f32 value) {
		if (setProperty(m_fovY, value, TC("FovY"))) {
			m_camera->setFov(value);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  クリップ範囲
	//@―---------------------------------------------------------------------------
	Range CameraComponent::getClipRange()const {
		return m_clipRange;
	}
	void CameraComponent::setClipRange(Range value) {
		if (setProperty(m_clipRange, value, TC("ClipRange"))) {
			m_camera->setClipRange(value);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  カメラ形式
	//@―---------------------------------------------------------------------------
	graphics::CameraType CameraComponent::getCameraType()const {
		return m_type;
	}
	void CameraComponent::setCameraType(graphics::CameraType value) {
		if (setProperty(m_type, value, TC("CameraType"))) {
			m_camera->setCameraType(value);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  Viewport Rect
	//@―---------------------------------------------------------------------------
	Rect CameraComponent::getVieportRect() {
		return m_rect;
	}
	void CameraComponent::setVieportRect(Rect value) {
		if (setProperty(m_rect, value, TC("ViewportRect"))) {
			m_camera->setVieportRect(m_rect);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  背景色
	//@―---------------------------------------------------------------------------
	Color CameraComponent::getClearColor()const {
		return m_clearColor;
	}
	void CameraComponent::setClearColor(const Color& value) {
		setProperty(m_clearColor, value, TC("ClearColor"));
	}

	//@―---------------------------------------------------------------------------
	//! @brief  描画先(ディスプレイ)
	//@―---------------------------------------------------------------------------
	s32 CameraComponent::getDisplay()const {
		return m_display;
	}
	void CameraComponent::setDisplay(s32 displayNo) {
		setProperty(m_display, displayNo, TC("Display"));
	}

	//@―---------------------------------------------------------------------------
	//! @brief  描画先(RenderTexture)
	//@―---------------------------------------------------------------------------
	auto CameraComponent::getRenderTarget()const -> const Ref<rhi::RenderTexture>& {
		return m_renderTexture;
	}
	void CameraComponent::setRenderTarget(const Ref<rhi::RenderTexture>& value) {
		m_renderTexture = value;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ビュー行列
	//@―---------------------------------------------------------------------------
	const Matrix& CameraComponent::getViewMatrix()const {
		OB_NOTIMPLEMENTED();
		return Matrix::Identity;
	}
	//@―---------------------------------------------------------------------------
	//! @brief  プロジェクション行列
	//@―---------------------------------------------------------------------------
	const Matrix& CameraComponent::getProjectionMatrix()const {
		OB_NOTIMPLEMENTED();
		return Matrix::Identity;
	}
	//@―---------------------------------------------------------------------------
	//! @brief  ビュー・プロジェクション行列
	//@―---------------------------------------------------------------------------
	const Matrix& CameraComponent::getViewProjectionMatrix()const {
		OB_NOTIMPLEMENTED();
		return Matrix::Identity;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ワールド座標をスクリーン座標に変換
	//@―---------------------------------------------------------------------------
	Vec3 CameraComponent::worldToScreen(const Vec3& position)const {
		OB_NOTIMPLEMENTED();
		return Vec3::Zero;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  スクリーン座標をワールド座標に変換
	//@―---------------------------------------------------------------------------
	Vec3 CameraComponent::screenToWorld(const Vec3& position, f32 distance)const {

		OB_NOTIMPLEMENTED();
		return Vec3::Zero;
	}


}