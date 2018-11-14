import csv
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import sys


if __name__ == '__main__':
    x = []
    fn = sys.argv[1]

    a = fn.split('=')[1]
    c = fn.split('L')[1]

    L = c.split('/')[0]
    T = a.split('.dat')[0]

    with open(fn) as csv_file:
        line_count = 0;
        csv_reader = csv.reader(csv_file, delimiter='\t')
        for row in csv_reader:
            if line_count > 1:
                x.append(float(row[1]));
            line_count += 1

    num_bins = 100
    n, bins, patches = plt.hist(x, num_bins, facecolor='blue', alpha=1.0)
    plt.xlabel('Magnetization per spin')
    plt.ylabel('Frequencies')
    plt.title(r'Histogram of magnetization: T = {}, L = {}'.format(T, L))
    plt.savefig('../plots/L{}/histogram_{}_{}.png'.format(L, L, T))
