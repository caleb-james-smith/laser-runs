#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TDirectory.h>
#include "string"
#include <sstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include "TMath.h"
#include <TGraph.h>
#include "TMultiGraph.h"
#include "TLatex.h"
#include "TColor.h"

//Data.....
//{CU,RBX,Run,pd_ch,uhtr_ch,shunt,max_adc,max_fc,result} 
#include "pd_array_test.h"
//{CU,RBX,Run,RM,sipm_ch,uhtr_ch,shunt,max_adc,max_fc,result}
#include "sipm_array_test.h"

TLatex* Entries(double x, double y, TH1* hist){
  char entries [100];
  int ent = hist->GetEntries();
  sprintf (entries,"Entries: %d", ent);
  TLatex* latex = new TLatex(x, y, entries);
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextAlign(31);
  return latex;
}
TLatex* Mean(double x, double y, TH1* hist){
  char mean [100];
  double me = hist->GetMean();
  sprintf (mean,"Mean: %0.2lf", me);
  TLatex* latex = new TLatex(x, y, mean);
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextAlign(31);
  return latex;
}
TLatex* StdDev(double x, double y, TH1* hist){
  char stddev [100];
  double std = hist->GetStdDev(); 
  sprintf (stddev,"StdDev: %0.2lf", std);
  TLatex* latex = new TLatex(x, y, stddev);
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextAlign(31);
  return latex;
}

void CU_Plots(){
  int NumChanPD = pd_array.size();
  int NumChanRM = sipm_array.size();
  double convert = 1000;
  std::cout<<"Number of Pin-Diode Channels:   "<<NumChanPD<<std::endl;
  std::cout<<"Number of RM Channels:          "<<NumChanRM<<std::endl;

  //----------------------------------------------------------------------
  //Make Pin-Diode Plots
  //----------------------------------------------------------------------
  Double_t xPD0[NumChanPD],xPD1[NumChanPD],xPD2[NumChanPD],xPD3[NumChanPD],xPD4[NumChanPD],xPD5[NumChanPD];
  Double_t yPD0[NumChanPD],yPD1[NumChanPD],yPD2[NumChanPD],yPD3[NumChanPD],yPD4[NumChanPD],yPD5[NumChanPD];
  TProfile* profile_pd0 = new TProfile("pd0","pd0",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_pd1 = new TProfile("pd1","pd1",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_pd2 = new TProfile("pd2","pd2",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_pd3 = new TProfile("pd3","pd3",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_pd4 = new TProfile("pd4","pd4",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_pd5 = new TProfile("pd5","pd5",44,-0.5,43.5,0,350000/convert);
  TH1F* allPD_dist = new TH1F("allPD","allPD",62,0,350000/convert);
  TH1F* PD0_dist   = new TH1F("PD0","PD0",62,0,350000/convert);
  TH1F* PD1_dist   = new TH1F("PD1","PD1",62,0,350000/convert);
  TH1F* PD2_dist   = new TH1F("PD2","PD2",62,0,350000/convert);
  TH1F* PD3_dist   = new TH1F("PD3","PD3",62,0,350000/convert);
  TH1F* PD4_dist   = new TH1F("PD4","PD4",62,0,350000/convert);
  TH1F* PD5_dist   = new TH1F("PD5","PD5",62,0,350000/convert);
  TH1F* PD2_3_4_5_dist   = new TH1F("PD 2 3 4 5","PD 2 3 4 5",62,0,350000/convert);
  std::cout<<"Running over Pid-Diode Channels"<<std::endl;
  char process[100];
  for(unsigned channel = 0; channel < NumChanPD; ++channel){    
    sprintf(process,"Processing Channel: %i/%i",channel,NumChanPD);
    if(channel%10==0){std::cout<<process<<std::endl;}
    if(pd_array[channel][8] == 1 && pd_array[channel][3] == 0){
      xPD0[channel] = pd_array[channel][0]; yPD0[channel] = pd_array[channel][7]/convert;
      profile_pd0->Fill(pd_array[channel][0],pd_array[channel][7]/convert);
      PD0_dist->Fill(pd_array[channel][7]/convert);
    }
    else{xPD0[channel] = -100;yPD0[channel] = -100;}
    if(pd_array[channel][8] == 1 && pd_array[channel][3] == 1){
      xPD1[channel] = pd_array[channel][0]; yPD1[channel] = pd_array[channel][7]/convert;
      profile_pd1->Fill(pd_array[channel][0],pd_array[channel][7]/convert);
      PD1_dist->Fill(pd_array[channel][7]/convert);
    }
    else{xPD1[channel] = -100;yPD1[channel] = -100;}
    if(pd_array[channel][8] == 1 && pd_array[channel][3] == 2){
      xPD2[channel] = pd_array[channel][0]; yPD2[channel] = pd_array[channel][7]/convert;
      profile_pd2->Fill(pd_array[channel][0],pd_array[channel][7]/convert);
      PD2_dist->Fill(pd_array[channel][7]/convert);
    }
    else{xPD2[channel] = -100;yPD2[channel] = -100;}
    if(pd_array[channel][8] == 1 && pd_array[channel][3] == 3){
      xPD3[channel] = pd_array[channel][0]; yPD3[channel] = pd_array[channel][7]/convert;
      profile_pd3->Fill(pd_array[channel][0],pd_array[channel][7]/convert);
      PD3_dist->Fill(pd_array[channel][7]/convert);
    }
    else{xPD3[channel] = -100;yPD3[channel] = -100;}
    if(pd_array[channel][8] == 1 && pd_array[channel][3] == 4){
      xPD4[channel] = pd_array[channel][0]; yPD4[channel] = pd_array[channel][7]/convert;
      profile_pd4->Fill(pd_array[channel][0],pd_array[channel][7]/convert);
      PD4_dist->Fill(pd_array[channel][7]/convert);
    }
    else{xPD4[channel] = -100;yPD4[channel] = -100;}
    if(pd_array[channel][8] == 1 && pd_array[channel][3] == 5){
      xPD5[channel] = pd_array[channel][0]; yPD5[channel] = pd_array[channel][7]/convert;
      profile_pd5->Fill(pd_array[channel][0],pd_array[channel][7]/convert);
      PD5_dist->Fill(pd_array[channel][7]/convert);
    }
    else{xPD5[channel] = -100;yPD5[channel] = -100;}
    if(pd_array[channel][8] == 1 && (pd_array[channel][3] == 2 || pd_array[channel][3] == 3 || pd_array[channel][3] == 4 || pd_array[channel][3] == 5 )){
      PD2_3_4_5_dist->Fill(pd_array[channel][7]/convert);
    }
    allPD_dist->Fill(pd_array[channel][7]/convert);
  }
  sprintf(process,"Done with Pid-Diode Channels: %i/%i",NumChanPD,NumChanPD);
  std::cout<<process<<std::endl;
  TGraph* graph_pd0 = new TGraph(NumChanPD,xPD0,yPD0);
  TGraph* graph_pd1 = new TGraph(NumChanPD,xPD1,yPD1);
  TGraph* graph_pd2 = new TGraph(NumChanPD,xPD2,yPD2);
  TGraph* graph_pd3 = new TGraph(NumChanPD,xPD3,yPD3);
  TGraph* graph_pd4 = new TGraph(NumChanPD,xPD4,yPD4);
  TGraph* graph_pd5 = new TGraph(NumChanPD,xPD5,yPD5);
  graph_pd0->SetMarkerColor(kMagenta);     profile_pd0->SetLineColor(kMagenta);
  graph_pd1->SetMarkerColor(kOrange);      profile_pd1->SetLineColor(kOrange);
  graph_pd2->SetMarkerColor(kBlue+2);      profile_pd2->SetLineColor(kBlue+2);
  graph_pd3->SetMarkerColor(kGray);        profile_pd3->SetLineColor(kGray);
  graph_pd4->SetMarkerColor(kYellow+2);    profile_pd4->SetLineColor(kYellow+2);
  graph_pd5->SetMarkerColor(kMagenta+2);   profile_pd5->SetLineColor(kMagenta+2);      
  graph_pd0->SetMarkerStyle(kFullSquare);
  graph_pd1->SetMarkerStyle(kFullSquare);
  graph_pd2->SetMarkerStyle(kFullSquare);
  graph_pd3->SetMarkerStyle(kFullSquare);
  graph_pd4->SetMarkerStyle(kFullSquare);
  graph_pd5->SetMarkerStyle(kFullSquare);

  //----------------------------------------------------------------------
  //Make RM Plots
  //----------------------------------------------------------------------
  Double_t xRM1[NumChanRM],xRM2[NumChanRM],xRM3[NumChanRM],xRM4[NumChanRM];
  Double_t yRM1[NumChanRM],yRM2[NumChanRM],yRM3[NumChanRM],yRM4[NumChanRM];
  TProfile* profile_rm1 = new TProfile("RM1","RM1",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_rm2 = new TProfile("RM2","RM2",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_rm3 = new TProfile("RM3","RM3",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_rm4 = new TProfile("RM4","RM4",44,-0.5,43.5,0,350000/convert);  
  TH1F* allRM_dist = new TH1F("allRM","allRM",62,0,350000/convert);
  TH1F* RM1_dist   = new TH1F("RM1 Dist","RM1 Dist",62,0,350000/convert);
  TH1F* RM2_dist   = new TH1F("RM2 Dist","RM2 Dist",62,0,350000/convert);
  TH1F* RM3_dist   = new TH1F("RM3 Dist","RM3 Dist",62,0,350000/convert);
  TH1F* RM4_dist   = new TH1F("RM4 Dist","RM4 Dist",62,0,350000/convert);
  std::cout<<"Running over RM Channels"<<std::endl;
  for(unsigned channel = 0; channel < NumChanRM; ++channel){
    sprintf(process,"Processing Channel: %i/%i",channel,NumChanRM);
    if(channel%100==0){std::cout<<process<<std::endl;}
    if(sipm_array[channel][9] == 1 && sipm_array[channel][3] == 1){
      xRM1[channel] = sipm_array[channel][0]; yRM1[channel] = sipm_array[channel][8]/convert;
      profile_rm1->Fill(sipm_array[channel][0],sipm_array[channel][8]/convert);
      RM1_dist->Fill(sipm_array[channel][8]/convert);
    }
    else{xRM1[channel] = -100;yRM1[channel] = -100;}
    if(sipm_array[channel][9] == 1 && sipm_array[channel][3] == 2){
      xRM2[channel] = sipm_array[channel][0]; yRM2[channel] = sipm_array[channel][8]/convert;
      profile_rm2->Fill(sipm_array[channel][0],sipm_array[channel][8]/convert);
      RM2_dist->Fill(sipm_array[channel][8]/convert);
    }
    else{xRM2[channel] = -100;yRM2[channel] = -100;}
    if(sipm_array[channel][9] == 1 && sipm_array[channel][3] == 3){
      xRM3[channel] = sipm_array[channel][0]; yRM3[channel] = sipm_array[channel][8]/convert;
      profile_rm3->Fill(sipm_array[channel][0],sipm_array[channel][8]/convert);
      RM3_dist->Fill(sipm_array[channel][8]/convert);
    }
    else{xRM3[channel] = -100;yRM3[channel] = -100;}
    if(sipm_array[channel][9] == 1 && sipm_array[channel][3] == 4){
      xRM4[channel] = sipm_array[channel][0]; yRM4[channel] = sipm_array[channel][8]/convert;
      profile_rm4->Fill(sipm_array[channel][0],sipm_array[channel][8]/convert);
      RM4_dist->Fill(sipm_array[channel][8]/convert);
    }
    else{xRM4[channel] = -100;yRM4[channel] = -100;}
    allRM_dist->Fill(sipm_array[channel][8]/convert);
  }
  sprintf(process,"Done with RM Channels: %i/%i",NumChanRM,NumChanRM);
  std::cout<<process<<std::endl;
  TGraph* graph_rm1 = new TGraph(NumChanRM,xRM1,yRM1);
  TGraph* graph_rm2 = new TGraph(NumChanRM,xRM2,yRM2);
  TGraph* graph_rm3 = new TGraph(NumChanRM,xRM3,yRM3);
  TGraph* graph_rm4 = new TGraph(NumChanRM,xRM4,yRM4);
  graph_rm1->SetMarkerColor(kRed);        profile_rm1->SetLineColor(kRed);
  graph_rm2->SetMarkerColor(kBlue);       profile_rm2->SetLineColor(kBlue);
  graph_rm3->SetMarkerColor(kBlack);      profile_rm3->SetLineColor(kBlack);
  graph_rm4->SetMarkerColor(kGreen+2);    profile_rm4->SetLineColor(kGreen+2);
  graph_rm1->SetMarkerStyle(kFullSquare);
  graph_rm2->SetMarkerStyle(kFullSquare);
  graph_rm3->SetMarkerStyle(kFullSquare);
  graph_rm4->SetMarkerStyle(kFullSquare);

  //----------------------------------------------------------------------
  //Make TLatex
  //----------------------------------------------------------------------
  TLatex* CMSPrelim1 = new TLatex(0.14, 0.91, "CMS #scale[0.9]{#font[52]{Preliminary}}");
  CMSPrelim1->SetNDC();
  CMSPrelim1->SetTextFont(62);
  
  TLatex* burnIn = new TLatex(0.95, 0.91, "904 Burn-in 2017");
  burnIn->SetNDC();
  burnIn->SetTextFont(42);
  burnIn->SetTextAlign(31);
  
  TLatex* PinDiode = new TLatex(0.14, 0.96, "Pin-Diodes");
  PinDiode->SetNDC();
  PinDiode->SetTextFont(42);

  TLatex* RM = new TLatex(0.14, 0.96, "Silicon Photomultiplier");
  RM->SetNDC();
  RM->SetTextFont(42);

  //----------------------------------------------------------------------
  //Plotting
  //----------------------------------------------------------------------
  std::cout<<"Plotting"<<std::endl;
  TCanvas *c0 = new TCanvas("MaxpCvsCU","",800,800);  
  TLegend* catLeg0 = new TLegend(0.68,0.65,0.96,0.88);
  catLeg0->SetBorderSize(0);
  catLeg0->SetFillStyle(0);
  catLeg0->SetTextSize(0.04);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* h0blank = new TH1F("Blank0","",250,0,70);
  h0blank->SetMinimum(0);
  h0blank->SetMaximum(120000/convert);
  //h0blank->GetXaxis()->SetRange(0,100);
  h0blank->GetXaxis()->SetTitle("CU");
  h0blank->GetYaxis()->SetTitle("Max Charge [pC]");
  h0blank->SetTitle("");
  h0blank->SetName("");
  h0blank->SetTitleSize(0.002);
  h0blank->SetTitleSize(0.05,"X");
  h0blank->SetTitleSize(0.05,"Y");
  h0blank->SetTitleOffset(1.0,"X");
  h0blank->SetTitleOffset(1.2,"Y");
  h0blank->SetLabelSize(0.04,"X");
  h0blank->SetLabelSize(0.04,"Y");
  h0blank->SetStats(false);
  h0blank->SetLineColor(0);
  h0blank->Draw("hist");
  
  graph_pd0->Draw("P same");
  graph_pd1->Draw("P same");
  graph_pd2->Draw("P same");
  graph_pd3->Draw("P same");
  graph_pd4->Draw("P same");
  graph_pd5->Draw("P same");
  catLeg0->AddEntry(graph_pd0,"Pin-Diode 0","P");
  catLeg0->AddEntry(graph_pd1,"Pin-Diode 1","P");
  catLeg0->AddEntry(graph_pd2,"Pin-Diode 2","P");
  catLeg0->AddEntry(graph_pd3,"Pin-Diode 3","P");
  catLeg0->AddEntry(graph_pd4,"Pin-Diode 4","P");
  catLeg0->AddEntry(graph_pd5,"Pin-Diode 5","P");

  catLeg0->Draw();  
  CMSPrelim1->Draw();
  burnIn->Draw();
  PinDiode->Draw();

  TCanvas *c1 = new TCanvas("1D PD","",800,800);
  //TLegend* catLeg1 = new TLegend(0.75,0.65,0.96,0.88);
  //catLeg1->SetBorderSize(0);
  //catLeg1->SetFillStyle(0);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* h1blank = new TH1F("Blank1","",62,0,350000/convert);
  h1blank->SetMinimum(0);
  h1blank->SetMaximum(50);
  //h1blank->GetXaxis()->SetRange(0,100);
  h1blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h1blank->GetYaxis()->SetTitle("Channels");
  h1blank->SetTitle("");
  h1blank->SetName("");
  h1blank->SetTitleSize(0.002);
  h1blank->SetTitleSize(0.05,"X");
  h1blank->SetTitleSize(0.05,"Y");
  h1blank->SetTitleOffset(1.0,"X");
  h1blank->SetTitleOffset(1.2,"Y");
  h1blank->SetLabelSize(0.04,"X");
  h1blank->SetLabelSize(0.04,"Y");
  h1blank->SetStats(false);
  h1blank->SetLineColor(1);
  h1blank->Draw("hist");

  TLatex* Entries1 = Entries(0.92,0.85,allPD_dist);
  TLatex* Mean1    = Mean(0.92, 0.8,allPD_dist);
  TLatex* StdDev1  = StdDev(0.92, 0.75,allPD_dist); 
  
  allPD_dist->Draw("same");
  CMSPrelim1->Draw();
  burnIn->Draw();
  Entries1->Draw();
  Mean1->Draw();
  StdDev1->Draw();
  PinDiode->Draw();
  
  TCanvas *c2 = new TCanvas("CUvsRM","",800,800);  
  TLegend* catLeg2 = new TLegend(0.68,0.65,0.96,0.88);
  catLeg2->SetBorderSize(0);
  catLeg2->SetFillStyle(0);
  catLeg2->SetTextSize(0.04);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* h2blank = new TH1F("Blank2","",250,0,70);
  h2blank->SetMinimum(0);
  h2blank->SetMaximum(350000/convert);
  //h2blank->GetXaxis()->SetRange(0,100);
  h2blank->GetXaxis()->SetTitle("CU");
  h2blank->GetYaxis()->SetTitle("Max Charge [pC]");
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
  
  graph_rm1->Draw("P same");
  graph_rm2->Draw("P same");
  graph_rm3->Draw("P same");
  graph_rm4->Draw("P same");
  catLeg2->AddEntry(graph_rm1,"RM 1","P");
  catLeg2->AddEntry(graph_rm2,"RM 2","P");
  catLeg2->AddEntry(graph_rm3,"RM 3","P");
  catLeg2->AddEntry(graph_rm4,"RM 4","P");

  catLeg2->Draw();  
  CMSPrelim1->Draw();
  burnIn->Draw();
  RM->Draw();

  TCanvas *c3 = new TCanvas("1D RM","",800,800);  
  //TLegend* catLeg1 = new TLegend(0.75,0.65,0.96,0.88);
  //catLeg1->SetBorderSize(0);
  //catLeg1->SetFillStyle(0);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* h3blank = new TH1F("Blank3","",62,0,350000/convert);
  h3blank->SetMinimum(0);
  h3blank->SetMaximum(1000);
  //h3blank->GetXaxis()->SetRange(0,100);
  h3blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h3blank->GetYaxis()->SetTitle("Channels");
  h3blank->SetTitle("");
  h3blank->SetName("");
  h3blank->SetTitleSize(0.002);
  h3blank->SetTitleSize(0.05,"X");
  h3blank->SetTitleSize(0.05,"Y");
  h3blank->SetTitleOffset(1.0,"X");
  h3blank->SetTitleOffset(1.2,"Y");
  h3blank->SetLabelSize(0.04,"X");
  h3blank->SetLabelSize(0.04,"Y");
  h3blank->SetStats(false);
  h3blank->SetLineColor(1);
  h3blank->Draw("hist");
  
  TLatex* Entries3 = Entries(0.92,0.85,allRM_dist);
  TLatex* Mean3    = Mean(0.92, 0.8,allRM_dist);
  TLatex* StdDev3  = StdDev(0.92, 0.75,allRM_dist); 

  allRM_dist->Draw("same");
  CMSPrelim1->Draw();
  burnIn->Draw();
  Entries3->Draw();
  Mean3->Draw();
  StdDev3->Draw();
  RM->Draw();

  TCanvas *c4 = new TCanvas("MaxpCvsCU 0,1, RM1-4","",800,800);  
  TLegend* catLeg4 = new TLegend(0.68,0.65,0.96,0.88);
  catLeg4->SetBorderSize(0);
  catLeg4->SetFillStyle(0);
  catLeg4->SetTextSize(0.04);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* h4blank = new TH1F("Blank4","",250,0,50);
  h4blank->SetMinimum(0);
  h4blank->SetMaximum(350000/convert);
  //h4blank->GetXaxis()->SetRange(0,100);
  h4blank->GetXaxis()->SetTitle("CU");
  h4blank->GetYaxis()->SetTitle("Max Charge [pC]");
  h4blank->SetTitle("");
  h4blank->SetName("");
  h4blank->SetTitleSize(0.002);
  h4blank->SetTitleSize(0.05,"X");
  h4blank->SetTitleSize(0.05,"Y");
  h4blank->SetTitleOffset(1.0,"X");
  h4blank->SetTitleOffset(1.2,"Y");
  h4blank->SetLabelSize(0.04,"X");
  h4blank->SetLabelSize(0.04,"Y");
  h4blank->SetStats(false);
  h4blank->SetLineColor(0);
  h4blank->Draw("hist");
  
  graph_pd0->Draw("P same");
  graph_pd1->Draw("P same");
  graph_rm1->Draw("P same");
  graph_rm2->Draw("P same");
  graph_rm3->Draw("P same");
  graph_rm4->Draw("P same");
  catLeg4->AddEntry(graph_pd0,"Pin-Diode 0","P");
  catLeg4->AddEntry(graph_pd1,"Pin-Diode 1","P");
  catLeg4->AddEntry(graph_rm1,"RM 1","P");
  catLeg4->AddEntry(graph_rm2,"RM 2","P");
  catLeg4->AddEntry(graph_rm3,"RM 3","P");
  catLeg4->AddEntry(graph_rm4,"RM 4","P");

  catLeg4->Draw();  
  CMSPrelim1->Draw();
  burnIn->Draw();

  TCanvas *c5 = new TCanvas("Profile: MaxpCvsCU 0,1, RM1-4","",800,800);  
  TLegend* catLeg5 = new TLegend(0.68,0.65,0.96,0.88);
  catLeg5->SetBorderSize(0);
  catLeg5->SetFillStyle(0);
  catLeg5->SetTextSize(0.04);
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  //gPad->SetLogy();
  
  TH1F* h5blank = new TH1F("Blank5","",250,0,50);
  h5blank->SetMinimum(0);
  h5blank->SetMaximum(350000/convert);
  //h5blank->GetXaxis()->SetRange(0,100);
  h5blank->GetXaxis()->SetTitle("CU");
  h5blank->GetYaxis()->SetTitle("Max Charge [pC]");
  h5blank->SetTitle("");
  h5blank->SetName("");
  h5blank->SetTitleSize(0.002);
  h5blank->SetTitleSize(0.05,"X");
  h5blank->SetTitleSize(0.05,"Y");
  h5blank->SetTitleOffset(1.0,"X");
  h5blank->SetTitleOffset(1.2,"Y");
  h5blank->SetLabelSize(0.04,"X");
  h5blank->SetLabelSize(0.04,"Y");
  h5blank->SetStats(false);
  h5blank->SetLineColor(0);
  h5blank->Draw("hist");
  
  profile_pd0->Draw("P same");
  profile_pd1->Draw("P same");
  profile_rm1->Draw("P same");
  profile_rm2->Draw("P same");
  profile_rm3->Draw("P same");
  profile_rm4->Draw("P same");
  catLeg5->AddEntry(profile_pd0,"Pin-Diode 0","P");
  catLeg5->AddEntry(profile_pd1,"Pin-Diode 1","P");
  catLeg5->AddEntry(profile_rm1,"RM 1","P");
  catLeg5->AddEntry(profile_rm2,"RM 2","P");
  catLeg5->AddEntry(profile_rm3,"RM 3","P");
  catLeg5->AddEntry(profile_rm4,"RM 4","P");

  catLeg4->Draw();  
  CMSPrelim1->Draw();
  burnIn->Draw();

  TCanvas *d5  = new TCanvas("1D PD0","",800,800);
  PD0_dist->Draw();  
  TCanvas *d6  = new TCanvas("1D PD1","",800,800);
  PD1_dist->Draw();  
  TCanvas *d7  = new TCanvas("1D PD2","",800,800);
  PD2_dist->Draw();  
  TCanvas *d8  = new TCanvas("1D PD3","",800,800);
  PD3_dist->Draw();  
  TCanvas *d9  = new TCanvas("1D PD4","",800,800);
  PD4_dist->Draw();  
  TCanvas *d10  = new TCanvas("1D PD5","",800,800);
  PD5_dist->Draw();  
  TCanvas *d11 = new TCanvas("1D PD2 3 4 5","",800,800);
  PD2_3_4_5_dist->Draw();
  TCanvas *d12 = new TCanvas("1D RM1","",800,800);
  RM1_dist->Draw();
  TCanvas *d13 = new TCanvas("1D RM2","",800,800);
  RM2_dist->Draw();
  TCanvas *d14 = new TCanvas("1D RM3","",800,800);
  RM3_dist->Draw();
  TCanvas *d15 = new TCanvas("1D RM4","",800,800);
  RM4_dist->Draw();

  c0->SaveAs("PDAllvsCU2D.pdf");
  c1->SaveAs("PDAll_1D.pdf");
  c2->SaveAs("RMAllvsCU2D.pdf");
  c3->SaveAs("RMAll_1D.pdf");
  c4->SaveAs("RM_PDvsCU2D.pdf");
  c5->SaveAs("profile_RM_PDvsCU2D.pdf");
  
  d5->SaveAs("PD0_1D.pdf");
  d6->SaveAs("PD1_1D.pdf");
  d7->SaveAs("PD2_1D.pdf");
  d8->SaveAs("PD3_1D.pdf");
  d9->SaveAs("PD4_1D.pdf");
  d10->SaveAs("PD5_1D.pdf");
  d11->SaveAs("PD2_3_4_5_1D.pdf");
  d12->SaveAs("RM1_1D.pdf");
  d13->SaveAs("RM2_1D.pdf");
  d14->SaveAs("RM3_1D.pdf");
  d15->SaveAs("RM4_1D.pdf");
} 

int main(){
  CU_Plots();
}
