/*
Author Rakitha Beminiwattha
contact rakithab@jlab.org
PlotTools is a set collection of routines that enables to plot multiple graphs reading a text file
v 0.0.0 Thu 17:57:56 2011 :
                       Read a text file which should have tab separated columns of X (X-Error) and Y(Y-Errors) values.
                       Plots on a multigraph with or without errors. For multigraphs only common X axis 
                       can be used. 
                       At the end of the data There must be following lines,
                          1. After data next must have 'END'
                          2. canvas name
                          3. canvas title
                          4. Legend title
                          5. names of Y axis parameter (line for each Y value)
                          6. Multi Graph Title
                          7. X axis name + unit
                          8. Y axis name + unit

		       IF no X or Y errors set the parameters hasXErrors and  hasYErrors to false and use the
		       routine Plot()

		       If X or/and Y has error set above parameters apropriately and use the routine,
		       PlotwithError()

		       Set parameter ny to no. of Y parameters
		       Set parameter n to no of data points

		       Then use,
		       root -l
		       .L PlotTools.C
		       Plot() or
		       PlotwithErrors()
		       

 */

#include <vector>
#include "TRandom.h"

#include <math.h>
#include <TH2F.h>
#include <TTree.h>
#include <iostream>
#include <fstream>
#include <new>
#include <string>
#include <TF1.h>
#include <Rtypes.h>
#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include <TString.h>
#include <TDatime.h>
#include <stdexcept>
#include <time.h>
#include <cstdio>
#include <TMath.h>

#include <cstdlib>


#define CANVAS_SPLIT_Col 2
#define CANVAS_SPLIT_Raw 2


Int_t Read_Data_file(TString data_file);
string GetNextToken(const string& line);
string GetNextToken(const string& line, const char& delim);
void PlotwithErrors();
const Int_t n = 4;//
const Int_t ny =1;
Double_t X[n], Y[ny][n];
//vector<Double_t> X;
//vector<<vector<Double_t>Double_t> Y;
//X.resize(n);
//Y.resize(ny);
Double_t eX[n], eY[ny][n];
Bool_t hasXErrors = kFALSE;//kTRUE;
Bool_t hasYErrors = kTRUE;//kFALSE;//kTRUE;

//These will be read from the data file

TString CanvasName;
TString MultiGraphName;
TString LegHeader;
TString GTitles[ny];
TString multigraphtitle;
TString xTitle;
TString yTitle;

TString multicanvas_name="Multi_C";
TString multicanvas_title="Multiple Plots";


//Use this routine to check the data file
Int_t Read_Data_file(TString data_file)
{
    TString line;
  vector<Double_t> data;
  ifstream inputdata(data_file);
  if(!inputdata)
    {
      std::cerr << "ERROR: file with values :"<<data_file<<" not found trying to read the default file\n";
      return(1);
    }
  else
    std::cout<<" file with values : "<<data_file<<" is open \n";


  while(inputdata){
    //line.ReadToken(inputdata);
    line.ReadLine(inputdata);
    //std::cout<<line<<std::endl;
    GetNextToken(line.Data(),'\t');
    if(line(0,1)=="#"){  //this is a line of commentary
      std::cout<<"this is a commentary line"<<std::endl;
      line.ReadLine(inputdata);
      
    }
    else{
      
    }
  }

  inputdata.close();
}

string GetNextToken(const string& line)
{
  string tmpstring = "";
  size_t pos1 = 0;
  size_t pos2 = line.length();//line.find_first_of(separatorchars.c_str(), pos1);
  size_t fCurrentPos=0;
  size_t count=0;
  printf("line len %i \n",line.length());
  while (fCurrentPos<line.length()){
    if (line.at(pos1)==' '){
      pos1++;
      //printf("pos++ %i \n",pos1);
      continue;
    }
      
    fCurrentPos = line.find_first_of(' ', pos1);
    //printf("fCurrentPos %i pos1 %i \n",fCurrentPos,pos1);
    tmpstring   = line.substr(pos1,(fCurrentPos-pos1));
    count++;
    printf("[%d] %s ",pos1,tmpstring.c_str());
    pos1=fCurrentPos+1;
    /*    if (tmpstring.compare(" ")==0){
      pos1++; 
      continue;
      }*/

    
  }
  printf("\n no.of tokens %i \n",count);

  return tmpstring;
};

string GetNextToken(const string& line, const char& delim)
{
  TString value;
  string tmpstring = "";
  size_t pos1 = 0;
  size_t pos2 = line.length();//line.find_first_of(separatorchars.c_str(), pos1);
  size_t fCurrentPos=0;
  size_t count=0;
  printf("line len %i \n",line.length());
  while (fCurrentPos<line.length()){
    if (pos1>0 && pos1<line.length()){
      //if (line.at(pos1)=='\n')
      //break;
      if (line.at(pos1)==' '){
	pos1++;
	//printf("pos++ %i \n",pos1);
	continue;
      }
    }
      
    fCurrentPos = line.find_first_of(delim, pos1);
    //printf("fCurrentPos %i pos1 %i \n",fCurrentPos,pos1);

    tmpstring   = line.substr(pos1,(fCurrentPos-pos1));
    //printf("before %i \n",tmpstring.length());
    TrimWhiteSpaces(tmpstring);
    //printf("After %i \n",tmpstring.length());
    if (tmpstring.length()>0){
      value=tmpstring.c_str();
      printf("[%d] %s %f",pos1,value.Data(),value.Atof());
      count++;
    }
        
    pos1=fCurrentPos+1;
    /*    if (tmpstring.compare(" ")==0){
      pos1++; 
      continue;
      }*/

    
  }
  printf("\n no.of tokens %i \n",count);

  return tmpstring;
};

void TrimWhiteSpaces(string &line){
  string tmpstring;
  size_t pos1 = 0;
  size_t pos2 = 0;
  pos2 = line.find_first_not_of(' ');
  if (line.length()==0)
    return;
  tmpstring=line.substr(pos2,line.length());
  //printf("line %s len %i \n",line.c_str(),line.length());
  //printf("line %s len %i \n",tmpstring.c_str(),tmpstring.length());
  line=tmpstring;
  pos2 = line.find_last_of(' ');
  tmpstring=line.substr(0,pos2);
  //printf("line %s len %i \n",tmpstring.c_str(),tmpstring.length());
  line=tmpstring; 
  pos2 = line.find_first_not_of('\t');
  tmpstring=line.substr(pos2,line.length());
  //printf("line %s len %i \n",line.c_str(),line.length());
  //printf("line %s len %i \n",tmpstring.c_str(),tmpstring.length());
  line=tmpstring;
  pos2 = line.find_last_of('\t');
  tmpstring=line.substr(0,pos2);
  // printf("line %s len %i \n",tmpstring.c_str(),tmpstring.length());
  line=tmpstring; 
}

vector<Double_t> Tokenize(const string& line, const char& delim){
  vector<Double_t> data;
  TString value;
  string tmpstring = "";
  size_t pos1 = 0;
  size_t fCurrentPos=0;
  size_t count=0;
  //printf("line len %i \n",line.length());
  while (fCurrentPos<line.length()){
    if (pos1>0 && pos1<line.length()){
      if (line.at(pos1)=='\n')
	break;
      if (line.at(pos1)==' '){
	pos1++;
	//printf("pos++ %i \n",pos1);
	continue;
      }
    }
      
    fCurrentPos = line.find_first_of(delim, pos1);
    //printf("fCurrentPos %i pos1 %i \n",fCurrentPos,pos1);
    
    tmpstring   = line.substr(pos1,(fCurrentPos-pos1));
    TrimWhiteSpaces(tmpstring);
    if (tmpstring.length()>0){
      value=tmpstring.c_str();
 
      printf("[%d] %f",pos1,value.Atof());
      data.push_back(value.Atof());
      count++;
    }
        
    pos1=fCurrentPos+1;
    
  }
  printf("\n no.of tokens %i \n",count);

  return data;
}

void DecodeDataFile(TString data_file){

  Int_t x,y;
  vector<Double_t> raw_data;//data for one raw
  TString line;

  ifstream inputdata(data_file);
  if(!inputdata)
    {
      std::cerr << "ERROR: file with values :"<<data_file<<" not found trying to read the default file\n";
      return(1);
    }
  else
    std::cout<<" file with values : "<<data_file<<" is open \n";

  x=0;//no of entries/raws
  y=0;//no.of data columns in a raw
  while(inputdata){
    /*
    if (x==n){
      printf("Arrays maximum limit reached %i \n",n);
      break;
    }
    */
    line.ReadLine(inputdata);

    if(line(0,1)=="#"){  //this is a line of commentary
      std::cout<<"Comment :"<<line<<std::endl;
      continue;
    }else if (line=="END"){
      line.ReadLine(inputdata);
      CanvasName=line;
      line.ReadLine(inputdata);
      MultiGraphName=line;
      line.ReadLine(inputdata);
      LegHeader=line;
	for (Int_t i=0;i<ny;i++){
	  line.ReadLine(inputdata);
	  GTitles[i]=line;
	}
      line.ReadLine(inputdata);
      multigraphtitle=line;
      line.ReadLine(inputdata);
      xTitle=line;
      line.ReadLine(inputdata);
      yTitle=line;
      break;
    }
    else{
      //GetNextToken(line.Data(),'\t');
      raw_data=Tokenize(line.Data(),'\t');
      FillDataArrays(x,raw_data);
      x++;
     }
    

  }

  inputdata.close();
}

void FillDataArrays(Int_t x,vector<Double_t> &raw_data){
  Int_t count_col=1;
  Int_t count=0;
  X[x]=raw_data.at(0);//x value
  if (hasXErrors){
    eX[x]=raw_data.at(1);//x value
    count_col++;
  }else
    eX[x]=0;

  while(count<ny){
     Y[count][x]=raw_data.at(count_col);//y1 value
     if (hasYErrors){
       eY[count][x]=raw_data.at(count_col+1);//y1 error value
       count_col+=2;
     }else
       count_col++;
     count++;
  }

}

void Plot() {
  //DecodeDataFile("./data_file_MD_2_TS_6393.in");
  //DecodeDataFile("./data_file_MD_2_TS_6394.in");
  //DecodeDataFile("./data_file_6393.in");
  //DecodeDataFile("./data_file_6393_MD6.in");
  //DecodeDataFile("./data_file_6394.in");
  //DecodeDataFile("./data_file_6394_MD7.in");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Beam_Trip_Analysis/Data_Beam_Trips_Stability_Cuts_R7143_SD_1");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Run_Summary/MD_ALL_Asym_Width_IHWP_IN");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Run_Summary/MD_ALL_Asym_Width_IHWP_OUT");


  //MD bar corrections terms
  DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_MD_Trig_qtor_scan_MD5_Corrections");

  TCanvas *c1 = new TCanvas(Form("C_%s",MultiGraphName.Data()),Form("Canvas-%s",CanvasName.Data()),200,10,700,500);
  c1->cd();
  //c1->SetFillColor(42);
  c1->SetGrid();

  TGraph * gr[ny];
  TLegend *leg = new TLegend(0.1,0.7,0.28,0.8);
  TMultiGraph *mg = new TMultiGraph(MultiGraphName,multigraphtitle);

  leg->SetHeader(LegHeader);
  leg->SetFillColor(41);
  for (Int_t i=0;i<ny;i++){
    gr[i] = new TGraph(n,X,Y[i]);
    //gr[i]->SetLineColor(1+i);
    gr[i]->SetLineWidth(4);
    gr[i]->SetMarkerColor(1+i);
    //gr[i]->SetMarkerStyle(21);
    gr[i]->SetMarkerStyle(i%3+2);
    gr[i]->SetTitle(GTitles[i]);
    leg->AddEntry(gr[i],gr[i]->GetTitle(), "p");
    mg->Add(gr[i],"P");
  }



  mg->Draw("a");
  mg->GetXaxis()->SetTitle(xTitle);
  mg->GetYaxis()->SetTitle(yTitle);

  leg->Draw();

  c1->Update();
  //c1->GetPad()->SetFillColor(21);
  //c1->GetFrame()->SetBorderSize(12);
  c1->Modified();




}

void PlotwithErrors() {
  //DecodeDataFile("./data_file_charge_asym.in");
  //DecodeDataFile("./data_file_6393_MD6.in");
  //DecodeDataFile("./data_file_6394_MD7.in");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Data_Beam_Trips_Stability_Cuts_R7143");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Data_Beam_Trips_Stability_Cuts_R7143_CoV_1p");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Data_Beam_Trips_Stability_Cuts_R7143_CoV_0.5p");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Beam_Trip_Analysis/Data_Beam_Trips_Stability_Cuts_R7143_SD_1");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Run_Summary/MD_ALL_IHWP_OUT_2ppm");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Run_Summary/MD_ALL_IHWP_IN_2ppm");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Run_Summary/Slug_Plot_IN_9278_9313_01-24-2011");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Run_Summary/Slug_Plot_OUT_9278_9313_01-24-2011");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Run_Summary/MD_ALL_BCM_IHWP_IN_Asym_Corr");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Fall10_Run_Summary/MD_All_BCM2_Asym_Corr_IHWP_IN_20ppm");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Fall10_Run_Summary/MD_ALL_IHWP_IN_2ppm_Corrected");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Fall10_Run_Summary/BCM1_Asym_IHWP_IN_20ppm");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Run_Summary/BCM1_Asym_IHWP_IN_2ppm");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/QwFeedback_Analysis/Data_Set_run9695_9703");
  //Qtor scans
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_TS_Trig_qtor_scan");
  //Corrected scaler rates
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_MD_Trig_qtor_scan_corrected");
  //comparisons for MD
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_TS_Trig_qtor_scan_MD_Compare");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_MD_Trig_qtor_scan_MD_Compare");

  //Large A_q runs analysis
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/BCM_Calib/data_set_large_Aq_10851-to-10854_IHWP_IN");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/BCM_Calib/data_set_large_Aq_10813-to-10816_IHWP_OUT");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/BCM_Calib/data_set_large_Aq_MDall_11293_IHWP_OUT");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/BCM_Calib/data_set_large_Aq_MDall_11292_IHWP_IN");

  //PITA Scan
  //04-07-2011
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/BCM_Calib/data_set_large_Aq_11153_IHWP_IN");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/BCM_Calib/data_set_large_Aq_11154_IHWP_OUT");
  //04-12-2011
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/BCM_Calib/data_set_large_Aq_11293_IHWP_OUT");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/BCM_Calib/data_set_large_Aq_11292_IHWP_IN");

  //Hall A IA scan
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/ha_bcm/data_set_ha_IA_scan_hc_bcm");
											   
  //corrected MD all bar asym 
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/BCM_Calib/data_set_Corrected_MD_ALL_10813-to-10816_IHWP_OUT");
  //LH2 qtor scan data
  //corrected scaler rates
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_TS_Trig_qtor_scan_corrected");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/BCM_Calib/data_set_Corrected_MD_All_10851-to-10854_IHWP_IN");
  //Lumi rates for LH2 QTOR scan
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_USLumi_rates");

  //Scaler rates norm to US Lumi
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_TS_Trig_qtor_scan_Norm_Lumi");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_TS_Trig_qtor_scan_MD_Compare_lumi_norm");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_MD_Trig_qtor_scan_MD_Compare_lumi_norm");


  //moller data analysis
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Moller_Data_Analysis/data_set_03-31-2011_IHWP-IN_A-q-adjusted");
  DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Moller_Data_Analysis/data_set_04-15-2011_IHWP-OUT-Slit-control");
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/Moller_Data_Analysis/data_set_04-15-2011_IHWP-IN");



  //scaler rates norm to US scaler rates
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_TS_Trig_qtor_scan_Norm_US_lumi");
  //US lumi calibration
  //DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_Lumi_Calibrated_Current");

  TCanvas *c1 = new TCanvas(Form("C_%s",MultiGraphName.Data()),Form("Canvas-%s",CanvasName.Data()),200,10,700,500);
  c1->cd();
  //c1->SetFillColor(42);
  c1->SetGrid();

  TGraphErrors * gr[ny];
  TLegend *leg = new TLegend(0.1,0.7,0.28,0.8);
  TMultiGraph *mg = new TMultiGraph(MultiGraphName,multigraphtitle);

  leg->SetHeader(LegHeader);
  leg->SetFillColor(41);
  for (Int_t i=0;i<ny;i++){
    gr[i] = new TGraphErrors(n,X,Y[i],eX,eY[i]);
    gr[i]->SetLineColor(1+i);
    gr[i]->SetLineWidth(1);
    gr[i]->SetMarkerColor(2+i);
    //gr[i]->SetMarkerStyle(21);//full square
    //gr[i]->SetMarkerStyle(32);//open triangle down
    gr[i]->SetMarkerStyle(i%3+2);
    gr[i]->SetMarkerSize(1.4);
    gr[i]->SetTitle(GTitles[i]);
    leg->AddEntry(gr[i],gr[i]->GetTitle(), "p");

    mg->Add(gr[i],"P");
  }


  
  mg->Draw("a");
  mg->GetXaxis()->SetTitle(xTitle);
  mg->GetYaxis()->SetTitle(yTitle);

  leg->Draw();
  c1->SetFillColor(19);
  c1->Update();
  c1->GetFrame()->SetFillColor(19);
  //c1->GetFrame()->SetBorderSize(12);
  c1->Modified();

}


void PlotMany() {
  TCanvas *multi_canvas = new TCanvas(Form("C_%s",multicanvas_name.Data()),Form("Canvas-%s",multicanvas_title.Data()),200,10,1000,1000);
  multi_canvas->Divide(CANVAS_SPLIT_Col,CANVAS_SPLIT_Raw);
  multi_canvas->SetGrid();
  multi_canvas->Modified();
  for(Int_t i=0;i<CANVAS_SPLIT_Col*CANVAS_SPLIT_Raw;i++){
    multi_canvas->cd(i);
    
  }
}

