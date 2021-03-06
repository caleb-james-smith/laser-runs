# Plot Laser Runs

from ROOT import *
from plotDictionary import sipmDict, pindiodeDict, shuntSipms, shuntPindiodes
from ADCConverter import ADCConverter
import os
import mapping
from argparse import ArgumentParser
from array import array

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

    for d in dictionary:
        file1   = d['file1']
        file2   = d['file2']
        chList  = d['ch']
        ch_type = d['type']
        for ch in chList:
            plotData(file1, file2, ch, ch_type, runDir, plotDir, shunts)

# Return maximum ADC value for a histogram (final nonzero bin)
# update to accept channel list and return a list of max values...
# this will be more efficient than opening the root file for each channel
def oldFindMaxADC(rootFile, ch, verb=False):
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
def findMaxADC(rootFile, ch, verb=False):
    if verb:
        print "file: %s channel: %s" % (rootFile, ch)
    # Get Histogram from file
    F = TFile(rootFile)
    H = F.Get(ch)
    # Find Max ADC
    Bin = 255       # starting bin; bin index 0 to 255
    passingBins = 0 # number of consecutive bins greater than cutoff
    required = 2    # required number of consecutive bins greater than cutoff
    cutoff = 1      # cutoff for accepting value in bin
    while Bin > 0:
        b = H.GetBinContent(Bin)
        if verb:
            print "Bin %-3i: %.3f" % (Bin, b)
        if b > cutoff:
            passingBins += 1
        else:
            passingBins = 0
        if passingBins >= required:
            # return first nonzero bin
            maxBin = Bin + passingBins - 1
            if verb:
                print "Max Bin: %-3i" % maxBin
            return maxBin
        Bin -= 1
    if verb:
        print "Max Bin: %-3i" % Bin
    return Bin  # Return Bin = 0

def plotMax(runDir, dictionary):
    gROOT.SetBatch()
    maxADC = 256 + 16 * 4   # Max ADC from 0 to 256 with additional 64 to shift data to the left (makes room for labels)
    nBins = maxADC / 16     # Number of bins is 320/16 = 20
    H1 = TH1F("H1", "H1", nBins, 1, maxADC)
    H2 = TH1F("H2", "H2", nBins, 1, maxADC) 
    for d in dictionary:
        file1   = runDir + d['file1']
        file2   = runDir + d['file2']
        chList  = d['ch']
        ch_type    = d['type']
        for ch in chList:
            for f in [file1, file2]:
                maxBin = findMaxADC(f, ch, False)
                #print "File: {0} | Channel: {1} | Max Bin: {2}".format(f, ch, maxBin)
                if f == file1:      # H1 and file1 are shunt 0
                    H1.Fill(maxBin)
                if f == file2:      # H2 and file2 are shunt 31
                    H2.Fill(maxBin)

    print "Type: {0}".format(ch_type)
    if ch_type == "SiPM":
        maxY = 400
    if ch_type == "Pindiode":
        maxY = 20
    # Formatting
    H1.SetLineColor(kRed)
    H1.SetLineStyle(2)
    H1.SetAxisRange(0,maxY,"Y")
    
    H2.SetLineColor(kBlue)
    H2.GetXaxis().SetTitle("Max ADC")
    H2.GetYaxis().SetTitle("No. " + ch_type)
    H2.SetAxisRange(0,maxY,"Y")
    H2.SetTitle("Max ADC Reached for " + ch_type)
    
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
    C1.SaveAs(runDir + 'max_adc_' + ch_type + '.pdf')

# Table options are "sipm", "pd", and "pindiodes"
def makeTable(runDir, tables, runList, unit, stability=False):
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
    col_width = 10
    adcConverter = ADCConverter(unit, 31) # unit and shunt
    for table in tables:
        cuBadChannels = {}
        total_channels = 0
        total_bad_channels = 0
        print "Creating {0} table".format(table)
        tfile = TFile(runDir + table + '.root', 'recreate')
        tree = TTree('t1', 't1')
        array_dict = {}
        with open (runDir + table + "_array.h", 'w') as a:
            with open (runDir + table + "_table.txt", 'w') as t:
                if table == "sipm":
                    columns = ["cu", "rbx", "run", "rm", table + "_ch", "uhtr_ch", "shunt", "max_adc", "max_pc", "result"]
                elif table == "pd":
                    columns = ["cu", "rbx", "run", table + "_ch", "uhtr_ch", "shunt", "max_adc", "max_pc", "result"]
                for key in columns:
                    array_dict[key] = array('f', [ 0. ])
                    float_name = '{0}/F'.format(key)
                    tree.Branch(key, array_dict[key], float_name)
                
                header_table = "".join(entry.ljust(col_width) for entry in columns) + "\n"
                header_array = "{" + ",".join(columns) + "}\n"
                
                t.write("# " + header_table)
                #print header_table
                
                array_string = ""
                a.write("#include <vector>\n\n")
                a.write("// " + header_array)
                array_string += "std::vector< std::vector<double> > %s_array = {\n" % table
                
                # Example Files
                # new_cu_data/CU_15/rbx0_shunt31_pd_1.root
                # new_cu_data/CU_15/rbx0_shunt31_uhtr1_1.root
                # new_cu_data/CU_15/rbx0_shunt31_uhtr2_1.root
                
                # order by cu, rbx, run, shunt, uHTR, rm, channel
                for icu in cuList:
                    print "Processing CU {0}".format(icu) 
                    # get RBX list from file names
                    rbxList = []
                    bad_channels = 0
                    for data_file in os.listdir(runDir + "CU_" + str(icu)):
                        if "rbx" in data_file and ".root" in data_file:
                            data_file = data_file.split("rbx")[-1]
                            data_file = data_file.split("_")[0]
                            rbx_int = int(data_file)
                            if rbx_int not in rbxList:
                                rbxList.append(rbx_int)
                    for irbx in rbxList:
                        for irun in runList:
                            for ishunt in shuntList:
                                RM = 0
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
                                    #print "file: %s" % (f)
                                    # RBX, CU and shunt are constant for one file
                                    cu = "%d" % icu
                                    run = "%d" % irun
                                    rbx_full = "%02d" % irbx
                                    rbx = "%d" % irbx
                                    shunt = "%d" % ishunt
                                    if shunt == "0":    cutoff = 150
                                    else:               cutoff = 100
                                    # RM, pindiode/sipm_ch, uHTR, uhtr_ch, max_adc will vary within a file 
                                    if table == "sipm": 
                                        chList = mapping.rbxSIPM[rbx_full][iuhtr-1]
                                    elif table == "pd": 
                                        chList = mapping.rbxPD[rbx_full]
                                    for i, channel in enumerate(chList):
                                        # mask pin-diodes that do not have light
                                        if stability:
                                            if table == "pd":
                                                if i > 1:
                                                    continue
                                        else:
                                            if table == "sipm":
                                                if irun > 3:
                                                    continue
                                            if table == "pd":
                                                # runs 1, 2, and 3 are pin-diode chs 0, 1
                                                if irun < 4:
                                                    if i > 1:
                                                        continue 
                                                # runs 4, 5, 6, and 7 are pin-diode chs 2, 3, 4, and 5
                                                else:
                                                    if i != irun-2:
                                                        continue
                                        rm_ch = str(i % 48)
                                        pd_ch = str(i % 6)
                                        if rm_ch == "0":
                                            RM += 1
                                        rm = "%d" % RM
                                        # mask out 4 dark channels per RBX
                                        # rbx0 has RM type 2 in slot 4 for stability run only!!!
                                        masked_rm = rm
                                        if masked_rm == "4":
                                            masked_rm = "2"
                                        if (masked_rm, rm_ch) in mapping.darkSipms:
                                            #print "mask out channel: RM %s SiPM %s masked rm: %s" % (rm, rm_ch, masked_rm)
                                            continue    
                                        
                                        uhtr_ch = channel.split("h")[-1]
                                        max_adc = str(findMaxADC(f, channel, False))
                                        max_charge  = "%.2f" % adcConverter.linearize(max_adc)
                                        total_channels += 1 
                                        if int(max_adc) >= cutoff:
                                            result = "1"
                                        else:
                                            result = "0"
                                            bad_channels += 1
                                            total_bad_channels += 1
                                        if table == "sipm":
                                            row = [cu, rbx, run, rm, rm_ch, uhtr_ch, shunt, max_adc, max_charge, result]
                                        if table == "pd":
                                            row = [cu, rbx, run, pd_ch, uhtr_ch, shunt, max_adc, max_charge, result]
                                        array_string += "{" + ", ".join(row) + "},\n"
                                        row_string =  "".join(entry.ljust(col_width) for entry in row)
                                        t.write(row_string + "\n") 
                                        tree_string = ""
                                        for i, key in enumerate(columns):
                                            array_dict[key][0] = float(row[i])
                                            tree_string += str(array_dict[key][0]) + " "
                                        #print tree_string
                                        tree.Fill()
                                        #if result == "0":
                                        #    print row_string
                                        if int(max_adc) > 240:
                                            print row_string
                    # calibration unit lopp
                    cuBadChannels[icu] = bad_channels
                # end of array
                if array_string[-2:] == ",\n":
                    array_string = array_string[:-2] + "\n"
                array_string += "};\n"
                a.write(array_string)
        # table loop
        tfile.Write()
        tfile.Close()
        for c in sorted(cuBadChannels):
            print "CU {0} : {1} {2} channels less than {3} ADC".format(c, cuBadChannels[c], table, cutoff)
        print "Total: {0} {1} channels less than {2} ADC out of {3} total channels".format(total_bad_channels, table, cutoff, total_channels)


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--directory", "-d", default="Nov17-18_Final_CU_Data", help="directory containing directories with CU data")
    parser.add_argument("--unit", "-u", default="fc", help="Charge unit (fc, pc, nc).")
    options = parser.parse_args()
    runDir = options.directory
    unit = options.unit
    
    # sipm: iterations 1, 2, 3
    # pd: iterations 1, 2, 3, 4, 5, 6, 7
    
    tables = ["sipm", "pd"]
    runList = [1,4,5,6,7]
    makeTable(runDir, tables, runList, unit)

    # iterations for stability runs: iteration 1 is bad for CU6
    #tables = ["sipm", "pd"]
    #runList = list(i for i in xrange(2,7))
    #makeTable(runDir, tables, runList, unit, stability=True)

    #findMaxADC("CU_8/rbx0_shunt31_pd_1.root", "h0", True)
    #findMaxADC("CU_8/rbx0_shunt31_pd_1.root", "h1", True)

