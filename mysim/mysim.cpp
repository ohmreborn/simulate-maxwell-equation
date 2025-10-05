#include "mysim.h"
#include <cblas.h>

#include <iostream>

Mysim::Mysim(simulation *real_signal, simulation *noise, size_t window_size)
	: real_signal(real_signal), noise(noise), window_size(window_size)
{
	y = new float[real_signal->wave_size * real_signal->wave_size];
	e = new float[real_signal->wave_size * real_signal->wave_size];
	buf_size = real_signal->wave_size + 2 * (window_size/2);
	buf = new float[buf_size * buf_size];
	window = new float[window_size * window_size];
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

	for (int row=0; row< real_signal->wave_size; row++){
		int add = real_signal->wave_size*row;
		cblas_scopy(noise->wave_size, noise->Ey + add, 1, buf+ buf_size*row +(window_size/2), 1);
	}

	int index = 0;
	for (int m = 0; m < real_signal->wave_size; m++) {
		for (int n = 0; n < real_signal->wave_size; n++) {

			// convolution (filtering noise reference)
			for (int i = 0; i < window_size; i++) {
				y[index] += cblas_sdot(window_size, window+i*window_size, 1, buf+(m+i)*buf_size+n, 1);
			}

			// compute error (desired - estimate)
			e[index] = noise->Ey[index] - y[index];

			// update weights
			for (int i = 0; i < window_size; i++) {
				for (int j = 0; j < window_size; j++) {
					window[i*window_size + j] += 2e-6 * e[index] * buf[(m+i)*buf_size+n+j];
				}
			}

			index++;
		}
	}

	cblas_saxpy(real_signal->wave_size * real_signal->wave_size, -1, y, 1, real_signal->Ey, 1);
	cblas_saxpy(real_signal->wave_size * real_signal->wave_size, -1, y, 1, noise->Ey, 1);
	total_error = cblas_snrm2(real_signal->wave_size * real_signal->wave_size, e, 1); 

}
