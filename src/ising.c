#include "../lib/utils.h"
#include "../lib/stats.h"
#include "../lib/physics.h"
#include "../lib/gnuplot.h"


char * data_type(int opt);

int sample_sz = 110;
double sample_temps[110] = {0.100, 0.200, 0.300, 0.500, 0.60, 0.650, 0.700, 0.725, 0.750,\
    0.775, 0.800, 0.825, 0.850, 0.875, 0.900, 0.905, 0.910, 0.915, 0.920, 0.925, 0.930,
    0.935, 0.940 ,0.945, 0.950, 0.955, 0.960, 0.965, 0.970, 0.975, 0.980, 0.985,\
    0.990, 0.995, 1.000, 1.005, 1.010, 1.015, 1.020, 1.025, 1.030, 1.035, 1.040,\
    1.045, 1.055, 1.060, 1.065, 1.070, 1.075, 1.080, 1.085, 1.090, 1.095, 1.100,\
    1.105, 1.110, 1.115, 1.120, 1.125, 1.130, 1.135, 1.140, 1.145, 1.150, 1.155,\
    1.160, 1.165, 1.170, 1.175, 1.180, 1.185, 1.190, 1.195, 1.200, 1.205, 1.210,\
    1.215, 1.220, 1.225, 1.230, 1.235, 1.240, 1.245, 1.250, 1.255, 1.260, 1.265,\
    1.270, 1.275, 1.280, 1.285, 1.290, 1.295, 1.300, 1.305, 1.310, 1.315, 1.320,\
    1.325, 1.330, 1.335, 1.340, 1.345, 1.350, 1.400, 1.500, 1.60, 1.70, 1.75, 1.80};

int main(int argc, char *argv[])
{

   char *s = argv[1];

    if (strcmp("--a", s) == 0) {
        int tmax;
        double *data;

        autocor_fn = argv[2];
        autocor_plot_xmax = atoi(argv[3]);

        read_data(autocor_fn, 1, &data, &tmax);
        fprintf(stderr, "\n\nautocorrelating...\n\n");
        autocor(data, tmax);
        free(data);
    } else if (strcmp("--siter", s) == 0) {
        int L, opt, plot, tmax, i;

        L = atoi(argv[2]);
        opt = atoi(argv[3]);
        tmax = atoi(argv[4]);
        plot = atoi(argv[5]);

        for (i = 0; i < sample_sz; i++) {
            if (i == 0) {
              init_lat(L, opt, sample_temps[i] * Tc, tmax, plot);
            } else {
              reinit_lat(sample_temps[i] * Tc);
            }
            fprintf(stderr,"\n\nat temperature %f\n\n", sample_temps[i] * Tc);
            sweep();
            write_lat();
        }
        fprintf(stderr,"\n\n...exited successfully....\n\n");
        free(lat);
    } else if (strcmp("--hm", s) == 0) {
        fprintf(stderr,"\n\n...plotting....\n\n");
        plot_lattice(argv[2]);
        fprintf(stderr,"\n\n...exited successfully....\n\n");
	} else if (strcmp("--sa", s) == 0) {
        char *fn, *f_cpy, *s_cpy;
        int E_num, M_num, tau, L;
        double *E_data, *M_data, T;

        fn = argv[2];
        read_data(fn, 1, &M_data, &E_num);
        read_data(fn, 2, &E_data, &M_num);

        f_cpy = strdup(fn);
        s_cpy = strdup(fn);
        T = atof(strtok((rindex(f_cpy, '=') + 1), "dat"));
        L = atoi(strtok((rindex(s_cpy, 'L') + 1), "/"));

        tau = get_tau(L, T);
        fprintf(stderr,"\n\n...sampling at temperature %f...\n\n", T);
        sample(E_data, M_data, E_num, tau, L, T);

        free(f_cpy);
        free(s_cpy);
        free(E_data);
        free(M_data);
        fprintf(stderr,"\n\n...exited successfully....\n\n");
		} else if (strcmp("--jk", s) == 0) {
      FILE *fp;
      char *fn, *f_cpy, *s_cpy, buf[1024];
      int opt, num, L;
      double *data, T, err, val;

      fn = argv[2];
      f_cpy = strdup(fn);
      s_cpy = strdup(fn);
      opt = atoi(argv[3]);

      T = atof(strtok((rindex(f_cpy, '=') + 1), "dat"));
      L = atoi(strtok((rindex(s_cpy, 'L') + 1), "/"));
      sprintf(buf, "../measurements/L%d/%s", L, data_type(opt));
      fp = fopen(buf, "a");

      fprintf(stderr,"\n\n...measuring at temperature %f...\n\n", T);
      switch (opt) {
        case 0: // mean_mag
          read_data(fn, 1, &data, &num);
          jackknife(num < 8000 ? num : 8000, data, 1.0 / T, &err, &val,\
            mean, mean_mag, jackknife_mean);
          fprintf(fp, "%f\t%f\t%f\n", T, val, err);
          break;
        case 1: // mean_energy
          read_data(fn, 2, &data, &num);

          jackknife(num < 8000 ? num : 8000, data, 1.0 / T, &err, &val,\
            mean, mean_energy, jackknife_mean);
          fprintf(fp, "%f\t%f\t%f\n", T, val, err);
          break;
        case 2: // magnetic_susceptibility
          read_data(fn, 1, &data, &num);
          jackknife(num < 8000 ? num : 8000, data, 1.0 / T, &err, &val,\
            variance, magnetic_susceptibility, jackknife_variance);
          fprintf(fp, "%f\t%f\t%f\n", T, val, err);
          break;
        case 3: // specific_heat
          read_data(fn, 2, &data, &num);
          jackknife(num < 8000 ? num : 8000, data, 1.0 / T, &err, &val,\
            variance, specific_heat, jackknife_variance);
          fprintf(fp, "%f\t%f\t%f\n", T, val, err);
          break;
        default:
          break;
      }
      fprintf(stderr,"\n\n...exited successfully...\n\n");
    } else if (strcmp("--p", s) == 0) {
      char *fn, *f_cpy;
      int opt;

      fn  = argv[2];
      opt = atoi(argv[3]);
      f_cpy = strdup(fn);
      L = atoi(strtok((rindex(f_cpy, 'L') + 1), "/"));
      plot_measurements(fn, opt, L);
    } else {
        fprintf(stderr, "\ninvalid command line args. exiting...\n");
    }

    exit(0);
}

char *data_type(int opt)
{
  switch(opt) {
    case 0:
      return "mag-temp.dat";
    case 1:
      return "energy-temp.dat";
    case 2:
      return "magsus-temp.dat";
    case 3:
      return "speci-temp.dat";
    default:
      return EXIT_SUCCESS;
    break;
  }

  return EXIT_SUCCESS;
}
