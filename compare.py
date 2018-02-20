import ROOT
import numpy as np
import matplotlib.pyplot as plt

def scatter(file_1, file_2):
    print "Creating scatter plot."
    print "First file: {0}".format(file_1)
    print "Second file: {0}".format(file_2)
    
    ROOT.gBenchmark.Start('run time')
    
    f1 = ROOT.TFile.Open(file_1)
    f2 = ROOT.TFile.Open(file_2)
 
    data_p5 = {}
    energy = list(event.energy for event in f2.t1)
    cu = list(event.cu for event in f2.t1)
    rbx = list(event.rbx for event in f2.t1)
    rm = list(event.rm for event in f2.t1)
    sipm_ch = list(event.sipm_ch for event in f2.t1)
    data_p5["energy"] = energy
    data_p5["cu"] = cu
    data_p5["rbx"] = rbx
    data_p5["rm"] = rm
    data_p5["sipm_ch"] = sipm_ch
    print "Number of  channels (P5): {0}".format(len(energy))

    data_904 = {}
    charge = list(event.max_charge for event in f1.t1)
    data_904["charge"] = charge
    print "Number of channels (904): {0}".format(len(charge))
  
    scatter = {}
    scatter["HEP_x"] = []
    scatter["HEP_y"] = []
    scatter["HEM_x"] = []
    scatter["HEM_y"] = []
    rbx = 0
    rbx_channels = []
    rbx_averages = []
    for i, energy in enumerate(data_p5["energy"]):
        # fill rbx_channels per RBX 
        r = data_p5["rbx"][i]
        if r != rbx:
            rbx_channels = []
            rbx = r
        if energy > 0.0:
            rbx_channels.append(energy)
        if len(rbx_channels) == 184: # all channels for RBX
            ave = np.mean(rbx_channels)
            rbx_averages.append(ave)
            if r > 0.0:
                scatter["HEP_y"].append(ave)
            else:
                scatter["HEM_y"].append(ave)
            print "RBX {0}: {1} channels".format(r, len(rbx_channels))
    
    print "Number of HEP RBXs: {0}".format(len(scatter["HEP_y"]))
    print "Number of HEM RBXs: {0}".format(len(scatter["HEM_y"]))
    
    #c1 = ROOT.TCanvas('c1', 'Charge for Laser Run', 0, 0, 100, 100)
    #f1 = tfiles[0]
    #t1 = f1.t1
    #t1.Draw('max_charge')
    
    #f2 = tfiles[1]
    #t2 = f2.t1
    #t2.Draw('energy')

    #c1.cd()
    #c1.Update()
    
    ROOT.gBenchmark.Show('run time')

if __name__ == "__main__":
    file_1 = "Nov17-18_Final_CU_Data/sipm.root"
    file_2 = "Point5_Data/processed_309738_Feb14_laser-HBHE-CU-Gsel0.root"
    scatter(file_1, file_2)

