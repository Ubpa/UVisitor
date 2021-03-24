#include <UVisitor/cVisitor.h>
#include <UVisitor/ncVisitor.h>
#include <UVisitor/cicVisitor.h>
#include <UVisitor/cincVisitor.h>
#include <UVisitor/ncicVisitor.h>
#include <UVisitor/ncincVisitor.h>

#include <iostream>
#include <memory>

using namespace Ubpa;
using namespace std;

struct IShape { virtual ~IShape() = default; };

struct Triangle : IShape {};
struct Sphere : IShape {};

class C {
	void f() {}
};

int main() {
	{ // non-const visitor
		Visitor<void(void*)> visitor;
		auto visit_tri = [](Triangle*) { cout << "Lambda(Triangle*)" << endl; };
		auto visit_sphere = [](Sphere*) { cout << "Lambda(Sphere*)" << endl; };
		visitor.Register(visit_tri, visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
	{ // const
		Visitor<void(const void*)> visitor;
		auto visit_tri = [](const Triangle*) { cout << "Lambda(const Triangle*)" << endl; };
		auto visit_sphere = [](const Sphere*) { cout << "Lambda(const Sphere*)" << endl; };
		visitor.Register(visit_tri, visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
	{ // non-const impl non-const visitor
		class V : public Visitor<void(V::*)(void*)> {
		public:
			V() {
				Register<Triangle>();
			}
		protected:
			void ImplVisit(Triangle*) {
				cout << "V::ImplVisit(Triangle*)" << endl;
			}
		};
		

		V visitor;
		auto visit_sphere = [](Sphere*) { cout << "Lambda(Sphere*)" << endl; };
		visitor.Register(visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
	{ // non-const impl const visitor
		class V : public Visitor<void(V::*)(const void*)> {
		public:
			V() {
				Register<Triangle>();
			}
		protected:
			void ImplVisit(const Triangle*) {
				cout << "V::ImplVisit(const Triangle*)" << endl;
			}
		};

		V visitor;
		auto visit_sphere = [](const Sphere*) { cout << "Lambda(const Sphere*)" << endl; };
		visitor.Register(visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
	{ // const impl non-const visitor
		class V : public Visitor<void(V::*)(void*)const> {
		public:
			V() {
				Register<Triangle>();
			}
		protected:
			void ImplVisit(Triangle*) const {
				cout << "(const V)::ImplVisit(Triangle*)" << endl;
			}
		};


		V visitor;
		auto visit_sphere = [](Sphere*) { cout << "Lambda(Sphere*)" << endl; };
		visitor.Register(visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
	{ // const impl const visitor
		class V : public Visitor<void(V::*)(const void*)const> {
		public:
			V() {
				Register<Triangle>();
			}
		protected:
			void ImplVisit(const Triangle*) const {
				cout << "(const V)::ImplVisit(const Triangle*)" << endl;
			}
		};

		V visitor;
		auto visit_sphere = [](const Sphere*) { cout << "Lambda(const Sphere*)" << endl; };
		visitor.Register(visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
}
