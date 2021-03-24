#pragma once

#include <type_traits>
#include <cassert>

namespace Ubpa::details {
	static const void* vtable(const void* ptr) noexcept {
		assert(ptr != nullptr);
		return *reinterpret_cast<const void* const*>(ptr);
	}
	static const void* vtable(void* ptr) noexcept {
		return vtable(static_cast<const void*>(ptr));
	}

	template<typename T>
	static const void* vtable(T* ptr) noexcept {
		static_assert(std::is_polymorphic_v<T>, "T must be polymorphic");
		return vtable(reinterpret_cast<const void*>(ptr));
	}

	template<typename T>
	struct vtable_of {
		static_assert(std::is_polymorphic_v<T>);

		static void regist(const T* ptr) noexcept { value = vtable(ptr); }

		static const void* get() noexcept(std::is_nothrow_default_constructible_v<T>) {
			if (!value) {
				if constexpr (std::is_default_constructible_v<T>) {
					T tmp;
					regist(&tmp);
				}
				else
					assert("vtable register fail. You should register the vtable by vtable_of<T>::regist");
			}
			return value;
		}
		
	private:
		inline static const void* value{ nullptr };
	};

	template<typename Derived, typename Base>
	static bool vtable_is(Base* ptr) noexcept {
		static_assert(std::is_polymorphic_v<Base>);
		static_assert(std::is_polymorphic_v<Derived>);
		static_assert(std::is_base_of_v<Base, Derived>);
		assert(ptr != nullptr);
		return vtable(ptr) == vtable_of<Derived>::get();
	}
}
