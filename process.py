import ROOT
from array import array
import re
import json
import os
import subprocess
import argparse
from compare import scatter

def makeNtuple(data_dir, root_file):
    if data_dir[-1] != "/":
        data_dir += "/"
    
    print "Creating ntuple in {0}".format(data_dir)
    
    with open("barcode.json", 'r') as b:
        barcodes = json.load(b)

    infile = ROOT.TFile.Open("{0}{1}".format(data_dir, root_file))
    outfile = ROOT.TFile("{0}processed_{1}".format(data_dir, root_file), "recreate")
    tree = ROOT.TTree('t1', 't1')
    array_dict = {}
    branches = ["rbx", "cu", "rm", "sipm_ch", "fiber", "fib_ch", "energy"]
    for key in branches:
        array_dict[key] = array('f', [0.0])
        name = "{0}/F".format(key)
        tree.Branch(key, array_dict[key], name)
    
    rbxList = []
    rbxList += list("HEP{0:02d}".format(i) for i in xrange(1,19))
    rbxList += list("HEM{0:02d}".format(i) for i in xrange(1,19))
    nchannels = 0
    rbx_dict = {}
    
    for rbxName in rbxList:
        m = re.search("([A-Z]*)([0-9]*)", rbxName)
        rbxStem = m.group(1)
        rbxNum = int(m.group(2))
        if "M" in rbxStem:
            rbxNum *= -1
        rbx_dict[rbxName] = rbxNum
        cu = barcodes["{0}-calib".format(rbxName)]

        for rm in xrange(1,5):
            canvas = infile.Get("Energy/{0}/{1}-{2}-Energy".format(rbxName, rbxName, rm))
            pad = 1
            sipm_ch = 0
            for fiber in xrange(8):
                for fib_ch in xrange(6):
                    histo = canvas.GetPad(pad).GetPrimitive("ENERGY_{0}_RM{1}_fiber{2}_channel{3}".format(rbxName, rm, fiber, fib_ch))
                    energy = histo.GetMean()
                    #print "{0} CU{1} RM{2} SiPM{3} ({4}, {5}) : {6}".format(rbxName, cu, rm, sipm_ch, fiber, fib_ch, energy)
                    array_dict["rbx"][0] = float(rbxNum)
                    array_dict["cu"][0] = float(cu)
                    array_dict["rm"][0] = float(rm)
                    array_dict["sipm_ch"][0] = float(sipm_ch)
                    array_dict["fiber"][0] = float(fiber)
                    array_dict["fib_ch"][0] = float(fib_ch)
                    array_dict["energy"][0] = float(energy)
                    tree.Fill()
                    pad += 1
                    sipm_ch += 1
                    nchannels += 1

   
    outfile.Write()
    outfile.Close()
    infile.Close()
    print "n channels: {0}".format(nchannels)
    return


def main():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--run", "-r", default="", help="run number")
    options = parser.parse_args()
    run = options.run
    
    # DQM runs from ~hcaldqm/xplotdqm/gui/runs 
    dqm_data_dir = "/nfshome0/hcaldqm/xplotdqm/gui/runs"
    my_data_dir = "Point5_Data"
    my_plot_dir = "Point5_Plots"
    dqm_file = ""
    my_file = ""
   
    if my_data_dir[-1] != "/":
        my_data_dir += "/"
    if my_plot_dir[-1] != "/":
        my_plot_dir += "/"

    if run:
        for f in os.listdir(dqm_data_dir):
            if run in f:
                print "Found DQM file: {0}".format(f)
                dqm_file = f
    else:
        print "Please provide run number using -r option."
        return
    

    my_file = dqm_file + ".root"
    full_dqm_file = dqm_data_dir + "/" + dqm_file
    print "Using DQM file: {0}".format(full_dqm_file)

    # rsync file from DQM directory to personal directory
    subprocess.call(["rsync", "-avz", full_dqm_file, my_data_dir + my_file])

    # process root file, output ntuple
    makeNtuple(my_data_dir, my_file)

    # files for scatter plot
    file_904 = "Nov17-18_Final_CU_Data/sipm.root"
    file_point5 = my_data_dir + "processed_" + my_file
    file_out = my_plot_dir + dqm_file
    
    # make scatter plot 
    scatter(file_904, file_point5, file_out)

if __name__ == "__main__":
    main()





