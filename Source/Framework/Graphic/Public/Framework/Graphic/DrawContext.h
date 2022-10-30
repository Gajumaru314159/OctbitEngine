//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/String/Name.h>

namespace ob::graphic {

    struct DrawTag {
        Name name;
        s32 offset;
    };


    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //! 
    //! @details    モデル描画のようなソート単位の描画処理。複数パスで描画される場合は同じ数だけ生成されます。
    //@―---------------------------------------------------------------------------
    class DrawItem {
    public:
        const Name& getTag()const;
        virtual void render()const =0;
    };



    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class DrawContext {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        Span<const DrawItem*> getDrawItems(const Name& name) {
            auto itr = m_itemMap.find(name);
            if (itr == m_itemMap.end())return {};
            return itr->second;
        }

        template<class T>
        T* allocateDrawItem(const Name& name) {
            // auto buffer = m_linearAllocator.alloc(sizeof(T));
            // auto pItem = new(buffer)T();
            // m_item[name].push_back(pItem);
            // return pItem;
        }

    private:

        void end() {
            for (auto& [tag, container] : m_itemMap) {
                container.clear();
            }
        }

    private:

        HashMap<Name,Array<const DrawItem*>> m_itemMap;

    };




    class DrawGroup {
    public:
        virtual ~DrawGroup(){}
    protected:
        virtual void render(DrawContext&) {

        }
    };

    class DrawGroupTest :public DrawGroup {
    public:
        DrawGroupTest() {
            m_tags.push_back(Name(TC("Opaque")));
        }

    private:
        void render(DrawContext& context) override{

            // 収集
            m_items.clear();
            for (const auto& tag : m_tags) {
                for (auto item : context.getDrawItems(tag)) {
                    // フィルタ
                    // if(fillter)continue;
                    m_items.push_back(item);
                }
            }

            // ソート
            std::sort(m_items.begin(), m_items.end(), [](const DrawItem* a, const DrawItem* b) {

            });

            // 描画
            for (auto item : m_items) {
                item->render();
            }
        }
    private:
        Array<Name> m_tags;
        Array<const DrawItem*> m_items;

    };


    class DrawItemTest :public DrawItem {
    public:

        void render()const override {

        }
    };


    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob