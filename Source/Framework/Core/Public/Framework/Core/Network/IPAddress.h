//***********************************************************
//! @file
//! @brief  IPアドレス
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

	//! IPアドレス
	//! @details ```cpp
	//! IPAddress ip(192, 168, 0, 1);
	//! IPAddress ip(0xc0a80001);
	//! IPAddress ip("192.168.0.1");
	//! ```
	struct IPAddress {
		IPAddress() :a(0), b(0), c(0), d(0) {}
		IPAddress(u8 a, u8 b, u8 c, u8 d) :a(a), b(b), c(c), d(d) {}
		IPAddress(u32 ip) {
			a = (ip >> 24) & 0xff;
			b = (ip >> 16) & 0xff;
			c = (ip >> 8) & 0xff;
			d = ip & 0xff;
		}
		IPAddress(const char* ip) {
			if(sscanf(ip, "%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d)){}
		}

		static IPAddress LocalHost() { return IPAddress(127, 0, 0, 1); }

		u8 a;
		u8 b;
		u8 c;
		u8 d;
	};

}


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct std::formatter<ob::core::IPAddress, ob::core::Char> : std::formatter<ob::core::f32, ob::core::Char> {
	using base = std::formatter<ob::core::u8, ob::core::Char>;
	template<typename FormatContext>
	auto format(const ob::core::IPAddress& value, FormatContext& ctx)  const -> decltype(ctx.out()) {
		ctx.advance_to(format_to(ctx.out(), "("));
		ctx.advance_to(base::format(value.a, ctx));
		ctx.advance_to(format_to(ctx.out(), ","));
		ctx.advance_to(base::format(value.b, ctx));
		ctx.advance_to(format_to(ctx.out(), ","));
		ctx.advance_to(base::format(value.c, ctx));
		ctx.advance_to(format_to(ctx.out(), ","));
		ctx.advance_to(base::format(value.d, ctx));
		ctx.advance_to(format_to(ctx.out(), ")"));
		return ctx.out();
	}
};
//! @endcond