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
//Correction Factors
#include "correctionFactors.h"

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
  blank->SetTitleOffset(1.4,"Y");
  blank->SetLabelSize(0.04,"X");
  blank->SetLabelSize(0.04,"Y");
  blank->SetStats(false);
  blank->SetLineColor(0);  
  return blank;
}
void fillMinMax(int CU, std::vector<double> v1,std::vector<double> v2,std::vector<double> v3,std::vector<double> v4, Double_t* x1,Double_t* x2,Double_t* x3,Double_t* x4, Double_t* y1, Double_t* y2, Double_t* y3, Double_t* y4){
  int length=50;
  auto b1 = std::max_element(std::begin(v1), std::end(v1)); auto s1 = std::min_element(std::begin(v1), std::end(v1));
  x1[CU]=CU; y1[CU]=*b1; x1[CU+length]=CU; y1[CU+length]=*s1;
  auto b2 = std::max_element(std::begin(v2), std::end(v2)); auto s2 = std::min_element(std::begin(v2), std::end(v2));
  x2[CU]=CU; y2[CU]=*b2; x2[CU+length]=CU; y2[CU+length]=*s2;
  auto b3 = std::max_element(std::begin(v3), std::end(v3)); auto s3 = std::min_element(std::begin(v3), std::end(v3));
  x3[CU]=CU; y3[CU]=*b3; x3[CU+length]=CU; y3[CU+length]=*s3;
  auto b4 = std::max_element(std::begin(v4), std::end(v4)); auto s4 = std::min_element(std::begin(v4), std::end(v4));
  x4[CU]=CU; y4[CU]=*b4; x4[CU+length]=CU; y4[CU+length]=*s4;
}
void CU_Plots(int doCorr = 1){
  //----------------------------------------------------------------------
  //Setup
  //----------------------------------------------------------------------
  //const char* infile1 = "passed_cu_data/pd.root";
  //const char* infile2 = "passed_cu_data/sipm.root";
  //const char* infile1 = "rework_cu_data/pd.root";
  //const char* infile2 = "rework_cu_data/sipm.root";
  //const char* infile1 = "stability_cu_data/pd.root";
  //const char* infile2 = "stability_cu_data/sipm.root"; 
  //const char* infile1 = "final_cu_data/pd.root";
  //const char* infile2 = "final_cu_data/sipm.root"; 
  const char* infile1 = "Nov17-18_Final_CU_Data/pd.root";
  const char* infile2 = "Nov17-18_Final_CU_Data/sipm.root"; 

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
  TTreeReaderValue<Float_t> pd_max_pc(tReader1, "max_pc");
  TTreeReaderValue<Float_t> pd_result(tReader1, "result"); 

  TTreeReaderValue<Float_t> sipm_CU(tReader2, "cu");
  TTreeReaderValue<Float_t> sipm_rbx(tReader2, "rbx");
  TTreeReaderValue<Float_t> sipm_run(tReader2, "run");
  TTreeReaderValue<Float_t> sipm_rm(tReader2, "rm");
  TTreeReaderValue<Float_t> sipm_ch(tReader2, "sipm_ch");
  TTreeReaderValue<Float_t> sipm_uhtr_ch(tReader2, "uhtr_ch");
  TTreeReaderValue<Float_t> sipm_shunt(tReader2, "shunt");
  TTreeReaderValue<Float_t> sipm_max_adc(tReader2, "max_adc");
  TTreeReaderValue<Float_t> sipm_max_pc(tReader2, "max_pc");
  TTreeReaderValue<Float_t> sipm_result(tReader2, "result"); 

  int NumChanPD = 0;
  int NumChanRM = 0;
  while(tReader1.Next()){NumChanPD+=1;}
  while(tReader2.Next()){NumChanRM+=1;}
  tReader1.Restart();
  tReader2.Restart();
  double convert = (1000/14);
  double pinDisMin = 0;
  double pinDisMax = 32;
  double rmDisMin = 0;
  double rmDisMax = 225;
  double disMax = 250000;
  double disMaxPD = 100000;
  if(doCorr == 1){
    pinDisMax = 43;
    rmDisMax = 410;
  }
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
  TH1F* allPD_dist = new TH1F("allPD","allPD",100,0,350000/convert);
  TH1F* PD0_dist   = new TH1F("PD0","PD0",100,0,350000/convert);
  TH1F* PD1_dist   = new TH1F("PD1","PD1",100,0,350000/convert);
  TH1F* PD2_dist   = new TH1F("PD2","PD2",100,0,350000/convert);
  TH1F* PD3_dist   = new TH1F("PD3","PD3",100,0,350000/convert);
  TH1F* PD4_dist   = new TH1F("PD4","PD4",100,0,350000/convert);
  TH1F* PD5_dist   = new TH1F("PD5","PD5",100,0,350000/convert);
  TH1F* PD2_3_4_5_dist   = new TH1F("PD 2 3 4 5","PD 2 3 4 5",62,0,350000/convert);
  std::cout<<"Running over Pin-Diode Channels"<<std::endl;
  char process[100];
  int channel1 = -1;
  std::cout<<"Size of pinDiode0 = "<<pinDiode0.size()<<std::endl;
  while(tReader1.Next()){
    channel1+=1;
    sprintf(process,"Processing Channel: %i/%i",channel1,NumChanPD);
    if(channel1%10==0){std::cout<<process<<std::endl;}
    double corr=1;
    if(doCorr == 1){
      for(int i = 0; i <= pinDiodeAvg.size(); i++){
	//std::cout<<*pd_CU<<"   "<<pinDiode0[i][0]<<std::endl;
	if(*pd_CU == pinDiodeAvg[i][0]){
    	//std::cout<<"TRUE"<<std::endl;
	  corr=pinDiodeAvg[i][1];
	  break;
	}	  
      }
    }
    if(*pd_ch == 0){
      xPD0[channel1] = *pd_CU; yPD0[channel1] = corr*(*pd_max_pc); zPD0[channel1] = *pd_run;
      profile_pd0->Fill(*pd_CU,corr*(*pd_max_pc));
      profile_stab_pd0->Fill(*pd_run,corr*(*pd_max_pc));
      PD0_dist->Fill(corr*(*pd_max_pc));
      //std::cout<<"PD0: CU = "<<*pd_CU<<"  maxPC = "<<corr*(*pd_max_pc)<<std::endl;
      //std::cout<<"PD0: CU = "<<*pd_CU<<"  maxADC = "<<*pd_max_adc<<std::endl;
    }
    else{xPD0[channel1] = -100;yPD0[channel1] = -100;zPD0[channel1] = -100;}
    if(*pd_ch == 1){
      xPD1[channel1] = *pd_CU; yPD1[channel1] = corr*(*pd_max_pc); zPD1[channel1] = *pd_run;
      profile_pd1->Fill(*pd_CU,corr*(*pd_max_pc));
      profile_stab_pd1->Fill(*pd_run,corr*(*pd_max_pc));
      PD1_dist->Fill(corr*(*pd_max_pc));
      //std::cout<<"PD1: CU = "<<*pd_CU<<"  maxPC = "<<corr*(*pd_max_pc)<<std::endl;
      //std::cout<<"PD1: CU = "<<*pd_CU<<"  maxADC = "<<*pd_max_adc<<std::endl;
    }
    else{xPD1[channel1] = -100;yPD1[channel1] = -100;zPD1[channel1] = -100;}
    if(*pd_ch == 2){
      corr=1;
      xPD2[channel1] = *pd_CU; yPD2[channel1] = corr*(*pd_max_pc); zPD2[channel1] = *pd_run;
      profile_pd2->Fill(*pd_CU,corr*(*pd_max_pc));
      profile_stab_pd2->Fill(*pd_run,corr*(*pd_max_pc));
      PD2_dist->Fill(corr*(*pd_max_pc));
    }
    else{xPD2[channel1] = -100;yPD2[channel1] = -100;zPD2[channel1] = -100;}
    if(*pd_ch == 3){
      corr=1;
      xPD3[channel1] = *pd_CU; yPD3[channel1] = corr*(*pd_max_pc); zPD3[channel1] = *pd_run;
      profile_pd3->Fill(*pd_CU,corr*(*pd_max_pc));
      profile_stab_pd3->Fill(*pd_run,corr*(*pd_max_pc));
      PD3_dist->Fill(corr*(*pd_max_pc));
    }
    else{xPD3[channel1] = -100;yPD3[channel1] = -100;zPD3[channel1] = -100;}
    if(*pd_ch == 4){
      corr=1;
      xPD4[channel1] = *pd_CU; yPD4[channel1] = corr*(*pd_max_pc); zPD4[channel1] = *pd_run;
      profile_pd4->Fill(*pd_CU,corr*(*pd_max_pc));
      profile_stab_pd4->Fill(*pd_run,corr*(*pd_max_pc));
      PD4_dist->Fill(corr*(*pd_max_pc));
    }
    else{xPD4[channel1] = -100;yPD4[channel1] = -100;zPD4[channel1] = -100;}
    if(*pd_ch == 5){
      corr=1;      
      xPD5[channel1] = *pd_CU; yPD5[channel1] = corr*(*pd_max_pc); zPD5[channel1] = *pd_run;
      profile_pd5->Fill(*pd_CU,corr*(*pd_max_pc));
      profile_stab_pd5->Fill(*pd_run,corr*(*pd_max_pc));
      PD5_dist->Fill(corr*(*pd_max_pc));
    }
    else{xPD5[channel1] = -100;yPD5[channel1] = -100;zPD5[channel1] = -100;}
    if((*pd_ch == 2 || *pd_ch == 3 || *pd_ch == 4 || *pd_ch == 5 )){
      corr=1;      
      PD2_3_4_5_dist->Fill(corr*(*pd_max_pc));
    }    
    allPD_dist->Fill(corr*(*pd_max_pc));
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
  TH1F* RM1_dist_ch20   = new TH1F("RM1 Dist ch 20","RM1 Dist ch 20",62,0,350000/convert);
  std::vector<double> RM1_CU2_Dist;  std::vector<double> RM2_CU2_Dist;  std::vector<double> RM3_CU2_Dist;  std::vector<double> RM4_CU2_Dist;
  std::vector<double> RM1_CU3_Dist;  std::vector<double> RM2_CU3_Dist;  std::vector<double> RM3_CU3_Dist;  std::vector<double> RM4_CU3_Dist;
  std::vector<double> RM1_CU4_Dist;  std::vector<double> RM2_CU4_Dist;  std::vector<double> RM3_CU4_Dist;  std::vector<double> RM4_CU4_Dist;
  std::vector<double> RM1_CU5_Dist;  std::vector<double> RM2_CU5_Dist;  std::vector<double> RM3_CU5_Dist;  std::vector<double> RM4_CU5_Dist;
  std::vector<double> RM1_CU6_Dist;  std::vector<double> RM2_CU6_Dist;  std::vector<double> RM3_CU6_Dist;  std::vector<double> RM4_CU6_Dist;
  std::vector<double> RM1_CU7_Dist;  std::vector<double> RM2_CU7_Dist;  std::vector<double> RM3_CU7_Dist;  std::vector<double> RM4_CU7_Dist;
  std::vector<double> RM1_CU8_Dist;  std::vector<double> RM2_CU8_Dist;  std::vector<double> RM3_CU8_Dist;  std::vector<double> RM4_CU8_Dist;
  std::vector<double> RM1_CU9_Dist;  std::vector<double> RM2_CU9_Dist;  std::vector<double> RM3_CU9_Dist;  std::vector<double> RM4_CU9_Dist;
  std::vector<double> RM1_CU10_Dist; std::vector<double> RM2_CU10_Dist; std::vector<double> RM3_CU10_Dist; std::vector<double> RM4_CU10_Dist;
  std::vector<double> RM1_CU11_Dist; std::vector<double> RM2_CU11_Dist; std::vector<double> RM3_CU11_Dist; std::vector<double> RM4_CU11_Dist;
  std::vector<double> RM1_CU12_Dist; std::vector<double> RM2_CU12_Dist; std::vector<double> RM3_CU12_Dist; std::vector<double> RM4_CU12_Dist;
  std::vector<double> RM1_CU13_Dist; std::vector<double> RM2_CU13_Dist; std::vector<double> RM3_CU13_Dist; std::vector<double> RM4_CU13_Dist;
  std::vector<double> RM1_CU14_Dist; std::vector<double> RM2_CU14_Dist; std::vector<double> RM3_CU14_Dist; std::vector<double> RM4_CU14_Dist;
  std::vector<double> RM1_CU15_Dist; std::vector<double> RM2_CU15_Dist; std::vector<double> RM3_CU15_Dist; std::vector<double> RM4_CU15_Dist;
  std::vector<double> RM1_CU16_Dist; std::vector<double> RM2_CU16_Dist; std::vector<double> RM3_CU16_Dist; std::vector<double> RM4_CU16_Dist;
  std::vector<double> RM1_CU17_Dist; std::vector<double> RM2_CU17_Dist; std::vector<double> RM3_CU17_Dist; std::vector<double> RM4_CU17_Dist;
  std::vector<double> RM1_CU18_Dist; std::vector<double> RM2_CU18_Dist; std::vector<double> RM3_CU18_Dist; std::vector<double> RM4_CU18_Dist;
  std::vector<double> RM1_CU19_Dist; std::vector<double> RM2_CU19_Dist; std::vector<double> RM3_CU19_Dist; std::vector<double> RM4_CU19_Dist;
  std::vector<double> RM1_CU20_Dist; std::vector<double> RM2_CU20_Dist; std::vector<double> RM3_CU20_Dist; std::vector<double> RM4_CU20_Dist;
  std::vector<double> RM1_CU21_Dist; std::vector<double> RM2_CU21_Dist; std::vector<double> RM3_CU21_Dist; std::vector<double> RM4_CU21_Dist;
  std::vector<double> RM1_CU22_Dist; std::vector<double> RM2_CU22_Dist; std::vector<double> RM3_CU22_Dist; std::vector<double> RM4_CU22_Dist;
  std::vector<double> RM1_CU23_Dist; std::vector<double> RM2_CU23_Dist; std::vector<double> RM3_CU23_Dist; std::vector<double> RM4_CU23_Dist;
  std::vector<double> RM1_CU24_Dist; std::vector<double> RM2_CU24_Dist; std::vector<double> RM3_CU24_Dist; std::vector<double> RM4_CU24_Dist;
  std::vector<double> RM1_CU25_Dist; std::vector<double> RM2_CU25_Dist; std::vector<double> RM3_CU25_Dist; std::vector<double> RM4_CU25_Dist;
  std::vector<double> RM1_CU26_Dist; std::vector<double> RM2_CU26_Dist; std::vector<double> RM3_CU26_Dist; std::vector<double> RM4_CU26_Dist;
  std::vector<double> RM1_CU27_Dist; std::vector<double> RM2_CU27_Dist; std::vector<double> RM3_CU27_Dist; std::vector<double> RM4_CU27_Dist;
  std::vector<double> RM1_CU29_Dist; std::vector<double> RM2_CU29_Dist; std::vector<double> RM3_CU29_Dist; std::vector<double> RM4_CU29_Dist;
  std::vector<double> RM1_CU30_Dist; std::vector<double> RM2_CU30_Dist; std::vector<double> RM3_CU30_Dist; std::vector<double> RM4_CU30_Dist;
  std::vector<double> RM1_CU31_Dist; std::vector<double> RM2_CU31_Dist; std::vector<double> RM3_CU31_Dist; std::vector<double> RM4_CU31_Dist;
  std::vector<double> RM1_CU32_Dist; std::vector<double> RM2_CU32_Dist; std::vector<double> RM3_CU32_Dist; std::vector<double> RM4_CU32_Dist;
  std::vector<double> RM1_CU33_Dist; std::vector<double> RM2_CU33_Dist; std::vector<double> RM3_CU33_Dist; std::vector<double> RM4_CU33_Dist;
  std::vector<double> RM1_CU34_Dist; std::vector<double> RM2_CU34_Dist; std::vector<double> RM3_CU34_Dist; std::vector<double> RM4_CU34_Dist;
  std::vector<double> RM1_CU35_Dist; std::vector<double> RM2_CU35_Dist; std::vector<double> RM3_CU35_Dist; std::vector<double> RM4_CU35_Dist;
  std::vector<double> RM1_CU36_Dist; std::vector<double> RM2_CU36_Dist; std::vector<double> RM3_CU36_Dist; std::vector<double> RM4_CU36_Dist;
  std::vector<double> RM1_CU37_Dist; std::vector<double> RM2_CU37_Dist; std::vector<double> RM3_CU37_Dist; std::vector<double> RM4_CU37_Dist;
  std::vector<double> RM1_CU38_Dist; std::vector<double> RM2_CU38_Dist; std::vector<double> RM3_CU38_Dist; std::vector<double> RM4_CU38_Dist;
  std::vector<double> RM1_CU39_Dist; std::vector<double> RM2_CU39_Dist; std::vector<double> RM3_CU39_Dist; std::vector<double> RM4_CU39_Dist;
  std::vector<double> RM1_CU40_Dist; std::vector<double> RM2_CU40_Dist; std::vector<double> RM3_CU40_Dist; std::vector<double> RM4_CU40_Dist;
  std::vector<double> RM1_CU41_Dist; std::vector<double> RM2_CU41_Dist; std::vector<double> RM3_CU41_Dist; std::vector<double> RM4_CU41_Dist;
  std::vector<double> RM1_CU42_Dist; std::vector<double> RM2_CU42_Dist; std::vector<double> RM3_CU42_Dist; std::vector<double> RM4_CU42_Dist;
  std::vector<double> RM1_CU43_Dist; std::vector<double> RM2_CU43_Dist; std::vector<double> RM3_CU43_Dist; std::vector<double> RM4_CU43_Dist;
  std::vector<double> RM1_CU44_Dist; std::vector<double> RM2_CU44_Dist; std::vector<double> RM3_CU44_Dist; std::vector<double> RM4_CU44_Dist;
  
  std::cout<<"Running over RM Channels"<<std::endl;
  int channel2 = -1;
  while(tReader2.Next()){    
    channel2+=1;
    sprintf(process,"Processing Channel: %i/%i",channel2,NumChanRM);
    if(channel2%500==0){std::cout<<process<<std::endl;}

    double corr=1;
    if(doCorr == 1){
      for(int i = 0; i <= rm1Avg.size(); i++){
	if(*sipm_CU == rm1Avg[i][0] && *sipm_rm == 1){
	  corr=rm1Avg[i][1];
	  break;
	}	  
	else if(*sipm_CU == rm2Avg[i][0] && *sipm_rm == 2){
	  corr=rm2Avg[i][1];
	  break;
	}	  
	else if(*sipm_CU == rm3Avg[i][0] && *sipm_rm == 3){
	  corr=rm3Avg[i][1];
	  break;
	}	  
	else if(*sipm_CU == rm4Avg[i][0] && *sipm_rm == 4){
	  corr=rm4Avg[i][1];
	  break;
	}	  
      }
    }
    if(*sipm_rm == 1){
      xRM1[channel2] = *sipm_CU; yRM1[channel2] = corr*(*sipm_max_pc); zRM1[channel2] = *sipm_run;
      profile_rm1->Fill(*sipm_CU,corr*(*sipm_max_pc));
      profile_stab_rm1->Fill(*sipm_run,corr*(*sipm_max_pc));
      RM1_dist->Fill(corr*(*sipm_max_pc));
      if(*sipm_CU == 2)  RM1_CU2_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 3)  RM1_CU3_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 4)  RM1_CU4_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 5)  RM1_CU5_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 6)  RM1_CU6_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 7)  RM1_CU7_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 8)  RM1_CU8_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 9)  RM1_CU9_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 10) RM1_CU10_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 11) RM1_CU11_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 12) RM1_CU12_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 13) RM1_CU13_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 14) RM1_CU14_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 15) RM1_CU15_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 16) RM1_CU16_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 17) RM1_CU17_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 18) RM1_CU18_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 19) RM1_CU19_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 20) RM1_CU20_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 21) RM1_CU21_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 22) RM1_CU22_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 23) RM1_CU23_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 24) RM1_CU24_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 25) RM1_CU25_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 26) RM1_CU26_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 27) RM1_CU27_Dist.push_back(corr*(*sipm_max_pc));
                                                               if(*sipm_CU == 29) RM1_CU29_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 30) RM1_CU30_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 31) RM1_CU31_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 32) RM1_CU32_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 33) RM1_CU33_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 34) RM1_CU34_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 35) RM1_CU35_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 36) RM1_CU36_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 37) RM1_CU37_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 38) RM1_CU38_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 39) RM1_CU39_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 40) RM1_CU40_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 41) RM1_CU41_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 42) RM1_CU42_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 43) RM1_CU43_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 44) RM1_CU44_Dist.push_back(corr*(*sipm_max_pc));      
      if(*sipm_ch == 20) RM1_dist_ch20->Fill(corr*(*sipm_max_pc));
    }
    else{xRM1[channel2] = -100;yRM1[channel2] = -100;zRM1[channel2] = -100;}
    if(*sipm_rm == 2){
      xRM2[channel2] = *sipm_CU; yRM2[channel2] = corr*(*sipm_max_pc); zRM2[channel2] = *sipm_run;
      profile_rm2->Fill(*sipm_CU,corr*(*sipm_max_pc));
      profile_stab_rm2->Fill(*sipm_run,corr*(*sipm_max_pc));
      RM2_dist->Fill(corr*(*sipm_max_pc));
      if(*sipm_CU == 2)  RM2_CU2_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 3)  RM2_CU3_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 4)  RM2_CU4_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 5)  RM2_CU5_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 6)  RM2_CU6_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 7)  RM2_CU7_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 8)  RM2_CU8_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 9)  RM2_CU9_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 10) RM2_CU10_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 11) RM2_CU11_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 12) RM2_CU12_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 13) RM2_CU13_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 14) RM2_CU14_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 15) RM2_CU15_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 16) RM2_CU16_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 17) RM2_CU17_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 18) RM2_CU18_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 19) RM2_CU19_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 20) RM2_CU20_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 21) RM2_CU21_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 22) RM2_CU22_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 23) RM2_CU23_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 24) RM2_CU24_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 25) RM2_CU25_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 26) RM2_CU26_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 27) RM2_CU27_Dist.push_back(corr*(*sipm_max_pc));
                                                               if(*sipm_CU == 29) RM2_CU29_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 30) RM2_CU30_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 31) RM2_CU31_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 32) RM2_CU32_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 33) RM2_CU33_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 34) RM2_CU34_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 35) RM2_CU35_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 36) RM2_CU36_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 37) RM2_CU37_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 38) RM2_CU38_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 39) RM2_CU39_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 40) RM2_CU40_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 41) RM2_CU41_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 42) RM2_CU42_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 43) RM2_CU43_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 44) RM2_CU44_Dist.push_back(corr*(*sipm_max_pc));
    }
    else{xRM2[channel2] = -100;yRM2[channel2] = -100;zRM2[channel2] = -100;}
    if(*sipm_rm == 3){
      xRM3[channel2] = *sipm_CU; yRM3[channel2] = corr*(*sipm_max_pc); zRM3[channel2] = *sipm_run;
      profile_rm3->Fill(*sipm_CU,corr*(*sipm_max_pc));
      profile_stab_rm3->Fill(*sipm_run,corr*(*sipm_max_pc));
      RM3_dist->Fill(corr*(*sipm_max_pc));
      if(*sipm_CU == 2)  RM3_CU2_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 3)  RM3_CU3_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 4)  RM3_CU4_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 5)  RM3_CU5_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 6)  RM3_CU6_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 7)  RM3_CU7_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 8)  RM3_CU8_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 9)  RM3_CU9_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 10) RM3_CU10_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 11) RM3_CU11_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 12) RM3_CU12_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 13) RM3_CU13_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 14) RM3_CU14_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 15) RM3_CU15_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 16) RM3_CU16_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 17) RM3_CU17_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 18) RM3_CU18_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 19) RM3_CU19_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 20) RM3_CU20_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 21) RM3_CU21_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 22) RM3_CU22_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 23) RM3_CU23_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 24) RM3_CU24_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 25) RM3_CU25_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 26) RM3_CU26_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 27) RM3_CU27_Dist.push_back(corr*(*sipm_max_pc));
                                                               if(*sipm_CU == 29) RM3_CU29_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 30) RM3_CU30_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 31) RM3_CU31_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 32) RM3_CU32_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 33) RM3_CU33_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 34) RM3_CU34_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 35) RM3_CU35_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 36) RM3_CU36_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 37) RM3_CU37_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 38) RM3_CU38_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 39) RM3_CU39_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 40) RM3_CU40_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 41) RM3_CU41_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 42) RM3_CU42_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 43) RM3_CU43_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 44) RM3_CU44_Dist.push_back(corr*(*sipm_max_pc));
    }
    else{xRM3[channel2] = -100;yRM3[channel2] = -100;zRM3[channel2] = -100;}
    if(*sipm_rm == 4){
      xRM4[channel2] = *sipm_CU; yRM4[channel2] = corr*(*sipm_max_pc); zRM4[channel2] = *sipm_run;
      profile_rm4->Fill(*sipm_CU,corr*(*sipm_max_pc));
      profile_stab_rm4->Fill(*sipm_run,corr*(*sipm_max_pc));
      RM4_dist->Fill(corr*(*sipm_max_pc));
      if(*sipm_CU == 2)  RM4_CU2_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 3)  RM4_CU3_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 4)  RM4_CU4_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 5)  RM4_CU5_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 6)  RM4_CU6_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 7)  RM4_CU7_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 8)  RM4_CU8_Dist.push_back(corr*(*sipm_max_pc)); if(*sipm_CU == 9)  RM4_CU9_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 10) RM4_CU10_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 11) RM4_CU11_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 12) RM4_CU12_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 13) RM4_CU13_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 14) RM4_CU14_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 15) RM4_CU15_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 16) RM4_CU16_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 17) RM4_CU17_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 18) RM4_CU18_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 19) RM4_CU19_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 20) RM4_CU20_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 21) RM4_CU21_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 22) RM4_CU22_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 23) RM4_CU23_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 24) RM4_CU24_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 25) RM4_CU25_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 26) RM4_CU26_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 27) RM4_CU27_Dist.push_back(corr*(*sipm_max_pc));
                                                               if(*sipm_CU == 29) RM4_CU29_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 30) RM4_CU30_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 31) RM4_CU31_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 32) RM4_CU32_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 33) RM4_CU33_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 34) RM4_CU34_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 35) RM4_CU35_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 36) RM4_CU36_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 37) RM4_CU37_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 38) RM4_CU38_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 39) RM4_CU39_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 40) RM4_CU40_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 41) RM4_CU41_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 42) RM4_CU42_Dist.push_back(corr*(*sipm_max_pc));if(*sipm_CU == 43) RM4_CU43_Dist.push_back(corr*(*sipm_max_pc));
      if(*sipm_CU == 44) RM4_CU44_Dist.push_back(corr*(*sipm_max_pc));
    }
    else{xRM4[channel2] = -100;yRM4[channel2] = -100;zRM4[channel2] = -100;}
    allRM_dist->Fill(corr*(*sipm_max_pc));
  }
  sprintf(process,"Done with RM Channels: %i/%i",NumChanRM,NumChanRM);
  std::cout<<process<<std::endl;

  int numMinMax = 100;
  Double_t xRMMinMax1[numMinMax],xRMMinMax2[numMinMax],xRMMinMax3[numMinMax],xRMMinMax4[numMinMax];
  Double_t yRMMinMax1[numMinMax],yRMMinMax2[numMinMax],yRMMinMax3[numMinMax],yRMMinMax4[numMinMax];
  for(int i = 0; i<101; i++){
    yRMMinMax1[i]=-100;yRMMinMax2[i]=-100;yRMMinMax3[i]=-100;yRMMinMax4[i]=-100;      
  }
  fillMinMax(2, RM1_CU2_Dist, RM2_CU2_Dist, RM3_CU2_Dist, RM4_CU2_Dist, xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(3, RM1_CU3_Dist, RM2_CU3_Dist, RM3_CU3_Dist, RM4_CU3_Dist, xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(5, RM1_CU5_Dist, RM2_CU5_Dist, RM3_CU5_Dist, RM4_CU5_Dist, xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(6, RM1_CU6_Dist, RM2_CU6_Dist, RM3_CU6_Dist, RM4_CU6_Dist, xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(7, RM1_CU7_Dist, RM2_CU7_Dist, RM3_CU7_Dist, RM4_CU7_Dist, xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(8, RM1_CU8_Dist, RM2_CU8_Dist, RM3_CU8_Dist, RM4_CU8_Dist, xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(9, RM1_CU9_Dist, RM2_CU9_Dist, RM3_CU9_Dist, RM4_CU9_Dist, xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(10,RM1_CU10_Dist,RM2_CU10_Dist,RM3_CU10_Dist,RM4_CU10_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(11,RM1_CU11_Dist,RM2_CU11_Dist,RM3_CU11_Dist,RM4_CU11_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(12,RM1_CU12_Dist,RM2_CU12_Dist,RM3_CU12_Dist,RM4_CU12_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(13,RM1_CU13_Dist,RM2_CU13_Dist,RM3_CU13_Dist,RM4_CU13_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(14,RM1_CU14_Dist,RM2_CU14_Dist,RM3_CU14_Dist,RM4_CU14_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(15,RM1_CU15_Dist,RM2_CU15_Dist,RM3_CU15_Dist,RM4_CU15_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(16,RM1_CU16_Dist,RM2_CU16_Dist,RM3_CU16_Dist,RM4_CU16_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(17,RM1_CU17_Dist,RM2_CU17_Dist,RM3_CU17_Dist,RM4_CU17_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(18,RM1_CU18_Dist,RM2_CU18_Dist,RM3_CU18_Dist,RM4_CU18_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(19,RM1_CU19_Dist,RM2_CU19_Dist,RM3_CU19_Dist,RM4_CU19_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(20,RM1_CU20_Dist,RM2_CU20_Dist,RM3_CU20_Dist,RM4_CU20_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(22,RM1_CU22_Dist,RM2_CU22_Dist,RM3_CU22_Dist,RM4_CU22_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(23,RM1_CU23_Dist,RM2_CU23_Dist,RM3_CU23_Dist,RM4_CU23_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(24,RM1_CU24_Dist,RM2_CU24_Dist,RM3_CU24_Dist,RM4_CU24_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(25,RM1_CU25_Dist,RM2_CU25_Dist,RM3_CU25_Dist,RM4_CU25_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(26,RM1_CU26_Dist,RM2_CU26_Dist,RM3_CU26_Dist,RM4_CU26_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(27,RM1_CU27_Dist,RM2_CU27_Dist,RM3_CU27_Dist,RM4_CU27_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(29,RM1_CU29_Dist,RM2_CU29_Dist,RM3_CU29_Dist,RM4_CU29_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(30,RM1_CU30_Dist,RM2_CU30_Dist,RM3_CU30_Dist,RM4_CU30_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(31,RM1_CU31_Dist,RM2_CU31_Dist,RM3_CU31_Dist,RM4_CU31_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(32,RM1_CU32_Dist,RM2_CU32_Dist,RM3_CU32_Dist,RM4_CU32_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(33,RM1_CU33_Dist,RM2_CU33_Dist,RM3_CU33_Dist,RM4_CU33_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(34,RM1_CU34_Dist,RM2_CU34_Dist,RM3_CU34_Dist,RM4_CU34_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(35,RM1_CU35_Dist,RM2_CU35_Dist,RM3_CU35_Dist,RM4_CU35_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(36,RM1_CU36_Dist,RM2_CU36_Dist,RM3_CU36_Dist,RM4_CU36_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(37,RM1_CU37_Dist,RM2_CU37_Dist,RM3_CU37_Dist,RM4_CU37_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(38,RM1_CU38_Dist,RM2_CU38_Dist,RM3_CU38_Dist,RM4_CU38_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(39,RM1_CU39_Dist,RM2_CU39_Dist,RM3_CU39_Dist,RM4_CU39_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(40,RM1_CU40_Dist,RM2_CU40_Dist,RM3_CU40_Dist,RM4_CU40_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(41,RM1_CU41_Dist,RM2_CU41_Dist,RM3_CU41_Dist,RM4_CU41_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(42,RM1_CU42_Dist,RM2_CU42_Dist,RM3_CU42_Dist,RM4_CU42_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(43,RM1_CU43_Dist,RM2_CU43_Dist,RM3_CU43_Dist,RM4_CU43_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  fillMinMax(44,RM1_CU44_Dist,RM2_CU44_Dist,RM3_CU44_Dist,RM4_CU44_Dist,xRMMinMax1,xRMMinMax2,xRMMinMax3,xRMMinMax4,yRMMinMax1,yRMMinMax2,yRMMinMax3,yRMMinMax4);
  TGraph* graph_rm1 = new TGraph(NumChanRM,xRM1,yRM1);
  TGraph* graph_rm2 = new TGraph(NumChanRM,xRM2,yRM2);
  TGraph* graph_rm3 = new TGraph(NumChanRM,xRM3,yRM3);
  TGraph* graph_rm4 = new TGraph(NumChanRM,xRM4,yRM4);
  TGraph* graph_stab_rm1 = new TGraph(NumChanRM,zRM1,yRM1);
  TGraph* graph_stab_rm2 = new TGraph(NumChanRM,zRM2,yRM2);
  TGraph* graph_stab_rm3 = new TGraph(NumChanRM,zRM3,yRM3);
  TGraph* graph_stab_rm4 = new TGraph(NumChanRM,zRM4,yRM4);  
  TGraph* graph_MinMax_rm1 = new TGraph(numMinMax,xRMMinMax1,yRMMinMax1);
  TGraph* graph_MinMax_rm2 = new TGraph(numMinMax,xRMMinMax2,yRMMinMax2);
  TGraph* graph_MinMax_rm3 = new TGraph(numMinMax,xRMMinMax3,yRMMinMax3);
  TGraph* graph_MinMax_rm4 = new TGraph(numMinMax,xRMMinMax4,yRMMinMax4);
  graph_rm1->SetMarkerColor(kMagenta);          profile_rm1->SetLineColor(kMagenta);           graph_MinMax_rm1->SetMarkerColor(kMagenta);
  graph_rm2->SetMarkerColor(kBlue);             profile_rm2->SetLineColor(kBlue);              graph_MinMax_rm2->SetMarkerColor(kBlue);
  graph_rm3->SetMarkerColor(kGray);             profile_rm3->SetLineColor(kGray);              graph_MinMax_rm3->SetMarkerColor(kGray);
  graph_rm4->SetMarkerColor(kGreen+2);          profile_rm4->SetLineColor(kGreen+2);           graph_MinMax_rm4->SetMarkerColor(kGreen+2);
  graph_stab_rm1->SetMarkerColor(kMagenta);     profile_stab_rm1->SetLineColor(kMagenta);
  graph_stab_rm2->SetMarkerColor(kBlue);        profile_stab_rm2->SetLineColor(kBlue);
  graph_stab_rm3->SetMarkerColor(kBlack);       profile_stab_rm3->SetLineColor(kBlack);
  graph_stab_rm4->SetMarkerColor(kGray);        profile_stab_rm4->SetLineColor(kGray);
  allRM_dist->SetLineColor(kBlack);
  RM1_dist->SetLineColor(kBlack);  
  RM2_dist->SetLineColor(kBlack);  
  RM3_dist->SetLineColor(kBlack);  
  RM4_dist->SetLineColor(kBlack);  
  RM1_dist_ch20->SetLineColor(kBlack);
  graph_rm1->SetMarkerStyle(kFullSquare);       graph_stab_rm1->SetMarkerStyle(kFullSquare);  graph_MinMax_rm1->SetMarkerStyle(kFullSquare);
  graph_rm2->SetMarkerStyle(kFullSquare);       graph_stab_rm2->SetMarkerStyle(kFullSquare);  graph_MinMax_rm2->SetMarkerStyle(kFullSquare);
  graph_rm3->SetMarkerStyle(kFullSquare);       graph_stab_rm3->SetMarkerStyle(kFullSquare);  graph_MinMax_rm3->SetMarkerStyle(kFullSquare);
  graph_rm4->SetMarkerStyle(kFullSquare);       graph_stab_rm4->SetMarkerStyle(kFullSquare);  graph_MinMax_rm4->SetMarkerStyle(kFullSquare);
  
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
  TLatex* PinDiode2345 = new TLatex(0.14, 0.91, "Pin-Diode 2,3,4,5");
  PinDiode2345->SetNDC();
  PinDiode2345->SetTextFont(42);
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
  TLatex* RM1_ch20 = new TLatex(0.2, 0.85, "RM 1 Channel 20");
  RM1_ch20->SetNDC();
  RM1_ch20->SetTextFont(42);

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
  TLatex* EntriesRM1_ch20   = Entries(0.92,0.85,RM1_dist_ch20);
  TLatex* MeanRM1_ch20      = Mean(0.92, 0.8,RM1_dist_ch20);
  TLatex* StdDevRM1_ch20    = StdDev(0.92, 0.75,RM1_dist_ch20);

  //----------------------------------------------------------------------
  //Plotting
  //----------------------------------------------------------------------
  std::cout<<"Plotting"<<std::endl;
  TLegend* catLeg0 = new TLegend(0.68,0.65,0.96,0.88);
  TCanvas *c0 = Canvas("MaxpCvsCU",800,800);
  gPad->SetLogy();
  catLeg0->SetBorderSize(0);
  catLeg0->SetFillStyle(0);
  catLeg0->SetTextSize(0.04);
  TH1F* h0blank = Blank("Blank0",125,0,70);
  h0blank->SetMinimum(5);
  h0blank->SetMaximum(80000/convert);
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

  TCanvas *c1 = Canvas("1D PD All",800,800);
  TH1F* h1blank = Blank("Blank1",62,0,disMaxPD/convert);
  h1blank->SetMinimum(pinDisMin);
  h1blank->SetMaximum(3*pinDisMax);
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
  h2blank->SetMaximum(disMax/convert);
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
  TH1F* h3blank = Blank("Blank3",62,0,disMax/convert);
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
  TH1F* h4blank = Blank("Blank4",125,0,65);
  h4blank->SetMinimum(0);
  h4blank->SetMaximum((disMax-50000)/convert);
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
  TH1F* h5blank = Blank("Blank5",125,0,65);
  h5blank->SetMinimum(0);
  h5blank->SetMaximum((disMax-50000)/convert);
  h5blank->GetXaxis()->SetTitle("CU");
  h5blank->GetYaxis()->SetTitle("Max Charge [pC]");
  h5blank->Draw("hist");  
  //profile_pd0->Draw("P same");
  //profile_pd1->Draw("P same");
  graph_pd0->Draw("P same");
  graph_pd1->Draw("P same");
  profile_rm1->Draw("P same");
  profile_rm2->Draw("P same");
  profile_rm3->Draw("P same");
  profile_rm4->Draw("P same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  catLeg5->Draw();
  //catLeg5->AddEntry(profile_pd0,"Pin-Diode 0","L");
  //catLeg5->AddEntry(profile_pd1,"Pin-Diode 1","L");
  catLeg5->AddEntry(graph_pd0,"Pin-Diode 0","P");
  catLeg5->AddEntry(graph_pd1,"Pin-Diode 1","P");
  catLeg5->AddEntry(profile_rm1,"RM 1","L");
  catLeg5->AddEntry(profile_rm2,"RM 2","L");
  catLeg5->AddEntry(profile_rm3,"RM 3","L");
  catLeg5->AddEntry(profile_rm4,"RM 4","L");

  TCanvas *c6  = Canvas("1D PD0",800,800);
  TH1F* h6blank = Blank("Blank6",125,0,disMaxPD/convert);
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
  TH1F* h7blank = Blank("Blank7",125,0,disMaxPD/convert);
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
  TH1F* h8blank = Blank("Blank8",125,0,disMaxPD/convert);
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
  TH1F* h9blank = Blank("Blank9",125,0,disMaxPD/convert);
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
  TH1F* h10blank = Blank("Blank10",125,0,disMaxPD/convert);
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
  TH1F* h11blank = Blank("Blank11",125,0,disMaxPD/convert);
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
  TH1F* h12blank = Blank("Blank12",125,0,disMaxPD/convert);
  h12blank->SetMinimum(pinDisMin);
  h12blank->SetMaximum(3*pinDisMax);
  h12blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h12blank->GetYaxis()->SetTitle("Channels");
  h12blank->Draw();
  PD2_3_4_5_dist->Draw("same");
  //CMSPrelim1->Draw();
  PinDiode2345->Draw();
  burnIn->Draw();
  //PinDiode->Draw();
  EntriesPD2_3_4_5->Draw();
  MeanPD2_3_4_5->Draw();
  StdDevPD2_3_4_5->Draw();
  TCanvas *c13 = Canvas("1D RM1",800,800);
  TH1F* h13blank = Blank("Blank13",125,0,disMax/convert);
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
  TH1F* h14blank = Blank("Blank14",125,0,disMax/convert);
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
  TH1F* h15blank = Blank("Blank15",125,0,disMax/convert);
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
  TH1F* h16blank = Blank("Blank16",125,0,disMax/convert);
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
  h17blank->SetMaximum(disMax/convert);
  h17blank->GetXaxis()->SetTitle("Runs");
  h17blank->GetYaxis()->SetTitle("Max Charge [pC]");
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
  h18blank->SetMaximum(disMax/convert);
  h18blank->GetXaxis()->SetTitle("Runs");
  h18blank->GetYaxis()->SetTitle("Max Charge [pC]");
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
  TCanvas *c19 = Canvas("1D RM1 ch20",800,800);
  TH1F* h19blank = Blank("Blank19",125,0,disMax/convert);
  h19blank->SetMinimum(rmDisMin);
  h19blank->SetMaximum(rmDisMax/20);
  if(doCorr == 1)h19blank->SetMaximum(rmDisMax/15);
  h19blank->GetXaxis()->SetTitle("Max Charge [pC]");
  h19blank->GetYaxis()->SetTitle("Channels");
  h19blank->Draw();
  RM1_dist_ch20->Draw("same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  RM->Draw();
  EntriesRM1_ch20->Draw();
  MeanRM1_ch20->Draw();
  StdDevRM1_ch20->Draw();
  RM1_ch20->Draw();

  TCanvas *c20 = Canvas("Min Max: MaxpCvsCU 0,1, RM1-4",800,800);  
  TLegend* catLeg20 = new TLegend(0.68,0.65,0.96,0.88);
  catLeg20->SetBorderSize(0);
  catLeg20->SetFillStyle(0);
  catLeg20->SetTextSize(0.04);
  TH1F* h20blank = Blank("Blank20",125,0,65);
  h20blank->SetMinimum(0);
  h20blank->SetMaximum((disMax-50000)/convert);
  h20blank->GetXaxis()->SetTitle("CU");
  h20blank->GetYaxis()->SetTitle("Max Charge [pC]");
  h20blank->Draw("hist");  
  graph_pd0->Draw("P same");
  graph_pd1->Draw("P same");
  graph_MinMax_rm1->Draw("P same");
  graph_MinMax_rm2->Draw("P same");
  graph_MinMax_rm3->Draw("P same");
  graph_MinMax_rm4->Draw("P same");
  //CMSPrelim1->Draw();
  burnIn->Draw();
  catLeg20->Draw();
  catLeg20->AddEntry(graph_pd0,"Pin-Diode 0","P");
  catLeg20->AddEntry(graph_pd1,"Pin-Diode 1","P");
  catLeg20->AddEntry(graph_MinMax_rm1,"RM 1","P");
  catLeg20->AddEntry(graph_MinMax_rm2,"RM 2","P");
  catLeg20->AddEntry(graph_MinMax_rm3,"RM 3","P");
  catLeg20->AddEntry(graph_MinMax_rm4,"RM 4","P");

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
  c19->SaveAs("RM1_1D_ch20.pdf");
  c20->SaveAs("MinMax_RM_PDvsCU2D.pdf");  
} 

int main(){
  CU_Plots();
}
