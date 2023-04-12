//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Name.h>

namespace ob::graphics {

    using Name = Name;

    struct DrawTag {
        Name name;
        s32 offset;
    };


    //@―---------------------------------------------------------------------------
    //! @brief      説明
    //! 
    //! @details    モデル描画のようなソート単位の描画処理。複数パスで描画される場合は同じ数だけ生成されます。
    //@―---------------------------------------------------------------------------
    class DrawItem {
    public:
        virtual const Name& getTag()const=0;
        virtual void render()const =0;
    };


    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class DrawContext {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        Span<const DrawItem*> getDrawItems(const Name& name) {
            auto itr = m_itemMap.find(name);
            if (itr == m_itemMap.end())return {};
            return itr->second;
        }

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        template<class T>
        T* createDrawItem(const Name& name) {

            static_assert(std::is_base_of<DrawItem, T>::value == true, "T is not DrawItem");

            auto buffer = m_linearAllocator.alloc(sizeof(T));
            auto pItem = new(buffer)T();
            m_itemMap[name].push_back(pItem);
            return pItem;
        }

    private:

        void end() {
            for (auto& [tag, container] : m_itemMap) {
                for(auto& item*container){
                    item->~DrawItem();
                }
                container.clear();
            }
        }

    private:
        HashMap<Name,Array<const DrawItem*>> m_itemMap;
    };

    class DrawGroup {
    public:
        DrawGroupTest(Span<Name> renderTags) {
            // 描画グループのタグ設定
            for(auto& tag:renderTags){
                m_tags.push_back(tag);
            }
        }
    private:
        void render(DrawContext& context) override{

            // 収集
            m_items.clear();
            for (const auto& tag : m_tags) {
                for (auto item : context.getDrawItems(tag)) {
                    // フィルタ
                    if(!m_filter||m_filter(item)){
                        m_items.push_back(item);
                    }
                }
            }

            // ソート
            if(m_sorter){
                std::sort(m_items.begin(), m_items.end(),m_sorter);
            }

            // 描画
            for (auto item : m_items) {
                item->render();
            }
        }
    private:
        Array<Name> m_tags;
        Array<const DrawItem*> m_items;
        using Filter = Func<bool(const DrawItem*)>;
        using Sorter = Func<bool(const DrawItem*,const DrawItem*)>;
        Filer m_filter;
        Sorter m_sorter;
    };


    class DrawItemTest :public DrawItem {
    public:
        void render()const override {

        }
    };
    
}