//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometory/Size.h>
#include <Framework/Core/Geometory/IntRect.h>
#include <Framework/Core/Graphic/Color.h>

namespace ob::core {

    enum class PixelFormat {
        RGBA8,
        RGBA32,
        Alpha,
    };

    enum class ImageFormat {
        Unspecified,
        PNG,
    };

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class Bitmap {
    public:
        using base_type                 = Array<Color>;                        //!< ベース型
        using iterator                  = base_type::iterator;                  //!< イテレータ
        using const_iterator            = base_type::const_iterator;            //!< const イテレータ
        using reverse_iterator          = base_type::reverse_iterator;          //!< 逆イテレータ
        using const_reverse_iterator    = base_type::const_reverse_iterator;    //!< const 逆イテレータ
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        Bitmap() = default;
        Bitmap(s32 width, s32 height, IntColor color = IntColor::white);
        template<class TGen>
        Bitmap(s32 width, s32 height,TGen generator);
        Bitmap(const Path& path,ImageFormat fotrmat = ImageFormat::Unspecified);

        s32 width()const noexcept;
        s32 height()const noexcept;
        Size size()const noexcept;

        s32 stribe()const noexcept;

        s32 pixelCount()const noexcept;
        size_t byteSize()const noexcept;

        bool empty()const noexcept;
        void shrink_to_fit();
        void clear();

        IntColor* operator[](size_t y);
        const IntColor* operator[](size_t y)const;


        IntColor* data();
        const IntColor* data()const;

        void fill(IntColor color);

        void resize(s32 width, s32 height);
        void resize(Size size);


        void flipX();
        void flipY();

        Bitmap clipped(const IntRect& rect)const;
        Bitmap clipped(s32 x,s32 y,s32 w,s32 h)const;

        template<class TFunc>
        Bitmap& forEach(TFunc func);
        template<class TFunc>
        const Bitmap& forEach(TFunc func)const;

        iterator begin() noexcept;
        iterator end() noexcept;
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
        reverse_iterator rbegin() noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;


        // セーブ
        
        // フィルタ

        // 回転

        // ポリゴン化


    private:

        Array<IntColor> m_pixels;
        Size m_size;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob::core