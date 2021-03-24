#pragma once

namespace Ubpa {
	template<typename Ret, typename... Args>
	Ret Visitor<Ret(const void*, Args...)>::Visit(std::size_t ID, const void* ptr, Args... args) const {
		assert("ERROR::Visitor::Visit: unregistered" && IsRegistered(ID));
		return callbacks.at(ID)(ptr, std::forward<Args>(args)...);
	}

	template<typename Ret, typename... Args>
	template<typename T>
	Ret Visitor<Ret(const void*, Args...)>::Visit(const T* ptr, Args... args) const {
		return Visit(details::CustomIDof(ptr), ptr, std::forward<Args>(args)...);
	}

	template<typename Ret, typename... Args>
	template<typename... Funcs>
	void Visitor<Ret(const void*, Args...)>::Register(Funcs&&... funcs) {
		(RegisterOne(std::forward<Funcs>(funcs)), ...);
	}

	template<typename Ret, typename... Args>
	template<typename Func>
	void Visitor<Ret(const void*, Args...)>::Register(std::size_t ID, Func&& func) {
		if constexpr (std::is_same_v<FuncTraits_Signature<Func>, Ret(const void*, Args...)>) {
			callbacks[ID] = std::forward<Func>(func);
		}
		else {
			using ArgList = FuncTraits_ArgList<Func>;
			using DerivedPointer = Front_t<ArgList>;
			using Derived = std::remove_pointer_t<DerivedPointer>;
			static_assert(std::is_const_v<Derived>, "<Derived> must be const");
			callbacks[ID] = [func = std::forward<Func>(func)](const void* p, Args... args) {
				return func(reinterpret_cast<const Derived*>(p), std::forward<Args>(args)...);
			};
		}
	}

	template<typename Ret, typename... Args>
	template<typename Func>
	void Visitor<Ret(const void*, Args...)>::RegisterOne(Func&& func) {
		using ArgList = FuncTraits_ArgList<Func>;
		using DerivedPointer = Front_t<ArgList>;
		static_assert(std::is_same_v<TypeList<DerivedPointer, Args...>, ArgList>,
			"arguments must be (const T*, Args...)");

		using Derived = std::remove_pointer_t<DerivedPointer>;

		static_assert(std::is_const_v<Derived>, "<Derived> must be const");
		using RawDerived = std::remove_const_t<Derived>;
		static_assert(!std::is_void_v<RawDerived>, "<Derived> must not be void");

		callbacks[details::CustomID<RawDerived>::get()] =
			[func = std::forward<Func>(func)](const void* p, Args... args) {
				return func(reinterpret_cast<const RawDerived*>(p), std::forward<Args>(args)...);
			};
	}

	template<typename Ret, typename... Args>
	bool Visitor<Ret(const void*, Args...)>::IsRegistered(std::size_t ID) const {
		return callbacks.contains(ID);
	}

	template<typename Ret, typename... Args>
	template<typename T>
	bool Visitor<Ret(const void*, Args...)>::IsRegistered() const {
		return IsRegistered(CustomID<T>());
	}

	template<typename Ret, typename... Args>
	bool Visitor<Ret(const void*, Args...)>::IsRegistered(const void* ptr) const {
		return IsRegistered(reinterpret_cast<std::size_t>(details::vtable(ptr)));
	}
}
