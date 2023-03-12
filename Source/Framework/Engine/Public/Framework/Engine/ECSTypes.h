//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::engine {

	enum class EntityFlag {
		Active,
		Visible,
	};
	using EntityFlags = BitFlags<EntityFlag>;

	enum class StaticFlag {
		GI,
		Occluder,
		Occludee,
		Batching,
		Navigation,
		ReflectionProbe,
	};
	using StaticFlags = BitFlags<EntityFlag>;



	class LayerMask {
	public:
		using value_type = BitSet<32>;
	public:
		static LayerMask FromName(StringView name);
	public:
		LayerMask() = default;
		LayerMask(s32 index) { 
			set(index,true);
		}

		void set(s32 index,bool value) {
			if (is_in_range(index, m_value)) {
				m_value.set(index, value);
			}
		}
		void reset() {
			m_value.reset();
		}
		bool operator[](size_t index)const {
			if (is_in_range(index, m_value)) {
				return m_value[index];
			}
			return false;
		}
		size_t size()const {
			return m_value.size();
		}

		bool contains(const LayerMask& rhs) {
			return (m_value & rhs.m_value).any();
		}

	private:
		value_type m_value;
	};



	using TagSet = Set<String, std::less<>>;

}// namespcae ob
