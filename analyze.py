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
    if runDir[-1] != "/":
        runDir += "/"
    cuList = []
    for directory in os.listdir(runDir):
        if "CU" in directory:
            cu = int(directory.split("CU_")[-1])
            cuList.append(cu)
    cuList.sort()
    shuntList = [31]
    pdList = list(i for i in xrange(6))
    col_width = 15
    adcConverter = ADCConverter()
    for table in tables:
        with open (runDir + table + "_table.txt", 'w') as t:
            if table == "sipm":
                columns = ["CU", "RBX", "Run", "RM", table + "_ch", "uhtr_ch", "shunt", "max_adc", "max_fc", "result"]
                RM = 0
            elif table == "pd":
                columns = ["CU", "RBX", "Run", table + "_ch", "uhtr_ch", "shunt", "max_adc", "max_fc", "result"]
            header = "# " + "".join(entry.ljust(col_width) for entry in columns) + "\n"
            t.write(header)
            
            # Example Files
            # new_cu_data/CU_15/rbx0_shunt31_pd_1.root
            # new_cu_data/CU_15/rbx0_shunt31_uhtr1_1.root
            # new_cu_data/CU_15/rbx0_shunt31_uhtr2_1.root
            
            # order by cu, rbx, run, shunt, uHTR, rm, channel
            for icu in cuList:
                for irbx in rbxList:
                    for irun in runList:
                        for ishunt in shuntList:
                            for iuhtr in [1,2]:
                                if table == "pd" and iuhtr == 2:
                                    continue    #do not do pd twice
                                # It is important to only include SiPM / PD data in correct table
                                #if table == "sipm": iterFlag = "rbx%d_shunt%d_%d.root" % (irbx, ishunt, iuhtr)
                                #elif table == "pd": iterFlag = "rbx%d_shunt%d_pd.root" % (irbx, ishunt)
                                if table == "sipm": f = "%sCU_%d/rbx%d_shunt%d_uhtr%d_%d.root" % (runDir, icu, irbx, ishunt, iuhtr, irun)
                                elif table == "pd": f = "%sCU_%d/rbx%d_shunt%d_pd_%d.root" % (runDir, icu, irbx, ishunt, irun)
                                if not os.path.isfile(f):
                                    continue # skip files that do not exist
                                print "file: %s" % (f)
                                # RBX, CU and shunt are constant for one file
                                cu = "%d" % icu
                                run = "%d" % irun
                                rbx = "%02d" % irbx
                                shunt = "%d" % ishunt
                                if shunt == "0":    cutoff = 150
                                else:               cutoff = 100
                                # RM, pindiode/sipm_ch, uHTR, uhtr_ch, max_adc will vary within a file 
                                if table == "sipm": 
                                    rmList = list(i for i in xrange(1,5))
                                    chList = mapping.rbxSIPM[rbx][iuhtr-1]
                                elif table == "pd": 
                                    rmList = [1]
                                    if irun < 4:
                                        chList = mapping.chs(0,1)
                                    else:
                                        chList = mapping.chs(irun-2, irun-2)
                                for RM in rmList:
                                    rm = "%d" % RM
                                    for channel in chList:
                                        #if (rm,channel) in mapping.darkSipms:
                                        #    #print "mask out channel: RM %s SiPM %s" % (rm, channel)
                                        #    continue    # mask out 4 dark channels per RBX
                                        uhtr_ch = channel.split("h")[-1]
                                        max_adc = str(findMaxADC(f, channel, False))
                                        max_fc  = "%.2f" % adcConverter.linearize(max_adc)
                                        if int(max_adc) > cutoff:   result = "1"
                                        else:                       result = "0"
                                        if table == "sipm":
                                            row = [cu, rbx, run, rm, channel, uhtr_ch, shunt, max_adc, max_fc, result]
                                        if table == "pd":
                                            row = [cu, rbx, run, channel, uhtr_ch, shunt, max_adc, max_fc, result]
                                        t.write("".join(entry.ljust(col_width) for entry in row) + "\n") 

if __name__ == "__main__":

    # RBX 0: iterations 1 - 7
    runDir = "new_cu_data"
    rbxList = [0] 
    # sipm: iterations 1, 2, 3
    tables = ["sipm"]
    runList = list(i for i in xrange(1,4))
    makeTable(runDir, tables, rbxList, runList)
    # pd: iterations 1, 2, 3, 4, 5, 6, 7
    tables = ["pd"]
    runList = list(i for i in xrange(1,8))
    makeTable(runDir, tables, rbxList, runList)

