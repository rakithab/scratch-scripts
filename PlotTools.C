/*
Author : Rakitha Beminiwattha
Contact: rakithab@jlab.org
Date   : 02-04-2011
Desc   : This script provides support routines to plot set of data point and error in a text file.
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
Int_t Read_Data_file(TString data_file);
string GetNextToken(const string& line);
string GetNextToken(const string& line, const char& delim);
const Int_t n = 3;
const Int_t ny = 1;
Double_t X[n], Y[ny][n];
Double_t eX[n], eY[ny][n];

//These will be read from the data file

TString CanvasName;
TString MultiGraphName;
TString LegHeader;
TString GTitles[ny];
TString multigraphtitle;
TString xTitle;
TString yTitle;

gStyle->SetOptFit(1);
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
  while(count<ny){
     Y[count][x]=raw_data.at(count_col);//y1 value
     eY[count][x]=raw_data.at(count_col+1);//y1 error value
     count_col+=2;
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
  //DecodeDataFile("/home/rakitha/Qweak_Analysis/data_set_large_Aq");

  TCanvas *c1 = new TCanvas(Form("C_%s",MultiGraphName.Data()),Form("Canvas-%s",CanvasName.Data()),200,10,700,500);
  c1->cd();
  c1->SetFillColor(42);
  c1->SetGrid();

  TGraph * gr[ny];
  TLegend *leg = new TLegend(0.1,0.7,0.28,0.8);
  TMultiGraph *mg = new TMultiGraph(MultiGraphName,multigraphtitle);

  leg->SetHeader(LegHeader);
  leg->SetFillColor(kWhite);
  for (Int_t i=0;i<ny;i++){
    gr[i] = new TGraph(n,X,Y[i]);
    //gr[i]->SetLineColor(1+i);
    gr[i]->SetLineWidth(4);
    gr[i]->SetMarkerColor(1+i);
    gr[i]->SetMarkerStyle(21);
    gr[i]->SetTitle(GTitles[i]);
    leg->AddEntry(gr[i],gr[i]->GetTitle(), "p");
    mg->Add(gr[i],"P");
  }



  mg->Draw("a");
  mg->GetXaxis()->SetTitle(xTitle);
  mg->GetYaxis()->SetTitle(yTitle);

  leg->Draw();

  c1->Update();
  c1->GetFrame()->SetFillColor(21);
  //c1->GetFrame()->SetBorderSize(12);
  c1->Modified();




}

void PlotwithErrors() {
  //DecodeDataFile("./data_file_charge_asym.in");
  //DecodeDataFile("./data_file_6393_MD6.in");
  //DecodeDataFile("./data_file_6394_MD7.in");
  //DecodeDataFile("/home/rakitha/Documents/Moller_Data_analysis/Data_Moller_Sol_Scan");
  //DecodeDataFile("/home/rakitha/Documents/Moller_Data_analysis/Data_Moller_Sol_Scan_bias");
  //DecodeDataFile("/home/rakitha/Documents/Moller_Data_analysis/Data_Moller_Sol_Scan_bias_total");
  //DecodeDataFile("/home/rakitha/Documents/Moller_Data_analysis/Data_Moller_Quad_Tune");
  //DecodeDataFile("/home/rakitha/Qweak_Analysis/DataSet1/data_set1_IHWP_OUT");
  //DecodeDataFile("/home/rakitha/Qweak_Analysis/DataSet1/data_set1_IHWP_IN");
  //DecodeDataFile("/home/rakitha/Documents/Moller_Data_analysis/data_set_Moller_01-31-2011");
  //DecodeDataFile("/home/rakitha/Documents/Moller_Data_analysis/data_set_Moller_02-04-2011");//moller measurement IHWP IN
  //DecodeDataFile("/home/rakitha/Documents/Moller_Data_analysis/data_set_Moller_02-04-2011_OUT");
  //DecodeDataFile("/home/rakitha/Qweak_Analysis/data_set_bcm1_IHWP_OUT_9594_9612");
  DecodeDataFile("/home/rakitha/Qweak_Analysis/data_set_large_Aq");
  TCanvas *c1 = new TCanvas(Form("C_%s",MultiGraphName.Data()),Form("Canvas-%s",CanvasName.Data()),200,10,700,500);
  c1->cd();
  c1->SetFillColor(42);
  c1->SetGrid();

  TGraphErrors * gr[ny];
  TLegend *leg = new TLegend(0.1,0.7,0.28,0.8);
  TMultiGraph *mg = new TMultiGraph(MultiGraphName,multigraphtitle);

  leg->SetHeader(LegHeader);
  leg->SetFillColor(kWhite);
  for (Int_t i=0;i<ny;i++){
    gr[i] = new TGraphErrors(n,X,Y[i],0,eY[i]);
    gr[i]->SetLineColor(2+i);
    gr[i]->SetLineWidth(1);
    gr[i]->SetMarkerColor(2+i);
    gr[i]->SetMarkerStyle(21);
    gr[i]->SetTitle(GTitles[i]);
    leg->AddEntry(gr[i],gr[i]->GetTitle(), "p");
    mg->Add(gr[i],"P");
  }



  mg->Draw("a");
  mg->GetXaxis()->SetTitle(xTitle);
  mg->GetYaxis()->SetTitle(yTitle);
  //gStyle->SetOptFit(1);

  leg->Draw();

  c1->Update();
  c1->GetFrame()->SetFillColor(21);
  //c1->GetFrame()->SetBorderSize(12);
  c1->Modified();

}
