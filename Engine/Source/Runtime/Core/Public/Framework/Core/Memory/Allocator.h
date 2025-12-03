//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <cstddef>

namespace ob::core {

    //! @brief 抽象アロケーター
    //! メモリの確保と解放の責務を持つ基底クラス
    //! 具体的な実装クラスを継承して様々なアロケーターを利用できる
    class Allocator {
    public:
        virtual ~Allocator() = default;

        //! @brief メモリを確保する
        //! @param size 確保するサイズ
        //! @param alignment アライメント
        //! @return 確保されたメモリのポインタ
        virtual void* alloc(size_t size, size_t alignment) = 0;

        //! @brief メモリを解放する
        //! @param ptr 解放するメモリのポインタ
        virtual void free(void* ptr) = 0;
    };

    //! @brief デフォルトアロケーターの設定
    //! @param allocator 設定するアロケーター
    void SetAllocator(Allocator& allocator);

    //! @brief 現在のアロケーターを取得
    //! @return 現在のアロケーター
    Allocator* GetAllocator();

}