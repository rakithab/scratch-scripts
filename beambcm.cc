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

#define TRUE 1
#define FALSE 0

Int_t c1;
Int_t c2;
Int_t i = 0;
TString cut;



Int_t GetTree(TString filename, TChain* chain);

void beamplot(Int_t run_number)
{
  TChain chain("Hel_Tree");
  // new path:
  //  TString directory =  "/net/cdaq/cdaql5data/qweak/rootfiles/";
  // old path: Qweak_*.*.root are still created here.
  TString directory =  "/net/cdaq/cdaql8data/qweak/rootfiles/";
  TString filename;
  


  Int_t chain_status = 0;
  filename = Form("QwPass1_%d*.root", run_number);//rakitha - 10-28-2010 (rakithab)
  chain_status = GetTree(filename, &chain);//rakitha - 10-28-2010 (rakithab)

  if(chain_status == 0) {
    return;
  }
 
  TCanvas *C1 = new TCanvas("c1", "BCM vs MPS_COUNTER", 800, 600);
    
   C1->cd();
     
   chain.Draw("yield_qwk_bcm1:mps_counter");
   //bcm_mps->SetTitle("bcm vs mps_counter (um)");
   
  std::cout << "*******************************************************" << std::endl; 
  std::cout << "In order to add cut to remaining plots "  << std::endl;
  std::cout << "double-click the bcm vs mps counter pad." << std::endl;
  std::cout << "*******************************************************" << std::endl;    
   C1->WaitPrimitive();

   std::cout << "Enter your event range cuts(format:: lower upper)" << std::endl;
   std::cin >> c1 >> c2; 
 
   cut = Form("mps_counter>%d && mps_counter<%d",c1,c2);

   C1->cd(); 
   chain.Draw("yield_qwk_bcm1:mps_counter",cut);
   

   TCanvas *C2 = new TCanvas("c2", "BPM vs ASYMMETRY ", 800, 600);

   C2->Divide(3,1);

   gStyle->SetOptStat(0);
   gStyle->SetOptFit();

   C2->cd(1);
   C2->SetLogy();
   
   chain.Draw("(diff_qwk_bpm3h09bX*1.0e3)>>bpmx", cut);
   bpmx->Fit("gaus");
   bpmx->SetTitle("3H09bX pos. diff. (um)");

   C2->cd(2);
   C2->SetLogy();  

   chain.Draw("(diff_qwk_bpm3h09bY*1.0e3)>>bpmy",cut);
   bpmy->Fit("gaus");
   bpmy->SetTitle("3H09bY pos. diff. (um)");

   C2->cd(3);
   C2->SetLogy();
   
   chain.Draw("(asym_qwk_bcm1*1.0e6)>>bcm1",cut);
   bcm1->Fit("gaus");
   bcm1->SetTitle("BCM1 charge asym. (ppm)");


   TCanvas *c3 = new TCanvas("c3", "BPM vs ASYMMETRY ", 800, 600);

   c3->Divide(3,1);

   gStyle->SetOptStat(0);
   gStyle->SetOptFit();

   c3->cd(1);
   c3->SetLogy();
   
   chain.Draw("(diff_qwk_bpm3h07cX*1.0e3)>>bpmx1", cut);
   bpmx1->Fit("gaus");
   bpmx1->SetTitle("3H07cX pos. diff. (um)");

   c3->cd(2);
   c3->SetLogy();  

   chain.Draw("(diff_qwk_bpm3h07cY*1.0e3)>>bpmy1",cut);
   bpmy1->Fit("gaus");
   bpmy1->SetTitle("3H07cY pos. diff. (um)");

   c3->cd(3);
   c3->SetLogy();
   
   chain.Draw("(asym_qwk_bcm1*1.0e6)>>bcm1_1",cut);
   bcm1_1->Fit("gaus");
   bcm1_1->SetTitle("BCM1 charge asym. (ppm)");
 
   //  C1->Close();
   //  C2->Close();
   return;

}

void beamPlotMPS(Int_t run_number)//rakitha - 10-28-2010 (rakithab)
{
  TChain chain("Mps_Tree");
  // new path:
  //  TString directory =  "/net/cdaq/cdaql5data/qweak/rootfiles/";
  // old path: Qweak_*.*.root are still created here.
  TString directory =  "/net/cdaq/cdaql8data/qweak/rootfiles/";
  TString filename;
  


  Int_t chain_status = 0;
  filename = Form("QwPass1_%d*.root", run_number);//rakitha - 10-28-2010 (rakithab)
  chain_status = GetTree(filename, &chain);//rakitha - 10-28-2010 (rakithab)

  if(chain_status == 0) {
    return;
  }
 
  TCanvas *C1 = new TCanvas("c1", "BCM vs MPS_COUNTER", 800, 600);
    
   C1->cd();
     
   chain.Draw("qwk_bcm1:CodaEventNumber");
   //bcm_mps->SetTitle("bcm vs mps_counter (um)");
   
   //std::cout << "*******************************************************" << std::endl; 
   //std::cout << "In order to add cut to remaining plots "  << std::endl;
   //std::cout << "double-click the bcm vs mps counter pad." << std::endl;
   //std::cout << "*******************************************************" << std::endl;    
  //C1->WaitPrimitive();
  C1->Update();//rakitha - 10-28-2010 (rakithab)

   std::cout << "Enter your event range cuts(format:: lower upper)" << std::endl;
   std::cin >> c1 >> c2; 
 
   cut = Form("CodaEventNumber>%d && CodaEventNumber<%d",c1,c2);

   C1->cd(); 
   chain.Draw("qwk_bcm1:CodaEventNumber",cut);
   

   TCanvas *C2 = new TCanvas("c2", "BPM 3H09b POSITIONS ", 800, 600);

   C2->Divide(3,1);

   gStyle->SetOptStat(0);
   gStyle->SetOptFit();

   C2->cd(1);
   C2->SetLogy();
   
   chain.Draw("(qwk_bpm3h09bX*1.0e3)>>bpmx", cut);
   bpmx->Fit("gaus");
   bpmx->SetTitle("3H09bX pos. (um)");

   C2->cd(2);
   C2->SetLogy();  

   chain.Draw("(qwk_bpm3h09bY*1.0e3)>>bpmy",cut);
   bpmy->Fit("gaus");
   bpmy->SetTitle("3H09bY pos. . (um)");

   C2->cd(3);
   C2->SetLogy();
   
   chain.Draw("(qwk_bcm1)>>bcm1",cut);
   bcm1->Fit("gaus");
   bcm1->SetTitle("BCM1 current (uA)");


   TCanvas *c3 = new TCanvas("c3", "BPM 3H07c POSITIONS ", 800, 600);

   c3->Divide(3,1);

   gStyle->SetOptStat(0);
   gStyle->SetOptFit();

   c3->cd(1);
   c3->SetLogy();
   
   chain.Draw("(qwk_bpm3h07cX*1.0e3)>>bpmx1", cut);
   bpmx1->Fit("gaus");
   bpmx1->SetTitle("3H07cX pos. (um)");

   c3->cd(2);
   c3->SetLogy();  

   chain.Draw("(qwk_bpm3h07cY*1.0e3)>>bpmy1",cut);
   bpmy1->Fit("gaus");
   bpmy1->SetTitle("3H07cY pos. (um)");

   c3->cd(3);
   c3->SetLogy();
   
   chain.Draw("(qwk_bcm1*1.0e6)>>bcm1_1",cut);
   bcm1_1->Fit("gaus");
   bcm1_1->SetTitle("BCM1 current (uA)");
 
   //  C1->Close();
   //  C2->Close();
   return;

}



TH1D* GetHisto(TTree *tree, const TString name, const TCut cut, Option_t* option = ""){ //rakitha - 10-28-2010 (rakithab)
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
  file_dir = "/home/cdaq/qweak/QwScratch";
  file_dir += "/rootfiles/";
 
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

