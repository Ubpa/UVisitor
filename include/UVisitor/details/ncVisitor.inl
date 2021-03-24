#pragma once

namespace Ubpa {
	template<typename Ret, typename... Args>
	Ret Visitor<Ret(void*, Args...)>::Visit(std::size_t ID, void* ptr, Args... args) const {
		return callbacks.at(ID)(ptr, std::forward<Args>(args)...);
	}

	template<typename Ret, typename... Args>
	template<typename T>
	Ret Visitor<Ret(void*, Args...)>::Visit(T* ptr, Args... args) const {
		static_assert(!std::is_const_v<T>, "Visitor::Visit: <T> must be non-const");
		return Visit(details::CustomIDof(ptr), ptr, std::forward<Args>(args)...);
	}

	template<typename Ret, typename... Args>
	template<typename... Funcs>
	void Visitor<Ret(void*, Args...)>::Register(Funcs&&... funcs) {
		(RegisterOne(std::forward<Funcs>(funcs)), ...);
	}

	template<typename Ret, typename... Args>
	template<typename Func>
	void Visitor<Ret(void*, Args...)>::Register(std::size_t ID, Func&& func) {
		if constexpr (std::is_same_v<FuncTraits_Signature<Func>, Ret(void*, Args...)>)
			callbacks[ID] = std::forward<Func>(func);
		else {
			using ArgList = FuncTraits_ArgList<Func>;
			using DerivedPointer = Front_t<ArgList>;
			using Derived = std::remove_pointer_t<DerivedPointer>;
			static_assert(!std::is_const_v<Derived>, "<Derived> must be non-const");
			callbacks[ID] = [func = std::forward<Func>(func)](void* p, Args... args) {
				return func(reinterpret_cast<Derived*>(p), std::forward<Args>(args)...);
			};
		}
	}

	template<typename Ret, typename... Args>
	template<typename Func>
	void Visitor<Ret(void*, Args...)>::RegisterOne(Func&& func) {
		using ArgList = FuncTraits_ArgList<Func>;
		using DerivedPointer = Front_t<ArgList>;
		static_assert(std::is_same_v<TypeList<DerivedPointer, Args...>, ArgList>,
			"arguments must be (T*, Args...)");

		using Derived = std::remove_pointer_t<DerivedPointer>;

		static_assert(!std::is_const_v<Derived> && !std::is_void_v<Derived>,
			"<Derived> must be non-const and non-void");
		
		callbacks[details::CustomID<Derived>::get()] =
			[func = std::forward<Func>(func)](void* p, Args... args) {
				return func(reinterpret_cast<Derived*>(p), std::forward<Args>(args)...);
			};
	}

	template<typename Ret, typename... Args>
	bool Visitor<Ret(void*, Args...)>::IsRegistered(std::size_t ID) const {
		return callbacks.contains(ID);
	}

	template<typename Ret, typename... Args>
	template<typename T>
	bool Visitor<Ret(void*, Args...)>::IsRegistered() const {
		return IsRegistered(CustomID<T>());
	}

	template<typename Ret, typename... Args>
	bool Visitor<Ret(void*, Args...)>::IsRegistered(const void* ptr) const {
		return IsRegistered(reinterpret_cast<std::size_t>(details::vtable(ptr)));
	}
}
