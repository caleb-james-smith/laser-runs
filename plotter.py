# python is happy
# make happy plots

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
    parser.add_argument("--data_dir", "-d", default="data",  help="directory containing data tables")
    parser.add_argument("--plot_dir", "-p", default="plots", help="directory to save plots")
    
    options = parser.parse_args()
    plot_dir = options.plot_dir
    data_dir = options.data_dir
    
    if plot_dir[-1] != "/":
        plot_dir += "/"
    if data_dir[-1] != "/":
        data_dir += "/"
    
    data = getData(data_dir)
    
    sipmInfo = {}
    sipmInfo["name"] = "sipm"
    sipmInfo["title"] = "Max Charge for SiPM Channels"
    sipmInfo["xtitle"] = "Maximum fC"
    sipmInfo["ytitle"] = "Number of Channels"
    sipmInfo["units"] = "fC"
    sipmInfo["nbins"] = 50
    sipmInfo["xstat"] = 200000
    sipmInfo["ystat"] = 200

    rm1Info = {}
    rm1Info["name"] = "rm1"
    rm1Info["title"] = "Max Charge for SiPM Channels, RM 1"
    rm1Info["xtitle"] = "Maximum fC"
    rm1Info["ytitle"] = "Number of Channels"
    rm1Info["units"] = "fC"
    rm1Info["nbins"] = 20
    rm1Info["xstat"] = 80000
    rm1Info["ystat"] = 60

    rm2Info = {}
    rm2Info["name"] = "rm2"
    rm2Info["title"] = "Max Charge for SiPM Channels, RM 2"
    rm2Info["xtitle"] = "Maximum fC"
    rm2Info["ytitle"] = "Number of Channels"
    rm2Info["units"] = "fC"
    rm2Info["nbins"] = 20
    rm2Info["xstat"] = 250000
    rm2Info["ystat"] = 80

    rm3Info = {}
    rm3Info["name"] = "rm3"
    rm3Info["title"] = "Max Charge for SiPM Channels, RM 3"
    rm3Info["xtitle"] = "Maximum fC"
    rm3Info["ytitle"] = "Number of Channels"
    rm3Info["units"] = "fC"
    rm3Info["nbins"] = 20
    rm3Info["xstat"] = 50000
    rm3Info["ystat"] = 70

    rm4Info = {}
    rm4Info["name"] = "rm4"
    rm4Info["title"] = "Max Charge for SiPM Channels, RM 4"
    rm4Info["xtitle"] = "Maximum fC"
    rm4Info["ytitle"] = "Number of Channels"
    rm4Info["units"] = "fC"
    rm4Info["nbins"] = 20
    rm4Info["xstat"] = 0
    rm4Info["ystat"] = 100

    pdInfo = {}
    pdInfo["name"] = "pd"
    pdInfo["title"] = "Max Charge for Pin-Diode Channels"
    pdInfo["xtitle"] = "Maximum fC"
    pdInfo["ytitle"] = "Number of Channels"
    pdInfo["units"] = "fC"
    pdInfo["nbins"] = 30
    pdInfo["xstat"] = 60000
    pdInfo["ystat"] = 7.5
    
    pd0Info = {}
    pd0Info["name"] = "pd0"
    pd0Info["title"] = "Max Charge for Quartz Fiber Pin-Diode Channel 0"
    pd0Info["xtitle"] = "Maximum fC"
    pd0Info["ytitle"] = "Number of Channels"
    pd0Info["units"] = "fC"
    pd0Info["nbins"] = 20
    pd0Info["xstat"] = 15000
    pd0Info["ystat"] = 5
    
    pd1Info = {}
    pd1Info["name"] = "pd1"
    pd1Info["title"] = "Max Charge for Quartz Fiber Pin-Diode Channel 1"
    pd1Info["xtitle"] = "Maximum fC"
    pd1Info["ytitle"] = "Number of Channels"
    pd1Info["units"] = "fC"
    pd1Info["nbins"] = 20
    pd1Info["xstat"] = 10000
    pd1Info["ystat"] = 5
    
    pdQuartzInfo = {}
    pdQuartzInfo["name"] = "pd_quartz"
    pdQuartzInfo["title"] = "Max Charge for Quartz Fiber Pin-Diode Channels 0 and 1"
    pdQuartzInfo["xtitle"] = "Maximum fC"
    pdQuartzInfo["ytitle"] = "Number of Channels"
    pdQuartzInfo["units"] = "fC"
    pdQuartzInfo["nbins"] = 30
    pdQuartzInfo["xstat"] = 15000
    pdQuartzInfo["ystat"] = 5
    
    pdMegatileInfo = {}
    pdMegatileInfo["name"] = "pd_megatile"
    pdMegatileInfo["title"] = "Max Charge for Megatile Pin-Diode Channels 2, 3, 4, and 5"
    pdMegatileInfo["xtitle"] = "Maximum fC"
    pdMegatileInfo["ytitle"] = "Number of Channels"
    pdMegatileInfo["units"] = "fC"
    pdMegatileInfo["nbins"] = 30
    pdMegatileInfo["xstat"] = 0
    pdMegatileInfo["ystat"] = 6
    
    plotHisto(data, plot_dir, sipmInfo)
    plotHisto(data, plot_dir, rm1Info)
    plotHisto(data, plot_dir, rm2Info)
    plotHisto(data, plot_dir, rm3Info)
    plotHisto(data, plot_dir, rm4Info)
    plotHisto(data, plot_dir, pdInfo)
    plotHisto(data, plot_dir, pd0Info)
    plotHisto(data, plot_dir, pd1Info)
    plotHisto(data, plot_dir, pdQuartzInfo)
    plotHisto(data, plot_dir, pdMegatileInfo)


