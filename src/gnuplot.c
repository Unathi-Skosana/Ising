#include "../lib/utils.h"
#include "../lib/gnuplot.h"


void plot_measurements(char *fn, int opt, int L)
{
    FILE *fp;
    fp = popen("gnuplot -persist","w");

    switch(opt) {
      case 0:
        fprintf(fp, "unset key\n");
        fprintf(fp, "set title 'T v. |M/N| time for L=%d\n", L);
        fprintf(fp, "set xrange [0:*]\n");
        fprintf(fp, "set xlabel 'T'\n");
        fprintf(fp, "set ylabel 'magnetization per spin'\n");
        fprintf(fp, "set term pngcairo size 1100, 800\n");
        fprintf(fp, "set output '../plots/L%d/mag.png'\n", L);
        fprintf(fp, "plot \"%s\" using 1:2:3 with yerrorbars\n", fn);
        break;
      case 1:
        fprintf(fp, "unset key\n");
        fprintf(fp, "set title 'T v. |E/N| for L=%d\n", L);
        fprintf(fp, "set xrange [0:*]\n");
        fprintf(fp, "set xlabel 'T'\n");
        fprintf(fp, "set ylabel 'energy per spin'\n");
        fprintf(fp, "set term pngcairo size 1100, 800\n");
        fprintf(fp, "set output '../plots/L%d/energy.png'\n", L);
        fprintf(fp, "plot \"%s\" using 1:2:3 with yerrorbars\n", fn);
        break;
      case 2:
        fprintf(fp, "unset key\n");
        fprintf(fp, "set title 'T v. X time for L=%d\n", L);
        fprintf(fp, "set xrange [0:*]\n");
        fprintf(fp, "set xlabel 'T'\n");
        fprintf(fp, "set ylabel 'magnetic susceptibility'\n");
        fprintf(fp, "set term pngcairo size 1100, 800\n");
        fprintf(fp, "set output '../plots/L%d/magnetic-susceptibility.png'\n", L);
        fprintf(fp, "plot \"%s\" using 1:2:3 with yerrorbars\n", fn);
        break;
      case 3:
        fprintf(fp, "unset key\n");
        fprintf(fp, "set title 'T v. C for L=%d\n", L);
        fprintf(fp, "set xrange [0:*]\n");
        fprintf(fp, "set xlabel 'T'\n");
        fprintf(fp, "set ylabel 'specific heat'\n");
        fprintf(fp, "set term pngcairo size 1100, 800\n");
        fprintf(fp, "set output '../plots/L%d/specific-heat.png'\n", L);
        fprintf(fp, "plot \"%s\" using 1:2:3 with yerrorbars\n", fn);
        break;
      default:
        break;
    }
}

void plot_correlation_times()
{
  double xs[110] = {0.100, 0.200, 0.300, 0.500, 0.60, 0.650, 0.700, 0.725, 0.750,\
    0.775, 0.800, 0.825, 0.850, 0.875, 0.900, 0.905, 0.910, 0.915, 0.920, 0.925, 0.930,
    0.935, 0.940 ,0.945, 0.950, 0.955, 0.960, 0.965, 0.970, 0.975, 0.980, 0.985,\
    0.990, 0.995, 1.000, 1.005, 1.010, 1.015, 1.020, 1.025, 1.030, 1.035, 1.040,\
    1.045, 1.055, 1.060, 1.065, 1.070, 1.075, 1.080, 1.085, 1.090, 1.095, 1.100,\
    1.105, 1.110, 1.115, 1.120, 1.125, 1.130, 1.135, 1.140, 1.145, 1.150, 1.155,\
    1.160, 1.165, 1.170, 1.175, 1.180, 1.185, 1.190, 1.195, 1.200, 1.205, 1.210,\
    1.215, 1.220, 1.225, 1.230, 1.235, 1.240, 1.245, 1.250, 1.255, 1.260, 1.265,\
    1.270, 1.275, 1.280, 1.285, 1.290, 1.295, 1.300, 1.305, 1.310, 1.315, 1.320,\
    1.325, 1.330, 1.335, 1.340, 1.345, 1.350, 1.400, 1.500, 1.60, 1.70, 1.75, 1.80};

  int taus100[110] = {2,2,3,3,3,3,3,3,4,4,4,5,6,8,11,12,13,15,16,19,20,25,30,\
      37,41,52,69,83,99,232,200,596,1411,1986,1594,2108,1552,1298,936,682,492,\
      360,293,264,167,134,114,107,92,81,71,64,57,51,46,44,39,37,34,31,29,26,26,\
      24,22,22,20,19,18,18,17,16,15,15,14,14,13,13,12,12,11,11,11,11,10,10,10,\
      9,9,9,9,8,8,8,8,8,8,7,7,7,7,7,7,7,6,4,4,3,3,3};

    FILE *fp;
    fp = popen("gnuplot -persist","w");
    fprintf(fp, "set title 'T/Tc v. autocorrelation time for L=100\n");
    fprintf(fp, "set xrange [0:*]\n");
    fprintf(fp, "set xlabel 'T/Tc'\n");
    fprintf(fp, "set ylabel 'autocorrelation times'\n");
    fprintf(fp, "set term pngcairo size 1100, 800\n");
    fprintf(fp, "set output '../autocor_times/L100/times.png'\n");
    fprintf(fp, "plot '-' with linespoints\n");

    for (int i = 0; i < 110; i++) {
      fprintf(fp, "%f %d\n", xs[i], taus100[i]);
    }

    fprintf(fp, "e\n");
    pclose(fp);
}


void plot_lattice(char *fn)
{
    FILE *fp;
    fp = popen("gnuplot -persist","w");

    // Magic
    char *f_cpy = strdup(fn);
    char *s_cpy = strdup(fn);

    int lat_sz = atoi(strtok((rindex(f_cpy, 'L') + 1), "/"));
    double sim_temp = atof(strtok((rindex(s_cpy, '=') + 1), "dat"));

    fprintf(fp, "unset key\n");
    fprintf(fp, "set cbrange [-1:1]\n");
    fprintf(fp, "set xrange [0:%d]\n", lat_sz - 1);
    fprintf(fp, "set yrange [0:%d]\n", lat_sz - 1);
    fprintf(fp, "set term pngcairo size 1100, 800\n");
    fprintf(fp, "set output '../plots/L%d/lat_%ld&T=%.5f.png'\n", lat_sz, time(NULL), sim_temp);
    fprintf(fp, "plot \"%s\" matrix with image\n", fn);
    fprintf(fp, "set terminal x11\n");
    fprintf(fp, "set output\n");
    fprintf(fp, "replot\n");
    pclose(fp);
    free(f_cpy);
    free(s_cpy);
}

void config_physics_gnuplot()
{
    gnuplot = popen("gnuplot -persist","w");
    fprintf(gnuplot, "set key outside\n");
    fprintf(gnuplot, "set xrange [0:%d]\n", sweeps);
    fprintf(gnuplot, "set xlabel 'time'\n");
    fprintf(gnuplot, "set term pngcairo size 1100, 800\n");
    fprintf(gnuplot, "set output '../plots/L%d/%ld&T=%.5f.png'\n", L, time(NULL), 1.0/beta);
    fprintf(gnuplot, "set grid back\n");

    switch(physics_plot) {
        case 0:
            fprintf(gnuplot, "set yrange [-1:1]\n");
            fprintf(gnuplot, "set ylabel '|M/N|'\n");
            fprintf(gnuplot, "set title '|M/N| vs t for L = %d, T = %f'\n", L, 1.0 / beta);
            break;
        case 1:
            fprintf(gnuplot, "set yrange [-2:2]\n");
            fprintf(gnuplot, "set ylabel '|E/N|'\n");
            fprintf(gnuplot, "set title '|E/N| vs t for L = %d, T = %f'\n", L, 1.0 / beta);
            break;
        case 2:
            fprintf(gnuplot, "set yrange [-2:2]\n");
            fprintf(gnuplot, "set ylabel 'values'\n");
            fprintf(gnuplot, "set title '|E/N| and |M/N| vs time for L = %d, T = %f'\n", L, 1.0 / beta);
            break;
        default:
            break;
    }
}

void start_physics_gnuplot()
{
    switch(physics_plot) {
        case 0:
            fprintf(gnuplot, "plot \"%s\" using 1:2 title '|M/N|' with points lt 2, \"%s\" \
                    using 1:4 title 'Exact M' with lines lt 7\n", sim_data_name, sim_data_name);
            break;
        case 1:
            fprintf(gnuplot, "plot \"%s\" using 1:3 title '|E/N|' with points lt 3\n", sim_data_name);
            break;
        case 2:
            fprintf(gnuplot, "plot \"%s\" using 1:2 title '|M/N|' with points lt 2, \"%s\" \
                    using 1:3 title '|E/N|' with points lt 7,\"%s\" \
                    using 1:4 title 'Exact |M/N|' with lines lt 4\n", sim_data_name, sim_data_name, sim_data_name);
            break;
         default:
            break;

    }

    fprintf(gnuplot, "set terminal x11\n");
    fprintf(gnuplot, "set output\n");
    fprintf(gnuplot, "replot\n");
    pclose(gnuplot);
}


void config_autocor_gnuplot()
{

    char *f_cpy = strdup(autocor_fn);
    char *s_cpy = strdup(autocor_fn);

    // Magic
    double file_temperature = atof(strtok((rindex(f_cpy, '=') + 1), "dat"));
    int file_lat_size = atoi(strtok((rindex(s_cpy, 'L') + 1), "/"));

    gnuplot = popen("gnuplot -persist","w");
    fprintf(gnuplot, "set title 'time v. |M/N| autocorrelation for L=%d,T=%f\n", file_lat_size, file_temperature);
    fprintf(gnuplot, "set xrange [0:%d]\n", autocor_plot_xmax);
    fprintf(gnuplot, "set xlabel 't'\n");
    fprintf(gnuplot, "set ylabel '|M/N| autocorrelation'\n");
    fprintf(gnuplot, "set arrow from 0,%f to %d,%f nohead lc rgb \'blue\'\n", 1/exp(1), autocor_plot_xmax, 1/exp(1));
    fprintf(gnuplot, "show arrow\n");
    fprintf(gnuplot, "plot '-' with lines\n");
    free(f_cpy);
    free(s_cpy);
}

void start_autocor_gnuplot()
{
    fprintf(gnuplot, "e\n");
    pclose(gnuplot);
}
