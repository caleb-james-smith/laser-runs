#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TDirectory.h>
//Data.....
//#include "CU_Data.h"
#include "pd_array.h" 

#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include "TMath.h"
#include <TGraph.h>
#include "TLatex.h"
#include "TColor.h"

TGraph* makePDTGraph(double iphi,double depth ,std::vector<std::vector<double>>& datain){
  int NumChan = pd_array.size(); //Need to change this
  Double_t x[NumChan], y[NumChan];
  for(unsigned channel = 0; channel < NumChan; ++channel){
    if(datain[channel][1] == iphi){
      if(datain[channel][2] == depth){
	x[channel] = datain[channel][0];
	y[channel] = datain[channel][7];	
      }
    }
  }
  TGraph* gtemp = new TGraph(NumChan,x,y);
  gtemp->SetMarkerStyle(kFullSquare);
  return gtemp;
}

void CU_Plots(){
  TCanvas *c0 = new TCanvas("PulseShape_203","",800,800);  
  TLegend* catLeg0 = new TLegend(0.75,0.45,0.96,0.88);
  catLeg0->SetBorderSize(0);
  catLeg0->SetFillStyle(0);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* hblank = new TH1F("","",250,16.1,22.9);
  hblank->SetMinimum(2);
  hblank->SetMaximum(6);
  //hblank->GetXaxis()->SetRange(0,100);
  hblank->GetXaxis()->SetTitle("i#eta");
  hblank->GetYaxis()->SetTitle("photoelectrons / MIP / layer");
  hblank->SetTitle("");
  hblank->SetName("");
  hblank->SetTitleSize(0.002);
  hblank->SetTitleSize(0.05,"X");
  hblank->SetTitleSize(0.05,"Y");
  hblank->SetTitleOffset(1.0,"X");
  hblank->SetTitleOffset(1.2,"Y");
  hblank->SetLabelSize(0.04,"X");
  hblank->SetLabelSize(0.04,"Y");
  hblank->SetStats(false);
  hblank->SetLineColor(1);
  hblank->Draw("hist");

  for(unsigned phi = 1; phi < 12; ++phi){
    for(unsigned depth1 = 2; depth1 < 7; ++depth1){
      TGraph* phi_depth1 = NULL;
      phi_depth1 = makePDTGraph(phi,depth1,dataStuff);
      if(depth1 == 2){phi_depth1->SetMarkerColor(kBlack);}
      if(depth1 == 3){phi_depth1->SetMarkerColor(kRed);}
      if(depth1 == 4){phi_depth1->SetMarkerColor(kBlue);}
      if(depth1 == 5){phi_depth1->SetMarkerColor(kGreen+2);}
      if(depth1 == 6){phi_depth1->SetMarkerColor(kOrange);}      
      if(phi == 5){phi_depth1->SetMarkerStyle(kFullCircle);}
      //std::cout<<depth1<<endl;
      phi_depth1->Draw("P same");
      //catLeg0->AddEntry(phi_depth1,"Depth");
    }
  }

  TLatex* CMSPrelim1 = new TLatex(0.14, 0.91, "CMS #scale[0.9]{#font[52]{Preliminary}}");
  CMSPrelim1->SetNDC();
  CMSPrelim1->SetTextFont(62);

  TLatex* testbeam = new TLatex(0.95, 0.91, "Testbeam 2017");
  testbeam->SetNDC();
  testbeam->SetTextFont(42);
  testbeam->SetTextAlign(31);

  CMSPrelim1->Draw();
  testbeam->Draw();
  
  //Double_t x1[1],y1[1];
  //catLeg0->AddEntry((TObject*)0,"iPhi 5","");
  //TGraph* phi5depth2 = new TGraph(1,x1,y1);
  //phi5depth2->Draw("same P");
  //phi5depth2->SetMarkerColor(kBlack);
  //phi5depth2->SetMarkerStyle(kFullCircle);
  //catLeg0->AddEntry(phi5depth2,"Depth 2","P");
  //TGraph* phi5depth3 = new TGraph(1,x1,y1);
  //phi5depth3->Draw("same P");
  //phi5depth3->SetMarkerColor(kRed);
  //phi5depth3->SetMarkerStyle(kFullCircle);
  //catLeg0->AddEntry(phi5depth3,"Depth 3","P");
  //TGraph* phi5depth4 = new TGraph(1,x1,y1);
  //phi5depth4->Draw("same P");
  //phi5depth4->SetMarkerColor(kBlue);
  //phi5depth4->SetMarkerStyle(kFullCircle);
  //catLeg0->AddEntry(phi5depth4,"Depth 4","P");
  //TGraph* phi5depth5 = new TGraph(1,x1,y1);
  //phi5depth5->Draw("same P");
  //phi5depth5->SetMarkerColor(kGreen+2);
  //phi5depth5->SetMarkerStyle(kFullCircle);
  //catLeg0->AddEntry(phi5depth5,"Depth 5","P");
  //TGraph* phi5depth6 = new TGraph(1,x1,y1);
  //phi5depth6->Draw("same P");
  //phi5depth6->SetMarkerColor(kOrange);
  //phi5depth6->SetMarkerStyle(kFullCircle);
  //catLeg0->AddEntry(phi5depth6,"Depth 6","P");
  //
  //catLeg0->AddEntry((TObject*)0, "", "");
  //
  //catLeg0->AddEntry((TObject*)0,"iPhi 6","");
  //TGraph* phi6depth2 = new TGraph(1,x1,y1);
  //phi6depth2->Draw("same P");
  //phi6depth2->SetMarkerColor(kBlack);
  //phi6depth2->SetMarkerStyle(kFullSquare);
  //catLeg0->AddEntry(phi6depth2,"Depth 2","P");
  //TGraph* phi6depth3 = new TGraph(1,x1,y1);
  //phi6depth3->Draw("same P");
  //phi6depth3->SetMarkerColor(kRed);
  //phi6depth3->SetMarkerStyle(kFullSquare);
  //catLeg0->AddEntry(phi6depth3,"Depth 3","P");
  //TGraph* phi6depth4 = new TGraph(1,x1,y1);
  //phi6depth4->Draw("same P");
  //phi6depth4->SetMarkerColor(kBlue);
  //phi6depth4->SetMarkerStyle(kFullSquare);
  //catLeg0->AddEntry(phi6depth4,"Depth 4","P");
  //TGraph* phi6depth5 = new TGraph(1,x1,y1);
  //phi6depth5->Draw("same P");
  //phi6depth5->SetMarkerColor(kGreen+2);
  //phi6depth5->SetMarkerStyle(kFullSquare);
  //catLeg0->AddEntry(phi6depth5,"Depth 5","P");
  //TGraph* phi6depth6 = new TGraph(1,x1,y1);
  //phi6depth6->Draw("same P");
  //phi6depth6->SetMarkerColor(kOrange);
  //phi6depth6->SetMarkerStyle(kFullSquare);
  //catLeg0->AddEntry(phi6depth6,"Depth 6","P");
  
  catLeg0->SetTextSize(0.04);
  //catLeg0->AddEntry(hblank,"current 203","l");
  catLeg0->Draw();

  TCanvas *c1 = new TCanvas("1D Channels vs PE/MIP/Layer","",800,800);  
  //TLegend* catLeg1 = new TLegend(0.75,0.65,0.96,0.88);
  //catLeg1->SetBorderSize(0);
  //catLeg1->SetFillStyle(0);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* hmip = new TH1F("","",20,-0.5,9.5);
  hmip->SetMinimum(0);
  hmip->SetMaximum(13);
  //hmip->GetXaxis()->SetRange(0,100);
  hmip->GetXaxis()->SetTitle("photoelectrons / MIP / layer");
  hmip->GetYaxis()->SetTitle("Channels");
  hmip->SetTitle("");
  hmip->SetName("");
  hmip->SetTitleSize(0.002);
  hmip->SetTitleSize(0.05,"X");
  hmip->SetTitleSize(0.05,"Y");
  hmip->SetTitleOffset(1.0,"X");
  hmip->SetTitleOffset(1.2,"Y");
  hmip->SetLabelSize(0.04,"X");
  hmip->SetLabelSize(0.04,"Y");
  hmip->SetStats(false);
  hmip->SetLineColor(1);
  hmip->Draw("hist");

  //for(unsigned channel = 0; channel < NumChan; ++channel){
  //  hmip->Fill(dataStuff[channel][3]);
  //}

  char entries [100];
  int ent = hmip->GetEntries();
  sprintf (entries,"Entries: %d", ent);

  char mean [100];
  double me = hmip->GetMean();
  sprintf (mean,"Mean: %0.2lf", me);
  
  char stddev [100];
  double std = hmip->GetStdDev(); 
  sprintf (stddev,"StdDev: %0.2lf", std);
  
  TLatex* Entries = new TLatex(0.92, 0.85, entries);
  Entries->SetNDC();
  Entries->SetTextFont(42);
  Entries->SetTextAlign(31);

  TLatex* Mean = new TLatex(0.92, 0.8, mean);
  Mean->SetNDC();
  Mean->SetTextFont(42);
  Mean->SetTextAlign(31);

  TLatex* StdDev = new TLatex(0.92, 0.75, stddev);
  StdDev->SetNDC();
  StdDev->SetTextFont(42);
  StdDev->SetTextAlign(31);

  CMSPrelim1->Draw();
  testbeam->Draw();
  Entries->Draw();
  Mean->Draw();
  StdDev->Draw();
  
  //c0->SaveAs("PEvsEta.pdf");
  //c1->SaveAs("ChannelvsPE.pdf");
  
} 
//
//int main(){
//  TestBeamMipPlots();
//}
