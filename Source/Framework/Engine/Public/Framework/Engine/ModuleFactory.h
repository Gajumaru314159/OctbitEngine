//***********************************************************
//! @file
//! @brief		モジュール生成
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/IModule.h>

namespace ob::engine {

	//! @cond
	namespace internal {

		//@―---------------------------------------------------------------------------
		//! @brief  モジュール構築基底クラス
		//@―---------------------------------------------------------------------------
		class ModuleConstructorBase {
		public:
			virtual IModule* construct(Engine&)const = 0;
			virtual size_t type()const = 0;
			virtual StringView name()const = 0;
		};

		//@―---------------------------------------------------------------------------
		//! @brief  モジュール構築クラス
		//@―---------------------------------------------------------------------------
		template<class T,
			typename = std::enable_if_t<
			/**/std::is_base_of<IModule, T>::value&&		// IModuleの派生クラス
			/**/std::is_constructible<T, Engine&>::value	// 生成可能
			>
		>
		class ModuleConstructor :public ModuleConstructorBase {
		public:
			ModuleConstructor() {
				StringEncoder::Encode(typeid(T).name(), m_name);
			}
			IModule* construct(Engine& engine)const override {
				return new T(engine);
			}
			size_t type()const override {
				return typeid(T).hash_code();
			}
			StringView name()const override {
				return m_name;
			}
		private:
			String m_name;
		};


		//@―---------------------------------------------------------------------------
		//! @brief		モジュール管理クラス
		//@―---------------------------------------------------------------------------
		class ModuleFactory : public Singleton<ModuleFactory> {
		public:

			//@―---------------------------------------------------------------------------
			//! @brief		コンストラクタ
			//@―---------------------------------------------------------------------------
			ModuleFactory() = default;

			//@―---------------------------------------------------------------------------
			//! @brief				生成可能なモジュールを追加
			//! 
			//! @tparam TModule		生成されるモジュール型
			//! @tparam TBase		基底型
			//@―---------------------------------------------------------------------------
			template<class TModule, class TBase = TModule>
			void add();

			//@―---------------------------------------------------------------------------
			//! @brief				モジュール生成
			//! 
			//! @details			生成可能なモジュールが複数ある場合、指定した優先度順に生成を試みます。
			//@―---------------------------------------------------------------------------
			template<class TModule>
			std::pair<TModule*, size_t> create(Engine&);

		private:

			using TypeHash = size_t;
			HashMap<TypeHash, Array<UPtr<ModuleConstructorBase>>> m_constructorMap;

		};


		//@―---------------------------------------------------------------------------
		//! @brief				生成可能なモジュールを登録
		//! 
		//! @details            モジュールのソースファイルにこのクラスを宣言すると自動的にModuleManagerに登録されます。
		//! @tparam TModule		生成されるモジュール型
		//! @tparam TBase		基底型
		//@―---------------------------------------------------------------------------
		template<class TModule, class TBase = TModule>
		class ModuleRegister {
		public:

			//@―---------------------------------------------------------------------------
			//! @brief  コンストラクタ
			//@―---------------------------------------------------------------------------
			ModuleRegister() {
				ModuleFactory::Get().add<TModule, TBase>();
			}

		};






		//===============================================================
		// インライン関数
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief				生成可能なモジュールを追加
		//! 
		//! @tparam TModule		生成されるモジュール型
		//! @tparam TBase		基底型
		//@―---------------------------------------------------------------------------
		template<class TModule, class TBase>
		void ModuleFactory::add() {
			auto hash = typeid(TBase).hash_code();
			auto& constructors = m_constructorMap[hash];
			auto& constructor = constructors.emplace_back(std::make_unique<internal::ModuleConstructor<TModule>>());

			LOG_TRACE_EX("Module", "ModuleFactoryに[{}]を追加", constructor->name());
		}

		//@―---------------------------------------------------------------------------
		//! @brief				モジュール生成
		//! 
		//! @details			生成可能なモジュールが複数ある場合、指定した優先度順に生成を試みます。
		//@―---------------------------------------------------------------------------
		template<class TModule>
		std::pair<TModule*, size_t> ModuleFactory::create(Engine& engine) {

			auto hash = typeid(TModule).hash_code();

			// ファクトリが存在するか
			auto& constructors = m_constructorMap[hash];
			if (constructors.empty()) {
				return false;
			}

			// 生成
			for (auto& constructor : constructors) {
				if (auto pModule = constructor->construct(engine)) {
					if (pModule->isValid() == false) {
						delete tmp;
					} else {
						LOG_TRACE_EX("Module", "Module[{}]を生成", constructor->name());
						return { pModule ,constructor->type() };
					}
				}
			}

			return { nullptr ,0 };
		}


	}

#define _internal_CAT2(a,b) a##b
#define _internal_CAT(a,b) _internal_CAT2(a,b)
#define _internal_REGISTER_MODULE_BASE(type,base,line) static ob::engine::internal::ModuleRegister<type,base> _internal_CAT(s_module,__LINE__)
	//! @endcond

}// namespcae ob


//@―---------------------------------------------------------------------------
//! @brief  モジュールを登録
//@―---------------------------------------------------------------------------
#define REGISTER_MODULE(type)               _internal_REGISTER_MODULE_BASE(type,type,__LINE__)

//@―---------------------------------------------------------------------------
//! @brief  モジュールを登録(基底クラス指定)
//@―---------------------------------------------------------------------------
#define REGISTER_MODULE_DERIVE(type,base)   _internal_REGISTER_MODULE_BASE(type,base,__LINE__)