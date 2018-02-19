import ROOT
from array import array
import re

def makeNtuple(root_file):
    print "making ntuple"
    
    infile = ROOT.TFile.Open(root_file)
    outfile = ROOT.TFile("point5_laser_data.root", "recreate")
    tree = ROOT.TTree('t1', 't1')
    array_dict = {}
    branches = ["rbx", "rm", "fiber", "channel", "energy"]
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

        for rm in xrange(1,5):
            canvas = infile.Get("Energy/{0}/{1}-{2}-Energy".format(rbxName, rbxName, rm))
            pad = 1
            for fiber in xrange(8):
                for channel in xrange(6):
                    histo = canvas.GetPad(pad).GetPrimitive("ENERGY_{0}_RM{1}_fiber{2}_channel{3}".format(rbxName, rm, fiber, channel))
                    energy = histo.GetMean()
                    print "{0}-{1} ({2}, {3}) : {4}".format(rbxName, rm, fiber, channel, energy)
                    array_dict["rbx"][0] = float(rbxNum)
                    array_dict["rm"][0] = float(rm)
                    array_dict["fiber"][0] = float(fiber)
                    array_dict["channel"][0] = float(channel)
                    array_dict["energy"][0] = float(energy)
                    tree.Fill()
                    pad += 1
                    nchannels += 1

   
    outfile.Write()
    outfile.Close()
    infile.Close()
    print "n channels: {0}".format(nchannels)
    return


if __name__ == "__main__":
    root_file = "~hcaldqm/xplotdqm/gui/runs/309738_Feb14_laser-HBHE-CU-Gsel0"
    makeNtuple(root_file)
