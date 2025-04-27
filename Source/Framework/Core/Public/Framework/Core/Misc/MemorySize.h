//***********************************************************
//! @file
//! @brief		バイナリデータ(Binary Large Object)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/Template/include.h>

namespace ob::core {

	struct MemorySize {

		size_t size;

		constexpr MemorySize() : size(0) {}
		constexpr MemorySize(size_t size) : size(size) {}
		constexpr operator size_t() const { return size; }
		constexpr size_t KiB() const { return size >> 10; }
		constexpr size_t MiB() const { return size >> 20; }
		constexpr size_t GiB() const { return size >> 30; }
		constexpr size_t TiB() const { return size >> 40; }
		constexpr f64 KiBF() const { return 1.0 * size / (1ull << 10); }
		constexpr f64 MiBF() const { return 1.0 * size / (1ull << 20); }
		constexpr f64 GiBF() const { return 1.0 * size / (1ull << 30); }
		constexpr f64 TiBF() const { return 1.0 * size / (1ull << 40); }
		constexpr size_t KB() const { return size / 1000ull; }
		constexpr size_t MB() const { return size / 1000000ull; }
		constexpr size_t GB() const { return size / 1000000000ull; }
		constexpr size_t TB() const { return size / 1000000000000ull; }
		constexpr f64 KBF() const { return size / 1000.0; }
		constexpr f64 MBF() const { return size / 1000000.0; }
		constexpr f64 GBF() const { return size / 1000000000.0; }
		constexpr f64 TBF() const { return size / 1000000000000.0; }

	};

}