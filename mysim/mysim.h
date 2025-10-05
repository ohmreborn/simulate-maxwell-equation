#ifndef MYSIM
#define MYSIM

#include "../simulation/simulation.h"
#include <cstdint>

class Mysim{
	public: 
		simulation *real_signal, *noise;
		Mysim(simulation *real_signal, simulation *noise, size_t window_size);
		~Mysim();
		void update();
		float *y;
		float *e;
		float *buf;
		float *window;
		float total_error;
		size_t window_size;
};

#endif
