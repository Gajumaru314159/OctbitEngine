//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::engine {

    OB_EVENT_NOTIFIER(PropertyChanged, StringView);

    //@―---------------------------------------------------------------------------
    //! @brief  プロパティ変更通知
    //@―---------------------------------------------------------------------------
    class NotificationObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief          デストラクタ
        //@―---------------------------------------------------------------------------
        virtual ~NotificationObject() = default;

        //@―---------------------------------------------------------------------------
        //! @brief          プロパティ変更イベントを購読
        //@―---------------------------------------------------------------------------
        void subscribePropertyChanged(PropertyChangedHandle& handle, PropertyChangedDelegate func) {
            m_propertyChanged.add(handle, func);
        }

    protected:

        //@―---------------------------------------------------------------------------
        //! @brief          プロパティ通知の抑制設定
        //@―---------------------------------------------------------------------------
        void setNotificationSuppression(bool suppression) {
            m_notificationSuppression = suppression;
        }

        //@―---------------------------------------------------------------------------
        //! @brief          プロパティ通知の抑制取得
        //@―---------------------------------------------------------------------------
        bool getNotificationSuppression()const {
            return m_notificationSuppression;
        }

        //@―---------------------------------------------------------------------------
        //! @brief          プロパティ値変更
        //! @param name     プロパティ名
        //@―---------------------------------------------------------------------------
        void raisePropertyChanged(StringView name) {
            if (m_notificationSuppression==false) {
                m_propertyChanged.invoke(name);
            }
        }

        //@―---------------------------------------------------------------------------
        //! @brief          プロパティ値変更
        //! @details        プロパティに変更があったときのみプロパティの変更を通知します。
        //! @param target   変更対象
        //! @param value    値
        //! @param name     プロパティ名
        //@―---------------------------------------------------------------------------
        template<class T, class U>
        bool setProperty(T& target, const U& value, StringView name) {
            if (std::equal_to<U>()(target, value)) {
                return false;
            }
            target = value;
            raisePropertyChanged(name);
            return false;
        }

    private:

        PropertyChangedNotifier m_propertyChanged;
        bool m_notificationSuppression = true;

    };

}