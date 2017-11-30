# python is happy
# make happy plots
import json
import matplotlib.pyplot as plt
import numpy as np
from argparse import ArgumentParser
from scipy.optimize import curve_fit

class Plotter:

    def __init__(self, data_dir, plot_dir):
        self.pd0_constants = {}
        self.pd1_constants = {}
        self.data_dir = data_dir
        self.plot_dir = plot_dir
        self.data = self.getData(data_dir)

    def getData(self, data_dir):
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
            for sipm in xrange(48):
                data["%s_sipm%d" % (element_name, sipm)] = []
    
        # sipm array: {CU,RBX,Run,RM,sipm_ch,uhtr_ch,shunt,max_adc,max_fc,result}
        # pd array: {CU,RBX,Run,pd_ch,uhtr_ch,shunt,max_adc,max_fc,result}
        cu_list = []
        charge_position = -2
        adc_position = -3
        element_position = 3
        sipm_position = 4
        for dictionary in dictionaries:
            f = dictionary["file"]
            tag = dictionary["tag"]
            element = dictionary["element"]
            with open(data_dir + f) as df:
                for line in df:
                    if line[0] == "#":
                        continue
                    s = line.split()
                    #if int(s[-1]) == 0:
                        # skip data that does not pass
                        #print line
                        #continue
                    #d = int(s[adc_position])
                    d = float(s[charge_position])
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
                        data[cu_name]["sipm"] = []
    
                    data["cu%d"%cu][element_name].append(d)
                    if tag == "sipm":
                        data["%s_sipm%s" % (element_name, s[sipm_position])].append(d)
                        data[cu_name]["sipm"].append(d)
       
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
    
    def logarithm(self, x, a, b, c):
        return a * np.log(b * x) + c
    
    def getStat(self, x_min, x_max, y_min, y_max, loc=1):
        xstat, ystat = 0, 0
        if loc == 0: # top left
            x_stat = x_min + (x_max - x_min) / 8.0
            y_stat = y_max - (y_max - y_min) / 3.0
        elif loc == 1: # top right
            x_stat = x_max - (x_max - x_min) / 3.0
            y_stat = y_max - (y_max - y_min) / 3.0
        elif loc == 2: # upper top left for scatter
            x_stat = x_min + (x_max - x_min) / 25.0
            y_stat = y_max - (y_max - y_min) / 4.0
        return (x_stat, y_stat)
    
    def plotHisto(self, data, info, fileName=""):
        name = info["name"]
        title = info["title"]
        xtitle = info["xtitle"]
        ytitle = info["ytitle"]
        nbins = info["nbins"]
        units = info["units"]
        setRange = info["setrange"] 
        statLocation = info["statloc"]
        if setRange:
            x_range = info["xrange"]
            y_range = info["yrange"]
        data_list = data[name]
        
        entries = len(data_list)
        mean = np.mean(data_list)
        std = np.std(data_list)
        var = 100.0 * std / mean
        min_val = min(data_list)
        max_val = max(data_list)
        stat_string = "Num Entries = %d\n" % entries
        stat_string += "Mean = %.2f %s\n" % (mean, units)
        stat_string += "Std Dev = %.2f %s\n" % (std, units)
        stat_string += "Variation = %.2f %%\n" % var
        stat_string += "Min = %.2f %s\n" % (min_val, units)
        stat_string += "Max = %.2f %s" % (max_val, units)
        
        if setRange:
            axes = plt.gca()
            axes.set_xlim(x_range)
            axes.set_ylim(y_range)
            h_y, h_x, h = plt.hist(data_list, bins=nbins, range=x_range)
            xstat, ystat = self.getStat(x_range[0], x_range[1], y_range[0], y_range[1], statLocation)
        else:
            h_y, h_x, h = plt.hist(data_list, bins=nbins)
            xstat, ystat = self.getStat(min(h_x), max(h_x), min(h_y), max(h_y), statLocation)
        
        plt.text(xstat, ystat, stat_string)
        plt.title(title)
        plt.xlabel(xtitle)
        plt.ylabel(ytitle)
        if fileName:
            plt.savefig(self.plot_dir + fileName + ".png")
            plt.savefig(self.plot_dir + fileName + ".pdf")
        else:
            plt.savefig(self.plot_dir + name + ".png")
            plt.savefig(self.plot_dir + name + ".pdf")
        plt.clf()
    
    def plotScatter(self, info):
        raw_colors = ["pinkish red","azure","bluish green","electric purple","tangerine","neon pink","dark sky blue","avocado"]
        colors = list("xkcd:{0}".format(c) for c in raw_colors)
        name = info["name"]
        ynames = info["ynames"]
        x = info["xdata"]
        ydata = info["ydata"]
        title = info["title"]
        xtitle = info["xtitle"]
        ytitle = info["ytitle"]
        plotFitTypes = info["plotfit"]
        setRange = info["setrange"] 
        statLocation = info["statloc"]
        if setRange:
            x_range = info["xrange"]
            y_range = info["yrange"]
        f_box = ""
        fig, ax = plt.subplots()
        deg = 1
        y_min = 10 ** 10
        y_max = -10 ** 10
    
        if len(ynames) != len(ydata):
            print "The length of the ynames list should be the same as the number of y data sets."
            return
        if len(plotFitTypes) != len(ydata):
            print "The length of the plotfit list should be the same as the number of y data sets."
            return
        
        print "number of x values: {0}".format(len(x))
        
        for i, y in enumerate(ydata): 
            if min(y) < y_min:
                y_min = min(y)
            if max(y) > y_max:
                y_max = max(y)
    
            plotFit = plotFitTypes[i]
            yname = ynames[i]
            color = colors[i % len(colors)] # in case there are more y data sets than colors
            
            print "number of y values for {0}: {1}".format(yname, len(y))
    
            if plotFit == 1:
                # calculate fit function
                z = np.polyfit(x, y, deg)
                f = np.poly1d(z)
                f_string = str(f)
                f_string = f_string.split("\n")[-1]
                f_string = "{0} : f(x) = {1}".format(yname, f_string)
                f_box += f_string + "\n"
                print f_string
    
                # calculate new x's and y's using fit function
                x_new = np.linspace(min(x), max(x), 100)
                y_new = f(x_new)
                
                ax.plot(x,y,'o',            c=color, label=yname, alpha=0.5)
                ax.plot(x_new, y_new, '--', c=color, label=yname+" fit")
            elif plotFit == 2:
                # calculate fit function
                popt, pcov = curve_fit(self.logarithm, x, y)
                # calculate new x's and y's using fit function
                x_new = np.linspace(min(x), max(x), 100)
                y_new = self.logarithm(x_new, *popt)
                if popt[2] >= 0.0:
                    f_string = "{0}: $f(x) = {1:.2f}\ \ln({2:.2f}\ x) + {3:.2f}$".format(yname, popt[0], popt[1], popt[2]) 
                else:
                    f_string = "{0}: $f(x) = {1:.2f}\ \ln({2:.2f}\ x) {3:.2f}$".format(yname, popt[0], popt[1], popt[2]) 
                print f_string
                f_box += f_string + "\n"
                ax.plot(x,y,'o',            c=color, label=yname, alpha=0.5)
                ax.plot(x_new, y_new, '--', c=color, label=yname+" fit")
            else:
                ax.plot(x,y,'o',            c=color, label=yname, alpha=0.5)
    
        if setRange:
            axes = plt.gca()
            axes.set_xlim(x_range)
            axes.set_ylim(y_range)
            xstat, ystat = self.getStat(x_range[0], x_range[1], y_range[0], y_range[1], statLocation)
        else:
            xstat, ystat = self.getStat(min(x), max(x), y_min, y_max, statLocation)
        
        if f_box:
            if f_box[-1] == "\n":
                f_box = f_box[:-1]
            ax.text(xstat, ystat, f_box)
    
        legend = ax.legend(loc='lower right')
        ax.grid(True)
            
        plt.gcf().subplots_adjust(bottom=0.1)
        plt.gcf().subplots_adjust(left=0.15)
        
        plt.title(title)
        plt.xlabel(xtitle)
        plt.ylabel(ytitle)
        plt.savefig(self.plot_dir + name + ".png")
        plt.savefig(self.plot_dir + name + ".pdf")
        plt.clf()

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--histo_config",   "-c", default="config/final_histo.json",    help="json config file for histograms")
    parser.add_argument("--scatter_config", "-s", default="config/final_scatter.json",  help="json config file for scatter plots")
    parser.add_argument("--data_dir",       "-d", default="Nov17-18_Final_CU_Data",     help="directory containing data tables")
    parser.add_argument("--plot_dir",       "-p", default="Nov17-18_Final_Plots",       help="directory to save plots")
    
    options = parser.parse_args()
    plot_dir = options.plot_dir
    data_dir = options.data_dir
    histo_config = options.histo_config
    scatter_config = options.scatter_config


    if data_dir[-1] != "/":
        data_dir += "/"
    if plot_dir[-1] != "/":
        plot_dir += "/"
    
    # calls getData to set self.data on initialization
    p = Plotter(data_dir, plot_dir)
   
    # choose which plots to create
    makeHistos = True
    makeScatter = False
    makeHistosPerCU = False

    ###################
    # make histograms #
    ###################
    
    if makeHistos:
        with open(histo_config) as json_file:
            info = json.load(json_file)
        
        for key in info:
            p.plotHisto(p.data, info[key])

    ###################### 
    # make scatter plots #
    ######################
    
    if makeScatter:
        # x data is one data set
        # y data is a list of data sets
        # multiple y data sets are plotted against one x data set
        pd0_data = p.data["pd0_ave"]
        pd1_data = p.data["pd1_ave"]
        rm_data = list(p.data["rm{0}_ave".format(j)] for j in xrange(1,5))

        # list of dictionaries containing plot information

        with open(scatter_config) as json_file:
            info = json.load(json_file)
        
        info["pd1_pd0"]["xdata"] = pd0_data 
        info["pd1_pd0"]["ydata"] = [pd1_data]
        info["rm_pd0"]["xdata"] = pd0_data 
        info["rm_pd0"]["ydata"] = rm_data
        info["rm_pd1"]["xdata"] = pd1_data 
        info["rm_pd1"]["ydata"] = rm_data
        info["rm_pd1_pd0"]["xdata"] = pd0_data 
        info["rm_pd1_pd0"]["ydata"] = rm_data + [pd1_data]
        
        for key in info:
            p.plotScatter(info[key])

    #########################
    # make one histo per CU # 
    #########################

    if makeHistosPerCU:
        cu_info = {}
        cu_info["name"] = "sipm"
        cu_info["title"] = ""
        cu_info["xtitle"] = "Max Charge (pC)"
        cu_info["ytitle"] = "Number of Channels"
        cu_info["nbins"] = 25
        cu_info["units"] = "pC"
        cu_info["setrange"] = 1
        cu_info["statloc"] = 1
        cu_info["xrange"] = [0, 2500]
        cu_info["yrange"] = [0, 100]
        
        for key in p.data:
            if "cu" in key:
                print "Plot histogram for {0}".format(key)
                cu_number = key.split("cu")[-1]
                cu_info["title"] = "SiPM Max Charge for CU {0}".format(cu_number)
                p.plotHisto(p.data[key], cu_info, "%s_sipm" % key)




