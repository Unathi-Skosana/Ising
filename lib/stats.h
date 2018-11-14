#ifndef STATS_H
#define STATS_H


int get_tau(int L, double T);
void autocor(double *data, int tmax);
void sample(double *E_data, double *M_data, int num, int tau, int lat_sz, double tem);
void mean(int n, double *data, double *m);
void variance(int n, double *data, double *v);
void jackknife(int n, double *data, double beta,\
  double *err, double *val, void (*f1)(int, double *, double *),\
  void (*f2)(double , double, double *), void (*f3)(int, int, double *, double *));
void jackknife_variance(int n, int index, double *data, double *v);
void jackknife_mean(int n, int index, double *data, double *m);

#endif
