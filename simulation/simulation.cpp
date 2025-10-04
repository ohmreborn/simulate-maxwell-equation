#include "simulation.h"
#include "source_wave/source_wave.h"

#include <cblas.h>

#include <iostream>
#include <vector>

simulation::simulation(size_t wave_size) : wave_size(wave_size)
{
	// init a value
	n = 0;

	eps0 = 8.854187817e-12;
	mu0 = 1.256e-6;
	c0 = 1/std::sqrt(eps0 * mu0);


	Ey = new float[wave_size * wave_size];
	Bx = new float[wave_size * wave_size];
	Bz = new float[wave_size * wave_size];
	cblas_sscal( wave_size*wave_size, 0, Ey, 1);
	cblas_sscal( wave_size*wave_size, 0, Bx, 1);
	cblas_sscal( wave_size*wave_size, 0, Bz, 1);

	const int a = 20;
	lambda = 5e-7;
	dx = lambda/a;
	dy = lambda/a;
	dz = lambda/a;
	dt = 1 / (c0 * std::sqrt( 1/(dx * dx) + 1/(dy * dy) + 1/(dz * dz)) );
//	dt =  lambda / ( 2 * c0);

	x_min = 0;
	x_max = wave_size;
	y_min = -2.5;
	y_max = 2.5;
	z_min = 0;
	z_max = wave_size;

	all_source = std::vector<void (*)(simulation *)>(0);

}

simulation::~simulation(){
	delete [] Ey;
	delete [] Bx;
	delete [] Bz;
}

void simulation::add(void (*func)(simulation *)){
	all_source.push_back(func);
}

inline void update_Bx(size_t wave_size, float dt, float dz, float *Ey, float *Bx){
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

}

inline void update_Bz(size_t wave_size, float dt, float dx, float *Ey, float *Bz){
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

}

inline void update_Ey(
		size_t wave_size, 
		float eps0, float mu0, 
		float dt, float dx, float dz, 
		float *Ey, float *Bx, float *Bz
		){
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

}

void simulation::update(){
	update_Bx(wave_size, dt, dz, Ey, Bx);
	update_Bz(wave_size, dt, dz, Ey, Bz);
	update_Ey(wave_size, eps0,  mu0, dt,  dx,  dz, Ey,  Bx,  Bz);

	for (int i=0;i<all_source.size();i++){
		parse_source(all_source[i]);
	}

	n++;
}

