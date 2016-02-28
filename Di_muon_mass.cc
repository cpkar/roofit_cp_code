#include <TLatex.h>
#include <TString.h>

void Di_muon_mass(){

  gSystem->Load("libRooFit");
  using namespace RooFit;

  TChain *ch = new TChain("tree");
  ch->Add("sel_BToKstarPsi2S-8TeV-merged-AllCuts.root");
  TTree *tr = ch;
      
  int nentries_ = tr->GetEntries();
  cout << "\n=> total entries in the 8TeV data tree = " << nentries_ << endl;
    

  RooRealVar Mumumass("Mumumass","M^{#mu#mu}", 3.3, 4.1);
//  RooRealVar Mumumasserr("Mumumasserr","Error of M^{#mu#mu}", 0., 10.);
 // RooRealVar Q2("Q2","dimuon inv mass squared", 0.5, 20.);
    
  RooDataSet data("data","dataset with Bmass", ch, RooArgSet( Mumumass));

 TCut cut = "(Mumumass > 0)";


 RooDataSet *redData = (RooDataSet*)data.reduce(cut);
   RooRealVar mean("mean","mean of the gaussians",3.68,3.3,4.1);
///////RooRealVar  mean("mean","mean of the gaussians", 5., 5.0, 5.56)

   RooRealVar  sigma1("sigma1","sigma of the gaussian",0.5,0.0,1.0);
   RooRealVar  sigma2("sigma2","sigma of the gaussian", 0.4,0., 1.);
   RooRealVar sig_frac("sig_frac","sigma frac",0.1,0.,1.);
   RooGaussian  sigG1("sigG1","signal component",Mumumass,mean,sigma1);
   RooGaussian sigG2("sigG2","signal",Mumumass,mean,sigma2);
 
  RooAddPdf sig("sig","signal Gaussian",RooArgList(sigG1,sigG2),sig_frac);
  sig.Print("t");

 RooFitResult* fitres=sig.fitTo(*redData,Save(true));


   cout << "\n====================================" << endl;
//   cout << "covariance quality: " << fitres->covQual() << endl;
 //  cout << "Fit Status : " << fitres->status() << endl;
  // cout << "Fit EDM : " << fitres->edm() << endl;
//   cout << "FCN : " << sig.getVal() << endl;
  //cout << "Chi2/DoF fit: " << xframe->chiSquare((*model)->getPlotLabel(),redData->GetName());                                                                        
   cout << "========================================" << endl;




  RooPlot *xframe = Mumumass.frame(Title("Mumumass"),Bins(18));
  redData->plotOn(xframe,LineColor(kRed));
  sig.plotOn(xframe);
  sig.plotOn(xframe, Components("sig"), LineStyle(kDashed), LineColor(kRed) );
 // model.plotOn(xframe, Components("bkg"), LineStyle(kDotted), LineColor(kRed) );


  TCanvas* c=new TCanvas("c","c",600,600);
  gPad->SetLeftMargin(0.15);
  xframe->GetYaxis()->SetTitleOffset(1.7);
  xframe->Draw();



  TPaveText* paveText = new TPaveText(0.62,0.70,0.80,0.88,"NDC");
  paveText->SetBorderSize(0.0);
  paveText->SetFillColor(kWhite);
  paveText->SetFillStyle(0);
  paveText->SetTextSize(0.02);
  paveText->AddText(Form("sigma1 = %.3f #pm %.3f Gev", sigma1.getVal(), sigma1.getError()));
  paveText->AddText(Form("sigma2= %.3f #pm %.3f Gev", sigma2.getVal(), sigma2.getError()));
  paveText->AddText(Form("mean = %.6f #pm %.6f Gev", mean.getVal(), mean.getError()));
  paveText->Draw();


   TLatex *LumiTex1 = new TLatex();
   LumiTex1->SetTextColor(kBlack);
   LumiTex1->SetNDC(true);

   TLatex *LumiTex2 = new TLatex();
   LumiTex2->SetTextColor(kBlack);
   LumiTex2->SetNDC(true);
   TLatex *LumiTex3 = new TLatex();
  //LumiTex3->SetTextFont(61);
  //LumiTex3->SetTextSize(0.05);
  LumiTex3->SetTextColor(kBlack);
  LumiTex3->SetNDC(true);
  TString LumiName1 = "CMS" ;
  TString LumiName2 = "#it{simulation}" ;
  TString LumiName3 = " Mumumass(8TeV)" ;


  LumiTex1->DrawLatex(0.18,0.9,LumiName1);
  LumiTex2->DrawLatex(0.3,0.9,LumiName2);
  LumiTex3->DrawLatex(0.6,0.9,LumiName3);
  c->Update();
  c->SaveAs("Di_muon_mass.pdf");


}

