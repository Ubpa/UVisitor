#pragma once

namespace Ubpa {
	template<typename Impl, typename Ret, typename... Args>
	Ret Visitor<Ret(Impl::*)(void*, Args...)const>::Visit(std::size_t ID, void* ptr, Args... args) const {
		assert("ERROR::Visitor::Visit: unregistered" && IsRegistered(ID));
		auto target = impl_callbacks.find(ID);
		if (target != impl_callbacks.end())
			return target->second(details::Accessor<Impl>::template cast(this), ptr, std::forward<Args>(args)...);
		else
			return this->callbacks.at(ID)(ptr, std::forward<Args>(args)...);
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename T>
	Ret Visitor<Ret(Impl::*)(void*, Args...)const>::Visit(T* ptr, Args... args) const {
		return Visit(details::CustomIDof(ptr), ptr, std::forward<Args>(args)...);
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename... Deriveds>
	void Visitor<Ret(Impl::*)(void*, Args...)const>::Register() {
		(RegisterOne<Deriveds>(), ...);
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename Derived>
	void Visitor<Ret(Impl::*)(void*, Args...)const>::RegisterOne() {
		impl_callbacks[details::CustomID<Derived>::get()] =
			[](const Impl* impl, void* ptr, Args... args) {
				return details::Accessor<Impl>::
					template run<Derived*, Ret>(impl, ptr, std::forward<Args>(args)...);
			};
	}

	template<typename Impl, typename Ret, typename... Args>
	bool Visitor<Ret(Impl::*)(void*, Args...)const>::IsRegistered(std::size_t ID) const {
		return impl_callbacks.contains(ID) || this->callbacks.contains(ID);
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename T>
	bool Visitor<Ret(Impl::*)(void*, Args...)const>::IsRegistered() const {
		return IsRegistered(CustomID<T>());
	}

	template<typename Impl, typename Ret, typename... Args>
	bool Visitor<Ret(Impl::*)(void*, Args...)const>::IsRegistered(const void* ptr) const {
		return IsRegistered(reinterpret_cast<std::size_t>(details::vtable(ptr)));
	}
}
