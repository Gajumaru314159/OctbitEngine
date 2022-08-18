//***********************************************************
//! @file
//! @brief		ハンドル管理式双方向連結リスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <assert.h>
#include <iterator>
#include <memory>
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/Thread/SpinLock.h>
#include <Framework/Core/Utility/Noncopyable.h>
#include <Framework/Core/Utility/Nonmovable.h>

namespace ob::core {


    //@―---------------------------------------------------------------------------
    //! @brief          ハンドル管理式双方向連結リスト
    //! 
    //! @details        このコンテナは要素をコンテナ内部で管理せずハンドル内で管理する。<br> 
    //!                 よって、ハンドルのデストラクタが呼ばれるとリストの管理から外される。<br> 
    //!                 要素のデストラクタが呼ばれるタイミングはハンドルが破棄されるタイミングである。<br> 
    //!                 通常のリストと比べ排他的制御が入るため速度は遅いが、delgateなどのインスタンスの寿命と合わせる必要がある場合に使用する。<br> 
    //!                 <br> 
    //!                 使用方法：<br> 
    //! ```
    //!                 ob::HandleList<ob::s32> ls;
    //!                 using Handle =  ob::HandleList<ob::s32>::Handle;
    //!                 {
    //!                     Handle handle;
    //!                     ls.emplace_back(handle,123);
    //!                     std::cout<<ls.size()<<std::endl;    // 1
    //!                 }
    //!                 std::cout<<ls.size()<<std::endl;    // 0
    //! ```
    //@―---------------------------------------------------------------------------
    template<typename T>
    class HandleList :Noncopyable {
    public:

        //! @cond
        //@―---------------------------------------------------------------------------
        //! @brief ハンドル基底
        //@―---------------------------------------------------------------------------
        class HandleBase {
            friend class HandleList<T>;
        public:

            //@―---------------------------------------------------------------------------
            //! @brief ハンドルが対象のリストの要素か判定する
            //@―---------------------------------------------------------------------------
            bool is_child_of(const HandleList<T>& parent)const noexcept {
                return pParent == std::addressof(parent);
            }

        protected:
            void clear()noexcept {
                pParent = nullptr;
                pPrev = nullptr;
                pNext = nullptr;
            }
        protected:
            HandleBase* pPrev = nullptr;
            HandleBase* pNext = nullptr;
            HandleList<T>* pParent = nullptr;
        };
        //! @endcond


        //@―---------------------------------------------------------------------------
        //! @brief ハンドル
        //@―---------------------------------------------------------------------------
        class Handle :public HandleBase, Noncopyable,Nonmovable {
            friend class HandleList<T>;
        public:

            //@―---------------------------------------------------------------------------
            //! @brief コンストラクタ
            //@―---------------------------------------------------------------------------
            Handle() = default;

            //@―---------------------------------------------------------------------------
            //! @brief デストラクタ
            //@―---------------------------------------------------------------------------
            ~Handle() {
                remove();
            }

            //@―---------------------------------------------------------------------------
            //! @brief ハンドルの要素をリストから削除する
            //@―---------------------------------------------------------------------------
            void remove() {
                if (!pParent)return;
                // 排他的制御ができていない？
                pParent->remove(*this);
            }
        private:

            T* get_ptr()noexcept {
                if (!pParent)return nullptr;
                return reinterpret_cast<T*>(instance);
            }

            const T* get_ptr()const noexcept {
                if (!pParent)return nullptr;
                return reinterpret_cast<T*>(instance);
            }

        private:
            alignas(alignof(T)) byte instance[sizeof(T)];
        };

    public:

        using this_type = HandleList<T>;       //!< 型
        using value_type = T;                   //!< 値型
        using size_type = size_t;               //!< サイズ
        using difference_type = int;            //!< ポインタ差分型

        using pointer = T*;                     //!< ポインタ型
        using const_pointer = const T*;         //!< constポインタ型
        using reference = T&;                   //!< 参照型
        using const_reference = const T&;       //!< const参照型


    public:

#pragma region Iterator

        //@―---------------------------------------------------------------------------
        //! @brief constイテレータ
        //@―---------------------------------------------------------------------------
        class const_iterator {
            friend class HandleList<T>;
        public:
            using this_type = const_iterator;                                                                               //!< 型
            using difference_type = typename HandleList<T>::difference_type;                                               //!< ポインタ差分型
            using value_type = T;                                                                                           //!< インスタンス型
            using pointer = T*;                                                                                             //!< ポインタ型
            using reference = value_type&;                                                                                  //!< 参照型
            using iterator_category = std::bidirectional_iterator_tag;                                                      //!< イテレータ・カテゴリ
        public:
            const_iterator(const this_type& x)noexcept :pHandle(const_cast<HandleBase*>(x.pHandle)) {}                     //!< ムーブコンストラクタ
            const_iterator& operator = (const this_type& x)noexcept { pHandle = const_cast<HandleBase*>(x.pHandle); return*this; }     //!< ムーブ代入演算子
            reference operator*() const noexcept { return *(static_cast<Handle*>(pHandle)->get_ptr()); }                    //!< インスタンス参照
            pointer   operator->() const noexcept { return *(static_cast<Handle*>(pHandle)->get_ptr()); }                   //!< メンバアクセス
            this_type& operator++() noexcept { pHandle = pHandle->pNext; return *this; }                                    //!< インクリメント
            this_type& operator--() noexcept { pHandle = pHandle->pPrev; return *this; }                                    //!< デクリメント
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                //!< 後置インクリメント
            this_type operator--(int)noexcept { auto temp = *this; --(*this); return temp; }                                //!< 後置デクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                          //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                 //!< 否等価演算子
        protected:
            const_iterator(const HandleBase* pHandle)noexcept :pHandle(const_cast<HandleBase*>(pHandle)) {}               //!< ハンドルのポインタから生成(内部用)
        private:
            bool is_child_of(const HandleList<T>& parent)const noexcept { return pHandle->is_child_of(parent); }           //!< ハンドルの所有者を確認
        protected:
            HandleBase* pHandle;                                                                                           //!< インスタンス・ポインタ
        };


        //@―---------------------------------------------------------------------------
        //! @brief イテレータ
        //@―---------------------------------------------------------------------------
        class iterator :public const_iterator {
            friend class HandleList<T>;
        public:
            using this_type = iterator;                                                                                     //!< 型
            using value_type = const T;                                                                                     //!< インスタンス型
            using pointer = const T*;                                                                                       //!< ポインタ型
            using reference = const T&;                                                                                     //!< 参照型
        public:
            iterator(const this_type& x)noexcept :const_iterator(x) {}                                                      //!< ムーブコンストラクタ
            iterator& operator = (const this_type& x)noexcept { const_iterator::operator=(x); return *this; }               //!< ムーブ代入演算子
            reference operator*() const noexcept { return *(static_cast<Handle*>(pHandle)->get_ptr()); }                    //!< インスタンス参照
            pointer   operator->() const noexcept { return *(static_cast<Handle*>(pHandle)->get_ptr()); }                   //!< メンバアクセス
            this_type& operator++() noexcept { pHandle = pHandle->pNext; return *this; }                                    //!< インクリメント
            this_type& operator--() noexcept { pHandle = pHandle->pPrev; return *this; }                                    //!< デクリメント
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                //!< 後置インクリメント
            this_type operator--(int)noexcept { auto temp = *this; --(*this); return temp; }                                //!< 後置デクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                          //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                 //!< 否等価演算子
        protected:
            iterator(const HandleBase* pHandle)noexcept :const_iterator(pHandle) {}                                        //!< ハンドルのポインタから生成(内部用)
        };


        //@―---------------------------------------------------------------------------
        //! @brief const逆イテレータ
        //@―---------------------------------------------------------------------------
        class const_reverse_iterator :public const_iterator {
            friend class HandleList<T>;
        public:
            using this_type = const_reverse_iterator;                                                                           //!< 型
            using value_type = T;                                                                                               //!< インスタンス型
            using pointer = T*;                                                                                                 //!< ポインタ型
            using reference = T&;                                                                                               //!< 参照型
        public:
            const_reverse_iterator(const this_type& x)noexcept :const_iterator(x) {}                                            //!< ムーブコンストラクタ
            const_reverse_iterator& operator = (const this_type& x)noexcept { const_iterator::operator=(x); return *this; }     //!< ムーブ代入演算子
            reference operator*() const noexcept { return *(static_cast<Handle*>(pHandle)->get_ptr()); }                        //!< インスタンス参照
            pointer   operator->() const noexcept { return *(static_cast<Handle*>(pHandle)->get_ptr()); }                       //!< メンバアクセス
            this_type& operator++() noexcept { pHandle = pHandle->pPrev; return *this; }                                        //!< インクリメント
            this_type& operator--() noexcept { pHandle = pHandle->pNext; return *this; }                                        //!< デクリメント
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                    //!< 後置インクリメント
            this_type operator--(int)noexcept { auto temp = *this; --(*this); return temp; }                                    //!< 後置デクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                              //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                     //!< 否等価演算子
        protected:
            const_reverse_iterator(const HandleBase* pHandle)noexcept :const_iterator(pHandle) {}                              //!< ハンドルのポインタから生成(内部用)
        };


        //@―---------------------------------------------------------------------------
        //! @brief 逆イテレータ
        //@―---------------------------------------------------------------------------
        class reverse_iterator :public const_reverse_iterator {
            friend class HandleList<T>;
        public:
            using this_type = reverse_iterator;                                                                                 //!< 型
            using value_type = const T;                                                                                         //!< インスタンス型
            using pointer = const T*;                                                                                           //!< ポインタ型
            using reference = const T&;                                                                                         //!< 参照型
        public:
            reverse_iterator(const this_type& x)noexcept :const_reverse_iterator(x) {}                                          //!< ムーブコンストラクタ
            reverse_iterator& operator = (const this_type& x)noexcept { const_iterator::operator=(x); return *this; }           //!< ムーブ代入演算子
            reference operator*() const noexcept { return *(static_cast<Handle*>(pHandle)->get_ptr()); }                        //!< インスタンス参照
            pointer   operator->() const noexcept { return *(static_cast<Handle*>(pHandle)->get_ptr()); }                       //!< メンバアクセス
            this_type& operator++() noexcept { pHandle = pHandle->pPrev; return *this; }                                        //!< インクリメント
            this_type& operator--() noexcept { pHandle = pHandle->pNext; return *this; }                                        //!< デクリメント
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                    //!< 後置インクリメント
            this_type operator--(int)noexcept { auto temp = *this; --(*this); return temp; }                                    //!< 後置デクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                              //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                     //!< 否等価演算子
        protected:
            reverse_iterator(const HandleBase* pHandle)noexcept :const_reverse_iterator(pHandle) {}                            //!< ハンドルのポインタから生成(内部用)
        };

#pragma endregion

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        HandleList();                                                          // コンストラクタ
        HandleList(this_type&& x)noexcept;                                     // ムーブコンストラクタ
        this_type& operator=(this_type&& x)noexcept;                            // ムーブ代入演算子
        ~HandleList();                                                         // デストラクタ


        //===============================================================
        // イテレータ
        //===============================================================

        iterator begin()noexcept;                                               // 先頭を指すイテレータを取得する
        iterator end()noexcept;                                                 // 末尾の次を指すイテレータを取得する
        const_iterator cbegin()const noexcept;                                  // 先頭を指すイテレータを取得する(const)
        const_iterator cend()const noexcept;                                    // 末尾の次を指すイテレータを取得する(const)

        reverse_iterator rbegin()noexcept;                                      // 先頭を指す逆イテレータを取得する
        reverse_iterator rend()noexcept;                                        // 末尾の次を指す逆イテレータを取得する
        const_reverse_iterator crbegin()const noexcept;                         // 先頭を指す逆イテレータを取得する(const)
        const_reverse_iterator crend()const noexcept;                           // 末尾の次を指す逆イテレータを取得する(const)

        //! @cond 
        const_iterator begin()const noexcept;                                   // 先頭を指すイテレータを取得する(const)
        const_iterator end()const noexcept;                                     // 末尾の次を指すイテレータを取得する(const)
        const_reverse_iterator rbegin()const noexcept;                          // 先頭を指す逆イテレータを取得する(const)
        const_reverse_iterator rend()const noexcept;                            // 末尾の次を指す逆イテレータを取得する(const)
        //! @endcond

        //===============================================================
        // 領域
        //===============================================================
        bool empty()const noexcept;                                             // コンテナが空かどうかを判定する
        size_type size() const noexcept;                                        // 要素数を取得する


        //===============================================================
        // 要素アクセス
        //===============================================================
        reference front();                                                      // 先頭要素への参照を取得する
        reference back();                                                       // 末尾要素への参照を取得する
        //! @cond 
        const_reference front() const;                                          // 先頭要素への参照を取得する(const)
        const_reference back() const;                                           // 末尾要素への参照を取得する(const)
        //! @endcond 


        //===============================================================
        // コンテナの変更
        //===============================================================
        void push_front(Handle& h, const T& x);                                 // 先頭に要素を追加する
        void push_front(Handle& h, T&& x);                                      // 先頭に要素を追加する
        template<class... Args>
        reference emplace_front(Handle& h, Args&&... args);                     // 先頭に要素を直接構築して追加する
        void push_back(Handle& h, const T& x);                                  // 末尾に要素を追加する
        void push_back(Handle& h, T&& x);                                       // 末尾に要素を追加する
        template<class... Args>
        reference emplace_back(Handle& h, Args&&... args);                      // 末尾に要素を直接構築して追加する
        iterator insert(Handle& h, const_iterator position, const T& x);        // 要素を指定位置に挿入する
        iterator insert(Handle& h, const_iterator position, T&& x);             // 要素を指定位置に挿入する
        template<class... Args>
        iterator emplace(Handle& h, const_iterator position, Args&&... args);   // 要素を指定位置に直接構築して挿入する
        void pop_front()noexcept;                                               // 先頭から要素を削除する
        void pop_back()noexcept;                                                // 末尾から要素を削除する
        iterator erase(const_iterator position);                                // 指定位置の要素を削除する
        iterator erase(const_iterator position, const_iterator last);           // 指定範囲の要素を削除する
        void clear()noexcept;                                                   // 全要素を削除する
        void swap(this_type& x)noexcept;                                        // コンテナを交換する


        //===============================================================
        // リスト操作
        //===============================================================
        void remove(Handle& h);                                                 // 要素を削除する
        template<class Predicate>
        void remove_if(Predicate pred);                                         // 条件に合った要素を削除する
        void reverse()noexcept;                                                 // コンテナを反転する

    private:

        void clear_impl()noexcept;                                              // リストをクリア(remove呼び出しなし)
        void move_impl(this_type&& src)noexcept;                                // ムーブ共通処理
        void reset_parent()noexcept;                                            // 全ての要素のpParentの更新
        void insert_impl(HandleBase* pPrev, Handle& h);                        // 要素を指定したハンドルの次に挿入
        void insert_front_impl(Handle& h);                                      // 先頭に要素を追加
        void insert_back_impl(Handle& h);                                       // 末尾に要素を追加
        iterator erase_impl(HandleBase* pHandle)noexcept;                      // 要素の削除

    private:

        mutable SpinLock m_mutex;
        HandleBase m_header;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline bool operator==(HandleList<T> const& a, HandleList<T> const& b)noexcept {
        ScopeLock lock(m_mutex);
        return (a.size() == b.size()) && ob::equal(a.begin(), a.end(), b.begin());
    }

    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline bool operator!=(HandleList<T> const& a, HandleList<T> const& b)noexcept {
        return !(*this == rhs);
    }



    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline HandleList<T>::HandleList()
        :m_header() {
        clear_impl();
    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブコンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline HandleList<T>::HandleList(this_type&& x)noexcept {
        clear_impl();
        move_impl(std::move(x));
    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブ代入演算子
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline HandleList<T>& HandleList<T>::operator=(this_type&& rhs)noexcept {
        if (this != &rhs) {
            clear();
            ScopeLock lock(m_mutex);
            move_impl(std::move(rhs));
        }
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief デストラクタ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline HandleList<T>::~HandleList() {
        clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::iterator HandleList<T>::begin()noexcept {
        return iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::const_iterator HandleList<T>::begin()const noexcept {
        return const_iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::iterator HandleList<T>::end()noexcept {
        return iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::const_iterator HandleList<T>::end()const noexcept {
        return const_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::const_iterator HandleList<T>::cbegin()const noexcept {
        return const_iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::const_iterator HandleList<T>::cend()const noexcept {
        return const_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指す逆イテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::reverse_iterator HandleList<T>::rbegin()noexcept {
        return reverse_iterator(m_header.pPrev);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指す逆イテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::const_reverse_iterator HandleList<T>::rbegin()const noexcept {
        return const_reverse_iterator(m_header.pPrev);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指す逆イテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::reverse_iterator HandleList<T>::rend()noexcept {
        return reverse_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指す逆イテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::const_reverse_iterator HandleList<T>::rend()const noexcept {
        return const_reverse_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指す逆イテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::const_reverse_iterator HandleList<T>::crbegin()const noexcept {
        return const_reverse_iterator(m_header.pPrev);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指す逆イテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::const_reverse_iterator HandleList<T>::crend()const noexcept {
        return const_reverse_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンテナが空かどうかを判定する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline bool HandleList<T>::empty()const noexcept {
        ScopeLock lock(m_mutex);
        return m_header.pNext == &m_header;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素数を取得する
    //! 
    //! @note  O(n)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::size_type HandleList<T>::size()const noexcept {
        ScopeLock lock(m_mutex);
        return std::distance(begin(), end());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素への参照を取得する
    //! 
    //! @note  コンテナが空の時はassertを発生させる。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::reference HandleList<T>::front() {
        ScopeLock lock(m_mutex);
        assert(m_header.pNext != &m_header);
        return *(static_cast<Handle*>(m_header.pNext)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素への参照を取得する(const)
    //! 
    //! @note  コンテナが空の時はassertを発生させる。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::const_reference HandleList<T>::front() const {
        ScopeLock lock(m_mutex);
        assert(m_header.pNext != &m_header);
        return *(static_cast<Handle*>(m_header.pNext)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾要素への参照を取得する
    //! 
    //! @note  コンテナが空の時はassertを発生させる。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::reference HandleList<T>::back() {
        ScopeLock lock(m_mutex);
        assert(m_header.pPrev != &m_header);
        return *(static_cast<Handle*>(m_header.pPrev)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾要素への参照を取得する(const)
    //! 
    //! @note  コンテナが空の時はassertを発生させる。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::const_reference HandleList<T>::back() const {
        ScopeLock lock(m_mutex);
        assert(m_header.pPrev != &m_header);
        return *(static_cast<Handle*>(m_header.pPrev)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::push_front(Handle& h, const T& x) {
        ScopeLock lock(m_mutex);
        insert_front_impl(h);
        *h.get_ptr() = x;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::push_front(Handle& h, T&& x) {
        ScopeLock lock(m_mutex);
        insert_front_impl(h);
        *h.get_ptr() = std::move(x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を直接構築して追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class... Args>
    inline typename HandleList<T>::reference HandleList<T>::emplace_front(Handle& h, Args&&... args) {
        ScopeLock lock(m_mutex);
        insert_front_impl(h);
        ob::construct_at(h.get_ptr(), std::forward<Args>(args)...);
        return *h.get_ptr();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::push_back(Handle& h, const T& x) {
        ScopeLock lock(m_mutex);
        insert_back_impl(h);
        *h.get_ptr() = x;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::push_back(Handle& h, T&& x) {
        ScopeLock lock(m_mutex);
        insert_back_impl(h);
        *h.get_ptr() = std::move(x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  末尾に要素を直接構築して追加する
    //! 
    //! @return 追加した要素の参照
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class... Args>
    inline typename HandleList<T>::reference HandleList<T>::emplace_back(Handle& h, Args&&... args) {
        ScopeLock lock(m_mutex);
        insert_back_impl(h);
        ob::construct_at(h.get_ptr(), std::forward<Args>(args)...);
        return *h.get_ptr();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  要素を指定位置に挿入する
    //! 
    //! @return 挿入した要素のイテレータ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::iterator HandleList<T>::insert(Handle& h, const_iterator position, const T& x) {
        ScopeLock lock(m_mutex);
        insert_impl(position.pHandle, h);
        *h.get_ptr() = x;
        return &h;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定位置に挿入する
    //! 
    //! @return 挿入した要素のイテレータ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::iterator HandleList<T>::insert(Handle& h, const_iterator position, T&& x) {
        ScopeLock lock(m_mutex);
        insert_impl(position.pHandle, h);
        *h.get_ptr() = x;
        return &h;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定位置に直接構築して挿入する
    //! 
    //! @return 挿入した要素のイテレータ
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class... Args>
    inline typename HandleList<T>::iterator HandleList<T>::emplace(Handle& h, const_iterator position, Args&&... args) {
        ScopeLock lock(m_mutex);
        insert_impl(position.pHandle, h);
        ob::construct_at(h.get_ptr(), std::forward<Args>(args)...);
        return &h;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭から要素を削除する
    //! 
    //! @details    コンテナが空の場合は何もしない。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::pop_front()noexcept {
        ScopeLock lock(m_mutex);
        if (m_header.pNext == &m_header)return;
        erase_impl(static_cast<Handle*>(m_header.pNext));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾から要素を削除する
    //! 
    //! @details    コンテナが空の場合は何もしない。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::pop_back()noexcept {
        ScopeLock lock(m_mutex);
        if (m_header.pPrev == &m_header)return;
        erase_impl(static_cast<Handle*>(m_header.pPrev));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 指定位置の要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::iterator HandleList<T>::erase(const_iterator position) {
        ScopeLock lock(m_mutex);
        return erase_impl(position.pHandle);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      指定範囲の要素を削除する
    //! 
    //! @details    [position, last)で示される範囲の要素が削除される。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::iterator HandleList<T>::erase(const_iterator position, const_iterator last) {
        ScopeLock lock(m_mutex);
        auto itr = position;
        while (position != last) {
            auto pHandle = position.pHandle;
            ++position;
            itr = erase_impl(pHandle);
        }
        return itr.pHandle;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 全要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::clear()noexcept {
        ScopeLock lock(m_mutex);
        auto itr = begin();
        while (itr != end()) {
            auto pHandle = itr.pHandle;
            ++itr;
            pHandle->clear();
        }
        clear_impl();
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンテナを交換する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::swap(this_type& x)noexcept {
        std::swap(*this, x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::remove(Handle& h) {
        ScopeLock lock(m_mutex);
        erase_impl(&h);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      条件に合った要素を削除する
    //! 
    //! @details    コンテナの全ての要素に対する各イテレータiにおいて、pred(*i) == trueとなる要素を削除する。
    //!             削除された要素に対するイテレータおよび参照は無効となる。
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class Predicate>
    inline void HandleList<T>::remove_if(Predicate pred) {
        ScopeLock lock(m_mutex);
        auto itr = begin();
        const auto itrEnd = end();
        while (itr != itrEnd) {
            if (pred(*itr)) {
                itr = erase_impl(itr.pHandle);
            } else {
                ++itr;
            }
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンテナを反転する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::reverse()noexcept {
        ScopeLock lock(m_mutex);

        auto itr = begin();
        const auto itrEnd = end();
        while (itr != itrEnd) {
            auto pHandle = itr.pHandle;
            ++itr;
            std::swap(pHandle->pPrev, pHandle->pNext);
        }
        std::swap(m_header.pPrev, m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief リストをクリア(remove呼び出しなし)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::clear_impl()noexcept {
        m_header.pNext = &m_header;
        m_header.pPrev = &m_header;
        m_header.pParent = this;

    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブ共通処理
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::move_impl(this_type&& src)noexcept {
        if (src.empty()) {
            m_header.pNext = &m_header;
            m_header.pPrev = &m_header;
        } else {
            m_header.pNext = src.m_header.pNext;
            m_header.pNext->pPrev = &m_header;
            m_header.pPrev = src.m_header.pPrev;
            m_header.pPrev->pNext = &m_header;
        }

        src.clear_impl();
        reset_parent();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 全ての要素のpParentの更新
    //@―---------------------------------------------------------------------------
    template<typename T>
    void HandleList<T>::reset_parent()noexcept {
        auto itr = begin();
        while (itr != end()) {
            auto pHandle = itr.pHandle;
            pHandle->pParent = this;
            ++itr;
        }
        m_header.pParent = this;
    }

    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定したハンドルの次に挿入
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::insert_impl(HandleBase* pPrev, Handle& h) {
        assert(pPrev != nullptr);
        assert(pPrev->is_child_of(*this));// 違うコンテナのイテレータならエラー

        h.remove();
        h.pParent = this;

        if (pPrev == m_header.pNext) {
            h.pPrev = &m_header;
            h.pNext = pPrev;

            m_header.pNext = &h;
            pPrev->pPrev = &h;
        } else {
            h.pPrev = pPrev->pPrev;
            h.pNext = pPrev;

            pPrev->pPrev->pNext = &h;
            pPrev->pPrev = &h;
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を追加
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::insert_front_impl(Handle& h) {
        insert_impl(m_header.pNext, h);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾に要素を追加
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleList<T>::insert_back_impl(Handle& h) {
        insert_impl(&m_header, h);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の削除
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleList<T>::iterator HandleList<T>::erase_impl(HandleBase* pHandle)noexcept {
        assert(pHandle != nullptr);
        assert(pHandle->is_child_of(*this));
        if (pHandle == &m_header)return end();

        pHandle->pPrev->pNext = pHandle->pNext;
        pHandle->pNext->pPrev = pHandle->pPrev;

        auto itr = iterator(pHandle->pNext);
        pHandle->clear();

        return itr;
    }


}// namespcae ob