﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Material.h>
#include <Framework/Graphic/Material/MaterialImpl.h>

namespace {

    template<class T>
    T& safeGet(ob::UPtr<T>& ptr) {
        OB_ASSERT_EXPR(prt != nullptr);
        return *ptr.get();
    }
    template<class T>
    T& safeGet(const ob::UPtr<T>& ptr) {
        OB_ASSERT_EXPR(prt != nullptr);
        return *ptr.get();
    }

}

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    Material::Material(const MaterialDesc& desc) {
        m_impl = std::make_unique<MaterialImpl>(desc);
    }


	//@―---------------------------------------------------------------------------
	//! @brief  Intのプロパティを持つか
	//@―---------------------------------------------------------------------------
    bool Material::hasInt(StringView name)const{
        return safeGet(m_impl).hasProprty(name, PropertyType::Int);
    }
    //@―---------------------------------------------------------------------------
    //! @brief  Floatのプロパティを持つか
    //@―---------------------------------------------------------------------------
    bool Material::hasFloat(StringView name)const {
        return safeGet(m_impl).hasProprty(name, PropertyType::Float);
    }
    //@―---------------------------------------------------------------------------
    //! @brief  Colorのプロパティを持つか
    //@―---------------------------------------------------------------------------
    bool Material::hasColor(StringView name)const {
        return safeGet(m_impl).hasProprty(name, PropertyType::Color);
    }
    //@―---------------------------------------------------------------------------
    //! @brief  Matrixのプロパティを持つか
    //@―---------------------------------------------------------------------------
    bool Material::hasMatrix(StringView name)const {
        return safeGet(m_impl).hasProprty(name, PropertyType::Matrix);
    }
    //@―---------------------------------------------------------------------------
    //! @brief  Textureのプロパティを持つか
    //@―---------------------------------------------------------------------------
    bool Material::hasTexture(StringView name)const {
        return safeGet(m_impl).hasProprty(name,PropertyType::Texture);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Floatプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::setFloat(StringView name, f32 value){
        return safeGet(m_impl).setFloat(name, value);
    }
    //@―---------------------------------------------------------------------------
    //! @brief  Colorプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::setColor(StringView name, Color value){
        return safeGet(m_impl).setColor(name, value);
    }
    //@―---------------------------------------------------------------------------
    //! @brief  Matrixプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::setMatrix(StringView name, const Matrix& value){
        return safeGet(m_impl).setMatrix(name, value);
    }
    //@―---------------------------------------------------------------------------
    //! @brief  Textureプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::setTexture(StringView name, const Ref<rhi::Texture>& value){
        return safeGet(m_impl).setTexture(name, value);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  グローバルFloatプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalFloat(StringView name, f32 value) {
        OB_NOTIMPLEMENTED();
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルColorプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalColor(StringView name, Color value) {
        OB_NOTIMPLEMENTED();
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルMatrixプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalMatrix(StringView name, const Matrix& value) {
        OB_NOTIMPLEMENTED();
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルTextureプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalTexture(StringView name, const Ref<Texture>& value) {
        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief  描画コマンドを記録
    //@―---------------------------------------------------------------------------
    void Material::record(Ref<rhi::CommandList>& recorder, engine::Name pass) const{
        safeGet(m_impl).record(recorder,pass);
    }

}// namespace ob