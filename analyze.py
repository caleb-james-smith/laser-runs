# Plot Laser Runs

from ROOT import *
from plotDictionary import sipmDict, pindiodeDict, shuntSipms, shuntPindiodes
from ADCConverter import ADCConverter
import os
import mapping

def plotData(file1, file2, ch, type, runDir, plotDir, shunts):
    # Batch Mode removes X-forwarding for canvas creation graphics
    # This makes everything MUCH faster!
    gROOT.SetBatch()

    # Setup file names
    if shunts:
        saveFile = file2.split(".root")[0]
    else:
        saveFile = file2.split("bright/")[1]
        saveFile = saveFile.split(".root")[0]
    plotLabel = saveFile + '_' + ch

    # Get Histograms from files
    F1 = TFile(runDir + file1)
    F2 = TFile(runDir + file2)
    H1 = F1.Get(ch)
    H2 = F2.Get(ch)
    H1.SetName("H1")
    H2.SetName("H2")

    # Formatting
    H1.SetLineColor(kRed)
    H1.SetLineStyle(2)
    H2.SetLineColor(kBlue)
    H2.GetXaxis().SetTitle("ADC")
    H2.GetYaxis().SetTitle("Events")
    H2.SetTitle(type + " Response to Laser inserted in CU")

    # Make a canvas 
    C1 = TCanvas("c1","c1")
    C1.SetLogy()
    H2.Draw()
    H1.Draw("same")

    # Make a label
    label = TText(0.2, 0.8, plotLabel)
    label.SetTextFont(43)
    label.SetTextSize(25)
    label.SetTextAngle(0)   # positive and negative angles are good!
    label.SetNDC()
    label.Draw()
    
    # Make a legend
    leg = TLegend(0.8,0.6,0.93,0.8)
    leg.SetHeader("Legend")
    if shunts:
        leg.AddEntry("H1","Shunt 0") 
        leg.AddEntry("H2","Shunt 31") 
    else:
        leg.AddEntry("H1","Dark") 
        leg.AddEntry("H2","Bright") 
    leg.Draw()

    # Save
    C1.Modified()
    C1.Update()
    C1.SaveAs(runDir + plotDir + plotLabel + '.pdf')


def makePlots(runDir, plotDir, dictionary, shunts):
    #file1 = "dark/uhtr62-2_dark.root"
    #file2 = "bright/uhtr62-2_rbx11_cu32_bright.root"
    #for ch in xrange(144):
    #    plotData(file1, file2, 'h'+str(ch), runDir)

    for dict in dictionary:
        file1   = dict['file1']
        file2   = dict['file2']
        chList  = dict['ch']
        type    = dict['type']
        for ch in chList:
            plotData(file1, file2, ch, type, runDir, plotDir, shunts)

# Return maximum ADC value for a histogram (final nonzero bin)
# update to accept channel list and return a list of max values...
# this will be more efficient than opening the root file for each channel
def oldFindMaxADC(rootFile, ch, verb):
    # Get Histogram from file
    F = TFile(rootFile)
    H = F.Get(ch)

    # Find Max ADC
    numZeros = 0    # Count number of consecutive bins with value 0
    Bin = 0         # Bin index 0 to 255
    limit = 10      # Return once numZeros > limit
    while Bin < 256:
        b = H.GetBinContent(Bin)
        if verb:
            print "Bin %-3i: %.3f" % (Bin, b)
        if b == 0:
            numZeros += 1
        else:
            numZeros = 0
        if numZeros > limit:
            if Bin == limit:
                return 0
            else:
                return Bin - limit - 1
        Bin += 1
    return Bin - 1      # Return bin in 0 to 255 range

# New Find Max that starts at ADC 256 and looks for bin with value > 1
def findMaxADC(rootFile, ch, verb):
    if verb:    print "file: %s channel: %s" % (rootFile, ch)
    # Get Histogram from file
    F = TFile(rootFile)
    H = F.Get(ch)
    # Find Max ADC
    Bin = 255   # Bin index 0 to 255
    while Bin > 0:
        b = H.GetBinContent(Bin)
        if verb:    print "Bin %-3i: %.3f" % (Bin, b)
        if b > 1:   return Bin
        Bin -= 1
    return Bin  # Return bin in 0 to 255 range

def plotMax(runDir, dictionary):
    gROOT.SetBatch()
    maxADC = 256 + 16 * 4   # Max ADC from 0 to 256 with additional 64 to shift data to the left (makes room for labels)
    nBins = maxADC / 16     # Number of bins is 320/16 = 20
    H1 = TH1F("H1", "H1", nBins, 1, maxADC)
    H2 = TH1F("H2", "H2", nBins, 1, maxADC) 
    for dict in dictionary:
        file1   = runDir + dict['file1']
        file2   = runDir + dict['file2']
        chList  = dict['ch']
        type    = dict['type']
        for ch in chList:
            for f in [file1, file2]:
                maxBin = findMaxADC(f, ch, False)
                #print "File: {0} | Channel: {1} | Max Bin: {2}".format(f, ch, maxBin)
                if f == file1:      # H1 and file1 are shunt 0
                    H1.Fill(maxBin)
                if f == file2:      # H2 and file2 are shunt 31
                    H2.Fill(maxBin)

    print "Type: {0}".format(type)
    if type == "SiPM":
        maxY = 400
    if type == "Pindiode":
        maxY = 20
    # Formatting
    H1.SetLineColor(kRed)
    H1.SetLineStyle(2)
    H1.SetAxisRange(0,maxY,"Y")
    
    H2.SetLineColor(kBlue)
    H2.GetXaxis().SetTitle("Max ADC")
    H2.GetYaxis().SetTitle("No. " + type)
    H2.SetAxisRange(0,maxY,"Y")
    H2.SetTitle("Max ADC Reached for " + type)
    
    # Make a canvas 
    C1 = TCanvas("c1","c1")
    H2.Draw()
    H1.Draw("same")
    
    # Make a legend
    leg = TLegend(0.8,0.6,0.93,0.8)
    leg.SetHeader("Legend")
    leg.AddEntry("H1","Shunt 0") 
    leg.AddEntry("H2","Shunt 31") 
    leg.Draw()
    
    # Save
    C1.Modified()
    C1.Update()
    C1.SaveAs(runDir + 'max_adc_' + type + '.pdf')

# Table options are "sipm", "pd", and "pindiodes"
def makeTable(runDir, tables, rbxList, runList):
    shuntList = [0]
    pdList = list(i for i in xrange(5))
    sipm_chlist = list("h%d" % i for i in xrange(96,144))
    #tables = ["sipm", "pd"]
    #tables = ["pindiodes"]
    col_width = 15
    adcConverter = ADCConverter()
    for table in tables:
        with open (runDir + table + "_table.txt", 'w') as t:
            if table == "sipm":
                columns = ["Run", "RBX", "CU", "RM", table + "_ch", "uhtr_ch", "shunt", "max_adc", "max_fc", "result"]
                RM = 0
            elif table == "pd":
                columns = ["Run", "RBX", "CU", table + "_ch", "uhtr_ch", "shunt", "max_adc", "max_fc", "result"]
            elif table == "pindiodes":
                columns = ["RBX", "CU", "pd_ch", "uhtr_ch", "shunt", "max_adc", "max_fc", "result"]
            header = "# " + "".join(entry.ljust(col_width) for entry in columns) + "\n"
            t.write(header)
            if table == "pindiodes":
                # make sure we order by (1) rbx, (2) shunt, (3) pd_ch
                for irbx in rbxList:
                    for ishunt in shuntList:
                        for ipd in pdList:
                            # RBX, shunt and pd_ch define file file
                            iterFlag = "rbx%d_shunt%d_pd%d.root" % (irbx, ishunt, ipd)
                            for f in os.listdir(runDir):
                                if f == iterFlag:
                                    print "run: %s file: %s" % (runDir, f)
                                    rbx = "%02d" % irbx 
                                    shunt = "%d" % ishunt 
                                    pd = "%d" % ipd
                                    cu = mapping.rbxCU[rbx]
                                    uhtr_chs = mapping.rbxPindiodes(rbx)
                                    if pd == "0":   chList = [0,1]
                                    else:           chList = [ipd + 1]
                                    if shunt == "0":    cutoff = 150
                                    else:               cutoff = 100
                                    # uhtr_ch, max_adc will vary within a file 
                                    for ch in chList:
                                        pd_ch = str(ch)
                                        uhtr_ch = uhtr_chs[ch]
                                        max_adc = str(findMaxADC(runDir + f, uhtr_ch, False))
                                        max_fc = "%.2f" % adcConverter.linearize(max_adc)
                                        uhtr_ch_cooked = uhtr_ch.split("h")[1]
                                        if int(max_adc) > cutoff:   result = "1"
                                        else:                       result = "0"
                                        #if int(max_adc) > cutoff:   result = "pass"
                                        #else:                       result = "fail"
                                        row = [rbx, cu, pd_ch, uhtr_ch, shunt, max_adc, max_fc, result]
                                        t.write("".join(entry.ljust(col_width) for entry in row) + "\n")
            else: 
                # order by (1) rbx, (2) shunt, (3) uHTR 1 or 2
                for irun in runList:
                    for irbx in rbxList:
                        for ishunt in shuntList:
                            for iuhtr in [1,2]:
                                if table == "pd" and iuhtr == 2:
                                    continue    #do not do pd twice
                                for f in os.listdir(runDir):
                                    # It is important to only include SiPM / PD data in correct table
                                    #if table == "sipm": iterFlag = "rbx%d_shunt%d_%d.root" % (irbx, ishunt, iuhtr)
                                    #elif table == "pd": iterFlag = "rbx%d_shunt%d_pd.root" % (irbx, ishunt)
                                    if table == "sipm": iterFlag = "rbx%d_shunt%d_uhtr%d_%d.root" % (irbx, ishunt, iuhtr, irun)
                                    elif table == "pd": iterFlag = "rbx%d_shunt%d_pd_%d.root" % (irbx, ishunt, irun)
                                    if f == iterFlag:
                                        print "run: %s file: %s" % (runDir, f)
                                        # RBX, CU and shunt are constant for one file
                                        run = "%d" % irun
                                        rbx = "%02d" % irbx
                                        shunt = "%d" % ishunt
                                        cu = mapping.rbxCU[rbx]
                                        if shunt == "0":    cutoff = 150
                                        else:               cutoff = 100
                                        # RM, pindiode/sipm_ch, uHTR, uhtr_ch, max_adc will vary within a file 
                                        if table == "sipm": chList = mapping.rbxSIPM[rbx][iuhtr-1]
                                        elif table == "pd": chList = mapping.rbxPD[rbx]
                                        # Run 4 uses first uHTR for RBX, last RM for this uHTR 
                                        if runDir == "run-4/" and table == "sipm": chList = sipm_chlist 
                                        for i, ch in enumerate(chList):
                                            channel = "%s" % (i % 48)   # sipm (0-47) or pindiode (0-5) channel
                                            if channel == "0":
                                                RM += 1
                                                if RM == 5: RM = 1
                                            rm = "%s" % RM
                                            # Run 4 uses special mapping for RMs.  
                                            if runDir == "run-4/": rm = mapping.rbxRM[rbx] 
                                            if (rm,channel) in mapping.darkSipms:
                                                #print "mask out channel: RM %s SiPM %s" % (rm, channel)
                                                continue    # mask out 4 dark channels per RBX
                                            uhtr_ch = ch.split("h")[1]
                                            max_adc = str(findMaxADC(runDir + f, ch, False))
                                            max_fc  = "%.2f" % adcConverter.linearize(max_adc)
                                            if int(max_adc) > cutoff:   result = "1"
                                            else:                       result = "0"
                                            #if int(max_adc) > cutoff:   result = "pass"
                                            #else:                       result = "fail"
                                            if table == "sipm":
                                                row = [run, rbx, cu, rm, channel, uhtr_ch, shunt, max_adc, max_fc, result]
                                            if table == "pd":
                                                row = [run, rbx, cu, channel, uhtr_ch, shunt, max_adc, max_fc, result]
                                            t.write("".join(entry.ljust(col_width) for entry in row) + "\n") 

if __name__ == "__main__":
    
    plotData("rbx0_shunt31_pd_1.root" ,     "rbx0_shunt31_pd_2.root",    "h0",  "Pindiode", "CU_40/", "", True)
    plotData("rbx0_shunt31_uhtr1_1.root" ,  "rbx0_shunt31_uhtr1_2.root", "h12", "SiPM",     "CU_40/", "", True)
    
    #runDir = "run-4/"
    #tables = ["sipm", "pd"]
    #rbxList = list(i for i in xrange(2,13))
    #makeTable(runDir, tables, rbxList)

    #runDir = "run-7/"
    #tables = ["pindiodes"]
    #rbxList = list(i for i in xrange(2,7))
    #makeTable(runDir, tables, rbxList)
    #
    #runDir = "run-8/"
    #tables = ["sipm", "pd"]
    #rbxList = list(i for i in xrange(13,19))
    #makeTable(runDir, tables, rbxList)

    #runDir = "run-9/"
    #tables = ["sipm", "pd"]
    #rbxList = list(i for i in xrange(16,17))
    #makeTable(runDir, tables, rbxList)

    #runDir = "run-10/"
    #tables = ["sipm", "pd"]
    #rbxList = [14,18] 
    #runList = list(i for i in xrange(3,29))
    #makeTable(runDir, tables, rbxList, runList)

    #shunts = True
    
    #plotDir = "sipmPlots/"
    #dictionary = shuntSipms
    #makePlots(runDir, plotDir, dictionary, shunts)
    
    #plotDir = "pindiodePlots/"
    #dictionary = shuntPindiodes
    #makePlots(runDir, plotDir, dictionary, shunts)
    
    #plotMax("run-4/rbx2_shunt31_pd.root", "h84", True)
    
    #plotMax(runDir, shuntSipms)
    #plotMax(runDir, shuntPindiodes)

    # Create NTuple (table)
    #print findMaxADC("run-7/rbx6_shunt31_pd0.root", "h132", True)
    #print findMaxADC("run-7/rbx6_shunt31_pd0.root", "h133", True)
    #print oldFindMaxADC("run-7/rbx6_shunt31_pd0.root", "h133", True)
    #print oldFindMaxADC("run-4/rbx2_shunt31_1.root", "h111", True)
    
    #makeTable(runDir, tables)
