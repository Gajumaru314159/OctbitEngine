﻿//***********************************************************
//! @file
//! @brief		バイナリデータビュー(Binary Large Object)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/Blob.h>
#include <Framework/Core/Log/Assertion.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief バイナリデータビュー(Binary Large Object View)
    //! 
    //! @details    Blob と異なり、内部にバイト列のコピーを持ちません。
    //@―---------------------------------------------------------------------------
    class BlobView {
    public:

        using const_iterator = const byte*;                   //!< イテレータ(const)

    public:

        //===============================================================
        // コンストラクタ
        //===============================================================
        
        //@―---------------------------------------------------------------------------
        //! @brief デフォルトコンストラクタ
        //@―---------------------------------------------------------------------------
        BlobView() = default;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(Blob指定)
        //@―---------------------------------------------------------------------------
        BlobView(const Blob& blob)
        {
            m_pData = blob.data();
            m_size = blob.size();
            OB_CHECK_ASSERT_EXPR(m_pData != nullptr);
        }


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(ポインタ指定)
        //@―---------------------------------------------------------------------------
        BlobView(const void* pData, size_t size)
        {
            m_pData = reinterpret_cast<const byte*>(pData);
            m_size = size;
            OB_CHECK_ASSERT_EXPR(m_pData !=nullptr);
        }


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(vector指定)
        //@―---------------------------------------------------------------------------
        template<class T>
        explicit BlobView(const Array<T>& data) {
            m_pData = static_cast<const byte*>(data.data());
            m_size = data.size()*sizeof(T);
        }


        //===============================================================
        // オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief コピー代入演算子
        //@―---------------------------------------------------------------------------
        BlobView& operator =(const BlobView& other) {
            m_pData = other.m_pData;
            m_size = other.m_size;
            return *this;
        }


        //@―---------------------------------------------------------------------------
        //! @brief ムーブ代入演算子
        //@―---------------------------------------------------------------------------
        BlobView& operator =(BlobView&& other) noexcept{
            m_pData = other.m_pData;
            m_size = other.m_size;
            return *this;
        }


        //@―---------------------------------------------------------------------------
        //! @brief コピー代入演算子(Blob)
        //@―---------------------------------------------------------------------------
        BlobView& operator =(const Blob& other) {
            m_pData = other.data();
            m_size = other.size();
            return *this;
        }


        //@―---------------------------------------------------------------------------
        //! @brief コピー代入演算子(Array)
        //@―---------------------------------------------------------------------------
        template<class T>
        BlobView& operator =(const Array<T>& other) {
            m_pData = static_cast<const byte*>(other.data());
            m_size = other.size()*sizeof(T);
            return *this;
        }


        //===============================================================
        // アクセス
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief バイトアクセス
        //@―---------------------------------------------------------------------------
        const byte& operator[](const size_t index) const {
            return m_pData[index];
        }


        //@―---------------------------------------------------------------------------
        //! @brief 先頭データにアクセス
        //@―---------------------------------------------------------------------------
        const byte* data() const noexcept {
            return m_pData;
        }


        //@―---------------------------------------------------------------------------
        //! @brief 空か
        //@―---------------------------------------------------------------------------
        bool empty() const noexcept {
            return m_pData==nullptr || m_size == 0;
        }


        //@―---------------------------------------------------------------------------
        //! @brief 空か
        //@―---------------------------------------------------------------------------
        explicit operator bool() const noexcept {
            return !empty();
        }


        //@―---------------------------------------------------------------------------
        //! @brief データサイズを取得
        //@―---------------------------------------------------------------------------
        size_t size() const noexcept {
            return m_size;
        }


        //@―---------------------------------------------------------------------------
        //! @brief データをクリア
        //@―---------------------------------------------------------------------------
        void clear() {
            m_pData = nullptr;
            m_size = 0;
        }


        //@―---------------------------------------------------------------------------
        //! @brief 開始イテレータ(const)
        //@―---------------------------------------------------------------------------
        const_iterator begin() const noexcept {
            return m_pData;
        }


        //@―---------------------------------------------------------------------------
        //! @brief 終了イテレータ(const)
        //@―---------------------------------------------------------------------------
        const_iterator end() const noexcept {
            return m_pData+m_size;
        }


        //@―---------------------------------------------------------------------------
        //! @brief 開始イテレータ(const)
        //@―---------------------------------------------------------------------------
        const_iterator cbegin() const noexcept {
            return m_pData;
        }


        //@―---------------------------------------------------------------------------
        //! @brief 終了イテレータ(const)
        //@―---------------------------------------------------------------------------
        const_iterator cend() const noexcept {
            return m_pData + m_size;
        }

    private:

        const byte* m_pData = nullptr;
        size_t m_size = 0;

    };


}// namespcae ob::core