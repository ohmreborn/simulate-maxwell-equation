#ifndef SIMULATION_H
#define SIMULATION_H

#include <cstdlib>
#include <vector>

class simulation{
	public:
		int n;

		float eps0;
		float mu0;
		float c0;

		size_t wave_size;

		float lambda;
		float dx;
		float dy;
		float dz;
		float dt;

		float *Ey;
		float *Bx;
		float *Bz;

		float x_min;
		float x_max;
		float y_min;
		float y_max;
		float z_min;
		float z_max;

		std::vector<void (*)(simulation *)> all_source;

		simulation(size_t wave_size);
		~simulation();
		void add(void (*func)(simulation *));

		void update();

		inline void parse_source(void (*func)(simulation *)){
			func(this);
		}

		inline int get_index(int i,int j){
			return j + i*wave_size;
		}

};

#endif
