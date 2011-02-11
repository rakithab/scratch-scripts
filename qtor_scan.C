#include <iostream>
#include <string>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TMath.h"
#include <TString.h>
#include <TChain.h>
#include <TChainElement.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TString.h>


UInt_t LL;
UInt_t UL;
TString cut;
TChain * tree_chain;

const Int_t n = 37;
const Int_t ny = 2;
Double_t dCuts[ny][n];
std::vector <TString> results;//to store output to be sent to a text file
//gRoot->ProcessLine(.L PlotTools.C);

Int_t  GetTree(TString filename, TChain* chain);
TH1D* GetHisto(TTree *tree, const TString name, const TCut cut, Option_t* option="");
void LoadTree(Int_t run_num,TString treename="event_tree");
vector<Double_t> Tokenize(const string& line, const char& delim);
void FillDataArrays(Int_t x,vector<Double_t> &raw_data);
void LoadCuts(TString data_file);
void TrimWhiteSpaces(string &line);
void DrawWithCuts(Int_t run_number);

void PlotScalers(Int_t run_number){
  Double_t ts1,ts2,ts1_err,ts2_err,tsmt,tsmt_err;
  Double_t md,md_err;
  Double_t md2tsratio,md2tsratio_err;
  TChain chain("event_tree");
  // new path:
  TString directory =  "/home/rakithab/scratch/rootfiles/";
  TString filename;
  
  TH1D* tmp;


  Int_t chain_status = 0;
  filename = Form("Qweak_%d*.root", run_number);
  chain_status = GetTree(filename, &chain);

  if(chain_status == 0) {
    return;
  }

  std::cout << "Enter your event range cuts(format:: lower upper)" << std::endl;
  std::cin >> LL >> UL; 
  LL*=1000;
  UL*=1000;
  cut = Form("CodaEventNumber>%d && CodaEventNumber<%d && CodaEventType==1 && trigscint.ts1mt_sca>0",LL,UL);

  //trigscint
  TCanvas *C1 = new TCanvas("c1", "TrigScint Scaler Rates", 800, 600);
  C1->Divide(3,1);
  C1->cd(1);
  chain.Draw("trigscint.ts1mt_sca",cut);
  tmp = (TH1D*)  gPad -> GetPrimitive("htemp");
  ts1=tmp->GetMean()/1000;
  ts1_err=tmp->GetMeanError()/1000;
  printf("trigscint.ts1mt_sca %5.3fKHz +/- %5.3fKHz \n", tmp->GetMean()/1000,tmp->GetMeanError()/1000);
  tmp=NULL;
  C1->cd(2);
  cut = Form("CodaEventNumber>%d && CodaEventNumber<%d && CodaEventType==1 && trigscint.ts2mt_sca>0",LL,UL);
  chain.Draw("trigscint.ts2mt_sca",cut);
  tmp = (TH1D*)  gPad -> GetPrimitive("htemp");
  ts2=tmp->GetMean()/1000;
  ts2_err=tmp->GetMeanError()/1000;
  printf("trigscint.ts2mt_sca %5.3fKHz +/- %5.3fKHz \n", tmp->GetMean()/1000,tmp->GetMeanError()/1000);
  tmp=NULL;

  cut = Form("CodaEventNumber>%d && CodaEventNumber<%d && CodaEventType==1 && trigscint.ts1mt_sca>0 && trigscint.ts2mt_sca>0",LL,UL);
  C1->cd(3);
  chain.Draw("trigscint.ts2mt_sca+trigscint.ts1mt_sca",cut);  
  tmp = (TH1D*)  gPad -> GetPrimitive("htemp");
  tsmt=tmp->GetMean()/1000;
  tsmt_err=tmp->GetMeanError()/1000;
  printf("trigscint.tsmt_sca %5.3fKHz +/- %5.3fKHz \n", tmp->GetMean()/1000,tmp->GetMeanError()/1000);
  tmp=NULL;

  //MD
  cut = Form("CodaEventNumber>%d && CodaEventNumber<%d && CodaEventType==1",LL,UL);

  TCanvas *C2 = new TCanvas("c2", "MD 6/7 Scaler Rates", 800, 600);
  C2->Divide(2,1);
  C2->cd(1);
  chain.Draw("(maindet.md6m_sca + maindet.md6p_sca)",cut);
  tmp = (TH1D*)  gPad -> GetPrimitive("htemp");
  md=tmp->GetMean()/1000;
  md_err=tmp->GetMeanError()/1000;
  printf("MD 6 %5.3fKHz +/- %5.3fKHz \n", tmp->GetMean()/1000,tmp->GetMeanError()/1000);
  //chain.Draw("maindet.md1m_sca + maindet.md2m_sca + maindet.md3m_sca + maindet.md4m_sca + maindet.md5m_sca + maindet.md6m_sca + maindet.md7m_sca + maindet.md8m_sca + maindet.md1p_sca + maindet.md2p_sca + maindet.md3p_sca + maindet.md4p_sca + maindet.md5p_sca + maindet.md6p_sca + maindet.md7p_sca + maindet.md8p_sca",cut);
  C2->cd(2);
  cut = Form("CodaEventNumber>%d && CodaEventNumber<%d && CodaEventType==1 && maindet.md6m_sca>0 && maindet.md6p_sca>0 && trigscint.ts1mt_sca>0 && trigscint.ts2mt_sca>0",LL,UL);
  chain.Draw("(maindet.md6m_sca + maindet.md6p_sca)/(trigscint.ts2mt_sca+trigscint.ts1mt_sca)",cut);
  tmp = (TH1D*)  gPad -> GetPrimitive("htemp");
  md2tsratio=tmp->GetMean();
  md2tsratio_err=tmp->GetMeanError();
  printf("MD/TS %5.3f +/- %5.3f \n", tmp->GetMean(),tmp->GetMeanError());
  tmp=NULL;
  printf("%5.3f \t %5.3f \t  %5.3f \t %5.3f \t %5.3f \t %5.3f \t %5.3f \t %5.3f \t %5.3f \t %5.3f \t\n",ts1, ts1_err,ts2, ts2_err,tsmt, tsmt_err,md,md_err,md2tsratio,md2tsratio_err);
  
  
}


TH1D* GetHisto(TTree *tree, const TString name, const TCut cut, Option_t* option){ //rakitha - 10-28-2010 (rakithab)
  tree ->Draw(name, cut, option);
  TH1D* tmp;
  tmp = (TH1D*)  gPad -> GetPrimitive("htemp");
  if(!tmp) {
    return 0;
  }
  return tmp;
}

Int_t GetTree(TString filename, TChain* chain){ //rakitha - 10-28-2010 (rakithab)
  TString file_dir;
  Int_t   chain_status = 0;

  //file_dir = gSystem->Getenv("QWSCRATCH");
  //if (!file_dir) 
  file_dir = "/home/rakithab/scratch/rootfiles/";

 
  chain_status = chain->Add(Form("%s%s", file_dir.Data(), filename.Data()));
  
  if(chain_status) {
    
    TString chain_name = chain -> GetName();
    TObjArray *fileElements = chain->GetListOfFiles();
    TIter next(fileElements);
    TChainElement *chain_element = NULL;
    while (( chain_element=(TChainElement*)next() )) 
      {
	std::cout << "File:  " 
		  << chain_element->GetTitle() 
		  << " is added into the Chain with the name ("
		  << chain_name
		  << ")."
		  << std::endl;
      }
  }
  else {
    std::cout << "There is (are) no "
	      << filename 
	      << "."
	      << std::endl;
  }
  return chain_status;
};

void LoadTree(Int_t run_num,TString treename ){
  TString filename;
  tree_chain=new TChain(treename);
  Int_t chain_status = 0;
  filename = Form("Qweak_%d*.root", run_num);
  chain_status = GetTree(filename, tree_chain);

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

void FillDataArrays(Int_t x,vector<Double_t> &raw_data){
  Int_t count=0;
  while(count<ny){
     dCuts[count][x]=raw_data.at(count);//y1 value
     count++;
  }

}

void LoadCuts(TString data_file){
  Int_t x,y;
  vector<Double_t> raw_data;//data for one raw
  TString line;

  std::ifstream inputdata(data_file);
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
    line.ReadLine(inputdata);

    if(line(0,1)=="#"){  //this is a line of commentary
      std::cout<<"Comment :"<<line<<std::endl;
      continue;
    }else if (line=="END"){
      /*
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
      */
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

void DrawWithCuts(Int_t run_number){
  Double_t tr_counts;
  Double_t factor=1000;//to converts to KHz
  TString scut[n];
  TChain chain("event_tree");
  // new path:
  TString directory =  "/home/rakithab/scratch/rootfiles/";
  TString filename;
  
  TH1D* tmp;


  Int_t chain_status = 0;
  filename = Form("Qweak_%d*.root", run_number);
  chain_status = GetTree(filename, &chain);

  if(chain_status == 0) {
    return;
  }

  //results
  LoadCuts("qtor_scan_6394_event_range"); 

  TCanvas *C1_1 = new TCanvas("c1_1", "MD7/TS  Scaler Rates Ratio", 800, 600);
  //c1_1->cd();
  C1_1->Divide(8,7);
  //&& maindet.md7m_sca>100000 
  for(Int_t i=0;i<n;i++){
    scut[i] = Form("CodaEventNumber>%f && CodaEventNumber<%f && CodaEventType==1 && maindet.md7m_sca>100000 && maindet.md7p_sca>100000 && trigscint.ts1mt_sca>0 && trigscint.ts2mt_sca>0",dCuts[0][i]*1000,dCuts[1][i]*1000);     
    C1_1->cd(i+1);
    //c1_1->Update();
    //tr_counts=chain.Draw("(maindet.md6m_sca)",scut[i]);
    //tr_counts=chain.Draw("(maindet.md6m_sca)",scut[i]);
    tr_counts=chain.Draw("(maindet.md7m_sca + maindet.md7p_sca)/(trigscint.ts2mt_sca+trigscint.ts1mt_sca)",scut[i]);
    tmp = (TH1D*)  gPad -> GetPrimitive("htemp");
    factor=1; //for MD/TS ration
    //factor=1000; //for MD rates
    results.push_back(Form("%f \t",tmp->GetMean()/factor));
    results.push_back(Form("%f \t",tmp->GetMeanError()/factor));
    results.push_back(Form("%f \n",tmp->GetRMS()/factor));
    tmp=NULL;
    printf("%s %f \n",scut[i].Data(),tr_counts);
  }
  //c1_1->Update();
  //c1_1->Modified();
  //gPad->Update();
  for (UInt_t j=0;j<results.size();j++){
    printf("%s",results.at(j).Data());
  }
}



