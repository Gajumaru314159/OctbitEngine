//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Rendering/RenderContext.h>
namespace ob::rendering
{

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------

        //@―---------------------------------------------------------------------------
        //! @brief      特定の描画タグを持つ描画アイテムを描画する
        //@―---------------------------------------------------------------------------
    void RenderContext::draw(/*tag,sort,filter*/) {
        // m_context.draw();
        auto& items = m_itemMap[tag];

        std::sort(items.begin(), items.end(), m_sorter[sort]);

        for (auto& item : items) {
            if (!item.getLayers().has(layer))continue;

            item.render(m_cmdBuffer,tag);

        }

    }


}// namespace ob