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

v 0.1.0                Replaced X,Y arrays with vectors of type Double_t

v 1.0.0                All the parameters are passed through text files - Tue Apr 26 21:14:57 EDT 2011
                       Each data file has three sections.
		       INIT
		       #no. of raws
		       #no. of y columns
		       #Y has errors 1 or 0
		       #X has errors 1 or 0
		       Data, X (+/- error) columns Y1 (+/- error) columns ....Yn (+/- error)
		       END
                          canvas name
                          canvas title
                          Legend title
                          names of Y axis parameter (line for each Y value)
                          Multi Graph Title
                          X axis name + unit
                          Y axis name + unit

		      Then many(1 to N) of above data files can be passed together to be drawn in a divided canvas
		      This main file contains list of data files and section 'END'
		      canvas name
		      canvas title
		      no.of row in the canvas
		      no. of columns in the canvas
		      
		      Make sure columns times the rows must greater than or equal to no. of data files in to be
		      drawn

		      To plot
		      .L PlotTools.C
		      PlotMany("/path/data_file_list_file_name")
		       





*/

#include <vector>
#include <algorithm>
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
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TGraphErrors.h>
#include <TFrame.h>



Int_t Read_Data_file(TString data_file);
string GetNextToken(const string& line);
string GetNextToken(const string& line, const char& delim);
void TrimWhiteSpaces(string &line);
vector<Double_t> Tokenize(const string& line, const char& delim);
void DecodeDataFile(TString data_file);
void FillDataArrays(Int_t x,vector<Double_t> &raw_data);
void Plot();
void PlotwithErrors();
void PlotMany(TString list_file_name);
void PlotData(TString data_file);
Int_t n;//no. of rows of data 
Int_t ny;//no. of y parameters

vector<Double_t> X;
vector< vector<Double_t> > Y;

vector<Double_t> eX;
vector< vector<Double_t> > eY;

Bool_t hasXErrors;// set at individual data files
Bool_t hasYErrors;// set at individual data files

//These will be read from the data file

TString CanvasName;
TString MultiGraphName;
TString LegHeader;
vector<TString> GTitles;
TString multigraphtitle;
TString xTitle;
TString yTitle;




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

  }

  inputdata.close();

  return 0;
}

string GetNextToken(const string& line)
{
  string tmpstring = "";
  size_t pos1 = 0;
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
      return;
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
	  GTitles.push_back(line);
	}
      line.ReadLine(inputdata);
      multigraphtitle=line;
      line.ReadLine(inputdata);
      xTitle=line;
      line.ReadLine(inputdata);
      yTitle=line;
      break;
    }else if (line=="INIT"){
      line.ReadLine(inputdata);
      n=line.Atoi();//no. of data rows
      line.ReadLine(inputdata);
      ny=line.Atoi();//no. of y columns
      line.ReadLine(inputdata);
      X.empty();
      X.resize(n);
      eX.empty();
      eX.resize(n);
      for(Int_t i=0;i<ny;i++){
	Y.empty();
	Y.push_back(X); 
	eY.empty();
	eY.push_back(X); 
      }
      if(line.Contains("1")){
	hasYErrors=kTRUE;
      }
      else
	hasYErrors=kFALSE;
      line.ReadLine(inputdata);
      if(line.Contains("1")){  
	hasXErrors=kTRUE;
      }
      else
	hasXErrors=kFALSE;
      //set X,Y sizes properly
 
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
    //eX[x]=0;
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

  //MD bar corrections terms
  DecodeDataFile("/home/rakithab/Qweak_Data_Analysis/qtor_scans/LH2_Scans/LH2_MD_Trig_qtor_scan_MD5_Corrections");


 //to convert the double vectors to arrays.
  Double_t *dd_x=new Double_t[X.size()];
  Double_t *dd_y=new Double_t[X.size()];
  copy(X.begin(),X.end(),dd_x);




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
    copy(Y[i].begin(),Y[i].end(),dd_y);
    gr[i] = new TGraph(n,dd_x,dd_y);
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
  DecodeDataFile("./data_file_charge_asym.in");
  

  TCanvas *c1 = new TCanvas(Form("C_%s",MultiGraphName.Data()),Form("Canvas-%s",CanvasName.Data()),200,10,700,500);
  c1->cd();
  //c1->SetFillColor(42);
  c1->SetGrid();

  //to convert the double vectors to arrays.
  Double_t *dd_x=new Double_t[X.size()];
  Double_t *dd_y=new Double_t[X.size()];
  copy(X.begin(),X.end(),dd_x);

  //copy errors
  Double_t *dd_ex=NULL;
  Double_t *dd_ey=NULL;

  if(hasXErrors){
    dd_ex=new Double_t[X.size()];
    copy(eX.begin(),eX.end(),dd_ex);
  }

  TGraphErrors* gr=NULL;

  TLegend *leg = new TLegend(0.1,0.7,0.28,0.8);
  TMultiGraph *mg = new TMultiGraph(MultiGraphName,multigraphtitle);

  leg->SetHeader(LegHeader);
  leg->SetFillColor(41);
  for (Int_t i=0;i<ny;i++){
    copy(Y[i].begin(),Y[i].end(),dd_y);
    if(hasYErrors){
      dd_ey=new Double_t[X.size()];
      copy(eY[i].begin(),eY[i].end(),dd_ey);
    }
    gr= new TGraphErrors(n,dd_x,dd_y,dd_ex,dd_ey);
    gr->SetLineColor(1+i);
    gr->SetLineWidth(1);
    gr->SetMarkerColor(2+i);
    //gr[i]->SetMarkerStyle(21);//full square
    //gr[i]->SetMarkerStyle(32);//open triangle down
    gr->SetMarkerStyle(i%3+2);
    gr->SetMarkerSize(1.4);
    gr->SetTitle(GTitles[i]);
    leg->AddEntry(gr,gr->GetTitle(), "p");

    mg->Add(gr,"P");
    gr=NULL;
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


void PlotMany(TString list_file_name) {
  TString  multicanvas_name,multicanvas_title;
  UInt_t c_rows;
  UInt_t c_columns;
  vector<TString> file_list;
  TString file_name;
  ifstream list_file(list_file_name);
  if(!list_file)
    {
      std::cerr << "ERROR: file with values :"<<list_file<<" not found trying to read the default file\n";
      return;
    }
  else
    std::cout<<" file with values : "<<list_file_name<<" is open \n";

  while(list_file){
    file_name.ReadLine(list_file);

    if(file_name(0,1)=="#"){  //this is a line of commentary
      std::cout<<"Comment :"<<file_name<<std::endl;
      continue;
    }else if(file_name=="END"){//end of file list
      multicanvas_name.ReadLine(list_file);
      multicanvas_title.ReadLine(list_file);
      file_name.ReadLine(list_file);
      c_rows=file_name.Atoi();
      c_columns=file_name.Atoi();	
      break;
    }else{
      file_list.push_back(file_name);
    }

  }
  

  TCanvas *multi_canvas = new TCanvas(Form("C_%s",multicanvas_name.Data()),Form("Canvas-%s",multicanvas_title.Data()),1);
  /*
    Create a new canvas with a predefined size form.
  If form < 0  the menubar is not shown.

  form = 1    700x500 at 10,10 (set by TStyle::SetCanvasDefH,W,X,Y)
  form = 2    500x500 at 20,20
  form = 3    500x500 at 30,30
  form = 4    500x500 at 40,40
  form = 5    500x500 at 50,50
   */

  vector<TMultiGraph *> mg_list;


  multi_canvas->Divide(c_columns,c_rows);
  multi_canvas->SetGrid();
  multi_canvas->Modified();
  for(UInt_t i=0;i<c_columns*c_rows;i++){
    if (i<file_list.size()){
      printf("file name = %s \n",file_list[i].Data());
      multi_canvas->cd(i+1);    
      PlotData(file_list[i]);
    } else
      printf("No more plots available \n");
  }
}


void PlotData(TString data_file){
  DecodeDataFile(data_file);
 
  //TCanvas *c1 = new TCanvas(Form("C_%s",MultiGraphName.Data()),Form("Canvas-%s",CanvasName.Data()),200,10,700,500);
  //c1->cd();
  //c1->SetFillColor(42);
  //c1->SetGrid();

  //to convert the double vectors to arrays.
  Double_t *dd_x=new Double_t[X.size()];
  Double_t *dd_y=new Double_t[X.size()];
  copy(X.begin(),X.end(),dd_x);

  //copy errors
  Double_t *dd_ex=NULL;
  Double_t *dd_ey=NULL;

  if(hasXErrors){
    dd_ex=new Double_t[X.size()];
    copy(eX.begin(),eX.end(),dd_ex);
  }

  TGraphErrors* gr=NULL;

  TLegend *leg = new TLegend(0.1,0.7,0.28,0.8);
  TMultiGraph *mg = new TMultiGraph(MultiGraphName,multigraphtitle);

  leg->SetHeader(LegHeader);
  leg->SetFillColor(41);
  for (Int_t i=0;i<ny;i++){
    copy(Y[i].begin(),Y[i].end(),dd_y);
    if(hasYErrors){
      dd_ey=new Double_t[X.size()];
      copy(eY[i].begin(),eY[i].end(),dd_ey);
    }
    gr= new TGraphErrors(n,dd_x,dd_y,dd_ex,dd_ey);
    gr->SetLineColor(1+i);
    gr->SetLineWidth(1);
    gr->SetMarkerColor(2+i);
    //gr[i]->SetMarkerStyle(21);//full square
    //gr[i]->SetMarkerStyle(32);//open triangle down
    gr->SetMarkerStyle(i%3+2);
    gr->SetMarkerSize(1.4);
    gr->SetTitle(GTitles[i]);
    leg->AddEntry(gr,gr->GetTitle(), "p");

    mg->Add(gr,"P");
    gr=NULL;
  }


  
  mg->Draw("a");
  mg->GetXaxis()->SetTitle(xTitle);
  mg->GetYaxis()->SetTitle(yTitle);

  leg->Draw();
  
  //c1->SetFillColor(19);
  //c1->Update();
  //c1->GetFrame()->SetFillColor(19);
  //c1->GetFrame()->SetBorderSize(12);
  //c1->Modified();  
};

