# python is happy
# make happy plots
import json
import matplotlib.pyplot as plt
import numpy as np
import argparse
from scipy.optimize import curve_fit

class Plotter:

    def __init__(self, data_dir, plot_dir):
        self.constants = {}
        self.data_dir = data_dir
        self.plot_dir = plot_dir
        self.data = self.getData(data_dir)
        xkcd_colors = ["pinkish red","azure","electric purple","bluish green","tangerine","neon pink","dark sky blue","avocado"]
        self.colors = list("xkcd:{0}".format(c) for c in xkcd_colors)

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

        data["iteration"] = []
        
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
        self.cu_list = []
        charge_position = -2
        adc_position = -3
        iteration_position = 2
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
                    iteration = int(s[iteration_position])
                    element_index = int(s[element_position])
                    element_name = "%s%d" % (element, element_index)
                    data[tag].append(d)
                    data[element_name].append(d)
                    
                    if iteration not in data["iteration"]:
                        data["iteration"].append(iteration)


                    # data organized by CU
                    cu = int(s[0]) 
                    if cu not in self.cu_list:
                        self.cu_list.append(cu) 
                        cu_name = "cu%d" % cu
                        data[cu_name] = {}
                        for pd in xrange(6):
                            data[cu_name]["pd%d" % pd] = []
                        for rm in xrange(1,5):
                            data[cu_name]["rm%d" % rm] = []
                        data[cu_name]["sipm"] = []
                        # constants for normalization
                        self.constants[cu_name] = {}
                        # constants for pd0, pd1, and average of pd0 and pd1
                        self.constants[cu_name]["pd0"] = 0.0
                        self.constants[cu_name]["pd1"] = 0.0
                        self.constants[cu_name]["pd_ave"] = 0.0
                        # constants for RM SiPMs (4 constants for pd0, pd1, and average of pd0 and pd1)
                        self.constants[cu_name]["sipm_pd0"] = []
                        self.constants[cu_name]["sipm_pd1"] = []
                        self.constants[cu_name]["sipm_ave"] = []

                    # add elements per CU
                    data["cu%d"%cu][element_name].append(d)
                    
                    if tag == "sipm":
                        data["%s_sipm%s" % (element_name, s[sipm_position])].append(d)
                        data[cu_name]["sipm"].append(d)
       
        data["pd_quartz"] = data["pd0"] + data["pd0"]
        data["pd_megatile"] = data["pd2"] + data["pd3"] + data["pd4"] + data["pd5"]
        
        for cu in self.cu_list:
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
    
    def plotHisto(self, data, info, fileName="", stacked=False):
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
        # data array can be a 1D or 2D matrix
        # data array is 2D for stacked histograms
        data_array = data[name]
        data_list = []
        stack_colors = []
        if stacked:
            for i, x in enumerate(data_array):
                stack_colors.append(self.colors[i % len(self.colors)])
                for d in x:
                    data_list.append(d)
        else:
            data_list = data_array
        
        stack_names = list("RM %d" % rm for rm in xrange(1,5))
        print "name = {0}".format(name)
        #print "data_list = {0}".format(data_list)
        if not data_list:
            print "There is no data for {0}.".format(name)
            return
        
        fig, ax = plt.subplots()
        
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
            if stacked:
                h_y, h_x, h = plt.hist(data_array, bins=nbins, range=x_range, color=stack_colors, label=stack_names, stacked=stacked)
            else:
                h_y, h_x, h = plt.hist(data_list, bins=nbins, range=x_range)
            xstat, ystat = self.getStat(x_range[0], x_range[1], y_range[0], y_range[1], statLocation)
        else:
            if stacked:
                h_y, h_x, h = plt.hist(data_array, bins=nbins, color=stack_colors, label=stack_names, stacked=stacked)
            else:
                h_y, h_x, h = plt.hist(data_list, bins=nbins)
            xstat, ystat = self.getStat(min(h_x), max(h_x), min(h_y), max(h_y), statLocation)
        
        legend = ax.legend(loc='upper left')
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
        plt.close()
    
    # makes scatter plots and calculates constants
    def plotScatter(self, info):
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
  
        sipm_mean = np.mean(self.data["sipm"])

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
            color = self.colors[i % len(self.colors)] # in case there are more y data sets than colors
            
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
                ax.plot(x_new, y_new, '--', c=color, label="%s fit" % yname)
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
                ax.plot(x,y,'o',            c=color, label="%s average" % yname, alpha=0.5)
                ax.plot(x_new, y_new, '--', c=color, label="%s fit" % yname)
                
                # calculate correction constants from fit function
                if name == "rm_pd0" or name == "rm_pd1":
                    pd_name = name.split("_")[-1]
                    for cu in self.cu_list:
                        cu_name = "cu%d" % cu
                        if len(self.data[cu_name][pd_name]) != 1:
                            print "There is not exactly one {0} value for {1}!".format(pd_name, cu_name)
                            return 
                        else: # there is only one value as expected
                            # the correction factor is the sipm mean divided by the expected value from the log fit
                            pd_value = self.data[cu_name][pd_name][0]
                            constant = sipm_mean / self.logarithm(pd_value, *popt)
                            self.constants[cu_name]["sipm_%s" % pd_name].append(constant)
                            print "CU {0} {1} {2}: pd_value = {3} : SiPM correction constant = {4}".format(cu, pd_name, yname, pd_value, constant)
            
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
        plt.close()

    # plot data vs iteration
    def plotIterations(self, info):
        name = info["name"]
        ynames = info["ynames"]
        x = info["xdata"]
        ydata = info["ydata"]
        title = info["title"]
        xtitle = info["xtitle"]
        ytitle = info["ytitle"]
        setRange = info["setrange"] 
        statLocation = info["statloc"]
        connect = info["connect"]
        
        if setRange:
            x_range = info["xrange"]
            y_range = info["yrange"]
        
        fig, ax = plt.subplots()
        print "number of x values: {0}".format(len(x))
        for i, y in enumerate(ydata): 
            print "number of y values for channel {0}: {1}".format(i, len(y))
            if not y:
                print "no y values for channel {0}".format(i)
                continue
            #yname = ynames[i]
            color = self.colors[i % len(self.colors)] # in case there are more y data sets than colors
            if connect:
                ax.plot(x, y, '-o', c=color, alpha=0.5)
            else:
                ax.plot(x, y, 'o', c=color, alpha=0.5)
  
        if setRange:
            axes = plt.gca()
            axes.set_xlim(x_range)
            axes.set_ylim(y_range)
            xstat, ystat = self.getStat(x_range[0], x_range[1], y_range[0], y_range[1], statLocation)
        
        legend = ax.legend(loc='upper right')
        ax.grid(True)
        
        plt.gcf().subplots_adjust(bottom=0.1)
        plt.gcf().subplots_adjust(left=0.15)
        
        plt.title(title)
        plt.xlabel(xtitle)
        plt.ylabel(ytitle)
        plt.savefig(self.plot_dir + name + ".png")
        plt.savefig(self.plot_dir + name + ".pdf")
        plt.clf()
        plt.close()

    # should be used after making scatter plot, which calculates constants
    def normalize(self):
        self.data["norm_pd0"] = []
        self.data["norm_pd1"] = []
        self.data["norm_sipm"] = []
        for cu in self.cu_list:
            cu_name = "cu%d" % cu
            # pin-diode corrections
            for pd in xrange(2):
                pd_name = "pd%d" % pd
                if len(self.data[cu_name][pd_name]) != 1:
                    print "There is not exactly one {0} value for {1}!".format(pd_name, cu_name)
                    return 
                else: # there is only one value as expected
                    # the correction factor is the pd mean divided by the orivinal pd value
                    pd_value = self.data[cu_name][pd_name][0]
                    pd_mean = np.mean(self.data[pd_name])
                    constant = pd_mean / pd_value
                    self.constants[cu_name][pd_name] = constant
            averagePindiodeCorrection = np.mean([self.constants[cu_name]["pd0"], self.constants[cu_name]["pd1"]])
            self.constants[cu_name]["pd_ave"] = averagePindiodeCorrection
            print "CU {0} pin-diode correction constant : {1}".format(cu, self.constants[cu_name]["pd_ave"])
            for pd in xrange(2):
                pd_name = "pd%d" % pd
                pd_value = self.data[cu_name][pd_name][0]
                averagePindiodeCorrection = self.constants[cu_name]["pd_ave"]
                self.data["norm_%s" % pd_name].append(averagePindiodeCorrection * pd_value)
            # sipm corrections
            averageSipmCorrections = list(np.mean([self.constants[cu_name]["sipm_pd0"][i], self.constants[cu_name]["sipm_pd1"][i]]) for i in xrange(4))
            self.constants[cu_name]["sipm_ave"] = averageSipmCorrections
            print "CU {0} SiPM correction constants : {1}".format(cu, self.constants[cu_name]["sipm_ave"])
            for rm in xrange(1,5):
                rm_name = "rm%d" % rm
                for d in self.data[cu_name][rm_name]:
                    # the normalized value is the constant (sipm mean / expected value from log fit) times the original value
                    self.data["norm_sipm"].append(self.constants[cu_name]["sipm_ave"][rm-1] * d)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--histo_config",       "-c", default="config/final_histo.json",            help="json config file for histograms")
    parser.add_argument("--iteration_config",   "-i", default="config/stability_iterations.json",   help="json config file for iteration plots")
    parser.add_argument("--scatter_config",     "-s", default="config/final_scatter.json",          help="json config file for scatter plots")
    parser.add_argument("--data_dir",           "-d", default="Nov17-18_Final_CU_Data",             help="directory containing data tables")
    parser.add_argument("--plot_dir",           "-p", default="Nov17-18_Final_Plots",               help="directory to save plots")
    
    options = parser.parse_args()
    plot_dir = options.plot_dir
    data_dir = options.data_dir
    histo_config = options.histo_config
    iteration_config = options.iteration_config
    scatter_config = options.scatter_config


    if data_dir[-1] != "/":
        data_dir += "/"
    if plot_dir[-1] != "/":
        plot_dir += "/"
    
    # calls getData to set self.data on initialization
    p = Plotter(data_dir, plot_dir)
   
    # choose which plots to create
    makeScatter = True
    makeIterations = False
    makeHistos = True
    makeHistosPerCU = True

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

        # important for normalized SiPM plots
        p.normalize()

    ########################
    # make iteration plots #
    ########################
    
    if makeIterations:
        iteration_data = p.data["iteration"]
        pd0_data = p.data["pd0"]
        pd1_data = p.data["pd1"]

        with open(iteration_config) as json_file:
            info = json.load(json_file)
        sipm_data = list(p.data["rm%d_sipm%d" % (rm, sipm)] for sipm in xrange(48) for rm in xrange(1,5))
        info["rm_pd_stability"]["xdata"] = iteration_data
        info["rm_pd_stability"]["ydata"] = sipm_data + [pd0_data] + [pd1_data]
        
        for key in info:
            p.plotIterations(info[key])


    ###################
    # make histograms #
    ###################
    
    if makeHistos:
        with open(histo_config) as json_file:
            info = json.load(json_file)
        
        for key in info:
            p.plotHisto(p.data, info[key])
    
    #########################
    # make one histo per CU # 
    #########################

    if makeHistosPerCU:
        cu_info = {}
        cu_info["name"] = ""
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
                cu_info["name"] = "sipm"
                cu_info["title"] = "SiPM Max Charge for CU {0}".format(cu_number)
                p.plotHisto(p.data[key], cu_info, "%s_sipm" % key)
                
                # make stacked histograms per CU per RM
                print "Plot stacked histograms for {0}".format(key)
                cu_info["name"] = "stacked_sipm"

                # check ordering. may need to be a transposed np.array
                cu_rm_data = {}
                cu_rm_data["stacked_sipm"] = list(p.data[key]["rm%d" % rm] for rm in xrange(1,5))
                p.plotHisto(cu_rm_data, cu_info, "%s_stacked_sipm" % key, stacked=True)


