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

void makePlots() {
    
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
   

    // loop through bins and print edge values and differences
    //printf("bin : value : diff\n");
    //Float_t diff = 0.0;
    //for (int i = 0; i < sizeof(xbins)/sizeof(*xbins); i++) {
    //    if (i>0) diff = xbins[i] - xbins[i-1];
    //    printf("%6d : %.2f : %.2f\n", i, xbins[i], diff);
    //}
    
    bool logScale = true;

    // setup root
    gROOT->SetBatch();
    gStyle->SetOptStat(111111);
    
    TString dir = gSystem->UnixPathName(__FILE__);
    TString runDir = "run-8/"; // choose carefully, mein freund
    TString plotDir;
    TString pdfile = "pd_table.txt";
    TString sipmfile = "sipm_table.txt";
    TString outputFile = "output.root";
    
    if (logScale)   plotDir = "logPlots/";
    else            plotDir = "linearPlots/";
    
    // chars
    char rbxName[100];
    char plotName[100];
    char variable[100];
    char rbxs[100];
    char sipms[100];
    char pindiode_0[100];
    char pindiode_1[100];
    dir.ReplaceAll("makePlots.C","");
    dir.ReplaceAll("/./","/");
    
    // Maps
    std::map<int,TGraph*> scatter0Map;
    std::map<int,TGraph*> scatter1Map;
    std::map<int,TH1F*> rbxHistoMap;
    std::map<int,int> kMap;
    std::map<int,int> colorMap;
    std::map<int,std::string> nameMap;
    colorMap[13] = kRed;
    colorMap[14] = kOrange + 1;
    colorMap[15] = kGreen + 2;
    colorMap[16] = kBlue + 3;
    colorMap[17] = kCyan + 4;
    colorMap[18] = kMagenta + 5;

    Long64_t nlines;
    int nBins = 40;
    int RM = 0;
    int minRM = 1;
    int maxRM = 4;
    int minRBX = 13;
    int maxRBX = 18;
    std::vector<int> rbx_rm_map;
    
    for (int i=0; i<6; i++) { 
        for (int rm=3; rm>0; rm--) { 
            rbx_rm_map.push_back(rm);
            //printf("%d : %d\n", i, rm);
        }
    }

    const float minFC = -10.0;
    const float maxFC = 350000.0;

    TTree *T1 = new TTree("pd_ntup","pd data from ascii file");
    nlines = T1->ReadFile(Form("%s%s%s",dir.Data(),runDir.Data(),pdfile.Data()),"rbx:cu:pdch:uhtrch:shunt:maxadc:maxfc:result");
    printf(" found %lld points in pd file\n",nlines);

    TTree *T2 = new TTree("sipm_ntup","sipm data from ascii file");
    nlines = T2->ReadFile(Form("%s%s%s",dir.Data(),runDir.Data(),sipmfile.Data()),"rbx:cu:rm:sipmch:uhtrch:shunt:maxadc:maxfc:result");
    printf(" found %lld points in sipm file\n",nlines);
    
    TH1F *hmeans = new TH1F("means","Means of Max Charge (SiPMs, Shunt 31)",nBins,minFC,maxFC);
    TH1F *hrms = new TH1F("rms","RMS of Max Charge (SiPMs, Shunt 31)",nBins,minFC,40000.0);
    TH1F *hdeviation = new TH1F("deviaiton","Max Deviation from Mean (SiPMs, Shunt 31)",nBins,-maxFC,maxFC);
    TH1F *hratio0 = new TH1F("ratio0","Ratio of Mean SiPM to Pindiode 0 (Shunt 31)",nBins,0,200);
    TH1F *hratio1 = new TH1F("ratio1","Ratio of Mean SiPM to Pindiode 1 (Shunt 31)",nBins,0,200);
    TH1F *hrmsmean = new TH1F("rmsmean","RMS over Mean of Max Charge (SiPMs, Shunt 31)",nBins,0,1);
    
    // 2-dim. scatter plots
    TH2F *hscatter0;
    TH2F *hscatter1;
    if (logScale)  hscatter0 = new TH2F("scatter0","Scatter of Mean SiPM to Pindiode 0 (Shunt 31)",256,xbins,256,xbins);
    else           hscatter0 = new TH2F("scatter0","Scatter of Mean SiPM to Pindiode 0 (Shunt 31)",200,minFC,maxFC,200,minFC,maxFC);
    if (logScale)  hscatter1 = new TH2F("scatter1","Scatter of Mean SiPM to Pindiode 1 (Shunt 31)",256,xbins,256,xbins);
    else           hscatter1 = new TH2F("scatter1","Scatter of Mean SiPM to Pindiode 1 (Shunt 31)",200,minFC,maxFC,200,minFC,maxFC);


    // parameters used for plots
    Double_t rm_mean = 0.0;     // histogram mean
    Double_t rms = 0.0;         // histogram rms
    Double_t max = 0.0;         // histogram maximum value
    Double_t min = 0.0;         // histogram minimum value
    Double_t max_diff = 0.0;    // histogram maximum - histogram mean
    Double_t min_diff = 0.0;    // histogram mean - histogram minimum
    Double_t pd0_mean = 0.0;    // histogram mean for pd 0
    Double_t pd1_mean = 0.0;    // histogram mean for pd 1

    TFile *f = new TFile(runDir + outputFile,"RECREATE");
    TCanvas *C0 = new TCanvas("c0","c0");
    TCanvas *C1 = new TCanvas("c1","c1");
    if (logScale) C0->SetLogx(); // try Log scale for x axis (ADC or fC)
    if (logScale) C1->SetLogx(); // try Log scale for x axis (ADC or fC)
    
    // Histograms for 48 SiPM channels 
    //for (int irbx = 2; irbx <= 12; irbx++) {
    for (int irbx = minRBX; irbx <= maxRBX; ++irbx) {
        C0->cd();
        sprintf(rbxName,    "RBX%d", irbx);
        sprintf(rbxs,       "rbx==%d && shunt==31", irbx);
        sprintf(variable,   "maxfc >> %s", rbxName);
        
        if (rbxHistoMap.find(irbx) == rbxHistoMap.end()) {
            rbxHistoMap[irbx] = new TH1F(rbxName,"Max Charge for RBXs (SiPMs, Shunt 31)",256,xbins);
        }
        
        nameMap[irbx] = rbxName;
        printf("irbx %d --- rbxName %s\n", irbx, nameMap[irbx].c_str()); 
        
        rbxHistoMap[irbx]->SetStats(0); 
        rbxHistoMap[irbx]->SetLineColor(colorMap[irbx]);
        rbxHistoMap[irbx]->SetFillColor(colorMap[irbx]);
        rbxHistoMap[irbx]->GetXaxis()->SetTitle("Max fC");
        rbxHistoMap[irbx]->GetYaxis()->SetTitle("SiPM Channels");
        if (irbx == minRBX) {
            T2->Draw(variable, rbxs); 
            rbxHistoMap[irbx]->Draw();
        } else {
            T2->Draw(variable, rbxs, "same"); 
            rbxHistoMap[irbx]->Draw("same");
        }

        //if (!rbx_rm_map.empty()) int irm = rbx_rm_map[irbx-1];
        
        for (int irm = minRM; irm <= maxRM; ++irm) {
            C1->cd();
            sprintf(plotName,       "RBX%d_RM%d", irbx, irm);
            sprintf(variable,       "maxfc >> %s", plotName);
            sprintf(sipms,          "rbx==%d && rm==%d && shunt==31", irbx, irm);
            sprintf(pindiode_0,     "rbx==%d && shunt==31 && pdch==0", irbx);
            sprintf(pindiode_1,     "rbx==%d && shunt==31 && pdch==1", irbx);

            TH1F *hpd0 = new TH1F("pd0","pd0",256,xbins);
            TH1F *hpd1 = new TH1F("pd1","pd1",256,xbins);
            TH1F *h2;
            if (logScale) h2 = new TH1F(plotName,plotName,256,xbins);
            else          h2 = new TH1F(plotName,plotName,200,minFC,maxFC);

            T1->Draw("maxfc >> pd0", pindiode_0);
            T1->Draw("maxfc >> pd1", pindiode_1);
            T2->Draw(variable, sipms);
            
            //printf("rbx %d : pd_0 : %.2f pd_1 : %.2f\n", irbx, hpd0->GetMean(), hpd1->GetMean());

            h2->SetLineColor(kBlue+2);
            h2->GetXaxis()->SetTitle("Max fC");
            h2->GetYaxis()->SetTitle("SiPM Channels");
            h2->Draw();
            
            rm_mean = h2->GetMean(1);
            rms = h2->GetRMS(1);
            max = h2->GetMaximum();
            min = h2->GetMinimum();
            max_diff = max - rm_mean;
            min_diff = rm_mean - min;
            pd0_mean = hpd0->GetMean();
            pd1_mean = hpd1->GetMean();

            hmeans->Fill(rm_mean);
            hrms->Fill(rms);   
            hrmsmean->Fill(rms / rm_mean);   

            if (max_diff > min_diff)    hdeviation->Fill(max_diff);
            else                        hdeviation->Fill(-min_diff);
            
            hratio0->Fill(rm_mean/pd0_mean);
            hratio1->Fill(rm_mean/pd1_mean);
            if (scatter0Map.find(irbx) == scatter0Map.end()) scatter0Map[irbx] = new TGraph();
            if (scatter1Map.find(irbx) == scatter1Map.end()) scatter1Map[irbx] = new TGraph();
            scatter0Map[irbx]->SetPoint(kMap[irbx],pd0_mean,rm_mean);
            scatter1Map[irbx]->SetPoint(kMap[irbx],pd1_mean,rm_mean);

            //printf("Scatter 0: RBX%d RM%d pd0_mean = %.2f and rm_mean = %.2f\n", irbx, irm, pd0_mean, rm_mean);
            //printf("Scatter 1: RBX%d RM%d pd1_mean = %.2f and rm_mean = %.2f\n", irbx, irm, pd1_mean, rm_mean);

            C1->Modified(); C1->Update();
            C1->SaveAs(runDir + plotDir + plotName + ".pdf");
            C1->SaveAs(runDir + plotDir + plotName + ".png");
            hpd0->~TH1F();
            hpd1->~TH1F();
            h2->~TH1F();

            ++kMap[irbx];
        }
    }

    C0->cd();
    C0->Modified(); C0->Update();
    C0->SaveAs(runDir + plotDir + "RBXs" + ".pdf");
    C0->SaveAs(runDir + plotDir + "RBXs" + ".png");

    hmeans->SetLineColor(kRed+2);
    hmeans->GetXaxis()->SetTitle("Mean of Max fC");
    hmeans->GetYaxis()->SetTitle("RMs");
    
    hrms->SetLineColor(kRed+2);
    hrms->GetXaxis()->SetTitle("RMS of Max fC");
    hrms->GetYaxis()->SetTitle("RMs");
    
    hrmsmean->SetLineColor(kRed+2);
    hrmsmean->GetXaxis()->SetTitle("RMS over Mean for Max fC");
    hrmsmean->GetYaxis()->SetTitle("RMs");
    
    hdeviation->SetLineColor(kRed+2);
    hdeviation->GetXaxis()->SetTitle("Deviation of Max fC");
    hdeviation->GetYaxis()->SetTitle("RMs");
    
    hratio0->SetLineColor(kGreen+2);
    hratio0->GetXaxis()->SetTitle("Ratio of SiPM to Pindiode 0");
    hratio0->GetYaxis()->SetTitle("RMs");
    
    hratio1->SetLineColor(kGreen+2);
    hratio1->GetXaxis()->SetTitle("Ratio of SiPM to Pindiode 1");
    hratio1->GetYaxis()->SetTitle("RMs");
    
    hscatter0->SetLineColor(kViolet+2);
    hscatter0->GetXaxis()->SetTitle("Pindiode 0 Max fC");
    hscatter0->GetYaxis()->SetTitle("Mean SiPM Max fC");
    
    hscatter1->SetLineColor(kViolet+2);
    hscatter1->GetXaxis()->SetTitle("Pindiode 1 Max fC");
    hscatter1->GetYaxis()->SetTitle("Mean SiPM Max fC");
    
    TCanvas *C2 = new TCanvas("c2","c2");
    //if (logScale) C2->SetLogx(); // try Log scale for x axis (ADC or fC)
    
    hmeans->Draw();
    C2->Modified(); C2->Update();
    C2->SaveAs(runDir + plotDir + "sipm_means.pdf");
    C2->SaveAs(runDir + plotDir + "sipm_means.png");
    
    hrms->Draw();
    C2->Modified(); C2->Update();
    C2->SaveAs(runDir + plotDir + "sipm_rms.pdf");
    C2->SaveAs(runDir + plotDir + "sipm_rms.png");

    TCanvas *C3 = new TCanvas("c3","c3"); // linear plots
    
    hrmsmean->Draw();
    C3->Modified(); C3->Update();
    C3->SaveAs(runDir + plotDir + "sipm_rms_over_mean.pdf");
    C3->SaveAs(runDir + plotDir + "sipm_rms_over_mean.png");

    hdeviation->Draw();
    C3->Modified(); C3->Update();
    C3->SaveAs(runDir + plotDir + "sipm_deviation.pdf");
    C3->SaveAs(runDir + plotDir + "sipm_deviation.png");
    
    hratio0->Draw();
    C3->Modified(); C3->Update();
    C3->SaveAs(runDir + plotDir + "ratio0.pdf");
    C3->SaveAs(runDir + plotDir + "ratio0.png");

    hratio1->Draw();
    C3->Modified(); C3->Update();
    C3->SaveAs(runDir + plotDir + "ratio1.pdf");
    C3->SaveAs(runDir + plotDir + "ratio1.png");
    
    TCanvas *C4 = new TCanvas("c4","c4");
    if (logScale) C4->SetLogx(); // try Log scale for x axis (ADC or fC)
    if (logScale) C4->SetLogy(); // try Log scale for y axis (ADC or fC)
    
    TLegend *leg = new TLegend(0.5,0.2,0.8,0.5);
    leg->SetHeader("RBX");
    
    hscatter0->SetStats(0);
    hscatter0->Draw();
    for (std::map<int,TGraph*>::iterator it=scatter0Map.begin(); it!=scatter0Map.end(); ++it) {
        it->second->SetMarkerStyle(kFullCircle);
        it->second->SetMarkerSize(0.5);
        it->second->SetMarkerColor(colorMap[it->first]);
        it->second->Draw("P same");
        leg->AddEntry(it->second,nameMap[it->first].c_str(),"p");
        printf("%d --- %s\n", it->first, nameMap[it->first].c_str());
    }

    leg->Draw();
    //C4->BuildLegend(0.2,0.2,0.4,0.4); // x1, y1, x2, y2
    C4->Modified(); C4->Update();
    C4->SaveAs(runDir + plotDir + "scatter0.pdf");
    C4->SaveAs(runDir + plotDir + "scatter0.png");

    hscatter1->Draw();
    hscatter1->SetStats(0);
    for (std::map<int,TGraph*>::iterator it=scatter1Map.begin(); it!=scatter1Map.end(); ++it) {
        it->second->SetMarkerStyle(kFullCircle);
        it->second->SetMarkerSize(0.5);
        it->second->SetMarkerColor(colorMap[it->first]);
        it->second->Draw("P same");
    }
    C4->Modified(); C4->Update();
    C4->SaveAs(runDir + plotDir + "scatter1.pdf");
    C4->SaveAs(runDir + plotDir + "scatter1.png");

    //gPad->Update();
    //TPaveStats *st0 = (TPaveStats*)hscatter0->FindObject("stats");
    //st0->SetX1NDC(400);
    //st0->SetX2NDC(500);
    //gPad->Update();
    //TPaveStats *st1 = (TPaveStats*)hscatter1->FindObject("stats");
    //st1->SetX1NDC(400);
    //st1->SetX2NDC(500);

    //T1->Write();
    //T2->Write();
    //hmeans->Write();
    //hrms->Write();

    exit(0);
}
