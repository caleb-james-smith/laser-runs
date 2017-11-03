# python is happy
# make happy plots
import json
import matplotlib.pyplot as plt
import numpy as np
from argparse import ArgumentParser

def getData(data_dir):
    sipm_file = "sipm_table.txt"
    pd_file = "pd_table.txt"
    dictionaries = [ { "file" : sipm_file, "tag" : "sipm", "element" : "rm" },
                     { "file" : pd_file,   "tag" : "pd",   "element" : "pd" } ]
    data = {}
    
    data["sipm"] = []
    data["rm1"] = []
    data["rm2"] = []
    data["rm3"] = []
    data["rm4"] = []
    
    data["pd"] = []
    data["pd0"] = []
    data["pd1"] = []
    data["pd2"] = []
    data["pd3"] = []
    data["pd4"] = []
    data["pd5"] = []
    data["pd_quartz"] = []
    data["pd_megatile"] = []
    
    # sipm array: {CU,RBX,Run,RM,sipm_ch,uhtr_ch,shunt,max_adc,max_fc,result}
    # pd array: {CU,RBX,Run,pd_ch,uhtr_ch,shunt,max_adc,max_fc,result}
    for dictionary in dictionaries:
        f = dictionary["file"]
        tag = dictionary["tag"]
        element = dictionary["element"]
        fc_position = -2
        element_position = 3
        with open(data_dir + f) as df:
            for line in df:
                if line[0] == "#":
                    continue
                s = line.split()
                if int(s[-1]) == 0:
                    # skip data that does not pass
                    print line
                    continue
                d = float(s[fc_position])
                element_value = int(s[element_position])
                element_name = "%s%d" % (element, element_value)
                data[tag].append(d)
                data[element_name].append(d)
   
    data["pd_quartz"] = data["pd0"] + data["pd0"]
    data["pd_megatile"] = data["pd2"] + data["pd3"] + data["pd4"] + data["pd5"]

    return data

def plotHisto(data, plot_dir, info):
    name = info["name"]
    title = info["title"]
    xtitle = info["xtitle"]
    ytitle = info["ytitle"]
    nbins = info["nbins"]
    units = info["units"]
    xstat = info["xstat"]
    ystat = info["ystat"]
    data_list = data[name]
    entries = len(data_list)
    mean = np.mean(data_list)
    std = np.std(data_list)
    var = 100* std / mean
    stat_string = "Num. Entries = %d\n" % entries
    stat_string += "Mean = %.2f %s\n" % (mean, units)
    stat_string += "St. Dev. = %.2f %s\n" % (std, units)
    stat_string += "Variation = %.2f %%" % var
    plt.hist(data_list, bins=nbins)
    plt.text(xstat, ystat, stat_string)
    plt.title(title)
    plt.xlabel(xtitle)
    plt.ylabel(ytitle)
    plt.savefig(plot_dir + name + ".png")
    plt.savefig(plot_dir + name + ".pdf")
    plt.clf()

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--config",   "-c", default="config/passed.json", help="json config file")
    parser.add_argument("--data_dir", "-d", default="passed_cu_data",     help="directory containing data tables")
    parser.add_argument("--plot_dir", "-p", default="passed_plots",       help="directory to save plots")
    
    options = parser.parse_args()
    plot_dir = options.plot_dir
    data_dir = options.data_dir
    config = options.config

    if plot_dir[-1] != "/":
        plot_dir += "/"
    if data_dir[-1] != "/":
        data_dir += "/"
    
    with open(config) as json_file:
        info = json.load(json_file)

    data = getData(data_dir)
    
    for key in info:
        plotHisto(data, plot_dir, info[key])


