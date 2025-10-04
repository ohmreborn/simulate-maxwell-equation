#include "mysim.h"

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
		for (int i=1;i<window_size;i++){
			buf[i] = buf[i-1];
		}
		buf[0] = noise->Ey[j];

		y[j] = 0; 
		for (int i=0;i<window_size;i++){
			y[j] += buf[i] * window[i]; 
		}
		e[j] = real_signal->Ey[j] - y[j];
		for (int i=0;i<window_size;i++){
			window[i] = window[i] + 2e-2*e[j] * buf[i];
		}
		real_signal->Ey[j] -= y[j];
		noise->Ey[j] -= y[j];
	} 

}
