//***********************************************************
//! @file
//! @brief		Bitmap
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/String/StringView.h>
#include <Framework/Core/Geometry/Size.h>
#include <Framework/Core/Geometry/IntRect.h>
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

    
    //! @brief  ビットマップ
    class Bitmap {
    public:
        using base_type                 = Vector<IntColor>;                     //!< ベース型
        using iterator                  = base_type::iterator;                  //!< イテレータ
        using const_iterator            = base_type::const_iterator;            //!< const イテレータ
        using reverse_iterator          = base_type::reverse_iterator;          //!< 逆イテレータ
        using const_reverse_iterator    = base_type::const_reverse_iterator;    //!< const 逆イテレータ
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        Bitmap() = default;
        Bitmap(s32 width, s32 height, IntColor color = IntColor::Black);
        template<class TGen>
        Bitmap(s32 width, s32 height,TGen generator);
        Bitmap(StringView path,ImageFormat fotrmat = ImageFormat::Unspecified);


        //===============================================================
        // ゲッター
        //===============================================================

        s32 width()const noexcept;
        s32 height()const noexcept;
        Size size()const noexcept;
        s32 stride()const noexcept;
        size_t pixelCount()const noexcept;
        size_t byteSize()const noexcept;
        bool empty()const noexcept;


        //===============================================================
        // 領域
        //===============================================================

        void shrink_to_fit();
        void clear();


        //===============================================================
        // データアクセス
        //===============================================================

        IntColor* operator[](size_t y);
        const IntColor* operator[](size_t y)const;

        IntColor& at(s32 x, s32 y);
        const IntColor& at(s32 x,s32 y)const;

        IntColor* data();
        const IntColor* data()const;


        //===============================================================
        // 加工
        //===============================================================

        void fill(IntColor color);

        void resize(s32 width, s32 height, IntColor color = IntColor::Black);
        void resize(Size size, IntColor color = IntColor::Black);


        void flipX();
        void flipY();

        Bitmap clipped(const IntRect& rect)const;
        Bitmap clipped(s32 x,s32 y,s32 w,s32 h)const;

        template<class TFunc>
        Bitmap& forEach(TFunc func);
        template<class TFunc>
        const Bitmap& forEach(TFunc func)const;


        //===============================================================
        // イテレータ
        //===============================================================

        iterator begin() noexcept { return m_pixels.begin(); }
        iterator end() noexcept { return m_pixels.end(); }
        const_iterator begin() const noexcept { return m_pixels.begin(); }
        const_iterator end() const noexcept { return m_pixels.end(); }
        const_iterator cbegin() const noexcept { return m_pixels.cbegin(); }
        const_iterator cend() const noexcept { return m_pixels.cend(); }
        reverse_iterator rbegin() noexcept { return m_pixels.rbegin(); }
        reverse_iterator rend() noexcept { return m_pixels.rend(); }
        const_reverse_iterator rbegin() const noexcept { return m_pixels.rbegin(); }
        const_reverse_iterator rend() const noexcept { return m_pixels.rend(); }
        const_reverse_iterator crbegin() const noexcept { return m_pixels.crbegin(); }
        const_reverse_iterator crend() const noexcept { return m_pixels.crend(); }

    private:

        Vector<IntColor> m_pixels;
        Size m_size;

    };




    template<class TGen>
    Bitmap::Bitmap(s32 width, s32 height, TGen generator) {
        resize(width, height);
		forEach(generator);
    }

    template<class TFunc>
    Bitmap& Bitmap::forEach(TFunc func) {
        size_t index = 0;
        for(s32 y = 0; y < m_size.height; ++y) {
			for(s32 x = 0; x < m_size.width; ++x) {
				func(m_pixels[index],x,y);
                index++;
			}
		}
        return *this;
    }

    template<class TFunc>
    const Bitmap& Bitmap::forEach(TFunc func)const {
        size_t index = 0;
        for (s32 y = 0; y < m_size.height; ++y) {
            for (s32 x = 0; x < m_size.width; ++x) {
                func(m_pixels[index], x, y);
                index++;
            }
        }
        return *this;
    }

}