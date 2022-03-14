//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクト
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      グラフィック・オブジェクト
    //! 
    //! @details    グラフィック・オブジェクトは描画時まで削除されない必要があります。
    //!             描画はフレーム内に収まらず数フレーム遅れる場合もあるため、
    //!             所有者の寿命で管理するのが難しくなします。
    //!             そこで、このクラスを継承するクラスは所有者のデストラクタで削除されずに
    //!             システムの遅延開放スタックに積まれます。
    //!             システムのスタックに積まれている間は破棄されていないことが保証できるため
    //!             GPUからのアクセスにも対応できます。
    //!```          
    //!             class ShaderResourceImpl:public GraphicObject{
    //!             private:
    //!                 ComPtr<ID3D12Resource> m_resource;  // 各種描画APIのグラフィック・オブジェクト
    //!             }
    //! 
    //!             class ShadreResource{
    //!             public:
    //!                 ShadreResource(){
    //!                     m_pImpl =new ShaderResourceImpl();
    //!                 }
    //!                 ~ShadreResource(){
    //!                     System::Ref().stackDelayDelete(m_pImpl);
    //!                 }
    //!             private:
    //!                 ShaderResourceImpl* m_pImpl;
    //!             }
    //! 
    //!             class System:public Singleton(System){
    //!             public:
    //!                 void stackDelayDelete(GraphicObject* pObject){
    //!                     m_delayDeleteStack.push_back(pObject);               
    //!                 }
    //!             }
    //! ```
    //@―---------------------------------------------------------------------------
    class GraphicObject {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        virtual ~GraphicObject();


        //@―---------------------------------------------------------------------------
        //! @brief      名前を取得
        //! 
        //! @details    デバッグビルド時のみ設定した名前が取得できます。
        //!             リリースビルド時は同じ名前が返されます。(無効メッセージ)
        //@―---------------------------------------------------------------------------
        const StringView getName()const;


    protected:

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        GraphicObject(StringView name=TC("NONAME"));


    private:

        GraphicObject* m_pStack;    //!< 削除スタック用ポインタ

#ifdef OB_DEBUG
        String      m_name;
#endif

    };



#pragma region DEFINE

    //@―---------------------------------------------------------------------------
    //! @brief      グラフィック・オブジェクトに必要な宣言を定義するマクロ
    //! @details    GraphicObjectを継承したImplクラスを保持するクラスに使用します。
    //!             ```
    //!             class SharerRsourceImpl:public GraphicObject{}
    //!             class SharerResource{
    //!                 OB_DEFINE_GRAPHIC_OBJECT(SharerResource);
    //!             }
    //!             ```
    //!             生成される宣言は以下の通りです。
    //!             * コンストラクタ宣言
    //!             * デストラクタ宣言             
    //@―---------------------------------------------------------------------------
#define OB_DEFINE_GRAPHIC_OBJECT(type)              \
            private:                                \
                class Impl* m_pImpl;                \
            public:                                 \
                type(const type&);                  \
                type(const type&&);                 \
                type& operator=(const type&);       \
                type& operator=(const type&&);      \
                ~type();                            \
                bool operator==(const type&)const;  \
                bool operator!=(const type&)const;  \
                operator bool()const;               \
                void release();                     \
                bool isEmpty()const;                    

#define OB_IMPLEMENT_GRAPHIC_OBJECT(type)                               \
                type::type(const type& another) {                       \
                    m_pImpl = another.m_pImpl;                          \
                    m_pImpl.addRef();                                   \
                }                                                       \
                type::type(const type&& another) {                      \
                    m_pImpl = another.m_pImpl;                          \
                    another.m_pImpl = nullptr;                          \
                }                                                       \
                type::~type() {                                         \
                    release();                                          \
                }                                                       \
                type& type::operator=(const type& another) {            \
                    m_pImpl = another.m_pImpl;                          \
                    m_pImpl->addRef();                                  \
                }                                                       \
                type& type::operator=(const type&& another) {           \
                    m_pImpl = another.m_pImpl;                          \
                    another.m_pImpl = nullptr;                          \
                }                                                       \
                bool type::operator==(const type& another)const {       \
                    return m_pImpl == another.m_pImpl;                  \
                }                                                       \
                bool type::operator!=(const type& another)const {       \
                    return !(*this == another);                         \
                }                                                       \
                type::operator bool()const {                            \
                    return m_pImpl != nullptr;                          \
                }                                                       \
                void type::release() {                                  \
                    if (m_pImpl)m_pImpl->releaseRef();                  \
                    m_pImpl = nullptr;                                  \
                }                                                       \
                bool type::isEmpty()const {                             \
                    return m_pImpl == nullptr;                          \
                }
#pragma endregion




    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob::graphic