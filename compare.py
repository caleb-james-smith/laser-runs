import ROOT
import numpy as np
import matplotlib.pyplot as plt
#from plotter import Plotter

def scatter(file_1, file_2, name):
    print "Creating scatter plot."
    print "First file: {0}".format(file_1)
    print "Second file: {0}".format(file_2)
    # XKCD Colors 
    pinkish_red = "#f10c45"
    azure = "#069af3" 
    
    ROOT.gBenchmark.Start('run time')
    
    f1 = ROOT.TFile.Open(file_1)
    f2 = ROOT.TFile.Open(file_2)
 
    # point 5 data
    data_p5 = {}
    tree = f2.t1
    energy = list(event.energy for event in tree)
    cu = list(event.cu for event in tree)
    rbx = list(event.rbx for event in tree)
    rm = list(event.rm for event in tree)
    sipm_ch = list(event.sipm_ch for event in tree)
    data_p5["energy"] = energy
    data_p5["cu"] = cu
    data_p5["rbx"] = rbx
    data_p5["rm"] = rm
    data_p5["sipm_ch"] = sipm_ch
    print "Number of  channels (P5): {0}".format(len(energy))

    # 904 data
    data_904 = {}
    tree = f1.t1
    charge = list(event.max_charge for event in tree)
    cu = list(event.cu for event in tree)
    rm = list(event.rm for event in tree)
    sipm_ch = list(event.sipm_ch for event in tree)
    data_904["charge"] = charge
    data_904["cu"] = cu
    data_904["rm"] = rm
    data_904["sipm_ch"] = sipm_ch
    print "Number of channels (904): {0}".format(len(charge))
  
    scatter = {}
    scatter["HEP_x"] = []
    scatter["HEP_y"] = []
    scatter["HEM_x"] = []
    scatter["HEM_y"] = []
    rbx_channels_p5 = []
    rbx_averages_p5 = []
    rbx_channels_904 = []
    rbx_averages_904 = []
    # get RBX averages for P5 and 904 
    for i, energy in enumerate(data_p5["energy"]):
        rbx = data_p5["rbx"][i]
        # get nonzero P5 data
        if energy > 0.0:
            rbx_channels_p5.append(energy)
            # get 904 data
            for j, charge in enumerate(data_904["charge"]):
                if data_p5["cu"][i] != data_904["cu"][j]:           continue
                if data_p5["rm"][i] != data_904["rm"][j]:           continue
                if data_p5["sipm_ch"][i] != data_904["sipm_ch"][j]: continue
                rbx_channels_904.append(charge)
       
        if len(rbx_channels_p5) == 184: # all channels for RBX
            ave = np.mean(rbx_channels_p5)
            rbx_averages_p5.append(ave)
            if rbx > 0.0:
                scatter["HEP_y"].append(ave)
            else:
                scatter["HEM_y"].append(ave)
            #print "RBX {0}: {1} channels (P5)".format(rbx, len(rbx_channels_p5))

            # 904 RBX0 RM types: 1, 2, 3, 2; masked 6 channels (186 total)
            # P5 maksed RM types 2 and 3 only (4 masked channels)
            # P5 masked Layer -1 channels (one per RM, 4 masked channels)
            if len(rbx_channels_904) != 184 - 2:
                print "There are {0} channels for 904 instead of 192 - 6 - 4 = 182 expectedi.".format(len(rbx_channels_904), rbx)
            else: # all channels for RBX
                ave = np.mean(rbx_channels_904)
                rbx_averages_904.append(ave)
                if rbx > 0.0:
                    scatter["HEP_x"].append(ave)
                else:
                    scatter["HEM_x"].append(ave)
                print "RBX {0}: {1} channels (904)".format(rbx, len(rbx_channels_904))

            # clear channel lists
            rbx_channels_p5 = []
            rbx_channels_904 = []
    
    print "Number of HEP RBXs (P5): {0}".format(len(scatter["HEP_y"]))
    print "Number of HEM RBXs (P5): {0}".format(len(scatter["HEM_y"]))
    print "Number of HEP RBXs (904): {0}".format(len(scatter["HEP_x"]))
    print "Number of HEM RBXs (904): {0}".format(len(scatter["HEM_x"]))

    #info = {}
    #info["name"] = "LaserToCU_Point5_vs_904_Scatter"
    #info["ynames"] = ["HEP"]
    #info["xdata"] = scatter["HEP_x"]
    #info["ydata"] = [scatter["HEP_y"]]
    #info["title"] = "Laser to CU: Point 5 vs 904"
    #info["xtitle"] = "Max Charge (fC) from 904"
    #info["ytitle"] = "Energy (fC) from Point 5"
    #info["statloc"] = 1
    #info["setrange"] = 0
    #info["plotfit"] = [1]

    #p = Plotter("Nov17-18_Final_CU_Data", "Nov17-18_Final_Plots", True)
    #p.plotScatter(info, True)

    # create scatter plot
    fig, ax = plt.subplots()
    
    ax.plot(scatter["HEP_x"], scatter["HEP_y"], 'o', c=pinkish_red, alpha=0.5, label="HEP RBXs")
    ax.plot(scatter["HEM_x"], scatter["HEM_y"], 'o', c=azure, alpha=0.5, label="HEM RBXs")

    title = "Laser to CU Data: Energy (P5) vs. Max Charge (904) per RBX"
    xtitle = "Max Charge (fC) from 904"
    ytitle = "Energy (fC) from P5"
        
    legend = ax.legend(loc='upper left')
    ax.grid(True)
    axes = plt.gca()
    axes.set_xlim([0, 2 * 10**6])
    axes.set_ylim([0, 50 * 10**3])
    
    plt.title(title)
    plt.xlabel(xtitle)
    plt.ylabel(ytitle)
    plt.savefig(name + ".png")
    plt.savefig(name + ".pdf")
  
    info = []
    entry = {}
    entry["x"] = scatter["HEP_x"]
    entry["y"] = scatter["HEP_y"]
    entry["color"] = pinkish_red
    entry["label"] = "HEP"
    info.append(entry)
    entry = {}
    entry["x"] = scatter["HEM_x"]
    entry["y"] = scatter["HEM_y"]
    entry["color"] = azure
    entry["label"] = "HEM"
    info.append(entry)

    f_box = ""
    for key in info:
        x = key["x"]
        y = key["y"]
        z = np.polyfit(x, y, 1)
        f = np.poly1d(z)
        f_string = str(f)
        f_string = f_string.split("\n")[-1]
        f_string = "{0} : f(x) = {1}".format(key["label"], f_string)
        f_box += f_string + "\n"
        print f_string
        # calculate new x's and y's using fit function
        x_new = np.linspace(min(x), max(x), 100)
        y_new = f(x_new)
        ax.plot(x_new, y_new, '--', c=key["color"], alpha=0.5, label=key["label"])

    if f_box:
        if f_box[-1] == "\n":
            f_box = f_box[:-1]
        ax.text(10**5, 25 * 10**3, f_box)
        
    plt.savefig(name + "_fit.png")
    plt.savefig(name + "_fit.pdf")
    
    plt.show()
    plt.clf()
    plt.close()
    
    ROOT.gBenchmark.Show('run time')

if __name__ == "__main__":
    file_904 = "Nov17-18_Final_CU_Data/sipm.root"
    files_point5 = []
    names = []
    files_point5.append("Point5_Data/processed_309738_Feb14_laser-HBHE-CU-Gsel0.root")
    #files_point5.append("Point5_Data/processed_310553_Feb27_laser-HBHE-CU-Gsel0.root")
    #files_point5.append("Point5_Data/processed_310554_Feb27_laser-HBHE-CU-Gsel0.root")
    files_point5.append("Point5_Data/processed_310600_Feb27_laser-HBHE-CU-Gsel0.root")
    files_point5.append("Point5_Data/processed_310602_Feb27_laser-HBHE-CU-Gsel0.root")
    files_point5.append("Point5_Data/processed_310603_Feb27_laser-HBHE-CU-Gsel0.root")
    names.append("Point5_Plots/309738_Feb14_laser-HBHE-CU-Gsel0")
    #names.append("Point5_Plots/310553_Feb27_laser-HBHE-CU-Gsel0")
    #names.append("Point5_Plots/310554_Feb27_laser-HBHE-CU-Gsel0")
    names.append("Point5_Plots/310600_Feb27_laser-HBHE-CU-Gsel0")
    names.append("Point5_Plots/310602_Feb27_laser-HBHE-CU-Gsel0")
    names.append("Point5_Plots/310603_Feb27_laser-HBHE-CU-Gsel0")
    for i in xrange(len(names)):
        scatter(file_904, files_point5[i], names[i])



