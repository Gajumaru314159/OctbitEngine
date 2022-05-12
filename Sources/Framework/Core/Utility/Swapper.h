﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	template<typename T>
	class Swapper {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================
		
		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		Swapper():m_index(0) {
		}


		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//! 
		//! @details	インスタンスの引数はコピー渡しされます。
		//! @param size	要素数
		//@―---------------------------------------------------------------------------
		template<class... Args>
		Swapper(size_t size,Args&... args):Swapper(){
			OB_CHECK_ASSERT(size, "Swapperのサイズは0にできません。");
			m_instances.reserve(size);
			for (size_t i = 0; i < size; ++i) {
				m_instances.emplace_back(args...);
			}
		}


		//===============================================================
		// 領域
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief		バックバッファの数を取得
		//@―---------------------------------------------------------------------------
		size_t size()const noexcept {
			return m_instances.size();
		}


		//@―---------------------------------------------------------------------------
		//! @brief		要素数を変更
		//@―---------------------------------------------------------------------------
		void resize(size_t newSize) {
			m_instances.resize(newSize);
		}


		//@―---------------------------------------------------------------------------
		//! @brief		要素数を変更
		//@―---------------------------------------------------------------------------
		void resize(size_t size_,const T& c) {
			m_instances.resize(size_,c);
		}


		//@―---------------------------------------------------------------------------
		//! @brief		空か
		//@―---------------------------------------------------------------------------
		bool empty()const noexcept {
			return m_instances.empty();
		}


		//@―---------------------------------------------------------------------------
		//! @brief		事前確保
		//@―---------------------------------------------------------------------------
		void reserve(size_t size_){
			m_instances.reserve(size_);
		}


		//===============================================================
		// 要素アクセス
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief		要素アクセス
		//@―---------------------------------------------------------------------------
		T& at(size_t index) {
			return m_instances.at(index);
		}


		//@―---------------------------------------------------------------------------
		//! @brief		要素アクセス(const)
		//@―---------------------------------------------------------------------------
		const T& at(size_t index) const{
			return m_instances.at(index);
		}


		//@―---------------------------------------------------------------------------
		//! @brief		現在のインデックスを取得
		//@―---------------------------------------------------------------------------
		size_t index()const noexcept {
			return m_index;
		}


		//@―---------------------------------------------------------------------------
		//! @brief		現在のインスタンスにアクセス
		//@―---------------------------------------------------------------------------
		T& current() {
			return m_instances[m_index];
		}


		//@―---------------------------------------------------------------------------
		//! @brief		現在のインスタンスにアクセス(const)
		//@―---------------------------------------------------------------------------
		const T& current()const {
			return m_instances[m_index];
		}

		//===============================================================
		// コンテナの変更
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief		新たな要素を末尾に追加
		//@―---------------------------------------------------------------------------
		void push_back(const T& x) {
			m_instances.push_back(x);
		}


		//@―---------------------------------------------------------------------------
		//! @brief		新たな要素を末尾に追加
		//@―---------------------------------------------------------------------------
		void push_back(T&& x) {
			m_instances.push_back(x);
		}


		//@―---------------------------------------------------------------------------
		//! @brief		直接構築で新たな要素を末尾に追加
		//@―---------------------------------------------------------------------------
		template <class... Args>
		T& emplace_back(Args&&... args) {
			m_instances.emplace_back(forward(args...));
		}


		//@―---------------------------------------------------------------------------
		//! @brief		全要素削除
		//@―---------------------------------------------------------------------------
		void clear() {
			m_instances.clear();
		}


		//===============================================================
		// 進行
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief		次のバッファに進める
		//@―---------------------------------------------------------------------------
		void next()noexcept {
			m_index++;
			if (m_instances.size() <= m_index) {
				m_index = 0;
			}
		}


		//@―---------------------------------------------------------------------------
		//! @brief		インデックスを変更
		//@―---------------------------------------------------------------------------
		void setIndex(size_t index)const noexcept {
			OB_CHECK_ASSERT(index<size(),"範囲外です。");
			m_index = index;
		}


	private:

		vector<T> m_instances;
		size_t m_index;

	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond



	//! @endcond
}// namespcae ob