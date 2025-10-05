#include "mysim.h"
#include <cblas.h>

#include <iostream>

Mysim::Mysim(simulation *real_signal, simulation *noise, size_t window_size)
	: real_signal(real_signal), noise(noise), window_size(window_size)
{
	y = new float[real_signal->wave_size * real_signal->wave_size];
	e = new float[real_signal->wave_size * real_signal->wave_size];
	buf = new float[window_size];
	window = new float[window_size];
}

Mysim::~Mysim(){
	delete[] y;
	delete[] e;
	delete[] buf;
	delete[] window;
}

void Mysim::update(){
	real_signal->update();
	noise->update();

	for (int j=0;j<(real_signal->wave_size * real_signal->wave_size);j++){
		cblas_scopy(window_size-1, buf, 1, buf+1, 1);
		buf[0] = noise->Ey[j];

		y[j] = cblas_sdot(window_size, buf, 1, window, 1);
		e[j] = real_signal->Ey[j] - y[j];

		cblas_saxpy(window_size, 1e-4*e[j], buf, 1, window, 1);
	} 

	cblas_saxpy(real_signal->wave_size * real_signal->wave_size, -1, y, 1, real_signal->Ey, 1);
	cblas_saxpy(real_signal->wave_size * real_signal->wave_size, -1, y, 1, noise->Ey, 1);

	total_error = cblas_snrm2(real_signal->wave_size * real_signal->wave_size, e, 1); 

}
