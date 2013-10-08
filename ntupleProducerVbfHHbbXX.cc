#include <iostream>
#include <string>
// ROOT headers
#include "TROOT.h"
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TChain.h>
#include <TClonesArray.h>
// Delphes headers
#include "ExRootAnalysis/ExRootTreeReader.h"
#include "classes/DelphesClasses.h"
// Verbosity
#define DEBUG 0



#include <boost/program_options.hpp>



bool isThisJetALepton(TLorentzVector* jet, TLorentzVector* l1, TLorentzVector* l2){
 double DRmax = 0.2;
 bool isLep = false;
 if (l1) if (jet->DeltaR(*l1) < DRmax) isLep = true;
 if (l2) if (jet->DeltaR(*l2) < DRmax) isLep = true;
 return isLep;
}

struct myclassMin {
 bool operator() (std::pair<float, std::pair <int,Int_t> > i, std::pair<float, std::pair <int,Int_t> > j) { 
  return (i.first < j.first);
 }
} myObjMin;


struct myclassMax {
 bool operator() (std::pair<float, std::pair <int,Int_t> > i, std::pair<float, std::pair <int,Int_t> > j) { 
  return (i.first > j.first);
 }
} myObjMax;



namespace po = boost::program_options;


int main (int argc, char **argv) {
 
 
 // declare arguments
 std::string inputfile;
 std::string outputfile;
 std::string outputtree;
 // print out passed arguments
 std::copy(argv, argv + argc, std::ostream_iterator<char*>(std::cout, " "));
 // argument parsing
 try {
  po::options_description desc("Allowed options");
  desc.add_options()
  ("help,h", "produce help message")
  ("inputfile,i", po::value<std::string>(&inputfile)->default_value("../GluGluToHHTo2B2G_M-125_8TeV_madgraph_v2_DEL_v03.root"), "input file")
  ("outputfile,o", po::value<std::string>(&outputfile)->default_value("output.root"), "output file")
  ("outputtree,ot", po::value<std::string>(&outputtree)->default_value("GluGluToHHTo2B2G_8TeV"), "output tree")
  ;
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  if (vm.count("help")) {
   std::cout << desc << "\n";
   return 1;
  }
 } catch(std::exception& e) {
  std::cerr << "error: " << e.what() << "\n";
  return 1;
 } catch(...) {
  std::cerr << "Exception of unknown type!\n";
 }
 // end of argument parsing
 //################################################
 
 
 
 float HiggsMass = 125.;
 
 TChain *chain = new TChain("Delphes");
 chain->Add(inputfile.c_str());      
 ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
 TFile *outfile = new TFile(outputfile.c_str(), "RECREATE");
 TTree *outtree = new TTree(outputtree.c_str(), "reduced");
 
 float jetpt1;
 float jetpt2;
 float bjetpt1;
 float bjetpt2;

 float jeteta1;
 float jeteta2;
 float bjeteta1;
 float bjeteta2;
 
 float mjj;
 float mbb;
 
 //---- h>bb
 float hbb_pt;
 float hbb_eta;
 float hbb_phi;
 
 //---- h>WW
 float hww_pt;
 float hww_phi;
 float hww_etap; //---- ambiguity on the sign
 float hww_etam;
 
 float pt1;
 float pt2;
 float nlep;
 float channel;
 float mll;
 float dphill;
 
 float pfmet;
 
 //---- x>hh_m (ww)
 float xhh_m_ww_pt;
 float xhh_m_ww_eta;
 float xhh_m_ww_phi;
 float xhh_m_ww_m;
 float xhh_p_ww_pt;
 float xhh_p_ww_eta;
 float xhh_p_ww_phi;
 float xhh_p_ww_m;
 
 
 outtree->Branch("jetpt1",  &jetpt1,  "jetpt1/F");
 outtree->Branch("jetpt2",  &jetpt2,  "jetpt2/F");
 outtree->Branch("bjetpt1", &bjetpt1, "bjetpt1/F");
 outtree->Branch("bjetpt2", &bjetpt2, "bjetpt2/F");

 outtree->Branch("jeteta1",  &jeteta1,  "jeteta1/F");
 outtree->Branch("jeteta2",  &jeteta2,  "jeteta2/F");
 outtree->Branch("bjeteta1", &bjeteta1, "bjeteta1/F");
 outtree->Branch("bjeteta2", &bjeteta2, "bjeteta2/F");
 
 outtree->Branch("mjj", &mjj, "mjj/F");
 outtree->Branch("mbb", &mbb, "mbb/F");

 outtree->Branch("hbb_pt", &hbb_pt, "hbb_pt/F");
 outtree->Branch("hbb_eta", &hbb_eta, "hbb_eta/F");
 outtree->Branch("hbb_phi", &hbb_phi, "hbb_phi/F");
 
 outtree->Branch("hww_pt", &hww_pt, "hww_pt/F");
 outtree->Branch("hww_etap", &hww_etap, "hww_etap/F");
 outtree->Branch("hww_etam", &hww_etam, "hww_etam/F");
 outtree->Branch("hww_phi", &hww_phi, "hww_phi/F");

 outtree->Branch("pfmet", &pfmet, "pfmet/F");
 outtree->Branch("pt1", &pt1, "pt1/F");
 outtree->Branch("pt2", &pt2, "pt2/F");
 outtree->Branch("nlep", &nlep, "nlep/F");
 outtree->Branch("channel", &channel, "channel/F");
 outtree->Branch("mll", &mll, "mll/F");
 outtree->Branch("dphill", &dphill, "dphill/F");
  
 outtree->Branch("xhh_p_ww_pt",  &xhh_p_ww_pt,  "xhh_p_ww_pt/F");
 outtree->Branch("xhh_p_ww_eta", &xhh_p_ww_eta, "xhh_p_ww_eta/F");
 outtree->Branch("xhh_p_ww_phi", &xhh_p_ww_phi, "xhh_p_ww_phi/F");
 outtree->Branch("xhh_p_ww_m",   &xhh_p_ww_m,   "xhh_p_ww_m/F");
 
 outtree->Branch("xhh_m_ww_pt",  &xhh_m_ww_pt,  "xhh_m_ww_pt/F");
 outtree->Branch("xhh_m_ww_eta", &xhh_m_ww_eta, "xhh_m_ww_eta/F");
 outtree->Branch("xhh_m_ww_phi", &xhh_m_ww_phi, "xhh_m_ww_phi/F");
 outtree->Branch("xhh_m_ww_m",   &xhh_m_ww_m,   "xhh_m_ww_m/F");
 
 
 //---- objects in Delphes format 
 TClonesArray *branchParticle = treeReader->UseBranch("Particle");
 TClonesArray *branchElectron = treeReader->UseBranch("Electron");
 TClonesArray *branchMuon = treeReader->UseBranch("Muon");
 TClonesArray *branchPhoton = treeReader->UseBranch("Photon");
 
 TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
 TClonesArray *branchEFlowTower = treeReader->UseBranch("EFlowTower");
 TClonesArray *branchEFlowMuon = treeReader->UseBranch("EFlowMuon");
 TClonesArray *branchJet = treeReader->UseBranch("Jet");
 
 TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
 
 
 GenParticle *particle;
 Electron *electron;
 Photon *photon;
 Muon *muon;
 MissingET *met;
 
 Track *track;
 Tower *tower;
 
 Jet *jet; // P4 returns a TLorentzVector
 TObject *object;
 
 TLorentzVector momentum;
 
 Float_t Eem, Ehad;
 Bool_t skip;
 
 Long64_t entry;
 
 Int_t i, j, pdgCode;
 
 
 
 //---- events
 Long64_t allEntries = treeReader->GetEntries();
 std::cout << "** Chain contains " << allEntries << " events" << std::endl;
 
 
 
 // Loop over all events
 for(entry = 0; entry < allEntries; entry++) {
  // Load selected branches with data from specified event
  treeReader->ReadEntry(entry);
  
  /**
   * check for leptons (at most 2!): 
   * if there is a lepton, then remove the corresponding jet
  */
  
  ///---- take the two highest pt leptons in the event (m or e)
  //    maps are ordered in ascending order
  std::map <float, int> m_maxptleptons;
  
  // Loop over all electrons in event
  for(i = 0; i < branchElectron->GetEntriesFast(); i++) {
   electron = (Electron*) branchElectron->At(i);
   double pt = electron->PT;
   m_maxptleptons[-pt] = -(i+1);
  }
  
  // Loop over all muons in event
  for(i = 0; i < branchMuon->GetEntriesFast(); i++) {
   muon = (Muon*) branchMuon->At(i);
   double pt = muon->PT;
   m_maxptleptons[-pt] = (i+1);
  }
  
  //---- at least 2 leptons ----
  if (m_maxptleptons.size() < 2) continue;
  
  // kind = 0/1 if m/e
  
  std::map<float, int>::iterator it_type_m_lepton = m_maxptleptons.begin();
  int flav1 = (it_type_m_lepton->second<0);  // m>0, e<0 ---> m=0, e=1
  
  it_type_m_lepton++;
  int flav2 = (it_type_m_lepton->second<0);  // m>0, e<0 ---> m=0, e=1
  
  nlep = 0;
  for(it_type_m_lepton = m_maxptleptons.begin(); it_type_m_lepton != m_maxptleptons.end(); it_type_m_lepton++) {
   if ( -(it_type_m_lepton->first) > 10) nlep++;
  }
   
  TLorentzVector l1, l2;
  it_type_m_lepton = m_maxptleptons.begin();
  
  if (nlep >= 1) {
   if (it_type_m_lepton->second>0) { l1 = ((Muon*)         branchMuon->At(  it_type_m_lepton->second - 1 ))->P4();}
   else                            { l1 = ((Electron*) branchElectron->At(-(it_type_m_lepton->second + 1)))->P4();}
   
   if (nlep >= 2) {
    it_type_m_lepton++;
    if (it_type_m_lepton->second>0) { l2 = ((Muon*)         branchMuon->At(  it_type_m_lepton->second - 1 ))->P4();}
    else                            { l2 = ((Electron*) branchElectron->At(-(it_type_m_lepton->second + 1)))->P4();}
   }
  }
  
  
  /**
   * Loop over all jets in event:
   * two highest mjj are VBF jets
   * the other two are "H>bb" jets
   * 
   * there must be at least 4 jets
   */
  
  //---- at least 4 jets with pt>MINPTJET GeV
  float MINPTJET = 15.;  
  int countJets = 0;
  for(i = 0; i < branchJet->GetEntriesFast(); i++) {
   jet = (Jet*) branchJet->At(i);
   TLorentzVector jetP4 = jet->P4();
   if (jet->PT > MINPTJET && !isThisJetALepton(&jetP4, &l1, &l2)) countJets++;
  }
  
  if (countJets < 4) {
   continue;
  }
  
  TLorentzVector jet1, jet2, jet3, jet4;
  TLorentzVector Jet1, Jet2, bJet1, bJet2; //---- the VBF jets and the H>bb jets
  int ijet = 0;
  for(i = 0; i < branchJet->GetEntriesFast(); i++) {
   jet = (Jet*) branchJet->At(i);
   TLorentzVector jetP4 = jet->P4();
   
   if (jet->PT > MINPTJET && !isThisJetALepton(&jetP4, &l1, &l2)) {
    if      (ijet == 0) {jet1 = jetP4; ijet++; }
    else if (ijet == 1) {jet2 = jetP4; ijet++; }
    else if (ijet == 2) {jet3 = jetP4; ijet++; }
    else if (ijet == 3) {jet4 = jetP4; ijet++; }
   }
  }
  
  
  if (jet4.Pt() < MINPTJET) {
   std::cout << "We have a problem; countJets = " << countJets << "; ijet = " << ijet << " and jet4.Pt() = " << jet4.Pt() << std::endl; 
  }
  
  float mjj12 = (jet1+jet2).M();
  float mjj13 = (jet1+jet3).M();
  float mjj14 = (jet1+jet4).M();
  float mjj23 = (jet2+jet3).M();
  float mjj24 = (jet2+jet4).M();
  float mjj34 = (jet3+jet4).M();
  
  std::map<float, int> m_maxmjj;
  m_maxmjj[-mjj12] = 1;
  m_maxmjj[-mjj13] = 2;
  m_maxmjj[-mjj14] = 3;
  m_maxmjj[-mjj23] = 4;
  m_maxmjj[-mjj24] = 5;
  m_maxmjj[-mjj34] = 6;
  
  
  std::map<float, int>::iterator it_type_m_maxmjj = m_maxmjj.begin();
    
  if (it_type_m_maxmjj->second == 1) { Jet1 = jet1; Jet2 = jet2; bJet1 = jet3; bJet2 = jet4; };
  if (it_type_m_maxmjj->second == 2) { Jet1 = jet1; Jet2 = jet3; bJet1 = jet2; bJet2 = jet4; };
  if (it_type_m_maxmjj->second == 3) { Jet1 = jet1; Jet2 = jet4; bJet1 = jet2; bJet2 = jet3; };
  if (it_type_m_maxmjj->second == 4) { Jet1 = jet2; Jet2 = jet3; bJet1 = jet1; bJet2 = jet4; };
  if (it_type_m_maxmjj->second == 5) { Jet1 = jet2; Jet2 = jet4; bJet1 = jet1; bJet2 = jet3; };
  if (it_type_m_maxmjj->second == 6) { Jet1 = jet3; Jet2 = jet4; bJet1 = jet1; bJet2 = jet2; };
  
  
  //---- sub-order in pt: jetpt1 > jetpt2
  if (Jet1.Pt() < Jet2.Pt()) {
   TLorentzVector tempjet = Jet1;
   Jet1 = Jet2;
   Jet2 = tempjet;
  }

  //---- sub-order in pt: bjetpt1 > bjetpt2
  if (bJet1.Pt() < bJet2.Pt()) {
   TLorentzVector tempjet = bJet1;
   bJet1 = bJet2;
   bJet2 = tempjet;
  }  
  
  
  //---- save information
  
  jetpt1 = Jet1.Pt();
  jetpt2 = Jet2.Pt();
  bjetpt1 = bJet1.Pt();
  bjetpt2 = bJet2.Pt();
  
  jeteta1 = Jet1.Eta();
  jeteta2 = Jet2.Eta();
  bjeteta1 = bJet1.Eta();
  bjeteta2 = bJet2.Eta();
  
  TLorentzVector hbb;
  hbb = bJet1 + bJet2;
  
  mjj = (Jet1 +  Jet2 ).M();
  mbb = (bJet1 + bJet2).M();
 
  //---- h>bb
  hbb_pt = (bJet1 +  bJet2 ).Pt();
  hbb_eta = (bJet1 +  bJet2 ).Eta();
  hbb_phi = (bJet1 +  bJet2 ).Phi();
 
  
  //-------------
  //---- met ----
  
  if(branchMissingET->GetEntriesFast() > 0) {
   met   = (MissingET*) branchMissingET->At(0);
   pfmet = met->MET;
  }
  else {
   pfmet = -99;
  }
  
  
  //-----------------
  //---- leptons ----
  
  xhh_m_ww_pt  = -99;
  xhh_m_ww_eta = -99;
  xhh_m_ww_phi = -99;
  xhh_m_ww_m   = -99;
  
  xhh_p_ww_pt  = -99;
  xhh_p_ww_eta = -99;
  xhh_p_ww_phi = -99;
  xhh_p_ww_m   = -99;    
  
  //---- at least 2 leptons ----
  
  if (m_maxptleptons.size() >= 2) {
  
  // kind = 0/1 if m/e
  
  std::map<float, int>::iterator it_type_m_lepton = m_maxptleptons.begin();
  int flav1 = (it_type_m_lepton->second<0);  // m>0, e<0 ---> m=0, e=1
  pt1 = - it_type_m_lepton->first;
  
  it_type_m_lepton++;
  int flav2 = (it_type_m_lepton->second<0);  // m>0, e<0 ---> m=0, e=1
  pt2 = - it_type_m_lepton->first;
  
  nlep = 0;
  for(it_type_m_lepton = m_maxptleptons.begin(); it_type_m_lepton != m_maxptleptons.end(); it_type_m_lepton++) {
   if ( -(it_type_m_lepton->first) > 10) nlep++;
  }
    
  //                       ee/mm          e   m           m    e
  channel =             flav1*flav2+2*(flav1>flav2)+3*(flav1<flav2);
  
  // # mumu #    channel == 0
  // # mue #     channel == 3
  // # emu #     channel == 2
  // # ee #      channel == 1
  
  pt1 = l1.Pt();
  pt2 = l2.Pt();
  mll = (l1+l2).M();
  dphill = l1.DeltaPhi(l2);
  
  
  TLorentzVector hww;
  TLorentzVector hwwp;
  TLorentzVector hwwm;
  
  if (pfmet != -99) {
   //   HiggsMass
   //---- h>ww
   TLorentzVector vmet;
   //--- IMPORTANT: h>ww, mll ~ mvv, otherwise something missing in higgs kinematic reconstruction
   vmet.SetPtEtaPhiM(met->MET, 0, met->Phi, mll);
   
   hww = l1 + l2 + vmet;
   
   hww_pt =  (l1 + l2 + vmet ).Pt();
   hww_phi = (l1 + l2 + vmet ).Phi();
   
   //---- kinematic fit for eta
   float sintheta2 = (hww_pt*hww_pt / (hww.E() * hww.E() - HiggsMass*HiggsMass ));
   float sintheta;
   if (sintheta2 > 0) sintheta = sqrt (sintheta2);
   if (sintheta2 > 0) {
    hww_etap = - log (tan ( asin ( sintheta ) / 2. )) ;
    hww_etam = - log (tan ( asin (-sintheta ) / 2. )) ;
    
    hwwp = hww;
    std::cout << " hww_pt = " << hww_pt << std::endl;
    hwwp.SetPtEtaPhiM(hww_pt, hww_etap, hww_phi, HiggsMass);
    hwwm.SetPtEtaPhiM(hww_pt, hww_etam, hww_phi, HiggsMass);
    
    //---- x>hh
    TLorentzVector xhh_p;
    TLorentzVector xhh_m;
    xhh_p = hwwm + hbb;
    xhh_m = hwwp + hbb;
    
    xhh_m_ww_pt  = xhh_m.Pt();
    xhh_m_ww_eta = xhh_m.Eta();
    xhh_m_ww_phi = xhh_m.Phi();
    xhh_m_ww_m   = xhh_m.M();

    xhh_p_ww_pt  = xhh_p.Pt();
    xhh_p_ww_eta = xhh_p.Eta();
    xhh_p_ww_phi = xhh_p.Phi();
    xhh_p_ww_m   = xhh_p.M();
    
   }
   else {
    hww_etap = -99;
    hww_etam = -99;
   }
   
  }
  else {
   //---- h>ww
   hww_pt = -99;
   hww_etam = -99;
   hww_etap = -99;
   hww_phi = -99;
  }
  
  
  }
  else {
   pt1 = -99;
   pt2 = -99;
   nlep = m_maxptleptons.size();
   channel = -1;
   mll = -99;
   dphill = -99;
  
   hww_pt = -99;
   hww_etam = -99;
   hww_etap = -99;
   hww_phi = -99;
  }
  
   
  
   
  
  outtree->Fill();
 }
 
 outfile->cd();
 outtree->Write();
 outfile->Close();
 
 return 0;
}
