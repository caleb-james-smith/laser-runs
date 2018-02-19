import ROOT

def makeNtuple(root_file):
    print "making ntuple"
    
    infile = ROOT.TFile.Open(root_file)
    outfile = ROOT.TFile("point5_laser_data.root", "recreate")
    tree = ROOT.TTree('t1', 't1')
    arracy_dict = {}
    
    rbxList = []
    rbxList += list("HEP{0:02d}".format(i) for i in xrange(1,19))
    rbxList += list("HEM{0:02d}".format(i) for i in xrange(1,19))
    nchannels = 0
    for rbx in rbxList:
        for rm in xrange(1,5):
            canvas = infile.Get("Energy/{0}/{1}-{2}-Energy".format(rbx, rbx, rm))
            pad = 1
            for fiber in xrange(8):
                for channel in xrange(6):
                    histo = canvas.GetPad(pad).GetPrimitive("ENERGY_{0}_RM{1}_fiber{2}_channel{3}".format(rbx, rm, fiber, channel))
                    print "{0}-{1} ({2}, {3}) : {4}".format(rbx, rm, fiber, channel, histo.GetMean())
                    pad += 1
                    nchannels += 1
    print "n channels: {0}".format(nchannels)
    return


if __name__ == "__main__":
    root_file = "~hcaldqm/xplotdqm/gui/runs/309738_Feb14_laser-HBHE-CU-Gsel0"
    makeNtuple(root_file)
