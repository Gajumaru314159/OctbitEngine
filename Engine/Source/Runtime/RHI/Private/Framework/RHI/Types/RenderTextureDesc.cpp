//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <magic_enum.hpp>
#include <Framework/RHI/Types/RenderTextureDesc.h>
#include <Framework/RHI/RenderTexture.h>

namespace ob::rhi {

    bool RenderTextureDesc::isValid() const {

        // サイズ
        if (!(0 < size.width && 0 < size.height && 1 == size.depth)) {
            LOG_ERROR("TextureDescのサイズが不正です。[name={},size={}]", name, size);
            return false;
        }

        // ステンシルを含んでいるとそのままShaderResourceとして使用できない
        if (TextureFormatUtility::HasStencil(format)) {
            // TODO D24S8の扱いを考える(Plane指定？)
            LOG_ERROR("ステンシルを含むテクスチャはShaderResourceとして使用できません [name={}]", name);
            return false;
        }

        // フォーマット
        if (!RenderTexture::Supports(format)) {
            LOG_ERROR("サポート外のフォーマットです [name={}, type={}]", name,magic_enum::enum_name(format));
            return false;
        }

        return true;
    }

}