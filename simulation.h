#ifndef SIMULATION_H
#define SIMULATION_H

#include <cstdlib>

class world{
	public:
		int n;

		float eps0;
		float mu0;
		float c0;

		size_t wave_size;

		int source_x;
		int source_z;

		float lambda;
		float dx;
		float dy;
		float dz;
		float dt;

		float *Ey;
		float *Ey_prev;
		float *Bx;
		float *Bz;
		
		float x_min;
		float x_max;
		float y_min;
		float y_max;
		float z_min;
		float z_max;

		world();
		~world();

		void update();

		float Source_Function(int t);

		inline int get_index(int i,int j){
			return j + i*wave_size;
		}

};

#endif
