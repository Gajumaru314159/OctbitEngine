//***********************************************************
//! @file
//! @brief		ハンドル管理式双方向連結リスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Runtime/Foundation/Base/Fwd.h>
#include <Runtime/Foundation/Template/algorithm.h>
#include <Runtime/Foundation/Template/allocator.h>
#include <Runtime/Foundation/Template/iterator.h>
#include <Runtime/Foundation/Template/utility.h>
#include <Runtime/Foundation/Template/memory.h>
#include <Runtime/Foundation/Template/thread/mutex.h>

namespace ob {


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
    //!                 ob::handle_list<ob::s32> ls;
    //!                 using Handle =  ob::handle_list<ob::s32>::handle;
    //!                 {
    //!                     Handle handle;
    //!                     ls.emplace_back(handle,123);
    //!                     std::cout<<ls.size()<<std::endl;    // 1
    //!                 }
    //!                 std::cout<<ls.size()<<std::endl;    // 0
    //! ```
    //@―---------------------------------------------------------------------------
    template<typename T>
    class handle_list:private Noncopyable {
    public:

        //! @cond
        //@―---------------------------------------------------------------------------
        //! @brief ハンドル基底
        //@―---------------------------------------------------------------------------
        class handle_base {
            friend class handle_list<T>;
        public:

            //@―---------------------------------------------------------------------------
            //! @brief ハンドルが対象のリストの要素か判定する
            //@―---------------------------------------------------------------------------
            bool is_child_of(const handle_list<T>& parent)const noexcept {
                return pParent == ob::addressof(parent);
            }

        protected:
            void clear()noexcept {
                pParent = nullptr;
                pPrev = nullptr;
                pNext = nullptr;
            }
        protected:
            handle_base* pPrev = nullptr;
            handle_base* pNext = nullptr;
            handle_list<T>* pParent = nullptr;
        };
        //! @endcond


        //@―---------------------------------------------------------------------------
        //! @brief ハンドル
        //@―---------------------------------------------------------------------------
        class handle :public handle_base,private Noncopyable,private Nonmovable {
            friend class handle_list<T>;
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

        using this_type = handle_list<T>;       //!< 型
        using value_type = T;                   //!< 値型
        using size_type = size_t;               //!< サイズ
        using difference_type = int;            //!< ポインタ差分型

        using pointer = T*;                     //!< ポインタ型
        using const_pointer = const T*;         //!< constポインタ型
        using reference = T&;                   //!< 参照型
        using const_reference = const T&;       //!< const参照型

    private:
        using mutex_type = ob::mutex;           //!<

    public:

#pragma region Iterator

        //@―---------------------------------------------------------------------------
        //! @brief constイテレータ
        //@―---------------------------------------------------------------------------
        class const_iterator {
            friend class handle_list<T>;
        public:
            using this_type = const_iterator;                                                                               //!< 型
            using difference_type = typename handle_list<T>::difference_type;                                               //!< ポインタ差分型
            using value_type = T;                                                                                           //!< インスタンス型
            using pointer = T*;                                                                                             //!< ポインタ型
            using reference = value_type&;                                                                                  //!< 参照型
            using iterator_category = ob::bidirectional_iterator_tag;                                                       //!< イテレータ・カテゴリ
        public:
            const_iterator(const this_type& x)noexcept :pHandle(const_cast<handle_base*>(x.pHandle)) {}                     //!< ムーブコンストラクタ
            const_iterator& operator = (const this_type & x)noexcept{ pHandle = const_cast<handle_base*>(x.pHandle); return*this; }     //!< ムーブ代入演算子
            reference operator*() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                    //!< インスタンス参照
            pointer   operator->() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                   //!< メンバアクセス
            this_type& operator++() noexcept { pHandle = pHandle->pNext; return *this; }                                    //!< インクリメント
            this_type& operator--() noexcept { pHandle = pHandle->pPrev; return *this; }                                    //!< デクリメント
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                //!< 後置インクリメント
            this_type operator--(int)noexcept { auto temp = *this; --(*this); return temp; }                                //!< 後置デクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                          //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                 //!< 否等価演算子
        protected:
            const_iterator(const handle_base* pHandle)noexcept :pHandle(const_cast<handle_base*>(pHandle)) {}               //!< ハンドルのポインタから生成(内部用)
        private:
            bool is_child_of(const handle_list<T>& parent)const noexcept { return pHandle->is_child_of(parent); }           //!< ハンドルの所有者を確認
        protected:
            handle_base* pHandle;                                                                                           //!< インスタンス・ポインタ
        };


        //@―---------------------------------------------------------------------------
        //! @brief イテレータ
        //@―---------------------------------------------------------------------------
        class iterator :public const_iterator {
            friend class handle_list<T>;
        public:
            using this_type = iterator;                                                                                     //!< 型
            using value_type = const T;                                                                                     //!< インスタンス型
            using pointer = const T*;                                                                                       //!< ポインタ型
            using reference = const T&;                                                                                     //!< 参照型
        public:
            iterator(const this_type& x)noexcept :const_iterator(x) {}                                                      //!< ムーブコンストラクタ
            iterator& operator = (const this_type & x)noexcept{ const_iterator::operator=(x); return *this; }               //!< ムーブ代入演算子
            reference operator*() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                    //!< インスタンス参照
            pointer   operator->() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                   //!< メンバアクセス
            this_type& operator++() noexcept { pHandle = pHandle->pNext; return *this; }                                    //!< インクリメント
            this_type& operator--() noexcept { pHandle = pHandle->pPrev; return *this; }                                    //!< デクリメント
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                //!< 後置インクリメント
            this_type operator--(int)noexcept { auto temp = *this; --(*this); return temp; }                                //!< 後置デクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                          //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                 //!< 否等価演算子
        protected:
            iterator(const handle_base* pHandle)noexcept :const_iterator(pHandle) {}                                        //!< ハンドルのポインタから生成(内部用)
        };


        //@―---------------------------------------------------------------------------
        //! @brief const逆イテレータ
        //@―---------------------------------------------------------------------------
        class const_reverse_iterator :public const_iterator {
            friend class handle_list<T>;
        public:
            using this_type = const_reverse_iterator;                                                                           //!< 型
            using value_type = T;                                                                                               //!< インスタンス型
            using pointer = T*;                                                                                                 //!< ポインタ型
            using reference = T&;                                                                                               //!< 参照型
        public:
            const_reverse_iterator(const this_type& x)noexcept :const_iterator(x) {}                                            //!< ムーブコンストラクタ
            const_reverse_iterator& operator = (const this_type & x)noexcept{ const_iterator::operator=(x); return *this; }     //!< ムーブ代入演算子
            reference operator*() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                        //!< インスタンス参照
            pointer   operator->() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                       //!< メンバアクセス
            this_type& operator++() noexcept { pHandle = pHandle->pPrev; return *this; }                                        //!< インクリメント
            this_type& operator--() noexcept { pHandle = pHandle->pNext; return *this; }                                        //!< デクリメント
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                    //!< 後置インクリメント
            this_type operator--(int)noexcept { auto temp = *this; --(*this); return temp; }                                    //!< 後置デクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                              //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                     //!< 否等価演算子
        protected:
            const_reverse_iterator(const handle_base* pHandle)noexcept :const_iterator(pHandle) {}                              //!< ハンドルのポインタから生成(内部用)
        };


        //@―---------------------------------------------------------------------------
        //! @brief 逆イテレータ
        //@―---------------------------------------------------------------------------
        class reverse_iterator :public const_reverse_iterator {
            friend class handle_list<T>;
        public:
            using this_type = reverse_iterator;                                                                                 //!< 型
            using value_type = const T;                                                                                         //!< インスタンス型
            using pointer = const T*;                                                                                           //!< ポインタ型
            using reference = const T&;                                                                                         //!< 参照型
        public:
            reverse_iterator(const this_type& x)noexcept :const_reverse_iterator(x) {}                                          //!< ムーブコンストラクタ
            reverse_iterator& operator = (const this_type & x)noexcept{ const_iterator::operator=(x); return *this; }           //!< ムーブ代入演算子
            reference operator*() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                        //!< インスタンス参照
            pointer   operator->() const noexcept { return *(static_cast<handle*>(pHandle)->get_ptr()); }                       //!< メンバアクセス
            this_type& operator++() noexcept { pHandle = pHandle->pPrev; return *this; }                                        //!< インクリメント
            this_type& operator--() noexcept { pHandle = pHandle->pNext; return *this; }                                        //!< デクリメント
            this_type operator++(int)noexcept { auto temp = *this; ++(*this); return temp; }                                    //!< 後置インクリメント
            this_type operator--(int)noexcept { auto temp = *this; --(*this); return temp; }                                    //!< 後置デクリメント
            bool operator==(const this_type& rhs)const noexcept { return pHandle == rhs.pHandle; }                              //!< 等価演算子
            bool operator!=(const this_type& rhs)const noexcept { return !(*this == rhs); }                                     //!< 否等価演算子
        protected:
            reverse_iterator(const handle_base* pHandle)noexcept :const_reverse_iterator(pHandle) {}                            //!< ハンドルのポインタから生成(内部用)
        };

#pragma endregion

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        handle_list();                                                          // コンストラクタ
        handle_list(this_type&& x)noexcept;                                     // ムーブコンストラクタ
        this_type& operator=(this_type&& x)noexcept;                            // ムーブ代入演算子
        ~handle_list();                                                         // デストラクタ
        

        //===============================================================
        // イテレータ
        //===============================================================
        iterator begin()noexcept;                                               // 先頭を指すイテレータを取得する
        const_iterator begin()const noexcept;                                   // 先頭を指すイテレータを取得する(const)
        iterator end()noexcept;                                                 // 末尾の次を指すイテレータを取得する
        const_iterator end()const noexcept;                                     // 末尾の次を指すイテレータを取得する(const)
        const_iterator cbegin()const noexcept;                                  // 先頭を指すイテレータを取得する(const)
        const_iterator cend()const noexcept;                                    // 末尾の次を指すイテレータを取得する(const)

        reverse_iterator rbegin()noexcept;                                      // 先頭を指す逆イテレータを取得する
        const_reverse_iterator rbegin()const noexcept;                          // 先頭を指す逆イテレータを取得する(const)
        reverse_iterator rend()noexcept;                                        // 末尾の次を指す逆イテレータを取得する
        const_reverse_iterator rend()const noexcept;                            // 末尾の次を指す逆イテレータを取得する(const)
        const_reverse_iterator crbegin()const noexcept;                         // 先頭を指す逆イテレータを取得する(const)
        const_reverse_iterator crend()const noexcept;                           // 末尾の次を指す逆イテレータを取得する(const)


        //===============================================================
        // 領域
        //===============================================================
        bool empty()const noexcept;                                             // コンテナが空かどうかを判定する
        size_type size() const noexcept;                                        // 要素数を取得する


        //===============================================================
        // 要素アクセス
        //===============================================================
        reference front();                                                      // 先頭要素への参照を取得する
        const_reference front() const;                                          // 先頭要素への参照を取得する(const)
        reference back();                                                       // 末尾要素への参照を取得する
        const_reference back() const;                                           // 末尾要素への参照を取得する(const)


        //===============================================================
        // コンテナの変更
        //===============================================================
        void push_front(handle& h, const T& x);                                 // 先頭に要素を追加する
        void push_front(handle& h, T&& x);                                      // 先頭に要素を追加する
        template<class... Args>
        reference emplace_front(handle& h, Args&&... args);                     // 先頭に要素を直接構築して追加する
        void push_back(handle& h, const T& x);                                  // 末尾に要素を追加する
        void push_back(handle& h, T&& x);                                       // 末尾に要素を追加する
        template<class... Args>
        reference emplace_back(handle& h, Args&&... args);                      // 末尾に要素を直接構築して追加する
        iterator insert(handle& h, const_iterator position, const T& x);        // 要素を指定位置に挿入する
        iterator insert(handle& h, const_iterator position, T&& x);             // 要素を指定位置に挿入する
        template<class... Args>
        iterator emplace(handle& h, const_iterator position, Args&&... args);   // 要素を指定位置に直接構築して挿入する
        void pop_front()noexcept;                                               // 先頭から要素を削除する
        void pop_back()noexcept;                                                // 末尾から要素を削除する
        iterator erase(const_iterator position);                                // 指定位置の要素を削除する
        iterator erase(const_iterator position, const_iterator last);           // 指定範囲の要素を削除する
        void clear()noexcept;                                                   // 全要素を削除する
        void swap(this_type& x)noexcept;                                        // コンテナを交換する


        //===============================================================
        // リスト操作
        //===============================================================
        void remove(handle& h);                                                 // 要素を削除する
        template<class Predicate>
        void remove_if(Predicate pred);                                         // 条件に合った要素を削除する
        void reverse()noexcept;                                                 // コンテナを反転する

    private:

        void clear_impl()noexcept;                                              // リストをクリア(remove呼び出しなし)
        void move_impl(this_type&& src)noexcept;                                // ムーブ共通処理
        void reset_parent()noexcept;                                            // 全ての要素のpParentの更新
        void insert_impl(handle_base* pPrev, handle& h);                        // 要素を指定したハンドルの次に挿入
        void insert_front_impl(handle& h);                                      // 先頭に要素を追加
        void insert_back_impl(handle& h);                                       // 末尾に要素を追加
        iterator erase_impl(handle_base* pHandle)noexcept;                      // 要素の削除

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
    inline bool operator==(handle_list<T> const& a, handle_list<T> const& b)noexcept {
        lock_guard<mutex_type> lg(mutex);
        return (a.size() == b.size()) && ob::equal(a.begin(), a.end(), b.begin());
    }

    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline bool operator!=(handle_list<T> const& a, handle_list<T> const& b)noexcept {
        return !(*this == rhs);
    }



    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline handle_list<T>::handle_list()
        :m_header() {
        clear_impl();
    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブコンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline handle_list<T>::handle_list(this_type&& x)noexcept {
        clear_impl();
        move_impl(ob::move(x));
    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブ代入演算子
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline handle_list<T>& handle_list<T>::operator=(this_type&& rhs)noexcept {
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
    inline handle_list<T>::~handle_list() {
        clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::iterator handle_list<T>::begin()noexcept {
        return iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::const_iterator handle_list<T>::begin()const noexcept {
        return const_iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::iterator handle_list<T>::end()noexcept {
        return iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::const_iterator handle_list<T>::end()const noexcept {
        return const_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::const_iterator handle_list<T>::cbegin()const noexcept {
        return const_iterator(m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指すイテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::const_iterator handle_list<T>::cend()const noexcept {
        return const_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指す逆イテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::reverse_iterator handle_list<T>::rbegin()noexcept {
        return reverse_iterator(m_header.pPrev);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指す逆イテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::const_reverse_iterator handle_list<T>::rbegin()const noexcept {
        return const_reverse_iterator(m_header.pPrev);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指す逆イテレータを取得する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::reverse_iterator handle_list<T>::rend()noexcept {
        return reverse_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指す逆イテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::const_reverse_iterator handle_list<T>::rend()const noexcept {
        return const_reverse_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭を指す逆イテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::const_reverse_iterator handle_list<T>::crbegin()const noexcept {
        return const_reverse_iterator(m_header.pPrev);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾の次を指す逆イテレータを取得する(const)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::const_reverse_iterator handle_list<T>::crend()const noexcept {
        return const_reverse_iterator(&m_header);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンテナが空かどうかを判定する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline bool handle_list<T>::empty()const noexcept {
        lock_guard<mutex_type> lg(mutex);
        return m_header.pNext == &m_header;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素数を取得する
    //! 
    //! @note  O(n)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::size_type handle_list<T>::size()const noexcept {
        lock_guard<mutex_type> lg(mutex);
        return ob::distance(begin(), end());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素への参照を取得する
    //! 
    //! @note  コンテナが空の時はassertを発生させる。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::reference handle_list<T>::front() {
        lock_guard<mutex_type> lg(mutex);
        assert(m_header.pNext != &m_header);
        return *(static_cast<handle*>(m_header.pNext)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭要素への参照を取得する(const)
    //! 
    //! @note  コンテナが空の時はassertを発生させる。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::const_reference handle_list<T>::front() const {
        lock_guard<mutex_type> lg(mutex);
        assert(m_header.pNext != &m_header);
        return *(static_cast<handle*>(m_header.pNext)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾要素への参照を取得する
    //! 
    //! @note  コンテナが空の時はassertを発生させる。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::reference handle_list<T>::back() {
        lock_guard<mutex_type> lg(mutex);
        assert(m_header.pPrev != &m_header);
        return *(static_cast<handle*>(m_header.pPrev)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾要素への参照を取得する(const)
    //! 
    //! @note  コンテナが空の時はassertを発生させる。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::const_reference handle_list<T>::back() const {
        lock_guard<mutex_type> lg(mutex);
        assert(m_header.pPrev != &m_header);
        return *(static_cast<handle*>(m_header.pPrev)->get_ptr());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::push_front(handle& h, const T& x) {
        lock_guard<mutex_type> lg(mutex);
        insert_front_impl(h);
        *h.get_ptr() = x;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::push_front(handle& h, T&& x) {
        lock_guard<mutex_type> lg(mutex);
        insert_front_impl(h);
        *h.get_ptr() = ob::move(x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を直接構築して追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class... Args>
    inline typename handle_list<T>::reference handle_list<T>::emplace_front(handle& h, Args&&... args) {
        lock_guard<mutex_type> lg(mutex);
        insert_front_impl(h);
        ob::construct_at(h.get_ptr(), ob::forward<Args>(args)...);
        return *h.get_ptr();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::push_back(handle& h, const T& x) {
        lock_guard<mutex_type> lg(mutex);
        insert_back_impl(h);
        *h.get_ptr() = x;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾に要素を追加する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::push_back(handle& h, T&& x) {
        lock_guard<mutex_type> lg(mutex);
        insert_back_impl(h);
        *h.get_ptr() = ob::move(x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  末尾に要素を直接構築して追加する
    //! 
    //! @return 追加した要素の参照
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class... Args>
    inline typename handle_list<T>::reference handle_list<T>::emplace_back(handle& h, Args&&... args) {
        lock_guard<mutex_type> lg(mutex);
        insert_back_impl(h);
        ob::construct_at(h.get_ptr(), ob::forward<Args>(args)...);
        return *h.get_ptr();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  要素を指定位置に挿入する
    //! 
    //! @return 挿入した要素のイテレータ
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::iterator handle_list<T>::insert(handle& h, const_iterator position, const T& x) {
        lock_guard<mutex_type> lg(mutex);
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
    inline typename handle_list<T>::iterator handle_list<T>::insert(handle& h, const_iterator position, T&& x) {
        lock_guard<mutex_type> lg(mutex);
        insert_impl(position.pHandle, h);
        *h.get_ptr()=x;
        return &h;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定位置に直接構築して挿入する
    //! 
    //! @return 挿入した要素のイテレータ
    //@―---------------------------------------------------------------------------
    template<typename T>
    template<class... Args>
    inline typename handle_list<T>::iterator handle_list<T>::emplace(handle& h, const_iterator position, Args&&... args) {
        lock_guard<mutex_type> lg(mutex);
        insert_impl(position.pHandle, h);
        ob::construct_at(h.get_ptr(), ob::forward<Args>(args)...);
        return &h;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭から要素を削除する
    //! 
    //! @details    コンテナが空の場合は何もしない。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::pop_front()noexcept {
        lock_guard<mutex_type> lg(mutex);
        if (m_header.pNext == &m_header)return;
        erase_impl(static_cast<handle*>(m_header.pNext));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾から要素を削除する
    //! 
    //! @details    コンテナが空の場合は何もしない。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::pop_back()noexcept {
        lock_guard<mutex_type> lg(mutex);
        if (m_header.pPrev == &m_header)return;
        erase_impl(static_cast<handle*>(m_header.pPrev));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 指定位置の要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::iterator handle_list<T>::erase(const_iterator position) {
        lock_guard<mutex_type> lg(mutex);
        return erase_impl(position.pHandle);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      指定範囲の要素を削除する
    //! 
    //! @details    [position, last)で示される範囲の要素が削除される。
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::iterator handle_list<T>::erase(const_iterator position, const_iterator last) {
        lock_guard<mutex_type> lg(mutex);
        auto itr = position;
        while (position != last) {
            auto pHandle = position.pHandle;
            ++position;
            itr=erase_impl(pHandle);
        }
        return itr.pHandle;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 全要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::clear()noexcept {
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
    //! @brief コンテナを交換する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::swap(this_type& x)noexcept {
        ob::swap(*this, x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を削除する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::remove(handle& h) {
        lock_guard<mutex_type> lg(mutex);
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
    inline void handle_list<T>::remove_if(Predicate pred) {
        lock_guard<mutex_type> lg(mutex);
        auto itr = begin();
        const auto itrEnd = end();
        while (itr != itrEnd) {
            if (pred(*itr)) {
                itr=erase_impl(itr.pHandle);
            } else {
                ++itr;
            }
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンテナを反転する
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::reverse()noexcept {
        lock_guard<mutex_type> lg(mutex);

        auto itr = begin();
        const auto itrEnd = end();
        while (itr != itrEnd) {
            auto pHandle = itr.pHandle;
            ++itr;
            ob::swap(pHandle->pPrev, pHandle->pNext);
        }
        ob::swap(m_header.pPrev, m_header.pNext);
    }


    //@―---------------------------------------------------------------------------
    //! @brief リストをクリア(remove呼び出しなし)
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::clear_impl()noexcept {
        m_header.pNext = &m_header;
        m_header.pPrev = &m_header;
        m_header.pParent = this;

    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブ共通処理
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::move_impl(this_type&& src)noexcept {
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
    void handle_list<T>::reset_parent()noexcept {
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
    inline void handle_list<T>::insert_impl(handle_base* pPrev, handle& h) {
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

            pPrev->pPrev->pNext= &h;
            pPrev->pPrev= &h;
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief 先頭に要素を追加
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::insert_front_impl(handle& h) {
        insert_impl(m_header.pNext, h);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 末尾に要素を追加
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void handle_list<T>::insert_back_impl(handle& h) {
        insert_impl(&m_header, h);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の削除
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline typename handle_list<T>::iterator handle_list<T>::erase_impl(handle_base* pHandle)noexcept {
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