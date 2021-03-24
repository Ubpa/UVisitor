#pragma once

#include "ncVisitor.h"

namespace Ubpa {
	// non-const impl non-const visitor
	template<typename Impl, typename Ret, typename... Args>
	class Visitor<Ret(Impl::*)(void*, Args...)> : private Visitor<Ret(void*, Args...)> {
	public:
		using Visitor<Ret(void*, Args...)>::Register;

		Ret Visit(std::size_t ID, void* ptr, Args... args);

		template<typename T>
		Ret Visit(T* ptr, Args... args);

		bool IsRegistered(std::size_t ID) const;
		template<typename T>
		bool IsRegistered() const;

		bool IsRegistered(const void* ptr) const;

	protected:
		// ImplVisit
		template<typename... Deriveds>
		void Register();

	private:
		// ImplVisit
		template<typename Derived>
		void RegisterOne();

	private:
		std::unordered_map<std::size_t, std::function<Ret(Impl*, void*, Args...)>> impl_callbacks;
	};
}

#include "details/ncincVisitor.inl"
