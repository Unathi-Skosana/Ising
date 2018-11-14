#include "../lib/utils.h"
#include "../lib/stats.h"
#include "../lib/gnuplot.h"


int sz = 110;
int taus20[110]  = {2,2,3,3,3,3,3,3,4,4,5,5,6,8,12,13,15,15,16,23,24,23,\
  24,32,31,39,45,45,45,52,56,63,63,66,65,67,65,67,65,66,62,62,60,58,55,\
  53,50,48,46,42,40,39,36,35,32,31,29,28,27,26,24,22,22,21,20,19,18,17,\
  17,16,16,15,14,13,13,13,12,12,12,11,11,11,10,10,10,9,9,9,9,9,8,8,8,8,\
  8,7,7,7,7,7,7,7,7,6,5,4,4,3,3,3};
int taus40[110]  = {7,2,3,3,3,3,3,3,4,4,4,5,6,8,11,12,14,15,19,20,21,24,\
  29,30,40,60,70,82,131,193,216,224,267,268,279,298,265,310,255,241,218,\
  185,180,163,124,108,99,85,78,71,62,56,53,48,45,41,38,35,33,30,28,27,25,\
  24,22,22,20,19,18,18,17,16,15,15,14,14,13,13,12,12,11,11,11,11,10,10,10,\
  9,9,9,8,8,8,8,8,8,7,7,7,7,7,7,7,7,6,4,4,3,3,3};
int taus80[110] = {4,2,3,3,3,3,3,3,4,4,4,5,6,8,11,12,13,15,18,20,22,25,29,\
  37,44,50,67,111,98,262,359,587,717,1198,1461,1308,1006,911,712,573,458,\
  362,279,230,160,132,116,99,90,78,69,60,54,50,48,42,39,36,34,30,29,28,26,\
  24,23,22,20,20,19,18,17,16,15,15,14,14,13,13,12,12,11,11,11,11,10,10,10,\
  9,9,9,9,9,8,8,8,8,8,7,7,7,7,7,7,7,6,4,4,3,3,3};
int taus100[110] = {2,2,3,3,3,3,3,3,4,4,4,5,6,8,11,12,13,15,16,19,20,25,30,\
  37,41,52,69,83,99,232,200,596,1411,1986,1594,2108,1552,1298,936,682,492,\
  360,293,264,167,134,114,107,92,81,71,64,57,51,46,44,39,37,34,31,29,26,26,\
  24,22,22,20,19,18,18,17,16,15,15,14,14,13,13,12,12,11,11,11,11,10,10,10,\
  9,9,9,9,8,8,8,8,8,8,7,7,7,7,7,7,7,6,4,4,3,3,3};

double temps[110] = {0.100, 0.200, 0.300, 0.500, 0.60, 0.650, 0.700, 0.725, 0.750,\
    0.775, 0.800, 0.825, 0.850, 0.875, 0.900, 0.905, 0.910, 0.915, 0.920, 0.925, 0.930,
    0.935, 0.940 ,0.945, 0.950, 0.955, 0.960, 0.965, 0.970, 0.975, 0.980, 0.985,\
    0.990, 0.995, 1.000, 1.005, 1.010, 1.015, 1.020, 1.025, 1.030, 1.035, 1.040,\
    1.045, 1.055, 1.060, 1.065, 1.070, 1.075, 1.080, 1.085, 1.090, 1.095, 1.100,\
    1.105, 1.110, 1.115, 1.120, 1.125, 1.130, 1.135, 1.140, 1.145, 1.150, 1.155,\
    1.160, 1.165, 1.170, 1.175, 1.180, 1.185, 1.190, 1.195, 1.200, 1.205, 1.210,\
    1.215, 1.220, 1.225, 1.230, 1.235, 1.240, 1.245, 1.250, 1.255, 1.260, 1.265,\
    1.270, 1.275, 1.280, 1.285, 1.290, 1.295, 1.300, 1.305, 1.310, 1.315, 1.320,\
    1.325, 1.330, 1.335, 1.340, 1.345, 1.350, 1.400, 1.500, 1.60, 1.70, 1.75, 1.80};

void autocor(double* data, int tmax)
{
    char *f_cpy, *s_cpy;
    int dt, tp, dt_max;
    double pre = 0, s0 = 0, s1 = 0, s2 = 0, x, xi;

    f_cpy = strdup(autocor_fn);
    s_cpy = strdup(autocor_fn);

    config_autocor_gnuplot();
    dt_max = autocor_plot_xmax;
    for (dt = 0; dt < dt_max; dt++) {
        s0 = 0, s1 = 0, s2 = 0;
        pre = 1.0 / (tmax - dt);
        for (tp = 0; tp < tmax - dt; tp++) {
            s0 += data[tp] * data[tp + dt];
            s1 += data[tp];
            s2 += data[tp + dt];
        }

        x =  pre * s0 - pre * pre * s1 * s2;

        // Store initial value for normalization
        if (dt == 0) {
            xi = x;
        }

        fprintf(gnuplot, "%d %f\n", dt, x / xi);
    }
    start_autocor_gnuplot();
    free(f_cpy);
    free(s_cpy);
}


void mean(int n, double *data, double *m)
{
  int i;
  double norm = 0, acc = 0;

  norm = 1.0 / n;
  for (i = 0; i < n; i++) {
      acc  += sqrt(data[i] * data[i]);
  }

  *m = 1.0 * acc * norm;
}

void variance(int n, double *data, double *v)
{
  int i;
  double norm = 0, acc = 0, accs = 0, prop_var;

  norm = 1.0 / n;
  for (i = 0; i < n; i++) {
      acc   += sqrt(data[i] * data[i]);
      accs  += data[i]*data[i];
  }

  prop_var = 1.0 * (accs * norm - pow(acc * norm, 2));
  *v = (prop_var < 0.0 ? 0.0 : prop_var);
}

void jackknife_mean(int n, int index, double *data, double *m)
{
  int i;
  double norm = 0, acc = 0;

  norm = 1.0 / n ;
  for (i = 0; i < n; i++) {
    if (i != index) acc  += sqrt(data[i] * data[i]);
  }

  *m = 1.0 * acc * norm;
}

void jackknife_variance(int n, int index, double *data, double *v)
{
  int i;
  double norm = 0, acc = 0, accs = 0, prop_var = 0;

  norm = 1.0 / n ;
  for (i = 0; i < n; i++) {
    if (i != index) {
      accs += data[i] * data[i];
      acc  += sqrt(data[i] * data[i]);
    }
  }

  prop_var = 1.0 * (accs * norm - pow(acc * norm, 2));
  *v = (prop_var < 0.0 ? 0.0 : prop_var);
}

void sample(double *E_data, double *M_data, int num, int tau, int L, double tem)
{
  FILE *fp;
  int i, n;
  char fn[1024];

  sprintf(fn, "../samples/L%d/%ld&T=%.5f.dat", L, time(NULL), tem);
  fp = fopen(fn, "w");
  n = num / (2.0 * tau);

  fprintf(fp, "#%d\n", n);
  fprintf(fp, "#time\tmagnetization\tenergy\t\n");
  for (i = 0; i < num; i += 2 * tau) {
    fprintf(fp, "%d\t%f\t%f\n", i / (2 * tau), M_data[i], E_data[i]);
  }

  fclose(fp);
}

int get_tau(int L, double T)
{
  int i, fIndex;
  double ratio = 1.0 * T / Tc;

  for (i = 0; i < sz; i++) {
    if (temps[i] - ratio < 1e-6) {
      fIndex = i;
      break;
    }
  }

  if (L == 20)  return taus20[fIndex];
  if (L == 40)  return taus40[fIndex];
  if (L == 80)  return taus80[fIndex];
  if (L == 100) return taus100[fIndex];
  return 2000;
}


void jackknife(int n, double *data, double beta,\
  double *err, double *val, void (*f1)(int, double *, double *),\
  void (*f2)(double , double, double *), void (*f3)(int, int, double *, double *))
{
  int i;
  double *func_vals, v = 0.0, vi = 0.0,  avg = 0.0, s = 0.0;

  func_vals = malloc(n * sizeof(double));

  for (i = 0; i < n; i++) {
    f3(n - 1, i , data, &v);
    f2(v, beta, &func_vals[i]);
  }

  mean(n, func_vals, &avg);
  f1(n, data, &v);
  f2(v, beta, &vi);

  for (i = 0; i < n ; i++) s += (func_vals[i] - avg) * (func_vals[i] - avg);

  *val = vi;
  *err = sqrt(1.00 * (n - 1) / n * s);
  free(func_vals);
}
