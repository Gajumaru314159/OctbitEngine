//***********************************************************
//! @file
//! @brief		ハンドル管理式単方向連結リスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <assert.h>
#include <iterator>
#include <Framework/Core/Thread/SpinLock.h>

namespace ob::core {


    //@―---------------------------------------------------------------------------
    //! @brief          ハンドル管理式単方向連結リスト
    //! 
    //! @details        このコンテナは要素をコンテナ内部で管理せずハンドル内で管理する。<br> 
    //!                 よって、ハンドルのデストラクタが呼ばれるとリストの管理から外される。<br> 
    //!                 要素のデストラクタが呼ばれるタイミングはハンドルが破棄されるタイミングである。<br> 
    //!                 通常のリストと比べ排他的制御が入るため速度は遅いが、delgateなどのインスタンスの寿命と合わせる必要がある場合に使用する。<br> 
    //!                 <br> 
    //!                 使用方法：<br> 
    //! ```
    //!                 ob::HandleForwardList<ob::s32> ls;
    //!                 using Handle =  ob::HandleForwardList<ob::s32>::Handle;
    //!                 {
    //!                     Handle Handle;
    //!                     ls.emplace_front(Handle,123);
    //!                     std::cout<<ls.size()<<std::endl;    // 1
    //!                 }
    //!                 std::cout<<ls.size()<<std::endl;    // 0
    //! ```
    //@―---------------------------------------------------------------------------
    template<typename T>
    class HandleForwardList :Noncopyable {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief ハンドル基底
        //@―---------------------------------------------------------------------------
        //!@cond
        class HandleBase {
            friend class HandleForwardList<T>;
        public:

            //@―---------------------------------------------------------------------------
            //! @brief ハンドルが対象のリストの要素か判定する
            //@―---------------------------------------------------------------------------
            bool is_child_of(const HandleForwardList<T>& parent)const noexcept {
                return pParent == std::addressof(parent);
            }

        protected:
            //@―---------------------------------------------------------------------------
            //! @brief クリア
            //@―---------------------------------------------------------------------------
            void clear()noexcept {
                pParent = nullptr;
                pNext = nullptr;
            }
        protected:
            HandleBase* pNext = nullptr;
            HandleForwardList<T>* pParent = nullptr;
        };
        //!@endcond


        //@―---------------------------------------------------------------------------
        //! @brief ハンドル
        //@―---------------------------------------------------------------------------
        class Handle :public HandleBase, private Noncopyable, private Nonmovable {
            friend class HandleForwardList<T>;
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

        using this_type = HandleForwardList<T>;   //!< 型
        using value_type = T;                       //!< アイテムの型
        using size_type = size_t;                   //!< サイズ型

        using pointer = T*;                         //!< ポインタ型
        using const_pointer = const T*;             //!< constポインタ型
        using reference = T&;                       //!< 参照型
        using const_reference = const T&;           //!< const参照型

        using difference_type = int;                //!< 差分型


        //@―---------------------------------------------------------------------------
        //! @brief constイテレータ
        //@―---------------------------------------------------------------------------
        class const_iterator {
            friend class HandleForwardList<T>;
        public:
            using this_type = const_iterator;                                                                               //!< 型
            using difference_type = typename HandleForwardList<T>::difference_type;                                               //!< ポインタ差分型
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
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                //!< 後置インクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                          //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                 //!< 否等価演算子
        protected:
            const_iterator(const HandleBase* pHandle)noexcept :pHandle(const_cast<HandleBase*>(pHandle)) {}               //!< ハンドルのポインタから生成(内部用)
        private:
            bool is_child_of(const HandleForwardList<T>& parent)const noexcept { return pHandle->is_child_of(parent); }           //!< ハンドルの所有者を確認
        protected:
            HandleBase* pHandle;                                                                                           //!< インスタンス・ポインタ
        };


        //@―---------------------------------------------------------------------------
        //! @brief イテレータ
        //@―---------------------------------------------------------------------------
        class iterator :public const_iterator {
            friend class HandleForwardList<T>;
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
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                //!< 後置インクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                          //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                 //!< 否等価演算子
        protected:
            iterator(const HandleBase* pHandle)noexcept :const_iterator(pHandle) {}                                        //!< ハンドルのポインタから生成(内部用)
        };


    public:

        HandleForwardList();
        HandleForwardList(this_type&& x)noexcept;
        this_type& operator=(this_type&& x)noexcept;
        ~HandleForwardList();

        iterator begin_before()noexcept;
        iterator begin()noexcept;
        iterator end()noexcept;
        const_iterator cbegin_before()const noexcept;
        const_iterator cbegin()const noexcept;
        const_iterator cend()const noexcept;
        
        //! @cond 
        const_iterator begin_before()const noexcept;
        const_iterator begin()const noexcept;
        const_iterator end()const noexcept;
        const_reference front() const;
        //! @endcond 

        bool empty()const noexcept;
        size_type size() const noexcept;

        reference front();

        template<class... Args>
        reference emplace_front(Handle& h, Args&&... args);
        void push_front(Handle& h, const T& x);
        void push_front(Handle& h, T&& x);
        void pop_front();
        template<class... Args>
        iterator emplace_after(Handle& h, const_iterator position, Args&&... args);
        iterator insert_after(Handle& h, const_iterator position, const T& x);
        iterator insert_after(Handle& h, const_iterator position, T&& x);
        iterator erase_after(const_iterator position);
        iterator erase_after(const_iterator position, const_iterator last);
        void swap(this_type& x)noexcept;
        void clear()noexcept;

        void remove(Handle& h);
        template<class Predicate>
        void remove_if(Predicate pred);
        void reverse()noexcept;

    private:

        void clear_impl()noexcept;
        void move_impl(this_type&& src)noexcept;
        void reset_parent()noexcept;
        void insert_impl(HandleBase* pPrev, Handle& h);
        void insert_front_impl(Handle& h);
        iterator erase_impl(HandleBase* pHandle);

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
    inline bool operator==(HandleForwardList<T> const& a, HandleForwardList<T> const& b)noexcept {
        ScopeLock lock(m_mutex);;
        return (a.size() == b.size()) && equal(a.begin(), a.end(), b.begin());
    }

    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline bool operator!=(HandleForwardList<T> const& a, HandleForwardList<T> const& b)noexcept {
        return !(*this == rhs);
    }



    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline HandleForwardList<T>::HandleForwardList()
        :m_header() {
        clear_impl();
    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブコンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline HandleForwardList<T>::HandleForwardList(this_type&& x)noexcept {
        clear_impl();
        move_impl(std::move(x));
    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブ代入演算子
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline HandleForwardList<T>& HandleForwardList<T>::operator=(this_type&& rhs)noexcept {
        if (this != &rhs) {
            clear();
            ScopeLock lock(m_mutex);;
            move_impl(std::move(rhs));
        }
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief デストラクタ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline HandleForwardList<T>::~HandleForwardList() {
        clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素の前を指すイテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::iterator HandleForwardList<T>::begin_before()noexcept {
        return iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素の前を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::const_iterator HandleForwardList<T>::begin_before()const noexcept {
        return const_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::iterator HandleForwardList<T>::begin()noexcept {
        return iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::const_iterator HandleForwardList<T>::begin()const noexcept {
        return const_iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::iterator HandleForwardList<T>::end()noexcept {
        return iterator(nullptr);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::const_iterator HandleForwardList<T>::end()const noexcept {
        return const_iterator(nullptr);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素の前を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::const_iterator HandleForwardList<T>::cbegin_before()const noexcept {
        return const_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::const_iterator HandleForwardList<T>::cbegin()const noexcept {
        return const_iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::const_iterator HandleForwardList<T>::cend()const noexcept {
        return const_iterator(nullptr);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンテナが空かどうかを判定する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline bool HandleForwardList<T>::empty()const noexcept {
        ScopeLock lock(m_mutex);;
        return m_header.pNext == nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素数を取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::size_type HandleForwardList<T>::size()const noexcept {
        ScopeLock lock(m_mutex);;
        return std::distance(begin(), end());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素への参照を取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::reference HandleForwardList<T>::front() {
        ScopeLock lock(m_mutex);;
        assert(m_header.pNext != nullptr);
        return *(static_cast<Handle*>(m_header.pNext)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素への参照を取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::const_reference HandleForwardList<T>::front() const {
        ScopeLock lock(m_mutex);;
        assert(m_header.pNext != nullptr);
        return *(static_cast<Handle*>(m_header.pNext)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleForwardList<T>::push_front(Handle& h, const T& x) {
        ScopeLock lock(m_mutex);;
        insert_front_impl(h);
        *h.get_ptr() = x;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を直接構築して追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class... Args>
    inline typename HandleForwardList<T>::reference HandleForwardList<T>::emplace_front(Handle& h, Args&&... args) {
        ScopeLock lock(m_mutex);;
        insert_front_impl(h);
        ob::construct_at(h.get_ptr(), std::forward<Args>(args)...);
        return *h.get_ptr();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleForwardList<T>::push_front(Handle& h, T&& x) {
        ScopeLock lock(m_mutex);;
        insert_front_impl(h);
        *h.get_ptr() = std::move(x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭から要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleForwardList<T>::pop_front() {
        ScopeLock lock(m_mutex);;
        if (m_header.pNext == nullptr)return;
        erase_impl(static_cast<Handle*>(&m_header));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定位置に直接構築して挿入する
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class... Args>
    inline typename HandleForwardList<T>::iterator HandleForwardList<T>::emplace_after(Handle& h, const_iterator position, Args&&... args) {
        ScopeLock lock(m_mutex);;
        insert_impl(position.pHandle, h);
        ob::construct_at(h.get_ptr(), std::forward<Args>(args)...);
        return &h;
    }



    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定位置に挿入する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::iterator HandleForwardList<T>::insert_after(Handle& h, const_iterator position, const T& x) {
        ScopeLock lock(m_mutex);;
        insert_impl(position.pHandle, h);
        *h.get_ptr() = x;
        return &h;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定位置に挿入する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::iterator HandleForwardList<T>::insert_after(Handle& h, const_iterator position, T&& x) {
        ScopeLock lock(m_mutex);;
        insert_impl(position.pHandle, h);
        *h.get_ptr() = x;
        return &h;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 指定位置の要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::iterator HandleForwardList<T>::erase_after(const_iterator position) {
        ScopeLock lock(m_mutex);;
        return erase_impl(position.pHandle);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 指定範囲の要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::iterator HandleForwardList<T>::erase_after(const_iterator position, const_iterator last) {
        ScopeLock lock(m_mutex);;
        auto itr = position;
        while (position != last) {
            auto pHandle = position.pHandle;
            ++position;
            itr = erase_impl(pHandle);
        }
        return itr.pHandle;
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンテナを交換する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleForwardList<T>::swap(this_type& x)noexcept {
        std::swap(*this, x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 全要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleForwardList<T>::clear()noexcept {
        ScopeLock lock(m_mutex);;
        auto itr = begin();
        while (itr != end()) {
            auto pHandle = itr.pHandle;
            ++itr;
            pHandle->clear();
        }
        clear_impl();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleForwardList<T>::remove(Handle& h) {
        ScopeLock lock(m_mutex);;
        auto itrPrev = begin_before();
        auto itr = begin();
        const auto itrEnd = end();
        while (itr != itrEnd) {
            if (itr.pHandle == &h) {
                ++itr;
                erase_impl(itrPrev.pHandle);
            } else {
                ++itr;
                ++itrPrev;
            }
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief 条件に合った要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class Predicate>
    inline void HandleForwardList<T>::remove_if(Predicate pred) {
        ScopeLock lock(m_mutex);;
        auto itrPrev = begin_before();
        auto itr = begin();
        const auto itrEnd = end();
        while (itr != itrEnd) {
            if (pred(*itr)) {
                ++itr;
                erase_impl(itrPrev.pHandle);
            } else {
                ++itr;
                ++itrPrev;
            }
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンテナを反転する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleForwardList<T>::reverse()noexcept {
        ScopeLock lock(m_mutex);;

        HandleBase* pPrev = nullptr;
        HandleBase* pPos = m_header.pNext;
        while (pPos != nullptr) {
            auto pNext = pPos->pNext;
            pPos->pNext = pPrev;
            pPrev = pPos;
            pPos = pNext;
        }
        m_header.pNext = pPrev;
    }


    //@―---------------------------------------------------------------------------
    //! @brief リストをクリア(remove呼び出しなし)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleForwardList<T>::clear_impl()noexcept {
        m_header.pNext = nullptr;
        m_header.pParent = this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleForwardList<T>::move_impl(this_type&& src)noexcept {
        if (src.empty()) {
            clear_impl();
        } else {
            m_header.pNext = src.m_header.pNext;
        }
        src.clear_impl();
        reset_parent();
    }


    //@―---------------------------------------------------------------------------
    //! @brief pParentの更新
    //@―---------------------------------------------------------------------------
    template<typename T>
    void HandleForwardList<T>::reset_parent()noexcept {
        auto itr = begin();
        while (itr != end()) {
            itr.pHandle->pParent = this;
            ++itr;
        }
    }

    //@―---------------------------------------------------------------------------
    //! @brief 要素を挿入
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleForwardList<T>::insert_impl(HandleBase* pPrev, Handle& h) {
        assert(pPrev != nullptr);
        assert(pPrev->is_child_of(*this));// 違うコンテナのイテレータならエラー

        h.remove();
        h.pParent = this;

        if (pPrev->pNext == nullptr) {
            // 最後に追加
            pPrev->pNext = &h;
            h.pNext = nullptr;
        } else {
            h.pNext = pPrev->pNext;
            pPrev->pNext = &h;
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を追加
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void HandleForwardList<T>::insert_front_impl(Handle& h) {
        insert_impl(&m_header, h);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の削除
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename HandleForwardList<T>::iterator HandleForwardList<T>::erase_impl(HandleBase* pHandle) {
        assert(pHandle != nullptr);
        assert(pHandle->pNext != nullptr);      // 最後の要素の次は消せない
        assert(pHandle->is_child_of(*this));

        auto pDelete = pHandle->pNext;
        pHandle->pNext = pDelete->pNext;

        pDelete->clear();

        return pHandle->pNext;
    }


}// namespcae ob