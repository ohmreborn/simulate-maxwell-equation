#include "simulation.h"

#include <cblas.h>

#include <iostream>
#include <vector>

world::world(){
	// init a value
	n = 0;

	eps0 = 8.854187817e-12;
	mu0 = 1.256e-6;
	c0 = 1/std::sqrt(eps0 * mu0);

	wave_size = 100;
	source_x = 1;
	source_z = 22;

	Ey = new float[wave_size * wave_size];
	Ey_prev = new float[wave_size * wave_size];
	Bx = new float[wave_size * wave_size];
	Bz = new float[wave_size * wave_size];

	const int a = 20;
	lambda = 5e-7;
	dx = lambda/a;
	dy = lambda/a;
	dz = lambda/a;
	dt = 1 / (c0 * std::sqrt( 1/(dx * dx) + 1/(dy * dy) + 1/(dz * dz)) );
//	dt =  lambda / ( 2 * c0);

	x_min = 0;
	x_max = (wave_size * dx) / 1;
	y_min = -2.5;
	y_max = 2.5;
	z_min = 0;
	z_max = (wave_size * dz) / 1;

}

world::~world(){
	delete [] Ey;
	delete [] Ey_prev;
	delete [] Bx;
	delete [] Bz;
}



void world::update(){

	// Update Bx
	for (int i=1; i<wave_size-1; i++){
		int index = wave_size * i + 1;
		cblas_saxpy(wave_size-2, dt/dz, Ey+index+1, 1, Bx + index, 1);
		cblas_saxpy(wave_size-2, -dt/dz, Ey+index, 1, Bx + index, 1);

		//	for (int j=1; j<wave_size-1; j++){
		//		int index = get_index(i,j);
		//		Bx[index] += (dt/dz) * (Ey[index + 1] - Ey[index]);
		//	}
	}

	// Update Bz
	for (int i=1; i<wave_size-1; i++){
		int index = wave_size * i + 1;
		cblas_saxpy(wave_size-2, -dt/dx, Ey+index+wave_size, 1, Bz + index, 1);
		cblas_saxpy(wave_size-2, dt/dx, Ey+index, 1, Bz + index, 1);

		//for (int j=1; j<wave_size-1; j++){
		//	int index = get_index(i,j);
		//	Bz[index] -= (dt/dx) * (Ey[index + wave_size] - Ey[index]);
		//}
	}

	// Update Ey
	for (int i=1; i<wave_size-1; i++){
		int index = wave_size * i + 1;
		cblas_saxpy(wave_size-2, dt/(dz * eps0 * mu0), Bx+index, 1, Ey + index, 1);
		cblas_saxpy(wave_size-2, -dt/(dz * eps0 * mu0), Bx+index-1, 1, Ey + index, 1);

		cblas_saxpy(wave_size-2, -dt/(dx * eps0 * mu0), Bz+index, 1, Ey + index, 1);
		cblas_saxpy(wave_size-2, dt/(dx * eps0 * mu0), Bz+index-wave_size, 1, Ey + index, 1);

		//for (int j=1; j<wave_size-1; j++){
		//	int index = get_index(i,j);
		//	Ey[index] += dt/(eps0 * mu0) * (
		//			(Bx[index] - Bx[index-1]) / dz -
		//			(Bz[index] - Bz[index-wave_size]) / dx
		//			);
		//}
	}

//	for (int i=0;i<wave_size;i++){
//		if ((60 < i || i < 50) && (40 < i || i < 30)) {
//			Ey[get_index(20,i)] = 0.0;
//		}
//	}

// Source in the center
	double source = Source_Function(n);
	int index = wave_size * source_x;
	for (int i=1;i<wave_size-1;i++){
		Ey[index + i] = source;
	}

	// mur boundary condition
	for (int i=0;i<wave_size;++i){
		int bound = get_index(0,i);
		int pre_bound = get_index(1,i);
		Ey[bound] = Ey_prev[pre_bound] + ((c0*dt - dx)/(c0*dt + dx)) * (Ey[bound] - Ey_prev[pre_bound]);

		bound = get_index(i,0);
		pre_bound = get_index(i,1);
		Ey[bound] = Ey_prev[pre_bound] + ((c0*dt - dx)/(c0*dt + dx)) * (Ey[bound] - Ey_prev[pre_bound]);

		bound = get_index(wave_size-1,i);
		pre_bound = get_index(wave_size-2,i);
		Ey[bound] = Ey_prev[pre_bound] + ((c0*dt - dx)/(c0*dt + dx)) * (Ey[bound] - Ey_prev[pre_bound]);

		bound = get_index(i,wave_size-1);
		pre_bound = get_index(i,wave_size-2);
		Ey[bound] = Ey_prev[pre_bound] + ((c0*dt - dx)/(c0*dt + dx)) * (Ey[bound] - Ey_prev[pre_bound]);
	}

	for (int i=0;i<wave_size * wave_size;++i){
		Ey_prev[i] = Ey[i];
	}

	n++;
}

float world::Source_Function(int t){
	float w0 = 2*M_PI*c0/lambda;
	float y = std::sin(w0*t*dt);
	return y;
}

