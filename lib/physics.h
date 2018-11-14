
#ifndef PHYSICS_H
#define PHYSICS_H

int calc_lat_magnetization();

int calc_lat_energy();
double analytic_magnetization();
int find_equilibrium(double cur, double prev);
void mean_mag(double mean, double beta, double *m);
void mean_energy(double mean, double beta, double *e);
void specific_heat(double var, double beta, double *c);
void magnetic_susceptibility(double var, double beta, double *x);

#endif
