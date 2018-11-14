#ifndef UTILS_H
#define UTILS_H


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define EQ_THRESHOLD 1e-4
#define Tc  2.269185
#define XNN 1
#define YNN L

int sweeps;         // Sweeps
int L;              // lattice size
int N;              // number of sites
int *lat;           // flat lattice
int E;              // curret energy of lattice
int M;              // current magnetization of lattice
double prob[5];     // probalities from boltzman distribution
double beta;        //  1 / kT
double mag;// Analytic magnetization value
char eq_data_name[1024];  // file name for data file
char sim_data_name[1024];  // file name for data file
int physics_plot;   // Physics plot type
int autocor_plot;   // Physics plot type
FILE *gnuplot;      // gnuplot pipe
FILE *data;      // data file
FILE *eq_data;      // data file

double d_abs(double v);
int calc_lat_magnetization();
int calc_lat_energy();
int get_lat_magnetization();
int get_lat_energy();
void *init_lat(int r, int opt, double temp, int sweeps, int plot);
void init_prob();
void sweep();
void free_allocated_resources();
void print_lat();
void console_log(int j, int delta);
void read_data(char *fn, int row, double **data, int *tmax);
void write_lat();
void *reinit_lat(double temp);

#endif
