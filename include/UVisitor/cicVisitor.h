#pragma once

#include "cVisitor.h"

namespace Ubpa {
	// const impl const visitor
	template<typename Impl, typename Ret, typename... Args>
	class Visitor<Ret(Impl::*)(const void*, Args...)const> : private Visitor<Ret(const void*, Args...)> {
	public:
		using Visitor<Ret(const void*, Args...)>::Register;

		Ret Visit(std::size_t ID, const void* ptr, Args... args) const;

		template<typename T>
		Ret Visit(const T* ptr, Args... args) const;

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
		std::unordered_map<std::size_t, std::function<Ret(const Impl*, const void*, Args...)>> impl_callbacks;
	};
}

#include "details/cicVisitor.inl"
