//***********************************************************
//! @file
//! @brief		時間間隔
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Misc/DateTime.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief		時間間隔
	//! 
	//! @details	長期の期間を扱うのには適していません。
	//!				内部表現に64bit整数を使用しているため±239年を超える場合は別な表現を検討してください。
	//!				経過年数を取得する場合は閏年を考慮する必要があるため独自に実装する必要があります。
	//@―---------------------------------------------------------------------------
	class Duration {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================
		Duration() = default;										//!< デフォルトコンストラクタ
		Duration(s64 ticks) noexcept;								//!< コンストラクタ(Tick初期化)
		Duration(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliSeconds) noexcept;	//!< コンストラクタ
		Duration(const DateTime& from, const DateTime& to) noexcept;	//!< コンストラクタ


		//===============================================================
		// オペレータ
		//===============================================================
		Duration operator-(const Duration& rhs)const noexcept;		//!< 加算演算子
		Duration operator+(const Duration& rhs)const noexcept;		//!< 減算演算子
		Duration operator*(f64 scalar)const noexcept;				//!< 乗算演算子
		Duration operator/(f64 scalar)const noexcept;				//!< 除算演算子
		Duration operator%(const Duration& rhs)const noexcept;		//!< Mod演算子
		Duration& operator+=(const Duration& rhs) noexcept;			//!< 加算代入演算子
		Duration& operator-=(const Duration& rhs) noexcept;			//!< 減算代入演算子
		Duration& operator*=(f64 scalar) noexcept;					//!< 乗算代入演算子
		Duration& operator/=(f64 scalar) noexcept;					//!< 除算代入演算子
		Duration& operator%=(const Duration& rhs) noexcept;			//!< Mod代入演算子

		Duration operator-()const noexcept;							//!< 単項マイナス演算子

		//===============================================================
		// 比較
		//===============================================================
		bool operator==(const Duration& rhs)const noexcept;			//!< 等価演算子
		bool operator!=(const Duration& rhs)const noexcept;			//!< 否等価演算子
		bool operator<(const Duration& rhs)const noexcept;			//!< 比較演算子
		bool operator<=(const Duration& rhs)const noexcept;			//!< 比較演算子
		bool operator>(const Duration& rhs)const noexcept;			//!< 比較演算子
		bool operator>=(const Duration& rhs)const noexcept;			//!< 比較演算子


		//===============================================================
		// 取得
		//===============================================================
		bool isZero()const noexcept;			//!< 間隔がゼロか
		Duration abs()const noexcept;			//!< 絶対値を取得

		s32 days()const noexcept;				//!< 経過時間の日数部分を取得
		s32 hours()const noexcept;				//!< 経過時間の時間部分を取得
		s32 minutes()const noexcept;			//!< 経過時間の分部分を取得
		s32 seconds()const noexcept;			//!< 経過時間の秒部分を取得

		f32 daysF()const noexcept;				//!< 経過時間の日数部分を取得(小数込み)
		f32 hoursF()const noexcept;				//!< 経過時間の時間部分を取得(小数込み)
		f32 minutesF()const noexcept;			//!< 経過時間の分部分を取得(小数込み)
		f32 secondsF()const noexcept;			//!< 経過時間の秒部分を取得(小数込み)

		s32 totalDays()const noexcept;			//!< 累計経過日数を取得
		s32 totalHours()const noexcept;			//!< 累計経過時間を取得
		s32 totalMinutes()const noexcept;		//!< 累計経過分を取得
		s32 totalSeconds()const noexcept;		//!< 累計経過秒を取得

		f32 totalDaysF()const noexcept;			//!< 累計経過日数を取得(小数込み)
		f32 totalHoursF()const noexcept;		//!< 累計経過時間を取得(小数込み)
		f32 totalMinutesF()const noexcept;		//!< 累計経過分を取得(小数込み)
		f32 totalSecondsF()const noexcept;		//!< 累計経過秒を取得(小数込み)

		s64 totalTicks()const noexcept;			//!< 累計経過Thick数を取得


		//===============================================================
		// 名前付きコンストラクタ
		//===============================================================
		static Duration Days(f64 days) noexcept;				//!< 日数から Duration を構築
		static Duration Hours(f64 hour) noexcept;				//!< 時間から Duration を構築 
		static Duration Minutes(f64 minutes) noexcept;			//!< 分から Duration を構築
		static Duration Seconds(f64 seconds) noexcept;			//!< 秒から Duration を構築
		static Duration MilliSeconds(f64 seconds) noexcept;		//!< ミリ秒から Duration を構築
		static Duration NanoSeconds(f64 nanoSeconds) noexcept;	//!< ナノ秒から Duration を構築
		static Duration MicroSeconds(f64 microSeconds) noexcept;//!< マイクロ秒から Duration を構築

		static Duration Max()noexcept;	//!< 2^63
		static Duration Min()noexcept;  //!< -(2^63)-1


		//===============================================================
		// static関数
		//===============================================================
		static Duration FromSystemLaunch();
		static Duration FromAppLaunch();


		//===============================================================
		// 定数
		//===============================================================
		static const s64 TicksPerMicroSeconds = 1;							//!< 1秒あたりのTick数
		static const s64 TicksPerNanoSeconds = TicksPerMicroSeconds * 1000;	//!< 1秒あたりのTick数
		static const s64 TicksPerMilliSeconds = TicksPerNanoSeconds * 1000;	//!< 1秒あたりのTick数
		static const s64 TicksPerSeconds = TicksPerMilliSeconds * 100;		//!< 1秒あたりのTick数
		static const s64 TicksPerMinutes = TicksPerSeconds * 60;			//!< 1分あたりのTick数 
		static const s64 TicksPerHours = TicksPerMinutes * 60;				//!< 1時間あたりのTick数
		static const s64 TicksPerDays = TicksPerHours * 24;					//!< 1時間あたりのTick数

		//===============================================================
		// ユーティリティ
		//===============================================================
		static double Ratio(const Duration& dividend, const Duration& divisor) noexcept;	//!< 時間の比率を計算

	private:

		s64 m_ticks;	//!< Tick数(内部表現)

	};








	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ(Thick数から構築)
	//@―---------------------------------------------------------------------------
	inline Duration::Duration(s64 ticks) noexcept
		:m_ticks(ticks)
	{
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	inline Duration::Duration(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliSeconds) noexcept
		: m_ticks(0)
	{
		m_ticks += days * TicksPerDays;
		m_ticks += hours * TicksPerHours;
		m_ticks += minutes * TicksPerMinutes;
		m_ticks += seconds * TicksPerSeconds;
		m_ticks += milliSeconds * TicksPerSeconds / 1000;
	}


	//===============================================================
	// オペレータ
	//===============================================================
	inline Duration Duration::operator+(const Duration& rhs)const noexcept {
		return Duration(m_ticks) += rhs;
	}
	inline Duration Duration::operator-(const Duration& rhs)const noexcept {
		return Duration(m_ticks) -= rhs;
	}
	inline Duration Duration::operator*(f64 scalar)const noexcept {
		return Duration(m_ticks) *= scalar;
	}
	inline Duration Duration::operator/(f64 scalar)const noexcept {
		return Duration(m_ticks) /= scalar;
	}
	inline Duration Duration::operator%(const Duration& rhs)const noexcept {
		return Duration(m_ticks) %= rhs;
	}
	inline Duration& Duration::operator+=(const Duration& rhs) noexcept {
		m_ticks += rhs.m_ticks;
		return *this;
	}
	inline Duration& Duration::operator-=(const Duration& rhs) noexcept {
		m_ticks += rhs.m_ticks;
		return *this;
	}
	inline Duration& Duration::operator*=(f64 scalar) noexcept {
		m_ticks = (s64)(m_ticks * scalar);
		return *this;
	}
	inline Duration& Duration::operator/=(f64 scalar) noexcept {
		m_ticks = (s64)(m_ticks / scalar);
		return *this;
	}
	inline Duration& Duration::operator%=(const Duration& rhs) noexcept {
		m_ticks %= rhs.m_ticks;
		return *this;
	}
	inline Duration Duration::operator-()const noexcept {
		return Duration(-m_ticks);
	}


	//===============================================================
	// 比較
	//===============================================================
	inline bool Duration::operator==(const Duration& rhs)const noexcept {
		return m_ticks == rhs.m_ticks;
	}
	inline bool Duration::operator!=(const Duration& rhs)const noexcept {
		return m_ticks != rhs.m_ticks;
	}
	inline bool Duration::operator<(const Duration& rhs)const noexcept {
		return m_ticks < rhs.m_ticks;
	}
	inline bool Duration::operator<=(const Duration& rhs)const noexcept {
		return m_ticks <= rhs.m_ticks;
	}
	inline bool Duration::operator>(const Duration& rhs)const noexcept {
		return m_ticks > rhs.m_ticks;
	}
	inline bool Duration::operator>=(const Duration& rhs)const noexcept {
		return m_ticks >= rhs.m_ticks;
	}


	//===============================================================
	// 取得
	//===============================================================
	inline bool Duration::isZero()const noexcept {
		return m_ticks == 0;
	}
	inline Duration Duration::abs()const noexcept {
		return Duration(0 < m_ticks ? m_ticks : -m_ticks);
	}


	// 経過時間取得
	inline s32 Duration::days()const noexcept {
		return totalDays();
	}
	inline s32 Duration::hours()const noexcept {
		return Duration(m_ticks % TicksPerDays).totalHours();
	}
	inline s32 Duration::minutes()const noexcept {
		return Duration(m_ticks % TicksPerHours).totalMinutes();
	}
	inline s32 Duration::seconds()const noexcept {
		return Duration(m_ticks % TicksPerMinutes).totalSeconds();
	}

	// 経過時間取得(小数込み)
	inline f32 Duration::daysF()const noexcept {
		return totalDaysF();
	}
	inline f32 Duration::hoursF()const noexcept {
		return Duration(m_ticks % TicksPerDays).totalHoursF();
	}
	inline f32 Duration::minutesF()const noexcept {
		return Duration(m_ticks % TicksPerHours).totalMinutesF();
	}
	inline f32 Duration::secondsF()const noexcept {
		return Duration(m_ticks % TicksPerMinutes).totalSecondsF();
	}

	// 累計経過時間取得
	inline s32 Duration::totalDays()const noexcept {
		return (s32)(m_ticks / TicksPerDays);
	}
	inline s32 Duration::totalHours()const noexcept {
		return (s32)(m_ticks / TicksPerHours);
	}
	inline s32 Duration::totalMinutes()const noexcept {
		return (s32)(m_ticks / TicksPerMinutes);
	}
	inline s32 Duration::totalSeconds()const noexcept {
		return (s32)(m_ticks / TicksPerSeconds);
	}

	// 累計経過時間取得(小数込み)
	inline f32 Duration::totalDaysF()const noexcept {
		return (f32)((f64)m_ticks / TicksPerDays);
	}
	inline f32 Duration::totalHoursF()const noexcept {
		return (f32)((f64)m_ticks / TicksPerHours);
	}
	inline f32 Duration::totalMinutesF()const noexcept {
		return (f32)((f64)m_ticks / TicksPerMinutes);
	}
	inline f32 Duration::totalSecondsF()const noexcept {
		return (f32)((f64)m_ticks / TicksPerSeconds);
	}

	// Tick数取得
	inline s64 Duration::totalTicks()const noexcept {
		return m_ticks;
	}


	//===============================================================
	// 名前付きコンストラクタ
	//===============================================================
	inline Duration Duration::Days(f64 days) noexcept {
		return Duration((s64)(days * TicksPerDays));
	}
	inline Duration Duration::Hours(f64 hour) noexcept {
		return Duration((s64)(hour * TicksPerHours));
	}
	inline Duration Duration::Minutes(f64 minutes) noexcept {
		return Duration((s64)(minutes * TicksPerMinutes));
	}
	inline Duration Duration::Seconds(f64 seconds) noexcept {
		return Duration((s64)(seconds * TicksPerSeconds));
	}
	inline Duration Duration::MilliSeconds(f64 milliSeconds) noexcept {
		return Duration((s64)(milliSeconds * TicksPerMilliSeconds));
	}
	inline Duration Duration::NanoSeconds(f64 nanoSeconds) noexcept {
		return Duration((s64)(nanoSeconds * TicksPerNanoSeconds));
	}
	inline Duration Duration::MicroSeconds(f64 microSeconds) noexcept {
		return Duration((s64)(microSeconds * TicksPerMicroSeconds));
	}

	inline Duration Duration::Max()noexcept {
		return Duration((std::numeric_limits<decltype(m_ticks)>::max)());
	}
	inline Duration Duration::Min()noexcept {
		return Duration((std::numeric_limits<decltype(m_ticks)>::min)());
	}


	//===============================================================
	// ユーティリティ
	//===============================================================
	inline double Duration::Ratio(const Duration& dividend, const Duration& divisor) noexcept {
		return (f64)dividend.m_ticks / divisor.m_ticks;
	}

	//! @endcond
}// namespcae ob::core


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::Duration, ob::core::Char> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(const ob::core::Duration& value, FormatContext& ctx) -> decltype(ctx.out()) {
		if (value.days())return format_to(ctx.out(), TC("{}d{:0>2}h{:0>2}m"), value.days(), value.hours(), value.minutes());
		if (value.hours())return format_to(ctx.out(), TC("{:0>2}h{:0>2}m{:0>2}s"), value.hours(), value.minutes(), value.seconds());
		if (value.minutes())return format_to(ctx.out(), TC("{:0>2}m{:.5}s"), value.minutes(), value.secondsF());
		if (value.seconds())return format_to(ctx.out(), TC("{:.5}s"), value.secondsF());
		return format_to(ctx.out(), TC("{}ticks"), value.totalTicks());
	}
};
//! @endcond