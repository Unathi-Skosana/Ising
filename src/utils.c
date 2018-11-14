#include "../lib/utils.h"
#include "../lib/physics.h"
#include "../lib/gnuplot.h"

#define ANIMATE 0
#define SPEED   60000


void *init_lat(int r, int spin, double temp, int tmax, int plot)
{
    N = r * r;
    L = r;
    lat = calloc(N, sizeof(int));
    beta = 1.0 / temp;
    sweeps = tmax;
    physics_plot = plot;
    snprintf(eq_data_name, sizeof(eq_data_name), "../eq_sim_data/L%d/%ld&T=%.5f.dat", L, time(NULL), 1.0/beta);
    snprintf(sim_data_name, sizeof(sim_data_name), "../sim_data/L%d/%ld&T=%.5f.dat", L, time(NULL), 1.0/beta);

    data = fopen(sim_data_name, "w");
    eq_data = fopen(eq_data_name, "w");

    init_prob();
    srand(time(NULL));

    for (int k = 0; k < N; ++k) {
        if (spin == -1) lat[k] = -1;
        if (spin == 0)  lat[k] = 2 * floor(2.0 * rand() / RAND_MAX) - 1;
        if (spin == 1)  lat[k] = 1;
    }

    E = calc_lat_energy();
    M = calc_lat_magnetization();
    mag = analytic_magnetization();

    return EXIT_SUCCESS;
}

void *reinit_lat(double temp)
{
    beta = 1.0 / temp;
    snprintf(eq_data_name, sizeof(eq_data_name), "../eq_sim_data/L%d/%ld&T=%.5f.dat", L, time(NULL), 1.0/beta);
    snprintf(sim_data_name, sizeof(sim_data_name), "../sim_data/L%d/%ld&T=%.5f.dat", L, time(NULL), 1.0/beta);

    data = fopen(sim_data_name, "w");
    eq_data = fopen(eq_data_name, "w");

    init_prob();

    E = calc_lat_energy();
    M = calc_lat_magnetization();
    mag = analytic_magnetization();

    return EXIT_SUCCESS;
}

double d_abs(double v)
{
  return sqrt(v*v);
}

int get_lat_energy()
{
    return E;
}

int get_lat_magnetization()
{
    return M;
}

void init_prob()
{
    int i;
    for (i = 2; i < 5; i += 2) prob[i] = exp(-2*beta*i);
}


void sweep()
{
    int i, j, k;
    int nn, sum, delta;
    double r;
    double old_E = E;

    int t_step = N;
    int t_eq = 0;
    int is_eq = 0, confirm_eq = 0;

    fprintf(data, "#%d\n", sweeps);
    fprintf(data, "#time\tmagnetization\tenergy\tanalytic_magnetization\n");
    config_physics_gnuplot();
    sum = 0, j = -1;

    while (++j < sweeps) {

        for (k = 0; k < N; k++) {
            i = rand() % N;

            if ((nn = i+XNN) >= N)   nn -= N;
            sum = lat[nn];

            if ((nn = i-XNN) <  0)   nn += N;
            sum += lat[nn];

            if ((nn = i+YNN) >= N)   nn -= N;
            sum += lat[nn];

            if ((nn = i-YNN) <  0)   nn += N;
            sum += lat[nn];

            delta = sum * lat[i];
            r = 1.0 * rand() / RAND_MAX;

            if (delta <= 0) {
              lat[i] = -lat[i];
              E += delta;
              M += 2 * lat[i];

            } else if (r <  prob[delta]) {
              lat[i] = -lat[i];
              E += delta;
              M += 2 * lat[i];
            }
        }

        if (j % t_step == 0 && j > 0 && !is_eq) {
            is_eq = find_equilibrium(E, old_E);
            old_E = E;
            if (is_eq && !confirm_eq) {
              confirm_eq = 1;
              is_eq = 0;
            } else if (!is_eq && confirm_eq) {
              confirm_eq = 0;
            } else if (is_eq && confirm_eq){
              printf("Equilibrium time step %d\n", j);
              fprintf(eq_data, "#%d\n", sweeps - j);
              fprintf(eq_data, "#time\t|M/N|\t|E/N|\texact |M|\n");
              t_eq = j;
            }
        }

        if (is_eq && confirm_eq) {
          fprintf(eq_data, "%d\t%f\t%f\t%f\n", j - t_eq, (1.0 * M / N), (1.0 * E / N), mag);
        }

        console_log(j, delta);
        fprintf(data, "%d\t%f\t%f\t%f\n", j, (1.0 * M / N), (1.0 * E / N), mag);
    }

    fclose(eq_data);
    fclose(data);
    start_physics_gnuplot();
    //write_lat();
}


void console_log(int j, int delta)
{
    #if ANIMATE
        system("clear");
        printf("\n\n");
        print_lat();
        printf("\n\n");
        printf("M / N : %f\nE / N : %f\ndE : %d J\nT : %f K\n", 1.0 * M/N, 1.0 * E/N, delta, 1.0/beta);
        usleep(SPEED);
    #endif
}


void print_lat()
{
    int i;
    for (i = 0; i < N; ++i) {
        if (i % L == 0 && i != 0) printf("\n");
        lat[i] == 1 ? printf("+") : printf(" ");
    }
    printf("\n\n");
}


void read_data(char *fn, int row, double **data, int *tmax)
{
    FILE *fp;
    int i = 0, j;
    char line[4098];
    fp = fopen(fn, "r");

    while (fgets(line, 4098, fp)) {
      j = 0;
      const char* tok;
        if (i < 2) {
            if (i == 0) {
                tok = strtok(line, "#\n");
                *tmax = atoi(tok);
                *data = (double *) malloc(*tmax * sizeof(double));
            }
        } else {
            for (tok = strtok(line, "\t"); tok && *tok;tok = strtok(NULL, "\t\n")) {
                if (j == row) {
                  data[0][i - 2] = atof(tok);
                }
                j = (j + 1) % 4;
            }
        }
	    ++i;
    }
    fclose(fp);
}

void write_lat()
{
    FILE *final_state;
    char fn[4098];
    int i = 0, j = 0;

    snprintf(fn, sizeof(fn), "../lats/L%d/%ld&T=%.5f.dat",L , time(NULL), 1.0 / beta);
    final_state = fopen(fn, "w");

    if (final_state == NULL) {
        printf("Error in opening the file");
        exit(0);
    }

    for (i = 0; i < N; ++i) {
        if (i % L == 0 && i != 0) {
            fprintf(final_state, "\n");
            j += 1;
        }
       fprintf(final_state, " %d", lat[i]);
    }
    fclose(final_state);
}


/*

References

***************************************************************************************
*    Title: Equilibrium check snippet
*    Author: rohanlolm
*    Date: 2016
*    Availability: https://github.com/rohanlolm/Ising_Magentism_Simulator_C/blob/master/Ising_sim.c
*
***************************************************************************************

***************************************************************************************
*    Title: Tokenizing data from a file snippet
*    Author: amirmasoudabdol
*    Date: 2016
*    Availability: https://gist.github.com/amirmasoudabdol/f1efda29760b97f16e0e
*
***************************************************************************************

*/
