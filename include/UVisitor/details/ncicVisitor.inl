#pragma once

namespace Ubpa {
	template<typename Impl, typename Ret, typename... Args>
	Ret Visitor<Ret(Impl::*)(const void*, Args...)>::Visit(std::size_t ID, const void* ptr, Args... args) {
		assert("ERROR::Visitor::Visit: unregistered" && IsRegistered(ID));
		auto target = impl_callbacks.find(ID);
		if (target != impl_callbacks.end())
			return target->second(details::Accessor<Impl>::template cast(this), ptr, std::forward<Args>(args)...);
		else
			return this->callbacks.at(ID)(ptr, std::forward<Args>(args)...);
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename T>
	Ret Visitor<Ret(Impl::*)(const void*, Args...)>::Visit(const T* ptr, Args... args) {
		return Visit(details::CustomIDof(ptr), ptr, std::forward<Args>(args)...);
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename... Deriveds>
	void Visitor<Ret(Impl::*)(const void*, Args...)>::Register() {
		(RegisterOne<Deriveds>(), ...);
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename Derived>
	void Visitor<Ret(Impl::*)(const void*, Args...)>::RegisterOne() {
		impl_callbacks[details::CustomID<Derived>::get()] =
			[](Impl* impl, const void* ptr, Args... args) {
				return details::Accessor<Impl>::
					template run<const Derived*, Ret>(impl, ptr, std::forward<Args>(args)...);
			};
	}

	template<typename Impl, typename Ret, typename... Args>
	bool Visitor<Ret(Impl::*)(const void*, Args...)>::IsRegistered(std::size_t ID) const {
		return impl_callbacks.find(ID) != impl_callbacks.end()
			|| this->callbacks.find(ID) != this->callbacks.end();
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename T>
	bool Visitor<Ret(Impl::*)(const void*, Args...)>::IsRegistered() const {
		return IsRegistered(CustomID<T>());
	}

	template<typename Impl, typename Ret, typename... Args>
	bool Visitor<Ret(Impl::*)(const void*, Args...)>::IsRegistered(const void* ptr) const {
		return IsRegistered(static_cast<std::size_t>(details::vtable(ptr)));
	}
}
