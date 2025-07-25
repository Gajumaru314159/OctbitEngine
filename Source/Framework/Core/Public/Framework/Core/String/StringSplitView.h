//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/String/StringView.h>

namespace ob::core {

    class StringSplitView {
    public:
        class Iterator {
        public:
            Iterator(StringView str, StringView delimiter, size_t start)
                : m_str(str), m_delimiter(delimiter), m_start(start), m_end(str.find(delimiter, start)) {}

            StringView operator*() const {
                return m_str.substr(m_start, m_end - m_start);
            }

            Iterator& operator++() {
                if (m_end == StringView::npos) {
                    m_start = m_end;
                    return *this;
                }
                m_start = m_end + m_delimiter.size();
                m_end = m_str.find(m_delimiter, m_start);
                return *this;
            }

            bool operator!=(const Iterator& other) const {
                return m_start != other.m_start;
            }
        private:
            StringView m_str;
            StringView m_delimiter;
            size_t m_start;
            size_t m_end;
        };

        StringSplitView(StringView str, StringView delimiter)
            : m_str(str), m_delimiter(delimiter) {}

        Iterator begin() const {
            return Iterator(m_str, m_delimiter, 0);
        }

        Iterator end() const {
            return Iterator(m_str, m_delimiter, StringView::npos);
        }
    private:
        StringView m_str;
        StringView m_delimiter;
    };

    class CommaSplitView : public StringSplitView {
    public:
        CommaSplitView(StringView str) : StringSplitView(str, ",") {}
    };

    class TabSplitView : public StringSplitView {
    public:
        TabSplitView(StringView str) : StringSplitView(str, "\t") {}
    };

    class LineBreakSplitView : public StringSplitView {
    public:
        LineBreakSplitView(StringView str) : StringSplitView(str, "\n") {}
    };

}