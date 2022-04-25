//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクトのホルダに関連するマクロ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      グラフィック・オブジェクトのホルダーに必要な定義を展開するマクロ
    //! @details    参照カウントの増減、解放などの共通処理を展開します。
    //!             ```
    //!             class Texture{
    //!                 OB_DEFINE_GRAPHIC_OBJECT_HOLDER(Texture);
    //!             private:
    //!                 ITexture m_pImpl;
    //!             }
    //!             ```
    //!             生成される宣言は以下の通りです。
    //!             * コピー/ムーブ
    //!             * 明示的解放             
    //!             * グラフィック・オブジェクトを持っているか
    //!             メンバ変数にGraphicObjectを継承したクラス```m_pImpl```がある必要があります。
    //@―---------------------------------------------------------------------------
    #define OB_DEFINE_GRAPHIC_OBJECT_HOLDER(type)           \
            public:                                         \
                type(const type&);                          \
                type(type&&);                               \
                type& operator=(const type&);               \
                type& operator=(type&&);                    \
                virtual ~type();                            \
                bool operator==(const type&)const noexcept; \
                bool operator!=(const type&)const noexcept; \
                operator bool()const noexcept;              \
                void release();                             \
                bool empty()const noexcept;               \
            private:                                        \
                class I##type* m_pImpl = nullptr;

    //@―---------------------------------------------------------------------------
    //! @copydoc OB_DEFINE_GRAPHIC_OBJECT_HOLDER(type)
    //@―---------------------------------------------------------------------------
    #define OB_DEFINE_GRAPHIC_OBJECT_HOLDER_OVERRIDE(type)      \
            public:                                         \
                type(const type&);                          \
                type(type&&);                               \
                type& operator=(const type&);               \
                type& operator=(type&&);                    \
                virtual ~type();                            \
                bool operator==(const type&)const noexcept; \
                bool operator!=(const type&)const noexcept; \
                operator bool()const noexcept;              \
                void release();                             \
                bool empty()const noexcept;               

    //@―---------------------------------------------------------------------------
    //! @brief      グラフィック・オブジェクトホルダーに必要な実装を生成するマクロ
    //! @details    参照カウントの増減、解放などの共通処理を展開します。
    //!             コンストラクタで```m_pImpl```にインスタンスを設定してください。
    //@―---------------------------------------------------------------------------
    #define OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(type)                        \
                type::type(const type& another) {                           \
                    m_pImpl = another.m_pImpl;                              \
                    if(m_pImpl)m_pImpl->addReference();                     \
                }                                                           \
                type::type(type&& another) {                                \
                    m_pImpl = another.m_pImpl;                              \
                    another.m_pImpl = nullptr;                              \
                }                                                           \
                type& type::operator=(const type& another) {                \
                    m_pImpl = another.m_pImpl;                              \
                    if(m_pImpl)m_pImpl->addReference();                     \
                    return *this;                                           \
                }                                                           \
                type& type::operator=(type&& another) {                     \
                    m_pImpl = another.m_pImpl;                              \
                    another.m_pImpl = nullptr;                              \
                    return *this;                                           \
                }                                                           \
                type::~type() {                                             \
                    release();                                              \
                }                                                           \
                bool type::operator==(const type& another)const noexcept{   \
                    return m_pImpl == another.m_pImpl;                      \
                }                                                           \
                bool type::operator!=(const type& another)const noexcept{   \
                    return !(*this == another);                             \
                }                                                           \
                type::operator bool()const noexcept {                       \
                    return m_pImpl != nullptr;                              \
                }                                                           \
                void type::release() {                                      \
                    if (m_pImpl)m_pImpl->releaseReference();                \
                    m_pImpl = nullptr;                                      \
                }                                                           \
                bool type::empty()const noexcept {                        \
                    return m_pImpl == nullptr;                              \
                }

}// namespcae ob::graphic