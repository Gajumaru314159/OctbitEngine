//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/RHI/Texture.h>

namespace ob::rhi {

    bool TextureDesc::isValid() const {

        // フォーマット
        if (format == TextureFormat::Unknown) {
            LOG_ERROR("TextureDescのフォーマットにUnknownは指定できません。[name={}]", name);
            return false;
        }

        // サイズ
        bool isValidSize = true;

        if (type == TextureType::Texture1D) {
            isValidSize &= 0 < size.width && 1 == size.height && 1 == size.depth;
        }
        if (type == TextureType::Texture2D) {
            isValidSize &= 0 < size.width && 0 < size.height && 1 == size.depth;
        }
        if (type == TextureType::Texture3D) {
            isValidSize &= 0 < size.width && 0 < size.height && 0 < size.depth;
        }
        if (type == TextureType::Cube) {
            isValidSize &= 0 < size.width && 0 < size.height && 1 == size.depth;
        }
        if (!isValidSize) {
            LOG_ERROR("TextureDescのサイズが不正です。[name={},size={}]", name, size);
            return false;
        }

        // 配列
        if (type == TextureType::Texture3D && 0 < arrayNum) {
            LOG_ERROR("Texture3Dは配列に対応していません [name={}]", name);
            return false;
        }

        // フォーマット
        if (!Texture::Supports(format, type)) {
            LOG_ERROR("サポート外のフォーマットです [name={}]", name);
            return false;
        }

        if (TextureFormatUtility::HasStencil(format) && flags & TextureFlag::ShaderResource) {
            // TODO D24S8の扱いを考える(Plane指定？)
            LOG_ERROR("ステンシルを含むテクスチャはShaderResourceとして使用できません [name={}]", name);
            return false;
        }

        return true;
    }

}