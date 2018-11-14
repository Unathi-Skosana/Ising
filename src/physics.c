#include "../lib/utils.h"

int calc_lat_magnetization()
{
    int i, sum;

    sum = 0;

    for (i = 0; i < N; ++i) {
        sum += lat[i];
    }

    return sum;
}

int calc_lat_energy()
{
    int nn, k, sum;

    sum = 0, nn = 0;

    for (k = 0; k < N; k += 2) {
        if ((nn = k+XNN) >= N)   nn -= N;
        sum += lat[nn] * lat[k];

        if ((nn = k-XNN) <  0)   nn += N;
        sum += lat[nn] * lat[k];

        if ((nn = k+YNN) >= N)   nn -= N;
        sum += lat[nn] * lat[k];

        if ((nn = k-YNN) <  0)   nn += N;
        sum += lat[nn] * lat[k];

    }

    return -sum;
}

double analytic_magnetization()
{
    if (1.0 / beta < Tc) return pow((1 - pow(sinh(2.0 * beta), -4.0)), 1.0/8.0);
    else return 0.0;
}

void mean_mag(double mean, double beta, double *m)
{
  *m = mean;
}

void mean_energy(double mean, double beta, double *e)
{
  *e = mean;
}

void specific_heat(double var, double beta, double *c)
{
  *c = beta * beta  * var;
}

void magnetic_susceptibility(double var, double beta, double *x)
{
  *x = beta * var;
}

int find_equilibrium(double cur, double prev)
{

  double change = (cur - prev) / cur;
  if (change < EQ_THRESHOLD) return 1;
  return 0;
}
