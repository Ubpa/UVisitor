#pragma once

#include "vtable.hpp"

#include <UTemplate/Type.hpp>

namespace Ubpa::details {
	template<typename T>
	struct CustomID {
		// return custom ID of T
		// if T is polymorphic, then return it's virtual table
		// * it will create a object of T, and get the virtual table from the object
		// * if T is not default constructable, you should call vtable_of<T>::regist(ptr) firstly
		// * or call get(ptr)
		// else return TypeID<T>
		static constexpr std::size_t get() noexcept {
			static_assert(!std::is_const_v<T> && !std::is_pointer_v<T>,
				"GetID: <T> must be non-const and non-pointer");
			if constexpr (std::is_polymorphic_v<T>)
				return reinterpret_cast<std::size_t>(vtable_of<T>::get());
			else
				return TypeID_of<T>;
		}

		// return custom ID of T
		// if T is polymorphic, then return it's virtual table getting from ptr
		// else return TypeID<T>
		static constexpr std::size_t get(const T* ptr) noexcept {
			assert(ptr != nullptr);
			if constexpr (std::is_void_v<T> || std::is_polymorphic_v<T>)
				return reinterpret_cast<std::size_t>(vtable(ptr));
			else
				return TypeID_of<T>;
		}
	};

	template<typename T>
	constexpr std::size_t CustomIDof(const T* ptr) noexcept {
		return CustomID<T>::get(ptr);
	}
}
