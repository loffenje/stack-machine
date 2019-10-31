#ifndef GC_HPP
#define GC_HPP

#include <cstdint>

namespace stackmachine {

	const int GC_THRESHOLD = 3;

	class machine;

	class GC {
	public:
		void sweep(machine *m);
		void boot(machine *m);
		void markObjects(machine *m);
	};
}

#endif
