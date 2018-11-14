#ifndef GNUPLOT_H
#define GNUPLOT_H

/* Place the content of your header file here */

int autocor_plot_xmax; // maximum value on the plot
char *autocor_fn;

void config_physics_gnuplot();
void start_physics_gnuplot();
void config_autocor_gnuplot();
void start_autocor_gnuplot();
void plot_correlation_times();
void plot_lattice(char *fn);
void plot_measurements(char *fn, int opt, int L);

#endif
