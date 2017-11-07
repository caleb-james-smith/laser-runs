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
    data["pd"] = []
    
    data["pd_quartz"] = []
    data["pd_megatile"] = []
    
    for pd in xrange(6):
        element_name = "pd%d" % pd
        data[element_name] = []
        data["%s_ave" % element_name] = []
    for rm in xrange(1,5):
        element_name = "rm%d" % rm
        data[element_name] = []
        data["%s_ave" % element_name] = []

    # sipm array: {CU,RBX,Run,RM,sipm_ch,uhtr_ch,shunt,max_adc,max_fc,result}
    # pd array: {CU,RBX,Run,pd_ch,uhtr_ch,shunt,max_adc,max_fc,result}
    cu_list = []
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
                #if int(s[-1]) == 0:
                    # skip data that does not pass
                    #print line
                    #continue
                d = float(s[fc_position])
                element_index = int(s[element_position])
                element_name = "%s%d" % (element, element_index)
                data[tag].append(d)
                data[element_name].append(d)
                
                # data organized by CU
                cu = int(s[0]) 
                if cu not in cu_list:
                    cu_list.append(cu) 
                    cu_name = "cu%d" % cu
                    data[cu_name] = {}
                    for pd in xrange(6):
                        data[cu_name]["pd%d" % pd] = []
                    for rm in xrange(1,5):
                        data[cu_name]["rm%d" % rm] = []

                data["cu%d"%cu][element_name].append(d)
   
    data["pd_quartz"] = data["pd0"] + data["pd0"]
    data["pd_megatile"] = data["pd2"] + data["pd3"] + data["pd4"] + data["pd5"]
    
    for cu in cu_list:
        cu_key = "cu%d" % cu
        for pd in xrange(6):
            element_key ="pd%d" % pd 
            ave = np.mean(data[cu_key][element_key])
            data["%s_ave" % element_key].append(ave)
        for rm in xrange(1,5):
            element_key ="rm%d" % rm
            ave = np.mean(data[cu_key][element_key])
            data["%s_ave" % element_key].append(ave)
    
    #for key in data:
    #    print "length of {0} data: {1}".format(key, len(data[key]))

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

def plotScatter(x, y, plot_dir, info):
    name = info["name"]
    title = info["title"]
    xtitle = info["xtitle"]
    ytitle = info["ytitle"]
    xstat = info["xstat"]
    ystat = info["ystat"]
    
    # calculate fit function
    z = np.polyfit(x, y, 1)
    f = np.poly1d(z)
    f_string = str(f)
    f_string = f_string.split("\n")[-1]
    f_string = "f(x) = {0}".format(f_string)
    print f_string

    # calculate new x's and y's using fit function
    x_new = np.linspace(min(x), max(x), 50)
    y_new = f(x_new)

    plt.plot(x,y,'o', x_new, y_new)
    plt.text(xstat, ystat, f_string)
    plt.title(title)
    plt.xlabel(xtitle)
    plt.ylabel(ytitle)
    plt.savefig(plot_dir + name + ".png")
    plt.savefig(plot_dir + name + ".pdf")
    plt.clf()

def plotRMvsPD(data, plot_dir, info):
    for pd in xrange(2):
        name = info["name"]
        title = info["title"]
        xtitle = info["xtitle"]
        ytitle = info["ytitle"]
        xstat = info["xstat"]
        ystat = info["ystat"]
        f_box = ""
        
        x = data["pd%d_ave"%pd] 
        print "number of pd %d ave: %d" % (pd, len(x))
        print "x = {0}".format(x)
        
        for rm in xrange(1,5):
            
            y = data["rm%d_ave"%rm] 
            print "number of rm %d ave: %d" % (rm, len(y))
            print "y = {0}".format(y)
            
            # calculate fit function
            z = np.polyfit(x, y, 1)
            f = np.poly1d(z)
            f_string = str(f)
            f_string = f_string.split("\n")[-1]
            f_string = "RM {0}: f(x) = {1}".format(rm, f_string)
            f_box += f_string + "\n"
            print f_string

            # calculate new x's and y's using fit function
            x_new = np.linspace(min(x), max(x), 50)
            y_new = f(x_new)

            plt.plot(x,y,'o', x_new, y_new)
        
        plt.text(xstat, ystat, f_box)
        
        title += " %d" % pd
        xtitle += " %d Max fC" % pd
        
        axes = plt.gca()
        plt.title(title)
        plt.xlabel(xtitle)
        plt.ylabel(ytitle)
        axes.set_xlim([0, 70000])
        axes.set_ylim([30000, 200000])
        plt.savefig(plot_dir + name + "_pd%d.png" % pd)
        plt.savefig(plot_dir + name + "_pd%d.pdf" % pd)
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

    # PD1 vs PD0
    pd1_pd0_info = {}
    pd1_pd0_info["name"] = "pd1_pd0"
    pd1_pd0_info["title"] = "Pin Diode 1 vs Pin Diode 0"
    pd1_pd0_info["xtitle"] = "Pin Diode 0 Max fC"
    pd1_pd0_info["ytitle"] = "Pin Diode 1 Max fC"
    pd1_pd0_info["xstat"] = 20000 
    pd1_pd0_info["ystat"] = 50000

    # RM1-4 vs PD0 and PD1
    rm_pd_info = {}
    rm_pd_info["name"] = "rm_pd"
    rm_pd_info["title"] = "SiPMs vs Pin Diode"
    rm_pd_info["xtitle"] = "Pin Diode"
    rm_pd_info["ytitle"] = "SiPM Max fC"
    rm_pd_info["xstat"] = 5000
    rm_pd_info["ystat"] = 160000

    x = data["pd0"]
    y = data["pd1"]
    plotScatter(x, y, plot_dir, pd1_pd0_info)

    plotRMvsPD(data, plot_dir, rm_pd_info)


