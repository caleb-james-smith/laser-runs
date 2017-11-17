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
//#include "pd_array_test.h"
//#include "passed_cu_data/pd_array.h"
//{CU,RBX,Run,RM,sipm_ch,uhtr_ch,shunt,max_adc,max_fc,result}
//#include "sipm_array_test.h"
//#include "passed_cu_data/sipm_array.h"

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
TCanvas* Canvas(const char* name,int length,int width){
  TCanvas* c = new TCanvas(name,name,length,width);  
  gPad->SetTopMargin(0.1);
  gPad->SetBottomMargin(0.12);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);  
  return c;
}
TH1F* Blank(const char* name,double num,double low,double high){
  TH1F* blank = new TH1F(name,name,num,low,high);
  //blank->GetXaxis()->SetRange(0,100);
  blank->SetTitle("");
  blank->SetName("");
  blank->SetTitleSize(0.002);
  blank->SetTitleSize(0.05,"X");
  blank->SetTitleSize(0.05,"Y");
  blank->SetTitleOffset(1.0,"X");
  blank->SetTitleOffset(1.2,"Y");
  blank->SetLabelSize(0.04,"X");
  blank->SetLabelSize(0.04,"Y");
  blank->SetStats(false);
  blank->SetLineColor(0);  
  return blank;
}

void CU_Plots(){
  //----------------------------------------------------------------------
  //Setup
  //----------------------------------------------------------------------
  //const char* infile1 = "passed_cu_data/pd.root";
  //const char* infile2 = "passed_cu_data/sipm.root";
  //const char* infile1 = "rework_cu_data/pd.root";
  //const char* infile2 = "rework_cu_data/sipm.root";
  const char* infile1 = "stability_cu_data/pd.root";
  const char* infile2 = "stability_cu_data/sipm.root"; 

  TFile *f1 = new TFile(infile1);
  if(f1->IsZombie()){
      cout << "Root file: " << infile1 << " not found!" << endl;
      return;
  }
  TFile *f2 = new TFile(infile2);
  if(f1->IsZombie()){
      cout << "Root file: " << infile2 << " not found!" << endl;
      return;
  }
  TTreeReader tReader1("t1",f1);
  TTreeReader tReader2("t1",f2);

  TTreeReaderValue<Float_t> pd_CU(tReader1, "cu");
  TTreeReaderValue<Float_t> pd_rbx(tReader1, "rbx");
  TTreeReaderValue<Float_t> pd_run(tReader1, "run");
  TTreeReaderValue<Float_t> pd_ch(tReader1, "pd_ch");
  TTreeReaderValue<Float_t> pd_uhtr_ch(tReader1, "uhtr_ch");
  TTreeReaderValue<Float_t> pd_shunt(tReader1, "shunt");
  TTreeReaderValue<Float_t> pd_max_adc(tReader1, "max_adc");
  TTreeReaderValue<Float_t> pd_max_fc(tReader1, "max_fc");
  TTreeReaderValue<Float_t> pd_result(tReader1, "result"); 

  TTreeReaderValue<Float_t> sipm_CU(tReader2, "cu");
  TTreeReaderValue<Float_t> sipm_rbx(tReader2, "rbx");
  TTreeReaderValue<Float_t> sipm_run(tReader2, "run");
  TTreeReaderValue<Float_t> sipm_rm(tReader2, "rm");
  TTreeReaderValue<Float_t> sipm_ch(tReader2, "sipm_ch");
  TTreeReaderValue<Float_t> sipm_uhtr_ch(tReader2, "uhtr_ch");
  TTreeReaderValue<Float_t> sipm_shunt(tReader2, "shunt");
  TTreeReaderValue<Float_t> sipm_max_adc(tReader2, "max_adc");
  TTreeReaderValue<Float_t> sipm_max_fc(tReader2, "max_fc");
  TTreeReaderValue<Float_t> sipm_result(tReader2, "result"); 

  int NumChanPD = 0;
  int NumChanRM = 0;
  while(tReader1.Next()){NumChanPD+=1;}
  while(tReader2.Next()){NumChanRM+=1;}
  tReader1.Restart();
  tReader2.Restart();
  double convert = 1000;
  double pinDisMin = 0;
  double pinDisMax = 6;
  double rmDisMin = 0;
  double rmDisMax = 100;
  std::cout<<"Number of Pin-Diode Channels:   "<<NumChanPD<<std::endl;
  std::cout<<"Number of RM Channels:          "<<NumChanRM<<std::endl;
  //----------------------------------------------------------------------
  //Make Pin-Diode Plots
  //----------------------------------------------------------------------
  Double_t xPD0[NumChanPD],xPD1[NumChanPD],xPD2[NumChanPD],xPD3[NumChanPD],xPD4[NumChanPD],xPD5[NumChanPD];
  Double_t yPD0[NumChanPD],yPD1[NumChanPD],yPD2[NumChanPD],yPD3[NumChanPD],yPD4[NumChanPD],yPD5[NumChanPD];
  Double_t zPD0[NumChanPD],zPD1[NumChanPD],zPD2[NumChanPD],zPD3[NumChanPD],zPD4[NumChanPD],zPD5[NumChanPD];
  TProfile* profile_pd0 = new TProfile("pd0","pd0",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_pd1 = new TProfile("pd1","pd1",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_pd2 = new TProfile("pd2","pd2",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_pd3 = new TProfile("pd3","pd3",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_pd4 = new TProfile("pd4","pd4",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_pd5 = new TProfile("pd5","pd5",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_stab_pd0 = new TProfile("pd0 stab","pd0 stab",10,-0.5,9.5,0,350000/convert);
  TProfile* profile_stab_pd1 = new TProfile("pd1 stab","pd1 stab",10,-0.5,9.5,0,350000/convert);
  TProfile* profile_stab_pd2 = new TProfile("pd2 stab","pd2 stab",10,-0.5,9.5,0,350000/convert);
  TProfile* profile_stab_pd3 = new TProfile("pd3 stab","pd3 stab",10,-0.5,9.5,0,350000/convert);
  TProfile* profile_stab_pd4 = new TProfile("pd4 stab","pd4 stab",10,-0.5,9.5,0,350000/convert);
  TProfile* profile_stab_pd5 = new TProfile("pd5 stab","pd5 stab",10,-0.5,9.5,0,350000/convert);
  TH1F* allPD_dist = new TH1F("allPD","allPD",800,0,350000/convert);
  TH1F* PD0_dist   = new TH1F("PD0","PD0",800,0,350000/convert);
  TH1F* PD1_dist   = new TH1F("PD1","PD1",800,0,350000/convert);
  TH1F* PD2_dist   = new TH1F("PD2","PD2",800,0,350000/convert);
  TH1F* PD3_dist   = new TH1F("PD3","PD3",800,0,350000/convert);
  TH1F* PD4_dist   = new TH1F("PD4","PD4",800,0,350000/convert);
  TH1F* PD5_dist   = new TH1F("PD5","PD5",800,0,350000/convert);
  TH1F* PD2_3_4_5_dist   = new TH1F("PD 2 3 4 5","PD 2 3 4 5",62,0,350000/convert);
  std::cout<<"Running over Pin-Diode Channels"<<std::endl;
  char process[100];
  int channel1 = -1;
  while(tReader1.Next()){
    channel1+=1;
    sprintf(process,"Processing Channel: %i/%i",channel1,NumChanPD);
    if(channel1%10==0){std::cout<<process<<std::endl;}
    if(*pd_ch == 0){
      xPD0[channel1] = *pd_CU; yPD0[channel1] = *pd_max_fc/convert; zPD0[channel1] = *pd_run;
      profile_pd0->Fill(*pd_CU,*pd_max_fc/convert);
      profile_stab_pd0->Fill(*pd_run,*pd_max_fc/convert);
      PD0_dist->Fill(*pd_max_fc/convert);
    }
    else{xPD0[channel1] = -100;yPD0[channel1] = -100;zPD0[channel1] = -100;}
    if(*pd_ch == 1){
      xPD1[channel1] = *pd_CU; yPD1[channel1] = *pd_max_fc/convert; zPD1[channel1] = *pd_run;
      profile_pd1->Fill(*pd_CU,*pd_max_fc/convert);
      profile_stab_pd1->Fill(*pd_run,*pd_max_fc/convert);
      PD1_dist->Fill(*pd_max_fc/convert);
    }
    else{xPD1[channel1] = -100;yPD1[channel1] = -100;zPD1[channel1] = -100;}
    if(*pd_ch == 2){
      xPD2[channel1] = *pd_CU; yPD2[channel1] = *pd_max_fc/convert; zPD2[channel1] = *pd_run;
      profile_pd2->Fill(*pd_CU,*pd_max_fc/convert);
      profile_stab_pd2->Fill(*pd_run,*pd_max_fc/convert);
      PD2_dist->Fill(*pd_max_fc/convert);
    }
    else{xPD2[channel1] = -100;yPD2[channel1] = -100;zPD2[channel1] = -100;}
    if(*pd_ch == 3){
      xPD3[channel1] = *pd_CU; yPD3[channel1] = *pd_max_fc/convert; zPD3[channel1] = *pd_run;
      profile_pd3->Fill(*pd_CU,*pd_max_fc/convert);
      profile_stab_pd3->Fill(*pd_run,*pd_max_fc/convert);
      PD3_dist->Fill(*pd_max_fc/convert);
    }
    else{xPD3[channel1] = -100;yPD3[channel1] = -100;zPD3[channel1] = -100;}
    if(*pd_ch == 4){
      xPD4[channel1] = *pd_CU; yPD4[channel1] = *pd_max_fc/convert; zPD4[channel1] = *pd_run;
      profile_pd4->Fill(*pd_CU,*pd_max_fc/convert);
      profile_stab_pd4->Fill(*pd_run,*pd_max_fc/convert);
      PD4_dist->Fill(*pd_max_fc/convert);
    }
    else{xPD4[channel1] = -100;yPD4[channel1] = -100;zPD4[channel1] = -100;}
    if(*pd_ch == 5){
      xPD5[channel1] = *pd_CU; yPD5[channel1] = *pd_max_fc/convert; zPD5[channel1] = *pd_run;
      profile_pd5->Fill(*pd_CU,*pd_max_fc/convert);
      profile_stab_pd5->Fill(*pd_run,*pd_max_fc/convert);
      PD5_dist->Fill(*pd_max_fc/convert);
    }
    else{xPD5[channel1] = -100;yPD5[channel1] = -100;zPD5[channel1] = -100;}
    if((*pd_ch == 2 || *pd_ch == 3 || *pd_ch == 4 || *pd_ch == 5 )){
      PD2_3_4_5_dist->Fill(*pd_max_fc/convert);
    }
    allPD_dist->Fill(*pd_max_fc/convert);
  }
  sprintf(process,"Done with Pin-Diode Channels: %i/%i",NumChanPD,NumChanPD);
  std::cout<<process<<std::endl;
  TGraph* graph_pd0 = new TGraph(NumChanPD,xPD0,yPD0);
  TGraph* graph_pd1 = new TGraph(NumChanPD,xPD1,yPD1);
  TGraph* graph_pd2 = new TGraph(NumChanPD,xPD2,yPD2);
  TGraph* graph_pd3 = new TGraph(NumChanPD,xPD3,yPD3);
  TGraph* graph_pd4 = new TGraph(NumChanPD,xPD4,yPD4);
  TGraph* graph_pd5 = new TGraph(NumChanPD,xPD5,yPD5);
  TGraph* graph_stab_pd0 = new TGraph(NumChanPD,zPD0,yPD0);
  TGraph* graph_stab_pd1 = new TGraph(NumChanPD,zPD1,yPD1);
  TGraph* graph_stab_pd2 = new TGraph(NumChanPD,zPD2,yPD2);
  TGraph* graph_stab_pd3 = new TGraph(NumChanPD,zPD3,yPD3);
  TGraph* graph_stab_pd4 = new TGraph(NumChanPD,zPD4,yPD4);
  TGraph* graph_stab_pd5 = new TGraph(NumChanPD,zPD5,yPD5);
  graph_pd0->SetMarkerColor(kRed);               profile_pd0->SetLineColor(kRed);
  graph_pd1->SetMarkerColor(kBlack);             profile_pd1->SetLineColor(kBlack);
  graph_pd2->SetMarkerColor(kBlue+2);            profile_pd2->SetLineColor(kBlue+2);
  graph_pd3->SetMarkerColor(kGray);              profile_pd3->SetLineColor(kGray);
  graph_pd4->SetMarkerColor(kYellow+2);          profile_pd4->SetLineColor(kYellow+2);
  graph_pd5->SetMarkerColor(kMagenta+2);         profile_pd5->SetLineColor(kMagenta+2);      
  graph_stab_pd0->SetMarkerColor(kRed);          profile_stab_pd0->SetLineColor(kRed); 
  graph_stab_pd1->SetMarkerColor(kBlack);        profile_stab_pd1->SetLineColor(kBlack);
  graph_stab_pd2->SetMarkerColor(kBlue+2);       profile_stab_pd2->SetLineColor(kBlue+2);
  graph_stab_pd3->SetMarkerColor(kGray);         profile_stab_pd3->SetLineColor(kGray);
  graph_stab_pd4->SetMarkerColor(kYellow+2);     profile_stab_pd4->SetLineColor(kYellow+2);
  graph_stab_pd5->SetMarkerColor(kMagenta+2);    profile_stab_pd5->SetLineColor(kMagenta+2);
  allPD_dist->SetLineColor(kBlack);
  PD0_dist->SetLineColor(kBlack);
  PD1_dist->SetLineColor(kBlack);
  PD2_dist->SetLineColor(kBlack);
  PD3_dist->SetLineColor(kBlack);
  PD4_dist->SetLineColor(kBlack);
  PD5_dist->SetLineColor(kBlack);
  PD2_3_4_5_dist->SetLineColor(kBlack);
  graph_pd0->SetMarkerStyle(kFullSquare);        graph_stab_pd0->SetMarkerStyle(kFullSquare);
  graph_pd1->SetMarkerStyle(kFullSquare);        graph_stab_pd1->SetMarkerStyle(kFullSquare);
  graph_pd2->SetMarkerStyle(kFullSquare);        graph_stab_pd2->SetMarkerStyle(kFullSquare);
  graph_pd3->SetMarkerStyle(kFullSquare);        graph_stab_pd3->SetMarkerStyle(kFullSquare);
  graph_pd4->SetMarkerStyle(kFullSquare);        graph_stab_pd4->SetMarkerStyle(kFullSquare);
  graph_pd5->SetMarkerStyle(kFullSquare);        graph_stab_pd5->SetMarkerStyle(kFullSquare);

  //----------------------------------------------------------------------
  //Make RM Plots
  //----------------------------------------------------------------------
  Double_t xRM1[NumChanRM],xRM2[NumChanRM],xRM3[NumChanRM],xRM4[NumChanRM];
  Double_t yRM1[NumChanRM],yRM2[NumChanRM],yRM3[NumChanRM],yRM4[NumChanRM];
  Double_t zRM1[NumChanRM],zRM2[NumChanRM],zRM3[NumChanRM],zRM4[NumChanRM];
  TProfile* profile_rm1 = new TProfile("RM1","RM1",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_rm2 = new TProfile("RM2","RM2",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_rm3 = new TProfile("RM3","RM3",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_rm4 = new TProfile("RM4","RM4",44,-0.5,43.5,0,350000/convert);  
  TProfile* profile_stab_rm1 = new TProfile("RM1 stab","RM1 stab",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_stab_rm2 = new TProfile("RM2 stab","RM2 stab",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_stab_rm3 = new TProfile("RM3 stab","RM3 stab",44,-0.5,43.5,0,350000/convert);
  TProfile* profile_stab_rm4 = new TProfile("RM4 stab","RM4 stab",44,-0.5,43.5,0,350000/convert);
  TH1F* allRM_dist = new TH1F("allRM","allRM",62,0,350000/convert);
  TH1F* RM1_dist   = new TH1F("RM1 Dist","RM1 Dist",62,0,350000/convert);
  TH1F* RM2_dist   = new TH1F("RM2 Dist","RM2 Dist",62,0,350000/convert);
  TH1F* RM3_dist   = new TH1F("RM3 Dist","RM3 Dist",62,0,350000/convert);
  TH1F* RM4_dist   = new TH1F("RM4 Dist","RM4 Dist",62,0,350000/convert);
  std::cout<<"Running over RM Channels"<<std::endl;
  int channel2 = -1;
  while(tReader2.Next()){    
    channel2+=1;
    sprintf(process,"Processing Channel: %i/%i",channel2,NumChanRM);
    if(channel2%500==0){std::cout<<process<<std::endl;}
    if(*sipm_rm == 1){
      xRM1[channel2] = *sipm_CU; yRM1[channel2] = *sipm_max_fc/convert; zRM1[channel2] = *sipm_run;
      profile_rm1->Fill(*sipm_CU,*sipm_max_fc/convert);
      profile_stab_rm1->Fill(*sipm_run,*sipm_max_fc/convert);
      RM1_dist->Fill(*sipm_max_fc/convert);
    }
    else{xRM1[channel2] = -100;yRM1[channel2] = -100;zRM1[channel2] = -100;}
    if(*sipm_rm == 2){
      xRM2[channel2] = *sipm_CU; yRM2[channel2] = *sipm_max_fc/convert; zRM2[channel2] = *sipm_run;
      profile_rm2->Fill(*sipm_CU,*sipm_max_fc/convert);
      profile_stab_rm2->Fill(*sipm_run,*sipm_max_fc/convert);
      RM2_dist->Fill(*sipm_max_fc/convert);
    }
    else{xRM2[channel2] = -100;yRM2[channel2] = -100;zRM2[channel2] = -100;}
    if(*sipm_rm == 3){
      xRM3[channel2] = *sipm_CU; yRM3[channel2] = *sipm_max_fc/convert; zRM3[channel2] = *sipm_run;
      profile_rm3->Fill(*sipm_CU,*sipm_max_fc/convert);
      profile_stab_rm3->Fill(*sipm_run,*sipm_max_fc/convert);
      RM3_dist->Fill(*sipm_max_fc/convert);
    }
    else{xRM3[channel2] = -100;yRM3[channel2] = -100;zRM3[channel2] = -100;}
    if(*sipm_rm == 4){
      xRM4[channel2] = *sipm_CU; yRM4[channel2] = *sipm_max_fc/convert; zRM4[channel2] = *sipm_run;
      profile_rm4->Fill(*sipm_CU,*sipm_max_fc/convert);
      profile_stab_rm4->Fill(*sipm_run,*sipm_max_fc/convert);
      RM4_dist->Fill(*sipm_max_fc/convert);
    }
    else{xRM4[channel2] = -100;yRM4[channel2] = -100;zRM4[channel2] = -100;}
    allRM_dist->Fill(*sipm_max_fc/convert);
  }
  sprintf(process,"Done with RM Channels: %i/%i",NumChanRM,NumChanRM);
  std::cout<<process<<std::endl;
  TGraph* graph_rm1 = new TGraph(NumChanRM,xRM1,yRM1);
  TGraph* graph_rm2 = new TGraph(NumChanRM,xRM2,yRM2);
  TGraph* graph_rm3 = new TGraph(NumChanRM,xRM3,yRM3);
  TGraph* graph_rm4 = new TGraph(NumChanRM,xRM4,yRM4);
  TGraph* graph_stab_rm1 = new TGraph(NumChanRM,zRM1,yRM1);
  TGraph* graph_stab_rm2 = new TGraph(NumChanRM,zRM2,yRM2);
  TGraph* graph_stab_rm3 = new TGraph(NumChanRM,zRM3,yRM3);
  TGraph* graph_stab_rm4 = new TGraph(NumChanRM,zRM4,yRM4);
  graph_rm1->SetMarkerColor(kRed);              profile_rm1->SetLineColor(kRed);
  graph_rm2->SetMarkerColor(kBlue);             profile_rm2->SetLineColor(kBlue);
  graph_rm3->SetMarkerColor(kBlack);            profile_rm3->SetLineColor(kBlack);
  graph_rm4->SetMarkerColor(kGreen+2);          profile_rm4->SetLineColor(kGreen+2);
  graph_stab_rm1->SetMarkerColor(kMagenta);     profile_stab_rm1->SetLineColor(kMagenta);
  graph_stab_rm2->SetMarkerColor(kBlue);        profile_stab_rm2->SetLineColor(kBlue);
  graph_stab_rm3->SetMarkerColor(kBlack);       profile_stab_rm3->SetLineColor(kBlack);
  graph_stab_rm4->SetMarkerColor(kGray);        profile_stab_rm4->SetLineColor(kGray);
  allRM_dist->SetLineColor(kBlack);
  RM1_dist->SetLineColor(kBlack);  
  RM2_dist->SetLineColor(kBlack);  
  RM3_dist->SetLineColor(kBlack);  
  RM4_dist->SetLineColor(kBlack);  
  graph_rm1->SetMarkerStyle(kFullSquare);       graph_stab_rm1->SetMarkerStyle(kFullSquare);
  graph_rm2->SetMarkerStyle(kFullSquare);       graph_stab_rm2->SetMarkerStyle(kFullSquare);
  graph_rm3->SetMarkerStyle(kFullSquare);       graph_stab_rm3->SetMarkerStyle(kFullSquare);
  graph_rm4->SetMarkerStyle(kFullSquare);       graph_stab_rm4->SetMarkerStyle(kFullSquare);
  
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
  
  TLatex* PinDiode = new TLatex(0.14, 0.91, "Pin-Diodes");
  PinDiode->SetNDC();
  PinDiode->SetTextFont(42);
  TLatex* PinDiode0 = new TLatex(0.14, 0.91, "Pin-Diode 0");
  PinDiode0->SetNDC();
  PinDiode0->SetTextFont(42);
  TLatex* PinDiode1 = new TLatex(0.14, 0.91, "Pin-Diode 1");
  PinDiode1->SetNDC();
  PinDiode1->SetTextFont(42);
  TLatex* PinDiode2 = new TLatex(0.14, 0.91, "Pin-Diode 2");
  PinDiode2->SetNDC();
  PinDiode2->SetTextFont(42);
  TLatex* PinDiode3 = new TLatex(0.14, 0.91, "Pin-Diode 3");
  PinDiode3->SetNDC();
  PinDiode3->SetTextFont(42);
  TLatex* PinDiode4 = new TLatex(0.14, 0.91, "Pin-Diode 4");
  PinDiode4->SetNDC();
  PinDiode4->SetTextFont(42);
  TLatex* PinDiode5 = new TLatex(0.14, 0.91, "Pin-Diode 5");
  PinDiode5->SetNDC();
  PinDiode5->SetTextFont(42);

  TLatex* RM = new TLatex(0.14, 0.91, "Silicon Photomultiplier");
  RM->SetNDC();
  RM->SetTextFont(42);
  TLatex* RM1 = new TLatex(0.2, 0.85, "RM 1");
  RM1->SetNDC();
  RM1->SetTextFont(42);
  TLatex* RM2 = new TLatex(0.2, 0.85, "RM 2");
  RM2->SetNDC();
  RM2->SetTextFont(42);
  TLatex* RM3 = new TLatex(0.2, 0.85, "RM 3");
  RM3->SetNDC();
  RM3->SetTextFont(42);
  TLatex* RM4 = new TLatex(0.2, 0.85, "RM 4");
  RM4->SetNDC();
  RM4->SetTextFont(42);

  TLatex* EntriesAllRM = Entries(0.92,0.85,allRM_dist);
  TLatex* MeanAllRM    = Mean(0.92, 0.8,allRM_dist);
  TLatex* StdDevAllRM  = StdDev(0.92, 0.75,allRM_dist); 
  TLatex* EntriesAllPD = Entries(0.92,0.85,allPD_dist);
  TLatex* MeanAllPD    = Mean(0.92, 0.8,allPD_dist);
  TLatex* StdDevAllPD  = StdDev(0.92, 0.75,allPD_dist); 
  TLatex* EntriesPD0   = Entries(0.92,0.85,PD0_dist);
  TLatex* MeanPD0      = Mean(0.92, 0.8,PD0_dist);
  TLatex* StdDevPD0    = StdDev(0.92, 0.75,PD0_dist); 
  TLatex* EntriesPD1   = Entries(0.92,0.85,PD1_dist);
  TLatex* MeanPD1      = Mean(0.92, 0.8,PD1_dist);
  TLatex* StdDevPD1    = StdDev(0.92, 0.75,PD1_dist); 
  TLatex* EntriesPD2   = Entries(0.92,0.85,PD2_dist);
  TLatex* MeanPD2      = Mean(0.92, 0.8,PD2_dist);
  TLatex* StdDevPD2    = StdDev(0.92, 0.75,PD2_dist); 
  TLatex* EntriesPD3   = Entries(0.92,0.85,PD3_dist);
  TLatex* MeanPD3      = Mean(0.92, 0.8,PD3_dist);
  TLatex* StdDevPD3    = StdDev(0.92, 0.75,PD3_dist); 
  TLatex* EntriesPD4   = Entries(0.92,0.85,PD4_dist);
  TLatex* MeanPD4      = Mean(0.92, 0.8,PD4_dist);
  TLatex* StdDevPD4    = StdDev(0.92, 0.75,PD4_dist); 
  TLatex* EntriesPD5   = Entries(0.92,0.85,PD5_dist);
  TLatex* MeanPD5      = Mean(0.92, 0.8,PD5_dist);
  TLatex* StdDevPD5    = StdDev(0.92, 0.75,PD5_dist); 
  TLatex* EntriesPD2_3_4_5   = Entries(0.92,0.85,PD2_3_4_5_dist);
  TLatex* MeanPD2_3_4_5      = Mean(0.92, 0.8,PD2_3_4_5_dist);
  TLatex* StdDevPD2_3_4_5    = StdDev(0.92, 0.75,PD2_3_4_5_dist); 
  TLatex* EntriesRM1   = Entries(0.92,0.85,RM1_dist);
  TLatex* MeanRM1      = Mean(0.92, 0.8,RM1_dist);
  TLatex* StdDevRM1    = StdDev(0.92, 0.75,RM1_dist); 
  TLatex* EntriesRM2   = Entries(0.92,0.85,RM2_dist);
  TLatex* MeanRM2      = Mean(0.92, 0.8,RM2_dist);
  TLatex* StdDevRM2    = StdDev(0.92, 0.75,RM2_dist); 
  TLatex* EntriesRM3   = Entries(0.92,0.85,RM3_dist);
  TLatex* MeanRM3      = Mean(0.92, 0.8,RM3_dist);
  TLatex* StdDevRM3    = StdDev(0.92, 0.75,RM3_dist); 
  TLatex* EntriesRM4   = Entries(0.92,0.85,RM4_dist);
  TLatex* MeanRM4      = Mean(0.92, 0.8,RM4_dist);
  TLatex* StdDevRM4    = StdDev(0.92, 0.75,RM4_dist); 

  //----------------------------------------------------------------------
  //Plotting
  //----------------------------------------------------------------------
  std::cout<<"Plotting"<<std::endl;
  TLegend* catLeg0 = new TLegend(0.68,0.65,0.96,0.88);
  TCanvas *c0 = Canvas("MaxpCvsCU",800,800);  
  catLeg0->SetBorderSize(0);
  catLeg0->SetFillStyle(0);
  catLeg0->SetTextSize(0.04);
  TH1F* h0blank = Blank("Blank0",125,0,70);
  h0blank->SetMinimum(0);
  h0blank->SetMaximum(120000/convert);
  h0blank->GetXaxis()->SetTitle("CU");
  h0blank->GetYaxis()->SetTitle("Max Charge [pC]");
  h0blank->Draw("hist");  
  graph_pd0->Draw("P same");
  graph_pd1->Draw("P same");
  graph_pd2->Draw("P same");
  graph_pd3->Draw("P same");
  graph_pd4->Draw("P same");
  graph_pd5->Draw("P same");
  catLeg0->Draw();  
  //CMSPrelim1->Draw();
  burnIn->Draw();
  PinDiode->Draw();
  catLeg0->Draw();  
  catLeg0->AddEntry(graph_pd0,"Pin-Diode 0","P");
  catLeg0->AddEntry(graph_pd1,"Pin-Diode 1","P");
  catLeg0->AddEntry(graph_pd2,"Pin-Diode 2","P");
  catLeg0->AddEntry(graph_pd3,"Pin-Diode 3","P");
  catLeg0->AddEntry(graph_pd4,"Pin-Diode 4","P");
  catLeg0->AddEntry(graph_pd5,"Pin-Diode 5","P");

  TCanvas *c1 = Canvas("1D PD",800,800);
  TH1F* h1blank = Blank("Blank1",62,0,10000/convert);
  h1blank->SetMinimum(pinDisMin);
  h1blank->SetMaximum(6);
  h1blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h1blank->GetYaxis()->SetTitle("Channels");
  h1blank->Draw("hist");  
  allPD_dist->Draw("same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  PinDiode->Draw();
  EntriesAllPD->Draw();
  MeanAllPD->Draw();
  StdDevAllPD->Draw();
  
  TCanvas *c2 = Canvas("CUvsRM",800,800);  
  TLegend* catLeg2 = new TLegend(0.68,0.65,0.96,0.88);
  catLeg2->SetBorderSize(0);
  catLeg2->SetFillStyle(0);
  catLeg2->SetTextSize(0.04);
  TH1F* h2blank = Blank("Blank2",125,0,70);
  h2blank->SetMinimum(0);
  h2blank->SetMaximum(125000/convert);
  h2blank->GetXaxis()->SetTitle("CU");
  h2blank->GetYaxis()->SetTitle("Max Charge [pC]");
  h2blank->Draw("hist");
  graph_rm1->Draw("P same");
  graph_rm2->Draw("P same");
  graph_rm3->Draw("P same");
  graph_rm4->Draw("P same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  RM->Draw();
  catLeg2->Draw();  
  catLeg2->AddEntry(graph_rm1,"RM 1","P");
  catLeg2->AddEntry(graph_rm2,"RM 2","P");
  catLeg2->AddEntry(graph_rm3,"RM 3","P");
  catLeg2->AddEntry(graph_rm4,"RM 4","P");

  TCanvas *c3 = Canvas("1D RM",800,800);  
  TH1F* h3blank = Blank("Blank3",62,0,125000/convert);
  h3blank->SetMinimum(rmDisMin);
  h3blank->SetMaximum(4*rmDisMax);
  h3blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h3blank->GetYaxis()->SetTitle("Channels");
  h3blank->Draw("hist");  
  allRM_dist->Draw("same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  RM->Draw();
  EntriesAllRM->Draw();
  MeanAllRM->Draw();
  StdDevAllRM->Draw();

  TCanvas *c4 = Canvas("MaxpCvsCU 0,1, RM1-4",800,800);  
  TLegend* catLeg4 = new TLegend(0.68,0.65,0.96,0.88);
  catLeg4->SetBorderSize(0);
  catLeg4->SetFillStyle(0);
  catLeg4->SetTextSize(0.04);
  TH1F* h4blank = Blank("Blank4",125,0,50);
  h4blank->SetMinimum(0);
  h4blank->SetMaximum(125000/convert);
  h4blank->GetXaxis()->SetTitle("CU");
  h4blank->GetYaxis()->SetTitle("Max Charge [pC]");
  h4blank->Draw("hist");  
  graph_pd0->Draw("P same");
  graph_pd1->Draw("P same");
  graph_rm1->Draw("P same");
  graph_rm2->Draw("P same");
  graph_rm3->Draw("P same");
  graph_rm4->Draw("P same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  catLeg4->Draw();
  catLeg4->AddEntry(graph_pd0,"Pin-Diode 0","P");
  catLeg4->AddEntry(graph_pd1,"Pin-Diode 1","P");
  catLeg4->AddEntry(graph_rm1,"RM 1","P");
  catLeg4->AddEntry(graph_rm2,"RM 2","P");
  catLeg4->AddEntry(graph_rm3,"RM 3","P");
  catLeg4->AddEntry(graph_rm4,"RM 4","P");

  TCanvas *c5 = Canvas("Profile: MaxpCvsCU 0,1, RM1-4",800,800);  
  TLegend* catLeg5 = new TLegend(0.68,0.65,0.96,0.88);
  catLeg5->SetBorderSize(0);
  catLeg5->SetFillStyle(0);
  catLeg5->SetTextSize(0.04);
  TH1F* h5blank = Blank("Blank5",125,0,50);
  h5blank->SetMinimum(0);
  h5blank->SetMaximum(125000/convert);
  h5blank->GetXaxis()->SetTitle("CU");
  h5blank->GetYaxis()->SetTitle("Max Charge [pC]");
  h5blank->Draw("hist");  
  profile_pd0->Draw("P same");
  profile_pd1->Draw("P same");
  profile_rm1->Draw("P same");
  profile_rm2->Draw("P same");
  profile_rm3->Draw("P same");
  profile_rm4->Draw("P same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  catLeg5->Draw();
  catLeg5->AddEntry(profile_pd0,"Pin-Diode 0","L");
  catLeg5->AddEntry(profile_pd1,"Pin-Diode 1","L");
  catLeg5->AddEntry(profile_rm1,"RM 1","L");
  catLeg5->AddEntry(profile_rm2,"RM 2","L");
  catLeg5->AddEntry(profile_rm3,"RM 3","L");
  catLeg5->AddEntry(profile_rm4,"RM 4","L");

  TCanvas *c6  = Canvas("1D PD0",800,800);
  TH1F* h6blank = Blank("Blank6",125,0,10000/convert);
  h6blank->SetMinimum(pinDisMin);
  h6blank->SetMaximum(pinDisMax);
  h6blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h6blank->GetYaxis()->SetTitle("Channels");
  h6blank->Draw();
  PD0_dist->Draw("same");  
  //CMSPrelim1->Draw();
  burnIn->Draw();
  //PinDiode->Draw();
  EntriesPD0->Draw();
  MeanPD0->Draw();
  StdDevPD0->Draw();
  PinDiode0->Draw();
  TCanvas *c7  = Canvas("1D PD1",800,800);
  TH1F* h7blank = Blank("Blank7",125,0,10000/convert);
  h7blank->SetMinimum(pinDisMin);
  h7blank->SetMaximum(pinDisMax);
  h7blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h7blank->GetYaxis()->SetTitle("Channels");
  h7blank->Draw();
  PD1_dist->Draw("same");  
  //CMSPrelim1->Draw();
  burnIn->Draw();
  //PinDiode->Draw();
  EntriesPD1->Draw();
  MeanPD1->Draw();
  StdDevPD1->Draw();
  PinDiode1->Draw();
  TCanvas *c8  = Canvas("1D PD2",800,800);
  TH1F* h8blank = Blank("Blank8",125,0,125000/convert);
  h8blank->SetMinimum(pinDisMin);
  h8blank->SetMaximum(pinDisMax);
  h8blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h8blank->GetYaxis()->SetTitle("Channels");
  h8blank->Draw();
  PD2_dist->Draw("same");  
  //CMSPrelim1->Draw();
  burnIn->Draw();
  //PinDiode->Draw();
  EntriesPD2->Draw();
  MeanPD2->Draw();
  StdDevPD2->Draw();
  PinDiode2->Draw();
  TCanvas *c9  = Canvas("1D PD3",800,800);
  TH1F* h9blank = Blank("Blank9",125,0,125000/convert);
  h9blank->SetMinimum(pinDisMin);
  h9blank->SetMaximum(pinDisMax);
  h9blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h9blank->GetYaxis()->SetTitle("Channels");
  h9blank->Draw();
  PD3_dist->Draw("same");  
  //CMSPrelim1->Draw();
  burnIn->Draw();
  //PinDiode->Draw();
  EntriesPD3->Draw();
  MeanPD3->Draw();
  StdDevPD3->Draw();
  PinDiode3->Draw();
  TCanvas *c10  = Canvas("1D PD4",800,800);
  TH1F* h10blank = Blank("Blank10",125,0,125000/convert);
  h10blank->SetMinimum(pinDisMin);
  h10blank->SetMaximum(pinDisMax);
  h10blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h10blank->GetYaxis()->SetTitle("Channels");
  h10blank->Draw();
  PD4_dist->Draw("same");  
  //CMSPrelim1->Draw();
  burnIn->Draw();
  //PinDiode->Draw();
  EntriesPD4->Draw();
  MeanPD4->Draw();
  StdDevPD4->Draw();
  PinDiode4->Draw();
  TCanvas *c11  = Canvas("1D PD5",800,800);
  TH1F* h11blank = Blank("Blank11",125,0,125000/convert);
  h11blank->SetMinimum(pinDisMin);
  h11blank->SetMaximum(pinDisMax);
  h11blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h11blank->GetYaxis()->SetTitle("Channels");
  h11blank->Draw();
  PD5_dist->Draw("same");  
  //CMSPrelim1->Draw();
  burnIn->Draw();
  //PinDiode->Draw();
  EntriesPD5->Draw();
  MeanPD5->Draw();
  StdDevPD5->Draw();
  PinDiode5->Draw();
  TCanvas *c12 = Canvas("1D PD2 3 4 5",800,800);
  TH1F* h12blank = Blank("Blank12",125,0,125000/convert);
  h12blank->SetMinimum(pinDisMin);
  h12blank->SetMaximum(3*pinDisMax);
  h12blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h12blank->GetYaxis()->SetTitle("Channels");
  h12blank->Draw();
  PD2_3_4_5_dist->Draw("same");
  CMSPrelim1->Draw();
  burnIn->Draw();
  //PinDiode->Draw();
  EntriesPD2_3_4_5->Draw();
  MeanPD2_3_4_5->Draw();
  StdDevPD2_3_4_5->Draw();
  TCanvas *c13 = Canvas("1D RM1",800,800);
  TH1F* h13blank = Blank("Blank13",125,0,125000/convert);
  h13blank->SetMinimum(rmDisMin);
  h13blank->SetMaximum(rmDisMax);
  h13blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h13blank->GetYaxis()->SetTitle("Channels");
  h13blank->Draw();
  RM1_dist->Draw("same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  RM->Draw();
  EntriesRM1->Draw();
  MeanRM1->Draw();
  StdDevRM1->Draw();
  RM1->Draw();
  TCanvas *c14 = Canvas("1D RM2",800,800);
  TH1F* h14blank = Blank("Blank14",125,0,125000/convert);
  h14blank->SetMinimum(rmDisMin);
  h14blank->SetMaximum(rmDisMax);
  h14blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h14blank->GetYaxis()->SetTitle("Channels");
  h14blank->Draw();
  RM2_dist->Draw("same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  RM->Draw();
  EntriesRM2->Draw();
  MeanRM2->Draw();
  StdDevRM2->Draw();
  RM2->Draw();
  TCanvas *c15 = Canvas("1D RM3",800,800);
  TH1F* h15blank = Blank("Blank15",125,0,125000/convert);
  h15blank->SetMinimum(rmDisMin);
  h15blank->SetMaximum(rmDisMax);
  h15blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h15blank->GetYaxis()->SetTitle("Channels");
  h15blank->Draw();
  RM3_dist->Draw("same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  RM->Draw();
  EntriesRM3->Draw();
  MeanRM3->Draw();
  StdDevRM3->Draw();
  RM3->Draw();
  TCanvas *c16 = Canvas("1D RM4",800,800);
  TH1F* h16blank = Blank("Blank16",125,0,125000/convert);
  h16blank->SetMinimum(rmDisMin);
  h16blank->SetMaximum(rmDisMax);
  h16blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h16blank->GetYaxis()->SetTitle("Channels");
  h16blank->Draw();
  RM4_dist->Draw("same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  RM->Draw();
  EntriesRM4->Draw();
  MeanRM4->Draw();
  StdDevRM4->Draw();
  RM4->Draw();
  TCanvas *c17 = Canvas("Stability Graph",800,800);
  TH1F* h17blank = Blank("Blank17",125,0,7);
  h17blank->SetMinimum(0);
  h17blank->SetMaximum(125000/convert);
  h17blank->GetXaxis()->SetTitle("Runs");
  h17blank->GetYaxis()->SetTitle("Channels");
  h17blank->Draw();
  graph_stab_pd0->Draw("P same");
  graph_stab_pd1->Draw("P same");
  graph_stab_rm1->Draw("P same");
  graph_stab_rm2->Draw("P same");
  graph_stab_rm3->Draw("P same");
  graph_stab_rm4->Draw("P same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  RM->Draw();
  TCanvas *c18 = Canvas("Stability Profile",800,800);
  TLegend* catLeg18 = new TLegend(0.68,0.65,0.96,0.88);
  catLeg18->SetBorderSize(0);
  catLeg18->SetFillStyle(0);
  catLeg18->SetTextSize(0.04);
  TH1F* h18blank = Blank("Blank18",125,0,7);
  h18blank->SetMinimum(0);
  h18blank->SetMaximum(125000/convert);
  h18blank->GetXaxis()->SetTitle("Runs");
  h18blank->GetYaxis()->SetTitle("Channels");
  h18blank->Draw();
  graph_stab_pd0->Draw("P same");
  graph_stab_pd1->Draw("P same");
  profile_stab_rm1->Draw("P same");
  profile_stab_rm2->Draw("P same");
  profile_stab_rm3->Draw("P same");
  profile_stab_rm4->Draw("P same");
  catLeg18->Draw();
  catLeg18->AddEntry(graph_stab_pd0,"Pin-Diode 0","P");
  catLeg18->AddEntry(graph_stab_pd1,"Pin-Diode 1","P");
  catLeg18->AddEntry(profile_stab_rm1,"RM 1","L");
  catLeg18->AddEntry(profile_stab_rm2,"RM 2","L");
  catLeg18->AddEntry(profile_stab_rm3,"RM 3","L");
  catLeg18->AddEntry(profile_stab_rm4,"RM 4","L");
  
  //CMSPrelim1->Draw();
  burnIn->Draw();
  RM->Draw();

  c0->SaveAs("PDAllvsCU2D.pdf");
  c1->SaveAs("PDAll_1D.pdf");
  c2->SaveAs("RMAllvsCU2D.pdf");
  c3->SaveAs("RMAll_1D.pdf");
  c4->SaveAs("RM_PDvsCU2D.pdf");
  c5->SaveAs("profile_RM_PDvsCU2D.pdf");  
  c6->SaveAs("PD0_1D.pdf");
  c7->SaveAs("PD1_1D.pdf");
  c8->SaveAs("PD2_1D.pdf");
  c9->SaveAs("PD3_1D.pdf");
  c10->SaveAs("PD4_1D.pdf");
  c11->SaveAs("PD5_1D.pdf");
  c12->SaveAs("PD2_3_4_5_1D.pdf");
  c13->SaveAs("RM1_1D.pdf");
  c14->SaveAs("RM2_1D.pdf");
  c15->SaveAs("RM3_1D.pdf");
  c16->SaveAs("RM4_1D.pdf");
  c17->SaveAs("stabilityVsiteration_graph.pdf");
  c18->SaveAs("stabilityVsiteration_profile.pdf");
} 

int main(){
  CU_Plots();
}
