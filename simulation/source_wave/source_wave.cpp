#include "../simulation.h"
#include "source_wave.h"

#include <cmath>

void simple_source(simulation *myworld){
	float w0 = 2*M_PI*myworld->c0/myworld->lambda;
	float source = std::sin(w0 * myworld->n * myworld->dt);
	int index = myworld->get_index(3,30);
	myworld->Ey[index] += source;
}


void noise_source(simulation *myworld){
	float w0 = 2*M_PI*(4*myworld->c0/myworld->lambda);
	float source = 0.2 * std::sin(w0 * myworld->n * myworld->dt);

	int index = myworld->get_index(1,5);
	myworld->Ey[index] += source;
}
