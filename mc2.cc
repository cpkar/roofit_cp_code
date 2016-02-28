#include <TLatex.h>
#include <TString.h>

void mc2(){

  gSystem->Load("libRooFit");
  using namespace RooFit;
  TChain *ch = new TChain("tree");
  ch->Add("sel_BToKstarMuMu-8TeV-merged-cut3_mc.lite_s0.root");
  TTree *tr = ch;
  int nentries_ = tr->GetEntries();
  cout << "\n=> total entries in the 8TeV data tree = " << nentries_<< endl;

  RooRealVar  Bmass("Bmass","M(B^{+}) [GeV/c^{2}]", 5.0, 5.56);
  RooRealVar Mumumass("Mumumass","M^{#mu#mu}", 0., 10.);
  RooRealVar Mumumasserr("Mumumasserr","Error of M^{#mu#mu}", 0., 10.);
  RooRealVar Q2("Q2","dimuon inv mass squared", 0.5, 20.);

  RooDataSet data("data","dataset with Bmass", ch, RooArgSet(Q2, Bmass, Mumumass, Mumumasserr));



  RooRealVar mean("mean","mean of the gaussians",5.27,5.0,5.56);
//RooRealVar  mean("mean","mean of the gaussians", 5., 5.0, 5.56);
  RooRealVar  sigma1("sigma1","sigma of the gaussian1",0.7,0.,1.);
  RooRealVar  sigma2("sigma2","sigma of the gaussian2", 0.8, 0.,1.);
  RooRealVar sigM_frac("sigM_frac","fraction of Gaussians",0.8, 0., 1.);
//  RooRealVar sigM_frac1("sigM_frac1","fraction of Gaussians",0.7,0.,1.);
  RooGaussian  sigG2("sigG","signal component", Bmass, mean, sigma2);
  RooGaussian sigG1("sigG1","signal component",Bmass,mean,sigma1);


  //RooRealVar a0("a0","a0",0.5,0.,1.) ;
  //RooRealVar a1("a1","a1",-0.2,0.,1.) ;
  //RooChebychev bkg("bkg","Background",Bmass,RooArgSet(a0,a1)) ;

  //RooRealVar meanBkgG3("meanBkgG3","mean of 3rd Gaussian", 5.47, 5.40, 5.56);
  //RooRealVar sigmaBkgG3("sigmaBkgG3","sigma of 3rd Gaussian", 0.06, 0.01, 0.1);
// RooGaussian bkgGau3("bkgGau3","bkg gaussian3 component", Bmass, meanBkgG3, sigmaBkgG3);


  RooRealVar nsigG("nsigG","signal events", 150, 0, 300);
  RooRealVar nbkg1("nbkg1","number of gaussian1 bkg events", 100, 0, 500);
  RooRealVar nbkgG2("nbkgG2","number of gaussian2 bkg events", 100, 0, 500); 

  RooAddPdf sig("sig","Double Gaussian",RooArgList(sigG1,sigG2),RooArgList(sigM_frac));
  //RooRealVar bkgfrac("bkgfrac","fraction of background",0.5,0.,1.) ;

 // RooAddPdf model("model","g1+g2+a",RooArgList(sum1,bkg),bkgfrac);


  sig.Print("t");

  RooFitResult* fitres=sig.fitTo(data,Save(true));
  RooPlot *xframe = Bmass.frame(Title("B mass"),Bins(20));
  data.plotOn(xframe,LineColor(kRed));
  sig.plotOn(xframe);

  sig.plotOn(xframe, Components("sig"), LineStyle(kDashed), LineColor(kRed) );

  

  TCanvas* c=new TCanvas("c","c",600,600);
  gPad->SetLeftMargin(0.15);
  xframe->GetYaxis()->SetTitleOffset(1.7);
  xframe->Draw();
  
  TPaveText* paveText = new TPaveText(0.62,0.70,0.80,0.88,"NDC");
  paveText->SetBorderSize(0.0);
  paveText->SetFillColor(kWhite);
  paveText->SetFillStyle(0);
  paveText->SetTextSize(0.02);

  paveText->AddText(Form("nsigG = %.0f #pm %.0f evts", nsigG.getVal(), nsigG.getError()));
 // paveText->AddText(Form("nbkg1 = %.0f #pm %.0f evts", nbkg1.getVal(), nbkg1.getError()));
 //  paveText->AddText(Form("nbkgG2 = %.0f #pm %.0f evts", nbkgG2.getVal(), nbkgG2.getError()));
 paveText->AddText(Form("sigma1= %.3f #pm %.6f Gev",sigma1.getVal(),sigma1.getError()));
  paveText->AddText(Form("sigma2= %.3f #pm %.3f Gev",sigma2.getVal(),sigma2.getError()));
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
  TString LumiName3 = " MuMu(8TeV)" ;


  LumiTex1->DrawLatex(0.18,0.9,LumiName1);
  LumiTex2->DrawLatex(0.3,0.9,LumiName2);
  LumiTex3->DrawLatex(0.6,0.9,LumiName3);





  c->Update();
  c->SaveAs("MC fit of MuMu.pdf");
}



