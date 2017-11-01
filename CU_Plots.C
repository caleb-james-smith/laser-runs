//#include <TFile.h>
//#include <TTree.h>
#include <TH1.h>
//#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
//#include <TDirectory.h>
//Data.....
//{CU,RBX,Run,pd_ch,uhtr_ch,shunt,max_adc,max_fc,result} 
#include "pd_array.h"
//{CU,RBX,Run,RM,sipm_ch,uhtr_ch,shunt,max_adc,max_fc,result}
#include "sipm_array.h"

//#include <string>
//#include <sstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
//#include "TMath.h"
#include <TGraph.h>
#include "TMultiGraph.h"
#include "TLatex.h"
//#include "TColor.h"

int NumChanPD = pd_array.size(); //Need to change this
int NumChanRM = sipm_array.size(); //Need to change this

TGraph* makePDTGraph(int pd,std::vector<std::vector<double>>& datain){
  Double_t x[NumChanPD], y[NumChanPD];
  for(unsigned channel = 0; channel < NumChanPD; ++channel){
    if(datain[channel][3] == pd){
      x[channel] = datain[channel][0];
      y[channel] = datain[channel][6];	
      //std::cout<<"Pin-Diode:  "<<pd<<"  CU:  "<<x[channel]<<"  MaxADC:  "<<y[channel]<<std::endl;
    }
    else{
      x[channel] = -100;
      y[channel] = -100;		
    }
  }
  TGraph* gtemp = new TGraph(NumChanPD,x,y);
  if(pd == 0){gtemp->SetMarkerColor(kBlack);}
  if(pd == 1){gtemp->SetMarkerColor(kRed);}
  if(pd == 2){gtemp->SetMarkerColor(kBlue);}
  if(pd == 3){gtemp->SetMarkerColor(kGreen+2);}
  if(pd == 4){gtemp->SetMarkerColor(kOrange);}
  if(pd == 5){gtemp->SetMarkerColor(kMagenta);}      
  gtemp->SetMarkerStyle(kFullSquare);
  return gtemp;
}

TGraph* makeRMTGraph(double rm,std::vector<std::vector<double>>& datain){
  Double_t x[NumChanRM], y[NumChanRM];
  for(unsigned channel = 0; channel < NumChanRM; ++channel){
    if(datain[channel][3] == rm){
      x[channel] = datain[channel][0];
      y[channel] = datain[channel][7];	
      //std::cout<<"RM:  "<<rm<<"  CU:  "<<x[channel]<<"  MaxADC:  "<<y[channel]<<std::endl;
    }
    else{
      x[channel] = -100;
      y[channel] = -100;		
    }
  }
  TGraph* gtemp = new TGraph(NumChanRM,x,y);
  if(rm == 1){gtemp->SetMarkerColor(kRed);};
  if(rm == 2){gtemp->SetMarkerColor(kBlue);};
  if(rm == 3){gtemp->SetMarkerColor(kGreen+2);};
  if(rm == 4){gtemp->SetMarkerColor(kBlack);};
  gtemp->SetMarkerStyle(kFullSquare);
  return gtemp;
}

void CU_Plots(){
  std::cout<<"Number of Pin-Diode Channels:   "<<NumChanPD<<std::endl;
  std::cout<<"Number of RM Channels:   "<<NumChanRM<<std::endl;
  
  TCanvas *c0 = new TCanvas("MacADCvsCU","",800,800);  
  TLegend* catLeg0 = new TLegend(0.68,0.65,0.96,0.88);
  catLeg0->SetBorderSize(0);
  catLeg0->SetFillStyle(0);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* hblank = new TH1F("","",250,0,65);
  hblank->SetMinimum(0);
  hblank->SetMaximum(300);
  //hblank->GetXaxis()->SetRange(0,100);
  hblank->GetXaxis()->SetTitle("CU");
  hblank->GetYaxis()->SetTitle("Max ADC");
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
  hblank->SetLineColor(0);
  hblank->Draw("hist");

  TGraph* graph_pd0 = makePDTGraph(0,pd_array);
  TGraph* graph_pd1 = makePDTGraph(1,pd_array);
  TGraph* graph_pd2 = makePDTGraph(2,pd_array);
  TGraph* graph_pd3 = makePDTGraph(3,pd_array);
  TGraph* graph_pd4 = makePDTGraph(4,pd_array);
  TGraph* graph_pd5 = makePDTGraph(5,pd_array);
  graph_pd0->Draw("P same");
  graph_pd1->Draw("P same");
  graph_pd2->Draw("P same");
  graph_pd3->Draw("P same");
  graph_pd4->Draw("P same");
  graph_pd5->Draw("P same");

  TLatex* CMSPrelim1 = new TLatex(0.14, 0.91, "CMS #scale[0.9]{#font[52]{Preliminary}}");
  CMSPrelim1->SetNDC();
  CMSPrelim1->SetTextFont(62);

  TLatex* burnIn = new TLatex(0.95, 0.91, "904 Burn In 2017");
  burnIn->SetNDC();
  burnIn->SetTextFont(42);
  burnIn->SetTextAlign(31);

  TLatex* PinDiode = new TLatex(0.14, 0.96, "Pin-Diodes");
  PinDiode->SetNDC();
  PinDiode->SetTextFont(42);
  
  CMSPrelim1->Draw();
  burnIn->Draw();
  PinDiode->Draw();
  
  Double_t x1[1]={-100},y1[1]={-100};
  //catLeg0->AddEntry((TObject*)0,"iPhi 5","");
  TGraph* pinDiode0 = new TGraph(1,x1,y1);
  pinDiode0->Draw("same P");
  pinDiode0->SetMarkerColor(kBlack);
  pinDiode0->SetMarkerStyle(kFullSquare);
  catLeg0->AddEntry(pinDiode0,"Pin-Diode 0","P");
  TGraph* pinDiode1 = new TGraph(1,x1,y1);
  pinDiode1->Draw("same P");
  pinDiode1->SetMarkerColor(kRed);
  pinDiode1->SetMarkerStyle(kFullSquare);
  catLeg0->AddEntry(pinDiode1,"Pin-Diode 1","P");
  TGraph* pinDiode2 = new TGraph(1,x1,y1);
  pinDiode2->Draw("same P");
  pinDiode2->SetMarkerColor(kBlue);
  pinDiode2->SetMarkerStyle(kFullSquare);
  catLeg0->AddEntry(pinDiode2,"Pin-Diode 2","P");
  TGraph* pinDiode3 = new TGraph(1,x1,y1);
  pinDiode3->Draw("same P");
  pinDiode3->SetMarkerColor(kGreen+2);
  pinDiode3->SetMarkerStyle(kFullSquare);
  catLeg0->AddEntry(pinDiode3,"Pin-Diode 3","P");
  TGraph* pinDiode4 = new TGraph(1,x1,y1);
  pinDiode4->Draw("same P");
  pinDiode4->SetMarkerColor(kOrange);
  pinDiode4->SetMarkerStyle(kFullSquare);
  catLeg0->AddEntry(pinDiode4,"Pin-Diode 4","P");
  TGraph* pinDiode5 = new TGraph(1,x1,y1);
  pinDiode5->Draw("same P");
  pinDiode5->SetMarkerColor(kMagenta);
  pinDiode5->SetMarkerStyle(kFullSquare);
  catLeg0->AddEntry(pinDiode5,"Pin-Diode 5","P");
  
  catLeg0->SetTextSize(0.04);
  catLeg0->Draw();

  TCanvas *c1 = new TCanvas("1D PD","",800,800);  
  //TLegend* catLeg1 = new TLegend(0.75,0.65,0.96,0.88);
  //catLeg1->SetBorderSize(0);
  //catLeg1->SetFillStyle(0);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* hmip = new TH1F("","",62,0,250);
  hmip->SetMinimum(0);
  hmip->SetMaximum(35);
  //hmip->GetXaxis()->SetRange(0,100);
  hmip->GetXaxis()->SetTitle("Max ADC");
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
  
  for(unsigned channel = 0; channel < NumChanPD; ++channel){
    hmip->Fill(pd_array[channel][6]);
  }
  
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
  burnIn->Draw();
  Entries->Draw();
  Mean->Draw();
  StdDev->Draw();
  PinDiode->Draw();
  
  TCanvas *c2 = new TCanvas("CUvsRM","",800,800);  
  TLegend* catLeg2 = new TLegend(0.68,0.65,0.96,0.88);
  catLeg2->SetBorderSize(0);
  catLeg2->SetFillStyle(0);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* h2blank = new TH1F("","",250,0,65);
  h2blank->SetMinimum(0);
  h2blank->SetMaximum(275);
  //h2blank->GetXaxis()->SetRange(0,100);
  h2blank->GetXaxis()->SetTitle("CU");
  h2blank->GetYaxis()->SetTitle("Max ADC");
  h2blank->SetTitle("");
  h2blank->SetName("");
  h2blank->SetTitleSize(0.002);
  h2blank->SetTitleSize(0.05,"X");
  h2blank->SetTitleSize(0.05,"Y");
  h2blank->SetTitleOffset(1.0,"X");
  h2blank->SetTitleOffset(1.2,"Y");
  h2blank->SetLabelSize(0.04,"X");
  h2blank->SetLabelSize(0.04,"Y");
  h2blank->SetStats(false);
  h2blank->SetLineColor(1);
  h2blank->Draw("hist");
  
  //TGraph* graph_rm1 = NULL;
  //TGraph* graph_rm2 = NULL;
  //TGraph* graph_rm3 = NULL;
  //TGraph* graph_rm4 = NULL;
  TGraph* graph_rm1 = makeRMTGraph(1,sipm_array);
  TGraph* graph_rm2 = makeRMTGraph(2,sipm_array);
  TGraph* graph_rm3 = makeRMTGraph(3,sipm_array);
  TGraph* graph_rm4 = makeRMTGraph(4,sipm_array);
  //graph_rm1->SetMarkerColor(kRed);
  //graph_rm2->SetMarkerColor(kBlue);
  //graph_rm3->SetMarkerColor(kGreen+2);
  //graph_rm4->SetMarkerColor(kBlack);
  graph_rm1->Draw("P same");
  graph_rm2->Draw("P same");
  graph_rm3->Draw("P same");
  graph_rm4->Draw("P same");

  TLatex* RM = new TLatex(0.14, 0.96, "Silicon Photomultiplier");
  RM->SetNDC();
  RM->SetTextFont(42);
  
  CMSPrelim1->Draw();
  burnIn->Draw();
  RM->Draw();
  
  //Double_t x1[1]={-100},y1[1]={-100};
  //catLeg2->AddEntry((TObject*)0,"iPhi 5","");
  TGraph* readOutModule1 = new TGraph(1,x1,y1);
  readOutModule1->Draw("same P");
  readOutModule1->SetMarkerColor(kRed);
  readOutModule1->SetMarkerStyle(kFullSquare);
  catLeg2->AddEntry(readOutModule1,"RM 1","P");
  TGraph* readOutModule2 = new TGraph(1,x1,y1);
  readOutModule2->Draw("same P");
  readOutModule2->SetMarkerColor(kBlue);
  readOutModule2->SetMarkerStyle(kFullSquare);
  catLeg2->AddEntry(readOutModule2,"RM 2","P");
  TGraph* readOutModule3 = new TGraph(1,x1,y1);
  readOutModule3->Draw("same P");
  readOutModule3->SetMarkerColor(kGreen+2);
  readOutModule3->SetMarkerStyle(kFullSquare);
  catLeg2->AddEntry(readOutModule3,"RM 3","P");
  TGraph* readOutModule4 = new TGraph(1,x1,y1);
  readOutModule4->Draw("same P");
  readOutModule4->SetMarkerColor(kBlack);
  readOutModule4->SetMarkerStyle(kFullSquare);
  catLeg2->AddEntry(readOutModule4,"RM 4","P");
  
  catLeg2->SetTextSize(0.04);
  catLeg2->Draw();  

  TCanvas *c3 = new TCanvas("1D RM","",800,800);  
  //TLegend* catLeg1 = new TLegend(0.75,0.65,0.96,0.88);
  //catLeg1->SetBorderSize(0);
  //catLeg1->SetFillStyle(0);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* h3 = new TH1F("","",62,0,250);
  h3->SetMinimum(0);
  h3->SetMaximum(1000);
  //h3->GetXaxis()->SetRange(0,100);
  h3->GetXaxis()->SetTitle("Max ADC");
  h3->GetYaxis()->SetTitle("Channels");
  h3->SetTitle("");
  h3->SetName("");
  h3->SetTitleSize(0.002);
  h3->SetTitleSize(0.05,"X");
  h3->SetTitleSize(0.05,"Y");
  h3->SetTitleOffset(1.0,"X");
  h3->SetTitleOffset(1.2,"Y");
  h3->SetLabelSize(0.04,"X");
  h3->SetLabelSize(0.04,"Y");
  h3->SetStats(false);
  h3->SetLineColor(1);
  h3->Draw("hist");
  
  for(unsigned channel = 0; channel < NumChanRM; ++channel){
    h3->Fill(sipm_array[channel][7]);
  }
  
  char entries3 [100];
  int ent3 = h3->GetEntries();
  sprintf (entries3,"Entries: %d", ent3);
  
  char mean3 [100];
  double me3 = h3->GetMean();
  sprintf (mean3,"Mean: %0.2lf", me3);
  
  char stddev3 [100];
  double std3 = h3->GetStdDev(); 
  sprintf (stddev3,"StdDev: %0.2lf", std3);
  
  TLatex* Entries3 = new TLatex(0.92, 0.85, entries3);
  Entries3->SetNDC();
  Entries3->SetTextFont(42);
  Entries3->SetTextAlign(31);
  
  TLatex* Mean3 = new TLatex(0.92, 0.8, mean3);
  Mean3->SetNDC();
  Mean3->SetTextFont(42);
  Mean3->SetTextAlign(31);
  
  TLatex* Stddev3 = new TLatex(0.92, 0.75, stddev3);
  Stddev3->SetNDC();
  Stddev3->SetTextFont(42);
  Stddev3->SetTextAlign(31);
  
  CMSPrelim1->Draw();
  burnIn->Draw();
  Entries3->Draw();
  Mean3->Draw();
  Stddev3->Draw();
  RM->Draw();
  
  c0->SaveAs("PDvsCU2D.pdf");
  c1->SaveAs("PDvsCU1D.pdf");
  c2->SaveAs("RMvsCU2D.pdf");
  c3->SaveAs("RMvsCU1D.pdf");
} 

int main(){
  CU_Plots();
}
