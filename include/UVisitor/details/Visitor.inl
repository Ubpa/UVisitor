#include "details/vtable.hpp"

#include "details/CustomID.hpp"

#include <UTemplate/Func.hpp>

#include <unordered_map>
#include <functional>

namespace Ubpa::details {
	template<typename Impl>
	struct Accessor : std::conditional_t<std::is_final_v<Impl>, std::type_identity<void>, Impl> {
		template<typename DerivedPtr, typename Ret, typename... Args>
		static Ret run(Impl* impl, const void* obj, Args... args) {
			// impl->ImplVisit
			return (impl->*MemFuncOf<Impl, Ret(DerivedPtr, Args...)>::template get(&Impl::ImplVisit))(
				reinterpret_cast<DerivedPtr>(const_cast<void*>(obj)),
				std::forward<Args>(args)...);
		}

		template<typename DerivedPtr, typename Ret, typename... Args>
		static Ret run(const Impl* impl, const void* obj, Args... args) {
			// impl->ImplVisit
			return (impl->*MemFuncOf<Impl, Ret(DerivedPtr, Args...)const>::template get(&Impl::ImplVisit))(
				reinterpret_cast<DerivedPtr>(const_cast<void*>(obj)),
				std::forward<Args>(args)...);
		}

		template<typename T>
		static Impl* cast(T* ptr) noexcept {
			return static_cast<Impl*>(ptr);
		}

		template<typename T>
		static const Impl* cast(const T* ptr) noexcept {
			return static_cast<const Impl*>(ptr);
		}
	};
}
