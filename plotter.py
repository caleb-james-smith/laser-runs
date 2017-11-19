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
            if data[cu_key][element_key]:
                ave = np.mean(data[cu_key][element_key])
                data["%s_ave" % element_key].append(ave)
            else:
                print "No pin-diode data for {0} {1}".format(cu_key, element_key)
        for rm in xrange(1,5):
            element_key ="rm%d" % rm
            if data[cu_key][element_key]:
                ave = np.mean(data[cu_key][element_key])
                data["%s_ave" % element_key].append(ave)
            else:
                print "No sipm data for {0} {1}".format(cu_key, element_key)
    
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

def plotScatter(plot_dir, info):
    raw_colors = ["pinkish red","azure","bluish green","electric purple","tangerine","neon pink","dark sky blue","avocado"]
    colors = list("xkcd:{0}".format(c) for c in raw_colors)
    name = info["name"]
    ynames = info["ynames"]
    x = info["xdata"]
    ydata = info["ydata"]
    title = info["title"]
    xtitle = info["xtitle"]
    ytitle = info["ytitle"]
    xstat = info["xstat"]
    ystat = info["ystat"]
    plotFit = info["plotfit"]
    setRange = info["setrange"] 
    if setRange:
        x_range = info["xrange"]
        y_range = info["yrange"]
    f_box = ""
    fig, ax = plt.subplots()
    deg = 2
    
    print "number of x values: {0}".format(len(x))
    
    for i, y in enumerate(ydata): 
        yname = ynames[i]
        color = colors[i % len(colors)] # in case there are more y data sets than colors
        
        print "number of y values for {0}: {1}".format(yname, len(y))

        if plotFit:
            # calculate fit function
            z = np.polyfit(x, y, deg)
            f = np.poly1d(z)
            f_string = str(f)
            f_string = f_string.split("\n")[-1]
            f_string = "{0} : f(x) = {1}".format(yname, f_string)
            f_box += f_string + "\n"
            print f_string

            # calculate new x's and y's using fit function
            x_new = np.linspace(min(x), max(x), 50)
            y_new = f(x_new)
            
            ax.plot(x,y,'o',            c=color, label=yname, alpha=0.5)
            ax.plot(x_new, y_new, '--', c=color, label=yname+" fit")
        else:
            ax.plot(x,y,'o',            c=color, label=yname, alpha=0.5)

    if f_box[-1] == "\n":
        f_box = f_box[:-1]

    if plotFit:
        ax.text(xstat, ystat, f_box)

    legend = ax.legend(loc='lower right')
    ax.grid(True)
        
    if setRange:
        axes = plt.gca()
        axes.set_xlim(x_range)
        axes.set_ylim(y_range)
    
    plt.gcf().subplots_adjust(bottom=0.1)
    plt.gcf().subplots_adjust(left=0.15)
    
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

    # x data is one data set
    # y data is a list of data sets
    # multiple y data sets are plotted against one x data set
    pd0_data = data["pd0_ave"]
    pd1_data = data["pd1_ave"]
    rm_data = list(data["rm{0}_ave".format(j)] for j in xrange(1,5))

    # list of dictionaries containing plot information

    info = []
    
    # PD1 vs PD0
    info.append({})
    info[-1]["name"] = "pd1_pd0"
    info[-1]["ynames"] = ["PD 1"]
    info[-1]["xdata"] = pd0_data 
    info[-1]["ydata"] = [pd1_data]
    info[-1]["title"] = "Pin Diode 1 vs Pin Diode 0 (Max Charge)"
    info[-1]["xtitle"] = "Pin Diode 0 Max fC"
    info[-1]["ytitle"] = "Pin Diode 1 Max fC"
    info[-1]["xstat"] = 20000 
    info[-1]["ystat"] = 52000
    info[-1]["xrange"] = [0, 100000]
    info[-1]["yrange"] = [0, 200000]
    info[-1]["plotfit"] = True
    info[-1]["setrange"] = False

    # RM1-4 vs PD0
    info.append({})
    info[-1]["name"] = "rm_pd0"
    info[-1]["ynames"] = ["RM 1", "RM 2", "RM 3", "RM 4"]
    info[-1]["xdata"] = pd0_data 
    info[-1]["ydata"] = rm_data
    info[-1]["title"] = "RM SiPMs vs Pin Diode 0 (Average Max Charge)"
    info[-1]["xtitle"] = "Pin Diode 0 Max fC"
    info[-1]["ytitle"] = "SiPM Average Max fC"
    info[-1]["xstat"] = 5000
    info[-1]["ystat"] = 170000
    info[-1]["xrange"] = [0, 100000]
    info[-1]["yrange"] = [0, 200000]
    info[-1]["plotfit"] = True
    info[-1]["setrange"] = True

    # RM1-4 vs PD1
    info.append({})
    info[-1]["name"] = "rm_pd1"
    info[-1]["ynames"] = ["RM 1", "RM 2", "RM 3", "RM 4"]
    info[-1]["xdata"] = pd1_data 
    info[-1]["ydata"] = rm_data
    info[-1]["title"] = "RM SiPMs vs Pin Diode 1 (Average Max Charge)"
    info[-1]["xtitle"] = "Pin Diode 1 Max fC"
    info[-1]["ytitle"] = "SiPM Average Max fC"
    info[-1]["xstat"] = 5000
    info[-1]["ystat"] = 170000
    info[-1]["xrange"] = [0, 100000]
    info[-1]["yrange"] = [0, 200000]
    info[-1]["plotfit"] = True
    info[-1]["setrange"] = True

    # RM1-4 and PD1 vs PD0
    info.append({})
    info[-1]["name"] = "rm_pd1_pd0"
    info[-1]["ynames"] = ["RM 1", "RM 2", "RM 3", "RM 4", "PD 1"]
    info[-1]["xdata"] = pd0_data 
    info[-1]["ydata"] = rm_data + [pd1_data]
    info[-1]["title"] = "RM SiPMs and Pin Diode 1 vs Pin Diode 0 (Average Max Charge)"
    info[-1]["xtitle"] = "Pin Diode 0 Max fC"
    info[-1]["ytitle"] = "SiPM and Pin Diode 1 Average Max fC"
    info[-1]["xstat"] = 15000
    info[-1]["ystat"] = 170000
    info[-1]["xrange"] = [0, 100000]
    info[-1]["yrange"] = [0, 200000]
    info[-1]["plotfit"] = True
    info[-1]["setrange"] = True

    # create scatter plots
    for plot_info in info:
        plotScatter(plot_dir, plot_info)

