void graph() {
   //Draw a simple graph
   // To see the output of this macro, click begin_html <a href="gif/graph.gif">here</a>. end_html
   //Author: Rene Brun
   
   //TCanvas *c1 = new TCanvas("c1","Real Time vs. Events Processed by QwAnalysis_BeamLine",200,10,700,500);
  TCanvas *c1 = new TCanvas("c1","Memory usage vs. Events for RT",200,10,700,500);

   c1->SetFillColor(42);
   c1->SetGrid();


   const Int_t n = 18;
   Double_t x[n], y[n];
   Double_t x1[n-1], y1[n-1];



   
   
   //For without trees events in k
   x[0]=100;
   x[1]=290;
   x[2]=404;
   x[3]=539;
   x[4]=736;
   x[5]=846;
   x[6]=898;
   x[7]=1000;
   x[8]=1250;
   x[9]=1897;
   x[10]=2000;
   x[11]=2250;
   x[12]=2500;
   x[13]=2700;
   x[14]=3000;
   x[15]=3250;
   x[16]=3400;
   x[17]=3500;
   //x[18]=;

   y[0]=2.9;
   y[1]=3.3;
   y[2]=3.4;
   y[3]=3.7;
   y[4]=4.1;
   y[5]=4.2;
   y[6]=4.3;
   y[7]=4.5;
   y[8]=4.8;
   y[9]=5.8;
   y[10]=6.0;
   y[11]=6.4;
   y[12]=6.7;
   y[13]=7.0;
   y[14]=7.4;
   y[15]=7.9;
   y[16]=8.1;
   y[17]=8.2;
   //y[18]=;
   
    //For with trees events in k
   x1[0]=63;
   x1[1]=100;
   x1[2]=150;
   x1[3]=200;
   x1[4]=301;
   x1[5]=477;
   x1[6]=500;
   x1[7]=650;
   x1[8]=725;
   x1[9]=800;
   x1[10]=1000;
   x1[11]=1104;
   x1[12]=1145;
   x1[13]=1250;
   x1[14]=1500;
   x1[15]=1700;
   x1[16]=2000;
   //x1[17]=3500;
   //x1[18]=;


   y1[0]=3.7;
   y1[1]=4.2;
   y1[2]=4.7;
   y1[3]=5.7;
   y1[4]=6.1;
   y1[5]=8.0;
   y1[6]=8.2;
   y1[7]=10.0;
   y1[8]=10.3;
   y1[9]=13.1;
   y1[10]=13.8;
   y1[11]=14.2;
   y1[12]=14.3;
   y1[13]=17.3;
   y1[14]=18.0;
   y1[15]=18.7;
   y1[16]=25.0;
   //y1[17]=8.2;
   //y1[18]=;


   
   //-----------------------------------------------


   
   ///
   TGraph * gr1,*gr2,*gr3;
   gr1 = new TGraph(n,x,y);
   gr1->SetLineColor(2);
   gr1->SetLineWidth(4);
   gr1->SetMarkerColor(4);
   gr1->SetMarkerStyle(21);
   gr1->SetTitle("qwparity --online --enable-mapfile --disable-histos");
   gr1->GetXaxis()->SetTitle("Events Input (K)");
   gr1->GetYaxis()->SetTitle("Memory %");
   //gr1->Draw("ACP");
   gr2 = new TGraph(n-1,x1,y1);
   gr2->SetLineColor(2);
   gr2->SetLineWidth(4);
   gr2->SetMarkerColor(3);
   gr2->SetMarkerStyle(21);
   gr2->SetTitle("qwparity --online --enable-mapfile --disable-histos");
   gr2->GetXaxis()->SetTitle("Events Input (K)");
   gr2->GetYaxis()->SetTitle("Memory %");
   TMultiGraph *mg = new TMultiGraph();

   //mg->SetLineColor(2);
   //mg->SetLineWidth(4);
   //mg->SetMarkerColor(4);
   //mg->SetMarkerStyle(21);
   //mg->SetTitle("qwparity --online --enable-mapfile --disable-histos");
   //mg->GetXaxis()->SetTitle("Events Input (K)");
   //mg->GetYaxis()->SetTitle("Memory %");
   //mg->Add(gr1,"ACP");
   mg->Add(gr2,"p");
   mg->Draw("a");
   // mg->Add(gr,"ACP");
   // TCanvas::Update() draws the frame, after which one can change it
   c1->Update();
   c1->GetFrame()->SetFillColor(21);
   //c1->GetFrame()->SetBorderSize(12);
   c1->Modified();
}
