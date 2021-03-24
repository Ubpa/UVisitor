#pragma once

#include "ncVisitor.h"

namespace Ubpa {
	// const impl non-const visitor
	template<typename Impl, typename Ret, typename... Args>
	class Visitor<Ret(Impl::*)(void*, Args...)const> : private Visitor<Ret(void*, Args...)> {
	public:
		using Visitor<Ret(void*, Args...)>::Register;

		Ret Visit(std::size_t ID, void* ptr, Args... args) const;

		template<typename T>
		Ret Visit(T* ptr, Args... args) const;

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
		std::unordered_map<std::size_t, std::function<Ret(const Impl*, void*, Args...)>> impl_callbacks;
	};
}

#include "details/cincVisitor.inl"
