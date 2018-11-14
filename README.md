# CP344

Repository for computational physics class

## Running

```
# autocorrelating

./run.sh --a filename tmax

# filename  = name of file containing data points
# tmax     = largest x value on gnuplot plot

# simulating for iteratively for multiple temperature, predefined in sample_temps array in utils.h

./run.sh --siter lattice-size spin sweeps plot-type

# lattice-size = 20, 80, 100, 256
# spin = -1, 0 or 1 where -1 = down aligned, 0 = randomly aligned, 1 = up aligned
# sweeps = number of sweeps
# plot-type = 0, 1, 2 where 0 = magnetization plot, 1 = energy plot or 2 = energy and magnetization plot

# jackknifing data

./run.sh --jk filename opt

# filename  = name of file to jackknife data from
# opt       = 0 = mean magnetization, 1 = mean energy, 2 = magneticsusceptibility  or 3 = specific heat

# plotting measurement data

./run.sh --p filename  opt

# filename  = name of file containing the measurement determined by opt
# opt       = 0 = mean magnetization, 1 = mean energy, 2 = magnetic susceptibility  or 3 = specific heat

# plotting spin lattice with pixel squares.

./run.sh --hm filename

# filename  = name of file containing the heat map data
# opt       = 0 = mean magnetization, 1 = mean energy, 2 = magnetic susceptibility  or 3 = specific heat

# sampling the trancuted data

./run.sh --sa filename

# filename  = name of file containing the heat map data
# opt       = 0 = mean magnetization, 1 = mean energy, 2 = magnetic susceptibility  or 3 = specific heat

```

Refer to scripts dir to use the above functions.
