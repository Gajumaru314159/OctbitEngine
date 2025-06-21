//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/AnyContainer.h>
#include <Framework/Graphics/Render/RenderScene.h>

namespace ob::graphics {

	struct RenderViewData {
		String name; //!< ビュー名
		s32 pipeline = 0; //!< パイプライン番号
		s32 priority = 0; //!< 優先度
	};


	//! @brief      レンダー・ビュー
	//! @ref 		RenderView
	class RenderView {
	public:

		//! @brief      コンストラクタ
		RenderView(RenderScene& scene, const RenderViewData& data);

		//! @brief      コンストラクタ
		RenderView(RenderScene& scene, StringView name, s32 pipeline = 0);

		//! @brief      デストラクタ
		~RenderView();

		//! @brief      シーンを取得
		auto getScene() -> RenderScene& { return m_scene; }
		auto getScene() const -> const RenderScene& { return m_scene; }

		//! @brief 指定した型Tのインスタンスへの参照を取得します。
		//! @tparam T 取得するインスタンスの型。
		//! @return 型Tのインスタンスへの参照。
		template<class T>
		auto get() -> std::enable_if_t<std::is_default_constructible<T>::value, T&> {
			return m_container.get<T>();
		}

		//! @brief 指定した型のインスタンスへの定数参照を取得します。
		//! @tparam T 取得するインスタンスの型。
		//! @return 型Tのインスタンスへのconst参照。指定した型がコンテナに存在しない場合はアサートに失敗します。
		template<class T>
		auto get() const -> std::enable_if_t<std::is_default_constructible<T>::value, const T&> {
			return m_container.get<T>();
		}

		//! @brief 指定した型の要素がコンテナに含まれているかどうかを判定します。
		//! @tparam T 検索する要素の型。
		//! @return 指定した型の要素がコンテナに含まれていれば true、そうでなければ false を返します。
		template<class T>
		bool contains() const {
			return m_container.contains<T>();
		}

		//! @brief 指定された型がコンテナに含まれているかどうかを判定します。
		//! @param type 検索対象となる型の参照。
		//! @return 型がコンテナに含まれていれば true、そうでなければ false を返します。
		bool contains(const Type& type) const {
			return m_container.contains(type);
		}

		//! @brief      RenderFeatureを見つける
		template<class T> T* findFeature()const;
		RenderFeature* findFeature(Type type)const;

	private:
		RenderScene& m_scene;
		AnyContainer            m_container;
	};

	template<class T>
	T* RenderView::findFeature()const {
		return reinterpret_cast<T*>(findFeature(Type::Get<T>()));
	}

}