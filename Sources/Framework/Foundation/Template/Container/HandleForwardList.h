//***********************************************************
//! @file
//! @brief		ハンドル管理式単方向連結リスト
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::foundation {


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
    //!                 ob::handle_forward_list<ob::s32> ls;
    //!                 using Handle =  ob::handle_forward_list<ob::s32>::handle;
    //!                 {
    //!                     Handle handle;
    //!                     ls.emplace_front(handle,123);
    //!                     std::cout<<ls.size()<<std::endl;    // 1
    //!                 }
    //!                 std::cout<<ls.size()<<std::endl;    // 0
    //! ```
    //@―---------------------------------------------------------------------------
    template<typename T>
    class handle_forward_list :private Noncopyable {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief ハンドル基底
        //@―---------------------------------------------------------------------------
        class handle_base {
            friend class handle_forward_list<T>;
        public:

            //@―---------------------------------------------------------------------------
            //! @brief ハンドルが対象のリストの要素か判定する
            //@―---------------------------------------------------------------------------
            bool is_child_of(const handle_forward_list<T>& parent)const noexcept {
                return pParent == ob::addressof(parent);
            }

        protected:
            void clear()noexcept {
                pParent = nullptr;
                pNext = nullptr;
            }
        protected:
            handle_base* pNext = nullptr;
            handle_forward_list<T>* pParent = nullptr;
        };


        //@―---------------------------------------------------------------------------
        //! @brief ハンドル
        //@―---------------------------------------------------------------------------
        class handle :public handle_base, private Noncopyable, private Nonmovable {
            friend class handle_forward_list<T>;
        public:

            //@―---------------------------------------------------------------------------
            //! @brief コンストラクタ
            //@―---------------------------------------------------------------------------
            handle() = default;

            //@―---------------------------------------------------------------------------
            //! @brief デストラクタ
            //@―---------------------------------------------------------------------------
            ~handle() {
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

        using this_type = handle_forward_list<T>;
        using value_type = T;
        using size_type = size_t;

        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

        using mutex_type = mutex;
        using difference_type = int;


        //@―---------------------------------------------------------------------------
        //! @brief constイテレータ
        //@―---------------------------------------------------------------------------
        class const_iterator {
            friend class handle_forward_list<T>;
        public:
            using this_type = const_iterator;                                                                               //!< 型
            using difference_type = typename handle_forward_list<T>::difference_type;                                               //!< ポインタ差分型
            using value_type = T;                                                                                           //!< インスタンス型
            using pointer = T*;                                                                                             //!< ポインタ型
            using reference = value_type&;                                                                                  //!< 参照型
            using iterator_category = bidirectional_iterator_tag;                                                       //!< イテレータ・カテゴリ
        public:
            const_iterator(const this_type& x)noexcept :pHandle(const_cast<handle_base*>(x.pHandle)) {}                     //!< ムーブコンストラクタ
            const_iterator& operator = (const this_type& x)noexcept { pHandle = const_cast<handle_base*>(x.pHandle); return*this; }     //!< ムーブ代入演算子
            reference operator*() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                    //!< インスタンス参照
            pointer   operator->() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                   //!< メンバアクセス
            this_type& operator++() noexcept { pHandle = pHandle->pNext; return *this; }                                    //!< インクリメント
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                //!< 後置インクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                          //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                 //!< 否等価演算子
        protected:
            const_iterator(const handle_base* pHandle)noexcept :pHandle(const_cast<handle_base*>(pHandle)) {}               //!< ハンドルのポインタから生成(内部用)
        private:
            bool is_child_of(const handle_forward_list<T>& parent)const noexcept { return pHandle->is_child_of(parent); }           //!< ハンドルの所有者を確認
        protected:
            handle_base* pHandle;                                                                                           //!< インスタンス・ポインタ
        };


        //@―---------------------------------------------------------------------------
        //! @brief イテレータ
        //@―---------------------------------------------------------------------------
        class iterator :public const_iterator {
            friend class handle_forward_list<T>;
        public:
            using this_type = iterator;                                                                                     //!< 型
            using value_type = const T;                                                                                     //!< インスタンス型
            using pointer = const T*;                                                                                       //!< ポインタ型
            using reference = const T&;                                                                                     //!< 参照型
        public:
            iterator(const this_type& x)noexcept :const_iterator(x) {}                                                      //!< ムーブコンストラクタ
            iterator& operator = (const this_type& x)noexcept { const_iterator::operator=(x); return *this; }               //!< ムーブ代入演算子
            reference operator*() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                    //!< インスタンス参照
            pointer   operator->() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                   //!< メンバアクセス
            this_type& operator++() noexcept { pHandle = pHandle->pNext; return *this; }                                    //!< インクリメント
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                //!< 後置インクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                          //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                 //!< 否等価演算子
        protected:
            iterator(const handle_base* pHandle)noexcept :const_iterator(pHandle) {}                                        //!< ハンドルのポインタから生成(内部用)
        };


    public:

        handle_forward_list();
        handle_forward_list(this_type&& x)noexcept;
        this_type& operator=(this_type&& x)noexcept;
        ~handle_forward_list();

        iterator begin_before()noexcept;
        const_iterator begin_before()const noexcept;
        iterator begin()noexcept;
        const_iterator begin()const noexcept;
        iterator end()noexcept;
        const_iterator end()const noexcept;
        const_iterator cbegin_before()const noexcept;
        const_iterator cbegin()const noexcept;
        const_iterator cend()const noexcept;

        bool empty()const noexcept;
        size_type size() const noexcept;

        reference front();
        const_reference front() const;

        template<class... Args>
        reference emplace_front(handle& h, Args&&... args);
        void push_front(handle& h, const T& x);
        void push_front(handle& h, T&& x);
        void pop_front();
        template<class... Args>
        iterator emplace_after(handle& h, const_iterator position, Args&&... args);
        iterator insert_after(handle& h, const_iterator position, const T& x);
        iterator insert_after(handle& h, const_iterator position, T&& x);
        iterator erase_after(const_iterator position);
        iterator erase_after(const_iterator position, const_iterator last);
        void swap(this_type& x)noexcept;
        void clear()noexcept;

        void remove(handle& h);
        template<class Predicate>
        void remove_if(Predicate pred);
        void reverse()noexcept;

    private:

        void clear_impl()noexcept;
        void move_impl(this_type&& src)noexcept;
        void reset_parent()noexcept;
        void insert_impl(handle_base* pPrev, handle& h);
        void insert_front_impl(handle& h);
        iterator erase_impl(handle_base* pHandle);

    private:

        mutable mutex_type mutex;
        handle_base m_header;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline bool operator==(handle_forward_list<T> const& a, handle_forward_list<T> const& b)noexcept {
        lock_guard<mutex_type> lg(mutex);
        return (a.size() == b.size()) && equal(a.begin(), a.end(), b.begin());
    }

    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline bool operator!=(handle_forward_list<T> const& a, handle_forward_list<T> const& b)noexcept {
        return !(*this == rhs);
    }



    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline handle_forward_list<T>::handle_forward_list()
        :m_header() {
        clear_impl();
    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブコンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline handle_forward_list<T>::handle_forward_list(this_type&& x)noexcept {
        clear_impl();
        move_impl(ob::move(x));
    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブ代入演算子
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline handle_forward_list<T>& handle_forward_list<T>::operator=(this_type&& rhs)noexcept {
        if (this != &rhs) {
            clear();
            lock_guard<mutex_type> lg(mutex);
            move_impl(ob::move(rhs));
        }
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief デストラクタ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline handle_forward_list<T>::~handle_forward_list() {
        clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素の前を指すイテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::iterator handle_forward_list<T>::begin_before()noexcept {
        return iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素の前を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::const_iterator handle_forward_list<T>::begin_before()const noexcept {
        return const_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::iterator handle_forward_list<T>::begin()noexcept {
        return iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::const_iterator handle_forward_list<T>::begin()const noexcept {
        return const_iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::iterator handle_forward_list<T>::end()noexcept {
        return iterator(nullptr);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::const_iterator handle_forward_list<T>::end()const noexcept {
        return const_iterator(nullptr);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素の前を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::const_iterator handle_forward_list<T>::cbegin_before()const noexcept {
        return const_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::const_iterator handle_forward_list<T>::cbegin()const noexcept {
        return const_iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::const_iterator handle_forward_list<T>::cend()const noexcept {
        return const_iterator(nullptr);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンテナが空かどうかを判定する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline bool handle_forward_list<T>::empty()const noexcept {
        lock_guard<mutex_type> lg(mutex);
        return m_header.pNext == nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素数を取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::size_type handle_forward_list<T>::size()const noexcept {
        lock_guard<mutex_type> lg(mutex);
        return ob::distance(begin(), end());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素への参照を取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::reference handle_forward_list<T>::front() {
        lock_guard<mutex_type> lg(mutex);
        assert(m_header.pNext != nullptr);
        return *(static_cast<handle*>(m_header.pNext)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素への参照を取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::const_reference handle_forward_list<T>::front() const {
        lock_guard<mutex_type> lg(mutex);
        assert(m_header.pNext != nullptr);
        return *(static_cast<handle*>(m_header.pNext)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_forward_list<T>::push_front(handle& h, const T& x) {
        lock_guard<mutex_type> lg(mutex);
        insert_front_impl(h);
        *h.get_ptr() = x;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を直接構築して追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class... Args>
    inline typename handle_forward_list<T>::reference handle_forward_list<T>::emplace_front(handle& h, Args&&... args) {
        lock_guard<mutex_type> lg(mutex);
        insert_front_impl(h);
        ob::construct_at(h.get_ptr(), ob::forward<Args>(args)...);
        return *h.get_ptr();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_forward_list<T>::push_front(handle& h, T&& x) {
        lock_guard<mutex_type> lg(mutex);
        insert_front_impl(h);
        *h.get_ptr() = ob::move(x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭から要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_forward_list<T>::pop_front() {
        lock_guard<mutex_type> lg(mutex);
        if (m_header.pNext == nullptr)return;
        erase_impl(static_cast<handle*>(&m_header));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定位置に直接構築して挿入する
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class... Args>
    inline typename handle_forward_list<T>::iterator handle_forward_list<T>::emplace_after(handle& h, const_iterator position, Args&&... args) {
        lock_guard<mutex_type> lg(mutex);
        insert_impl(position.pHandle, h);
        ob::construct_at(h.get_ptr(), ob::forward<Args>(args)...);
        return &h;
    }



    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定位置に挿入する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::iterator handle_forward_list<T>::insert_after(handle& h, const_iterator position, const T& x) {
        lock_guard<mutex_type> lg(mutex);
        insert_impl(position.pHandle, h);
        *h.get_ptr() = x;
        return &h;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定位置に挿入する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::iterator handle_forward_list<T>::insert_after(handle& h, const_iterator position, T&& x) {
        lock_guard<mutex_type> lg(mutex);
        insert_impl(position.pHandle, h);
        *h.get_ptr() = x;
        return &h;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 指定位置の要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::iterator handle_forward_list<T>::erase_after(const_iterator position) {
        lock_guard<mutex_type> lg(mutex);
        return erase_impl(position.pHandle);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 指定範囲の要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::iterator handle_forward_list<T>::erase_after(const_iterator position, const_iterator last) {
        lock_guard<mutex_type> lg(mutex);
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
    inline void handle_forward_list<T>::swap(this_type& x)noexcept {
        ob::swap(*this, x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 全要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_forward_list<T>::clear()noexcept {
        lock_guard<mutex_type> lg(mutex);
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
    inline void handle_forward_list<T>::remove(handle& h) {
        lock_guard<mutex_type> lg(mutex);
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
    inline void handle_forward_list<T>::remove_if(Predicate pred) {
        lock_guard<mutex_type> lg(mutex);
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
    inline void handle_forward_list<T>::reverse()noexcept {
        lock_guard<mutex_type> lg(mutex);

        handle_base* pPrev = nullptr;
        handle_base* pPos = m_header.pNext;
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
    inline void handle_forward_list<T>::clear_impl()noexcept {
        m_header.pNext = nullptr;
        m_header.pParent = this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_forward_list<T>::move_impl(this_type&& src)noexcept {
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
    void handle_forward_list<T>::reset_parent()noexcept {
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
    inline void handle_forward_list<T>::insert_impl(handle_base* pPrev, handle& h) {
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
    inline void handle_forward_list<T>::insert_front_impl(handle& h) {
        insert_impl(&m_header, h);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の削除
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_forward_list<T>::iterator handle_forward_list<T>::erase_impl(handle_base* pHandle) {
        assert(pHandle != nullptr);
        assert(pHandle->pNext != nullptr);      // 最後の要素の次は消せない
        assert(pHandle->is_child_of(*this));

        auto pDelete = pHandle->pNext;
        pHandle->pNext = pDelete->pNext;

        pDelete->clear();

        return pHandle->pNext;
    }


}// namespcae ob