#include "gc.hpp"
#include "machine.hpp"

using namespace stackmachine;

void GC::sweep(machine* m)
{
	Object** obj = &m->m_firstObject;
	while (*obj) {
		if (!(*obj)->marked) {
			Object* unreached = *obj; // obj wasn't reached, so remove it from the list

			*obj = unreached->next;
			delete unreached;

			m->m_numObjects--;

		}
		else {
			(*obj)->marked = false; // this obj was reached, so unmark it for the next GC
			obj = &(*obj)->next;
		}
	}
}

void GC::boot(machine* m)
{
	markObjects(m);
	sweep(m);
}

void GC::markObjects(machine* m)
{
	for (const auto& obj : m->m_allocObjStack) {
		obj->mark();
	}
}

