#include <vector>
#include <map>
#include <string>

#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TColor.h"

void stabilityPlots() {
    
    // be sure to add a # at the beginning of the first line of your pd_table.txt and sipm_table.txt files!
    // then they can be read properly
    //
    // use root makePlots.C
    
    // xbins defines bin edges for variable bin width
    // you need 257 bin edges
    // make sure that your xbins are in increaing order!!
    // this is especially important where ranges overlap: 
    // ADC 0 and 1, 64 and 65, 128 and 129, 192 and 193
    // I have removed 680000 from the top of the range
    // I have added -1.57 to the bottom of the range
    // this was to include -0.05 fC
    Float_t xbins[257] = 
           {-1.57,  1.58,   4.73,   7.88,   11.0,   14.2,   17.3,   20.5,   23.6,
            26.8,   29.9,   33.1,   36.2,   39.4,   42.5,   45.7,   48.8,
            53.6,   60.1,   66.6,   73.0,   79.5,   86.0,   92.5,   98.9,
            105,    112,    118,    125,    131,    138,    144,    151,
            157,    164,    170,    177,    186,    199,    212,    225,
            238,    251,    264,    277,    289,    302,    315,    328,
            341,    354,    367,    380,    393,    406,    418,    431,
            444,    464,    490,    516,    542,    568,    594,    620,
            645,    670,    695,    720,    745,
            771,    796,    821,    846,    871,    897,    922,    947,
            960,    1010,   1060,   1120,   1170,   1220,   1270,   1320,
            1370,   1430,   1480,   1530,   1580,   1630,   1690,   1740,
            1790,   1840,   1890,   1940,   2020,   2120,   2230,   2330,
            2430,   2540,   2640,   2740,   2850,   2950,   3050,   3150,
            3260,   3360,   3460,   3570,   3670,   3770,   3880,   3980,
            4080,   4240,   4450,   4650,   4860,   5070,   5280,   5490,
            5680,   5880,   6080,   6280,   6480,
            6680,   6890,   7090,   7290,   7490,   7690,   7890,   8090,
            8400,   8810,   9220,   9630,   10000,  10400,  10900,  11300,
            11700,  12100,  12500,  12900,  13300,  13700,  14100,  14500,
            15000,  15400,  15800,  16200,  16800,  17600,  18400,  19300,
            20100,  20900,  21700,  22500,  23400,  24200,  25000,  25800,
            26600,  27500,  28300,  29100,  29900,  30700,  31600,  32400,
            33200,  34400,  36100,  37700,  39400,  41000,  42700,  44300,
            45900,  47600,  49200,  50800,  52500,
            54100,  55700,  57400,  59000,  60600,  62200,  63900,  65500,
            68000,  71300,  74700,  78000,  81400,  84700,  88000,  91400,
            94700,  98100,  101000, 105000, 108000, 111000, 115000, 118000,
            121000, 125000, 128000, 131000, 137000, 145000, 152000, 160000,
            168000, 176000, 183000, 191000, 199000, 206000, 214000, 222000,
            230000, 237000, 245000, 253000, 261000, 268000, 276000, 284000,
            291000, 302000, 316000, 329000, 343000, 356000, 370000, 384000, 398000,
            410000, 430000, 450000, 470000, 520000, 550000, 580000, 640000};
   

    bool logScale = true;

    // setup root
    gROOT->SetBatch();
    gStyle->SetOptStat(111111);
    
    TString dir = gSystem->UnixPathName(__FILE__);
    TString runDir = "run-10/"; // choose carefully, mein freund
    TString plotDir;
    TString pdfile = "pd_table.txt";
    TString sipmfile = "sipm_table.txt";
    
    if (logScale)   plotDir = "stabilityPlots/";
    else            plotDir = "linearPlots/";
    
    // chars
    char meanName[100];
    char plotName[100];
    char rmName[100];
    char rbxName[100];
    char variable[100];
    char rbxs[100];
    char sipms[100];
    char pindiode_0[100];
    char pindiode_1[100];
    char fileName[100];
    dir.ReplaceAll("stabilityPlots.C",""); // this needs to match the file name 
    dir.ReplaceAll("/./","/");
    
    // Maps
    std::map<int,TH1F*> rbxHistoMeans;
    std::map<std::string,TH1F*> rmHistoMeans;
    std::map<int,int> colorMap;
    std::map<int,int> styleMap;
    std::map<int,std::string> nameMap;
    colorMap[1] = kRed;
    colorMap[2] = kCyan+2;
    colorMap[3] = kBlue;
    colorMap[4] = kViolet;
    colorMap[5] = kAzure;
    colorMap[6] = kCyan+1;
    colorMap[7] = kRed+3;
    colorMap[8] = kMagenta+3;
    colorMap[9] = kViolet+3;
    colorMap[10] = kBlue+3;
    colorMap[11] = kAzure+3;
    colorMap[12] = kGreen+2;
    colorMap[13] = kRed-3;
    colorMap[14] = kMagenta-3;
    colorMap[15] = kViolet-3;
    colorMap[16] = kBlue-3;
    colorMap[17] = kAzure-3;
    colorMap[18] = kGreen-2;
    styleMap[1] = 1;
    styleMap[2] = 2;
    styleMap[3] = 3;
    styleMap[4] = 4;

    Long64_t nlines;
    int shunt = 0;
    int nBins = 40;
    int RM = 0;
    int minRM = 1;
    int maxRM = 4;
    int minRun = 17;
    int maxRun = 28;
    int nCycles = (maxRun - minRun + 1) / 2;
    int cycleCount = 1;
    int cycleColor = 0;
    int rmColor = 0;
    int rmStyle = 0;
    int rbxColor = 0;
    std::vector<int> rbxList;

    rbxList.push_back(14);
    rbxList.push_back(18);
    
    const float minFC = -10.0;
    const float maxFC = 400000.0;

    TTree *T1 = new TTree("pd_ntup","pd data from ascii file");
    nlines = T1->ReadFile(Form("%s%s%s",dir.Data(),runDir.Data(),pdfile.Data()),"run:rbx:cu:pdch:uhtrch:shunt:maxadc:maxfc:result");
    printf(" found %lld points in pd file\n",nlines);

    TTree *T2 = new TTree("sipm_ntup","sipm data from ascii file");
    nlines = T2->ReadFile(Form("%s%s%s",dir.Data(),runDir.Data(),sipmfile.Data()),"run:rbx:cu:rm:sipmch:uhtrch:shunt:maxadc:maxfc:result");
    printf(" found %lld points in sipm file\n",nlines);
    
    TCanvas *C0 = new TCanvas("c0","c0");
    TCanvas *C1 = new TCanvas("c1","c1");
    TCanvas *C2 = new TCanvas("c2","c2");
    TCanvas *C3 = new TCanvas("c3","c3");
    if (logScale) C0->SetLogx(); // try Log scale for x axis (ADC or fC)
    if (logScale) C1->SetLogx(); // try Log scale for x axis (ADC or fC)
    
    // Histograms for 46 (RM 2/3) or 48 (RM 1/4) SiPM channels 
    for (int irun = minRun; irun <= maxRun; ++ irun) {
        for (int jrbx = 0; jrbx < int(rbxList.size()); ++jrbx) {
            int irbx = rbxList[jrbx];
            // Laser in RBX 14 for odd run numbers and Laser in RBX 18 for even run numbers
            if ((irbx==14 && (irun & 1) == 1) || (irbx==18 && (irun & 1) == 0)) {
                C0->cd();
                sprintf(rbxName,    "RBX%d", irbx);
                sprintf(plotName,   "RBX%d_Run%d", irbx, irun);
                sprintf(meanName,   "Means for RBX%d RMs", irbx);
                sprintf(rbxs,       "rbx==%d && shunt==%d && run==%d", irbx, shunt, irun);
                sprintf(variable,   "maxfc >> %s", plotName);
                
                TH1F *hrbx;
                if (logScale) hrbx = new TH1F(plotName,plotName,256,xbins);
                else          hrbx = new TH1F(plotName,plotName,200,minFC,maxFC);

                if (rbxHistoMeans.find(irbx) == rbxHistoMeans.end()) {
                    rbxHistoMeans[irbx] = new TH1F(rbxName,"Means for RBXs",nCycles+2,minRun-2,maxRun+2);
                    rbxHistoMeans[irbx]->GetYaxis()->SetRangeUser(minFC,maxFC);
                }
                
                T2->Draw(variable, rbxs); 
                rbxHistoMeans[irbx]->Fill(irun,hrbx->GetMean());
                rbxHistoMeans[irbx]->SetBinError(cycleCount+1,hrbx->GetRMS());
                nameMap[irbx] = rbxName;
                cycleColor = colorMap[cycleCount];
                
                hrbx->SetStats(kTRUE); 
                hrbx->SetLineColor(cycleColor);
                hrbx->SetFillColor(cycleColor);
                hrbx->GetXaxis()->SetTitle("Max fC");
                hrbx->GetYaxis()->SetTitle("SiPM Channels");
                hrbx->Draw();
                
                C0->Modified(); C0->Update();
                C0->SaveAs(runDir + plotDir + plotName + ".pdf");
                C0->SaveAs(runDir + plotDir + plotName + ".png");
                hrbx->~TH1F();
                
                for (int irm = minRM; irm <= maxRM; ++irm) {
                    C1->cd();
                    sprintf(rmName,         "RBX%d_RM%d", irbx, irm);
                    sprintf(plotName,       "RBX%d_RM%d_Run%d", irbx, irm, irun);
                    sprintf(variable,       "maxfc >> %s", plotName);
                    sprintf(sipms,          "rbx==%d && rm==%d && shunt==%d && run==%d", irbx, irm, shunt, irun);

                    TH1F *hrm;
                    if (logScale) hrm = new TH1F(plotName,plotName,256,xbins);
                    else          hrm = new TH1F(plotName,plotName,200,minFC,maxFC);

                    if (rmHistoMeans.find(rmName) == rmHistoMeans.end()) {
                        rmHistoMeans[rmName] = new TH1F(rmName,meanName,nCycles+2,minRun-2,maxRun+2);
                        rmHistoMeans[rmName]->GetYaxis()->SetRangeUser(minFC,maxFC);
                    }
                    
                    T2->Draw(variable, sipms);
                    rmHistoMeans[rmName]->Fill(irun,hrm->GetMean());
                    rmHistoMeans[rmName]->SetBinError(cycleCount+1,hrm->GetRMS());
                    
                    hrm->SetStats(kTRUE);
                    hrm->SetLineColor(cycleColor);
                    hrm->SetFillColor(cycleColor);
                    hrm->GetXaxis()->SetTitle("Max fC");
                    hrm->GetYaxis()->SetTitle("SiPM Channels");
                    hrm->Draw();
                    
                    C1->Modified(); C1->Update();
                    C1->SaveAs(runDir + plotDir + plotName + ".pdf");
                    C1->SaveAs(runDir + plotDir + plotName + ".png");
                    hrm->~TH1F();
                }
                
                // only increase cycle after 2 runs and 2 RBXs
                // the first run is odd and the first jrbx is even
                printf("%s : run %d : cycle %d : color %d\n", plotName, irun, cycleCount, cycleColor); 
                if ((irun & 1) == 0 && (jrbx & 1) == 1)    ++cycleCount;
            }
        }
    }
    const float x1 = 0.83;
    const float y1 = 0.78;
    const float x2 = 0.98;
    const float y2 = 0.98;
    TLegend *leg1 = new TLegend(x1, y1, x2, y2); // x1, y1, x2, y2 from 0.0 to 1.0
    TLegend *leg2 = new TLegend(x1, y1, x2, y2); // x1, y1, x2, y2 from 0.0 to 1.0
    TLegend *leg3 = new TLegend(x1, y1, x2, y2); // x1, y1, x2, y2 from 0.0 to 1.0
    leg1->SetHeader("RBX Legend");
    leg2->SetHeader("RM Legend");
    leg3->SetHeader("RM Legend");
    
    C2->cd();
    for (std::map<int,TH1F*>::iterator it=rbxHistoMeans.begin(); it!=rbxHistoMeans.end(); ++it) {
        for (int bin=0;bin<it->second->GetSize();++bin) {
            printf("Bin: %d Value: %.3f Error: %.3f\n", bin, it->second->GetBinContent(bin), it->second->GetBinError(bin));
        }
        rbxColor = colorMap[it->first]; 
        it->second->SetStats(kFALSE);
        it->second->SetLineColor(rbxColor);
        it->second->GetXaxis()->SetTitle("Iteration Number");
        it->second->GetYaxis()->SetTitle("Mean of Max fC");
        leg1->AddEntry(it->second,nameMap[it->first].c_str(),"l");
        if (it->first == 14)    it->second->Draw("][ e1");
        else                    it->second->Draw("][ e1 same");
    }

    leg1->Draw();
    C2->Modified(); C2->Update();
    C2->SaveAs(runDir + plotDir + "Means for RBXs" + ".pdf");
    C2->SaveAs(runDir + plotDir + "Means for RBXs" + ".png");

    C3->cd();
    for (std::vector<int>::iterator it=rbxList.begin(); it!=rbxList.end(); ++it) {
        TH1F *h;
        int rbx = *it;
        sprintf(rbxName,"RBX%d", rbx);
        for (int rm=minRM; rm<=maxRM; ++rm) {
            rmColor = colorMap[rm];
            rmStyle = styleMap[rm];
            sprintf(rmName, "RBX%d_RM%d", rbx, rm);
            h = rmHistoMeans[rmName];
            for (int bin=0;bin<h->GetSize();++bin) {
                printf("Bin: %d Value: %.3f Error: %.3f\n", bin, h->GetBinContent(bin), h->GetBinError(bin));
            }
            
            if (rbx == 14)  leg2->AddEntry(h,rmName,"l");
            else            leg3->AddEntry(h,rmName,"l");
            
            h->SetStats(kFALSE);
            h->SetLineColor(rmColor);
            h->SetLineStyle(rmStyle);
            h->GetXaxis()->SetTitle("Iteration Number");
            h->GetYaxis()->SetTitle("Mean of Max fC");
            
            if (rm == 1)    h->Draw("][ e1");
            else            h->Draw("][ e1 same");
        }
        
        if (rbx==14)    leg2->Draw();
        else            leg3->Draw();
        
        C3->Modified(); C3->Update();
        C3->SaveAs(runDir + plotDir + "Means for " + rbxName + ".pdf");
        C3->SaveAs(runDir + plotDir + "Means for " + rbxName + ".png");
        h->~TH1F();
    } 

    exit(0);
}
