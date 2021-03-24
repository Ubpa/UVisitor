#pragma once

#include "Visitor.h"

namespace Ubpa {
	// non-const visitor
	template<typename Ret, typename... Args>
	class Visitor<Ret(void*, Args...)> {
	public:
		Ret Visit(std::size_t ID, void* ptr, Args... args) const;
		template<typename T>
		Ret Visit(T* ptr, Args... args) const;

		template<typename... Funcs>
		void Register(Funcs&&... funcs);

		template<typename Func>
		void Register(std::size_t ID, Func&& func);

		bool IsRegistered(std::size_t ID) const;
		template<typename T>
		bool IsRegistered() const;

		bool IsRegistered(const void* ptr) const;

	private:
		template<typename Func>
		void RegisterOne(Func&& func);

	protected:
		std::unordered_map<std::size_t, std::function<Ret(void*, Args...)>> callbacks; // ID -> func
	};
}

#include "details/ncVisitor.inl"
