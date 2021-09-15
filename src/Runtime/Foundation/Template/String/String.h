//***********************************************************
//! @file
//! @brief		文字列
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>

#include <string>
#include <string_view>

namespace ob
{
    using String = std::basic_string<Char>;
    using StringView = std::basic_string_view<Char>;

    /*
    class String;



    //-----------------------------------------------------------------------------
    //! @brief 文字列比較のモード
    //-----------------------------------------------------------------------------
    enum class CharCompareMode {
        Strict,     //!< 厳密比較
        Case,       //!< 大文字/小文字を同一判定
    };


    //-----------------------------------------------------------------------------
    //! @brief 読み取り専用の文字列型
    //-----------------------------------------------------------------------------
    class OB_API StringView {
    public:

        StringView() noexcept;
        StringView(const StringView& str) noexcept;
        StringView(const StringView& str, int len);
        StringView(const StringView& str, int startIndex, int len);
        explicit StringView(const Char* str);
        explicit StringView(const Char* str, int len);
        explicit StringView(const Char* begin, const Char* end);

        ~StringView() { clear(); }

        constexpr const Char& operator[](int index) const { return *(data() + index); }

        StringView& operator=(const StringView& str);
        StringView& operator=(StringView&& str);

        // TODO 比較演算子


        bool Equals(const StringView& other, CharCompareMode cs = CharCompareMode::Strict)const;

        bool StartsWith(const StringView& str, CharCompareMode cs = CharCompareMode::Strict) const;
        bool EndsWith(const StringView& str, CharCompareMode cs = CharCompareMode::Strict) const;
        bool Contains(const StringView& str, CharCompareMode cs = CharCompareMode::Strict)const;

        //Iterator Find(const StringView& str, CharCompareMode cs = CharCompareMode::Strict);
        //Iterator RFind(const StringView& str, CharCompareMode cs = CharCompareMode::Strict);


        constexpr int Length() const noexcept { return m_len; }
        constexpr bool IsEmpty() const noexcept { return (m_str == nullptr || Length() <= 0); }
        constexpr const Char* Data() const noexcept { return m_str; }
        StringView SubString(int start, int count) const;

        void RemovePrefix(size_t count);
        void RemoveSufix(size_t count);
        void Clear();

        void Trim()noexcept;
        void TrimStart()noexcept;
        void TrimEnd()noexcept;



    private:
        size_t GetHashCode() const;
        void clear();

    protected:
        const Char* m_begin;
        const Char* m_end;
    };


    class OB_API String:public StringView{
    public:
        using value_type = Char;
    public:

        String();
        ~String();
        String(const String& str);
        String(String&& str) noexcept;
        String& operator=(const String& str);
        String& operator=(String&& str) noexcept;

        String(const String& str, int begin);
        String(const String& str, int begin, int length);
        String(const Char* str);
        String(const Char* str, int length);
        String(const Char* begin, const Char* end);
        String(int count, Char ch);
        String(const StringView& str);

        const Char* c_str() const noexcept;


        bool IsEmpty() const noexcept;
        s32 Length() const noexcept;
        int Capacity() const noexcept;
        void Clear();
        void Resize(int newLength);
        void Resize(int newLength, Char ch);
        void Reserve(int size);
        void Assign(const Char* str);
        void Assign(const Char* str, int length);
        void Assign(int count, Char ch);         
        void Assign(const StringView& str);      
        void Append(const Char* str, int length);
        void Append(const String& str); 
        bool Contains(const StringView& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
        bool Contains(Char ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const; 
        int indexOf(const StringView& str, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
        int indexOf(Char ch, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

        int lastIndexOf(const StringView& str, int startIndex = -1, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
        int lastIndexOf(Char ch, int startIndex = -1, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

        bool startsWith(const StringView& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
        bool startsWith(Char ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

        bool endsWith(const StringView& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
        bool endsWith(Char ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

        StringView substr(int start, int count = -1) const;

        StringView left(int count) const;

        StringView right(int count) const;

        String trim() const;

        String toUpper() const;

        String toLower() const;

        String toTitleCase() const;

        String remove(const StringView& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
        String remove(Char ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

        String replace(const StringView& from, const StringView& to, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

        String insert(int startIndex, const StringView& value) const;

        List<String> split(const StringView& delim, StringSplitOptions option = StringSplitOptions::None) const;

        int toInt(int base = 0) const;
        int8_t toInt8(int base = 0) const;
        int16_t toInt16(int base = 0) const;   
        int32_t toInt32(int base = 0) const;   
        int64_t toInt64(int base = 0) const;   
        uint8_t toUInt8(int base = 0) const;   
        uint16_t toUInt16(int base = 0) const; 
        uint32_t toUInt32(int base = 0) const; 
        uint64_t toUInt64(int base = 0) const; 
        bool tryToInt(int* outValue, int base = 0) const;
        bool tryToInt8(int8_t* outValue, int base = 0) const;
        bool tryToInt16(int16_t* outValue, int base = 0) const;   
        bool tryToInt32(int32_t* outValue, int base = 0) const;   
        bool tryToInt64(int64_t* outValue, int base = 0) const;   
        bool tryToUInt8(uint8_t* outValue, int base = 0) const;   
        bool tryToUInt16(uint16_t* outValue, int base = 0) const; 
        bool tryToUInt32(uint32_t* outValue, int base = 0) const; 
        bool tryToUInt64(uint64_t* outValue, int base = 0) const; 

        std::string toStdString() const;

        std::wstring toStdWString() const;

        static String concat(const StringView& str1, const StringView& str2);
        static String concat(const StringView& str1, const StringView& str2, const StringView& str3);                       
        static String concat(const StringView& str1, const StringView& str2, const StringView& str3, const StringView& str4); 

        static String join(const List<String>& list, const StringView& delim);

        template<typename... TArgs>
        static String format(const StringView& format, TArgs&&... args);
        template<typename... TArgs>
        static String format(const Locale& locale, const StringView& format, TArgs&&... args);

        static int compare(const String& str1, const String& str2, CaseSensitivity cs = CaseSensitivity::CaseSensitive);
        static int compare(const StringView& str1, int index1, const StringView& str2, int index2, int length = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive);

        static String fromCString(const char* str, int length = -1, TextEncoding* encoding = nullptr);

        static String fromCString(const wchar_t* str, int length = -1);

        static String fromStdString(const std::string& str, TextEncoding* encoding = nullptr);

        static String fromStdString(const std::wstring& str);

        static String fromNumber(int32_t value, Char format = 'D');
        static String fromNumber(int64_t value, Char format = 'D');                   
        static String fromNumber(uint32_t value, Char format = 'D');                  
        static String fromNumber(uint64_t value, Char format = 'D');                  
        static String fromNumber(float value, Char format = 'F', int precision = 6);  
        static String fromNumber(double value, Char format = 'F', int precision = 6); 

        CharRef operator[](int index);

        const Char& operator[](int index) const noexcept;

        String& operator=(const StringView& rhs);
        String& operator=(const Char* rhs);
        String& operator=(Char ch);
        String& operator=(const Path& rhs);

        String& operator+=(const String& rhs);
        String& operator+=(const StringView& rhs);
        String& operator+=(const Char* rhs);
        String& operator+=(Char rhs);

#ifdef LN_STRING_FUZZY_CONVERSION
        String& operator=(const char* rhs);
        String& operator+=(const char* rhs);
#endif

        static const String& newLine();

    private:
        static std::size_t const SSOCapacity = 15;

        // resource management
        void init() noexcept;
        void release() noexcept;
        void copy(const String& str);
        void move(String&& str) noexcept;
        Char* lockBuffer(int requestSize, detail::StringLockContext* context);
        void unlockBuffer(int confirmedSize, detail::StringLockContext* context);
        Char* getBuffer();
        const Char* getBuffer() const noexcept;

        // sso operation
        void setSSOLength(int len);
        int getSSOLength() const noexcept;
        void setSSO();
        void setNonSSO();
        bool isSSO() const noexcept { return !detail::getLSB<0>(static_cast<uint8_t>(m_data.sso.length)); }
        bool isNonSSO() const noexcept { return detail::getLSB<0>(static_cast<uint8_t>(m_data.sso.length)); }

        // utils
        void assignFromCStr(const char* str, int length = -1, bool* outUsedDefaultChar = nullptr, TextEncoding* encoding = nullptr);
        void assignFromCStr(const wchar_t* str, int length = -1, bool* outUsedDefaultChar = nullptr, TextEncoding* encoding = nullptr);
        void setAt(int index, Char ch);
        int getByteCount() const { return length() * sizeof(Char); }
        uint32_t getHashCode() const;

        //static ByteBuffer convertTo(const String& str, const TextEncoding* encoding, bool* outUsedDefaultChar = nullptr);

        union Data     {
            detail::UStringCore* core;

            struct SSO         {
                Char buffer[SSOCapacity];
                Char length; // ---xxxxy    : x=size y:flag(0=sso,1=non sso)
            } sso;
        } m_data;

        friend class CharRef;


        static const String Empty;
    };
    */


}// namespcae ob