# python is happy
# make happy plots

import matplotlib.pyplot as plt
import numpy as np
from argparse import ArgumentParser

def getData(data_dir):
    sipm_file = "sipm_table.txt"
    pd_file = "pd_table.txt"
    data = {}
    dataNames = ["sipm", "pd"]
    i = 0
    for f in [sipm_file, pd_file]:
        data_list = []
        with open(data_dir + f) as df:
            for line in df:
                if line[0] == "#":
                    continue
                s = line.split()
                d = float(s[-2])
                data_list.append(d)
                print d
        data[dataNames[i]] = data_list
        i += 1

    return data

def plot(data, plot_dir):
    n_bins = 50
    #N_points = 1000
    #x = np.random.randn(N_points)
    #y = .4 * x + np.random.randn(N_points) + 5
    fig, axs = plt.subplots(1, 2, sharey=True, tight_layout=True)
    axs[0].hist(data["sipm"], bins=n_bins)
    axs[1].hist(data["pd"],   bins=n_bins)
    plt.savefig("plots/histo.png")

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--plot_dir", "-p", default="plots", help="directory containing plots")
    parser.add_argument("--data_dir", "-d", default="data",  help="directory containing data")
    options = parser.parse_args()
    plot_dir = options.plot_dir
    data_dir = options.data_dir
    if plot_dir[-1] != "/":
        plot_dir += "/"
    if data_dir[-1] != "/":
        data_dir += "/"
    data = getData(data_dir)
    plot(data, plot_dir)


