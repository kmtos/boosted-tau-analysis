// -*- C++ -*-
//
// Package:    TauAnalyzer
// Class:      GenAnalyzer
// 
/**\class GenAnalyzer GenAnalyzer.cc 
   BoostedTauAnalysis/TauAnalyzer/src/GenAnalyzer.cc

   Description: plot gen quantities

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Rachel Yohay,512 1-010,+41227670495,
//         Created:  Wed Jul 18 16:40:51 CEST 2012
// $Id: GenAnalyzer.cc,v 1.1 2012/09/25 11:50:20 yohay Exp $
//
//


// system include files
#include <memory>
#include <string>
#include <sstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "BoostedTauAnalysis/Common/interface/Common.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "HLTrigger/HLTanalyzers/interface/HLTInfo.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "HLTrigger/HLTcore/interface/HLTConfigData.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TLegend.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace trigger;
//
// class declaration
//

class GenAnalyzer : public edm::EDAnalyzer {
public:
  explicit GenAnalyzer(const edm::ParameterSet&);
  ~GenAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  //delete memory
  void reset(const bool);

  // ----------member data ---------------------------

  //hltConfig for trigger matching
  HLTConfigProvider hltConfig_;

  //pointer to output file object
  TFile* out_;

  //name of output file
  std::string outFileName_;

  //gen particle tag
  edm::InputTag genParticleTag_;

  //PU info tag
  edm::InputTag PUTag_;

  //reco muon tag
  edm::InputTag muonTag_;

  //gen tau-->mu tag
  //  edm::InputTag genTauMuTag_;

  //set of parameters for GenTauDecayID class
  edm::ParameterSet genTauDecayIDPSet_;

  //trigger stuff
  edm::InputTag triggerEventTag_;
  edm::InputTag triggerResultsTag_;
  double delRMatchingCut_;
  std::vector<edm::InputTag> hltTags_;
  edm::InputTag theRightHLTTag_;
  edm::InputTag theRightHLTSubFilter_;
  std::vector<edm::InputTag> HLTSubFilters_;

  //mass of pseudoscalar a
  TH1F* aMass_;

  //histogram of dR between gen objects from a1 decay
  TH1F* dRA1TauDaughters_;

  //histogram of dR between gen objects from a1 decay after gen match
  TH1F* dRA1TauDaughtersGenMatch_;

  //histogram of Higgs pT
  TH1F* HPT_;

  //histogram of mu+had mu pT
  TH1F* tauMuPT_;

  //histogram of mu+had mu pT after gen match
  TH1F* tauMuPTGenMatch_;

  //histogram of mu+had mu's sister tau pT after gen match
  TH1F* tauSisterPTGenMatch_;

  //histogram of mu+had mu's sister tau decay mode after gen match
  TH1F* tauSisterDecayModeGenMatch_;

  //histogram of gen-matched reco mu eta
  TH1F* recoMuEtaGenMatch_;

  //histogram of mu+had had pT
  TH1F* tauHadPT_;

  //histogram of gen-matched reco mu PFRelIso
  TH1F* recoMuPFRelIso_;

  //histogram of true no. in-time interactions
  TH1D* trueNInt_;

  //a2 tau pair decay type vs. a1 tau pair decay type
  TH2F* a2TauPairDecayVsA1TauPairDecay_;

  //histogram of tau_mu pT vs dR(tau_mu, sister tau)
  TH2F* tauMuPTVsdR_;

  //histogram of tau_sister pT vs dR(tau_mu, sister tau)
  TH2F* tauSisterPTVsdR_;

  //histogram of tau_sister pT vs decay mode after gen matching
  TH2F* tauSisterPTVsDecayMode_;

  //histogram of gen-matched reco mu PFRelIso vs dR(tau_mu, sister tau)
  TH2F* recoMuPFRelIsoVsdRA1TauDaughters_;

  //histogram of gen-matched reco mu PFRelIso vs pT of sister tau
  TH2F* recoMuPFRelIsoVsTauSisterPT_;

  //histogram of gen-matched reco mu pT vs pT of sister tau
  TH2F* recoMuPTVsTauSisterPT_;

  //histogram of gen-matched gen tau_mu pT vs pT of sister tau
  TH2F* tauMuPTVsTauSisterPT_;

  //histogram of gen-matched reco mu PFRelIso vs reco mu pT
  TH2F* recoMuPFRelIsoVsRecoMuPT_;

  //histogram of gen-matched reco mu PFRelIso vs decay mode of sister tau
  TH2F* recoMuPFRelIsoVsTauSisterDecayMode_;

  //reco muon eta bins
  std::vector<double> recoMuEtaBins_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
GenAnalyzer::GenAnalyzer(const edm::ParameterSet& iConfig):hltConfig_(),
  outFileName_(iConfig.getParameter<std::string>("outFileName")),
  genParticleTag_(iConfig.getParameter<edm::InputTag>("genParticleTag")),
  PUTag_(iConfig.getParameter<edm::InputTag>("PUTag")),
  muonTag_(iConfig.existsAs<edm::InputTag>("muonTag") ? 
	   iConfig.getParameter<edm::InputTag>("muonTag") : edm::InputTag()),
  genTauDecayIDPSet_(iConfig.getParameter<edm::ParameterSet>("genTauDecayIDPSet"))
{
  //now do what ever initialization is needed
  reset(false);
  const edm::InputTag dTriggerEventTag("hltTriggerSummaryAOD","","HLT");
  triggerEventTag_ = iConfig.getUntrackedParameter<edm::InputTag>("triggerEventTag",dTriggerEventTag);
  const edm::InputTag dTriggerResults("TriggerResults","","HLT");
  // By default, trigger results are labeled "TriggerResults" with process name "HLT" in the event.
  triggerResultsTag_ = iConfig.getUntrackedParameter<edm::InputTag>("triggerResultsTag",dTriggerResults);
  delRMatchingCut_ = iConfig.getUntrackedParameter<double>("triggerDelRMatch", 0.30);
  hltTags_ = iConfig.getParameter<std::vector<edm::InputTag> >("hltTags");
  //  hltConfig_ = iConfig.getParameter<HLTConfigProvider>("hltConfig");
  theRightHLTTag_ = iConfig.getParameter<edm::InputTag>("theRightHLTTag");
  theRightHLTSubFilter_ = iConfig.getParameter<edm::InputTag>("theRightHLTSubFilter");
  //Whether using HLT trigger path name or the actual trigger filter name. Trigger path is default.
  HLTSubFilters_ = iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("HLTSubFilters",std::vector<edm::InputTag>());

  recoMuEtaBins_.push_back(-2.4);
  recoMuEtaBins_.push_back(-2.1);
  recoMuEtaBins_.push_back(-1.2);
  recoMuEtaBins_.push_back(-0.9);
  recoMuEtaBins_.push_back(0.0);
  recoMuEtaBins_.push_back(0.9);
  recoMuEtaBins_.push_back(1.2);
  recoMuEtaBins_.push_back(2.1);
  recoMuEtaBins_.push_back(2.4);

}

GenAnalyzer::~GenAnalyzer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  reset(true);
}


//
// member functions
//

// ------------ method called for each event  ------------
void GenAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //get gen particle collection
  edm::Handle<reco::GenParticleCollection> pGenParticles;
  iEvent.getByLabel(genParticleTag_, pGenParticles);

  //get PU info
  edm::Handle<std::vector<PileupSummaryInfo> > pPU;
  iEvent.getByLabel(PUTag_, pPU);

  //get reco muons
  edm::Handle<reco::MuonCollection> pMuons;
  if (muonTag_ == edm::InputTag()) {}
  else iEvent.getByLabel(muonTag_, pMuons);

   // Trigger Info
  edm::Handle<trigger::TriggerEvent> trgEvent;
  iEvent.getByLabel(triggerEventTag_,trgEvent);
  edm::Handle<edm::TriggerResults> pTrgResults;
  iEvent.getByLabel(triggerResultsTag_, pTrgResults);
  std::map<std::string, bool> triggerInMenu;
  std::string myHLTFilter = "";
  int index = 9999;

  // get names of active HLT paths in this event
  std::vector<std::string> activeHLTPathsInThisEvent = hltConfig_.triggerNames();
  std::cout << "no. of active HLT paths = " << hltConfig_.triggerNames().size() << std::endl;
  // loop over active HLT paths to search for desired path
  for (std::vector<std::string>::const_iterator iHLT = activeHLTPathsInThisEvent.begin(); 
       iHLT != activeHLTPathsInThisEvent.end(); ++iHLT) { // active paths loop
    for (std::vector<edm::InputTag>::const_iterator iMyHLT = hltTags_.begin(); 
	 iMyHLT != hltTags_.end(); ++iMyHLT) {
      if ((*iMyHLT).label() == *iHLT) {
 	cout << "######## " << *iHLT << endl;
	myHLTFilter = (*iMyHLT).label();
	triggerInMenu[(*iMyHLT).label()] = true;
 	std::cout << "(*iMyHLT).label() = " << (*iMyHLT).label() << std::endl;
 	std::cout << "hltConfig_.prescaleValue(iEvent, iSetup, *iHLT) = ";
 	std::cout << hltConfig_.prescaleValue(iEvent, iSetup, *iHLT) << std::endl;
      }
    }
  } // active paths loop

   edm::InputTag filterTag;
   // loop over these objects to see whether they match
   const trigger::TriggerObjectCollection& TOC( trgEvent->getObjects() );

   //choose the right sub-filter depending on the HLT path name
   std::vector<std::string> filters;
   try { filters = hltConfig_.moduleLabels( theRightHLTTag_.label() ); }
   catch (std::exception ex) { std::cout << "bad trigger\n"; }

   for(int i=0; i != trgEvent->sizeFilters(); ++i) {
     std::string label(trgEvent->filterTag(i).label());
     //if( label == theRightHLTSubFilter_.label() ) index = i;
     if( label.find(theRightHLTSubFilter_.label()) != std::string::npos )
       {
 	 std::cout << "filterTag label: " << label << std::endl;
	 std::cout << "found subfilter!" << std::endl;
	 index = i;
       }
   }
   //    cout << "index = " << index << endl;
   // find how many objects there are
   if (index == 9999)
     index = 0;
   const trigger::Keys& KEYS(trgEvent->filterKeys(index));
   const size_type nK(KEYS.size());
   std::cout << "nK = " << nK << std::endl;

   //did this event fire the HLT?
   const edm::TriggerNames &trgNames = iEvent.triggerNames(*pTrgResults);
   const unsigned int trgIndex = trgNames.triggerIndex(myHLTFilter);
   std::cout << "trgIndex = " << trgIndex << " and trgNames.size() = " << trgNames.size() << std::endl;
   bool firedHLT = (trgIndex < trgNames.size()) && (pTrgResults->accept(trgIndex));


  //look at gen muon parentage
  for (reco::GenParticleCollection::const_iterator iGenParticle = pGenParticles->begin(); 
       iGenParticle != pGenParticles->end(); ++iGenParticle) {
    if (fabs(iGenParticle->pdgId()) == 35)
      { // if it's an H
	std::cout << "mass of H = " << iGenParticle->mass() << std::endl;
	HPT_->Fill(iGenParticle->pt());
      } // if it's an H
    if (fabs(iGenParticle->pdgId()) == 36)
      { // if it's an A
	if(iGenParticle->status() == 2)
	  aMass_->Fill(iGenParticle->mass());
      } // if it's an A
  }

  std::vector<reco::Muon*> recoMuPtrs;
  for(reco::MuonCollection::const_iterator iMuon = pMuons->begin(); iMuon != pMuons->end(); ++iMuon)
    {
      recoMuPtrs.push_back(const_cast<reco::Muon*>((&*iMuon)));
    }


  //find a1 tau decay products
  std::vector<GenTauDecayID> aDecayProducts;
  for (reco::GenParticleCollection::const_iterator iGenParticle = pGenParticles->begin(); 
       iGenParticle != pGenParticles->end(); ++iGenParticle) {
    //    if (iGenParticle->pdgId() == 35)
    //     std::cout << "Mass of a = " << iGenParticle->mass() << std::endl;
    try {
      GenTauDecayID tauDecay(genTauDecayIDPSet_, pGenParticles, 
			     iGenParticle - pGenParticles->begin());
      if (tauDecay.isStatus3DecayProduct()) aDecayProducts.push_back(tauDecay);
    }
    catch (std::string& ex) { throw cms::Exception("GenAnalyzer") << ex; }
  }
  
  //containers for a tau decay types
  std::vector<std::pair<GenTauDecayID::DecayType, GenTauDecayID::DecayType> > aDecay;
  
  //loop over a1 tau daughters
  std::vector<unsigned int> keysToIgnore;
  for (std::vector<GenTauDecayID>::iterator iTau = aDecayProducts.begin(); 
       iTau != aDecayProducts.end(); ++iTau) {
    try {
      const unsigned int tauKey = iTau->getTauIndex();
      
      //find sister
      iTau->findSister();
      const unsigned int iSister = iTau->getSisterIndex();

      //if sister wasn't already looped over...
      if (std::find(keysToIgnore.begin(), keysToIgnore.end(), iSister) == keysToIgnore.end()) {

	//...plot dR(sisters)
	dRA1TauDaughters_->Fill(reco::deltaR(*reco::GenParticleRef(pGenParticles, tauKey), 
					     *reco::GenParticleRef(pGenParticles, iSister)));
	if (reco::deltaR(*reco::GenParticleRef(pGenParticles, tauKey), 
			 *reco::GenParticleRef(pGenParticles, iSister)) > 0.3)
	  {
	    std::cout << "dR = " << reco::deltaR(*reco::GenParticleRef(pGenParticles, tauKey), 
						 *reco::GenParticleRef(pGenParticles, iSister)) << std::endl;
	    std::cout << "tau's decay type: " << iTau->tauDecayType(false, true).second << std::endl;
	    std::cout << "sister tau's decay type: " << iTau->sisterDecayType(false,true).second << std::endl;
	  }

	//...save pair decay mode
	aDecay.push_back(std::pair<GenTauDecayID::DecayType, 
			 GenTauDecayID::DecayType>(iTau->tauDecayType(false, true).second, 
						   iTau->sisterDecayType(false, true).second));

	//ignore this tau in the future
	keysToIgnore.push_back(tauKey);
      }
      // get tau decay and tau sister decay
      std::pair<reco::PFTau::hadronicDecayMode, GenTauDecayID::DecayType> thisDecay = 
	iTau->tauDecayType(false, true);
      std::pair<reco::PFTau::hadronicDecayMode, GenTauDecayID::DecayType> sisterDecay = 
	iTau->sisterDecayType(false, true);

      //is this a mu+had decay?
      if (((thisDecay.second == GenTauDecayID::MU) && 
	   (sisterDecay.second == GenTauDecayID::HAD)) || 
	  ((thisDecay.second == GenTauDecayID::HAD) && 
	   (sisterDecay.second == GenTauDecayID::MU))) {

	//plot mu pT and had pT
	reco::LeafCandidate::LorentzVector visibleP4 = iTau->getVisibleTauP4();
	if (thisDecay.second == GenTauDecayID::MU)
	  {
	    tauMuPT_->Fill(visibleP4.Pt());
	    double delR = reco::deltaR(*reco::GenParticleRef(pGenParticles, iTau->getTauIndex()), 
				       *reco::GenParticleRef(pGenParticles, iTau->getSisterIndex()));
	    tauMuPTVsdR_->Fill(delR, visibleP4.Pt());
	  }
	if (thisDecay.second == GenTauDecayID::HAD) tauHadPT_->Fill(visibleP4.Pt());
      }

      //is this a mu+X decay?
      if (thisDecay.second == GenTauDecayID::MU)
	{ // if this tau decayed to a mu

	  //get ref to gen mu
	  const reco::GenParticle* genMuRef = NULL;
	  for (unsigned int i = 0; i < (*reco::GenParticleRef(pGenParticles, iTau->getTauIndex())).numberOfDaughters(); ++i)
	    {
	      if (fabs((*reco::GenParticleRef(pGenParticles, iTau->getTauIndex())).daughter(i)->pdgId()) == 15)
		{
		  for (unsigned int j = 0; j < (*reco::GenParticleRef(pGenParticles, iTau->getTauIndex())).daughter(i)->numberOfDaughters(); ++j)
		    {
		      if ((*reco::GenParticleRef(pGenParticles, iTau->getTauIndex())).daughter(i)->daughter(j)->pdgId() == 13)
			genMuRef = (&*reco::GenParticleRef(pGenParticles, iTau->getTauIndex()));
		    }
		}
	    }

	  if (genMuRef != NULL)
	    {
	      //loop over reco mu refs to find match (dR < 0.3)
	      double delR_recogen = 9999.;
	      unsigned int muMatch = -1;
	      for (unsigned int iRecoMu = 0; iRecoMu != recoMuPtrs.size(); ++iRecoMu)
		{
		  //double compareDR = reco::deltaR(*genMuRef, **iRecoMu);
		  double compareDR = reco::deltaR(*genMuRef, *recoMuPtrs.at(iRecoMu));
		  if (compareDR < delR_recogen)
		    {
		      delR_recogen = compareDR;
		      muMatch = iRecoMu;
		    }
		}
	      
	      //make sure this reco mu is matched to a trigger object
	      bool trigger_matched = false;
	      if (firedHLT)
		{ // firedHLT
		  // Did this reco muon cause an HLT trigger?
		  
		  for(int ipart = 0; ipart != nK; ++ipart) { 
		    
		    const trigger::TriggerObject& TO = TOC[KEYS[ipart]];	
		    
		    std::cout << "dR(reco mu, TO) = " << deltaR((*recoMuPtrs.at(muMatch)), TO) << std::endl;
		    //save RECO objects matched to trigger objects
		    if ((deltaR((*recoMuPtrs.at(muMatch)), TO) < delRMatchingCut_)) {
		      trigger_matched = true;
		    }
		  }
		} //firedHLT
	      
	      //if there was a match...
	      if ((delR_recogen < 0.3) && trigger_matched)
		{
		  //if reco mu pT > 25 and |eta| < 2.1 ...
		  if (((*recoMuPtrs.at(muMatch)).pt() > 25.) && ((*recoMuPtrs.at(muMatch)).eta() < 2.1))
		    {
		      // - plot delR(gen tau_mu, gen tau_sister)
		      dRA1TauDaughtersGenMatch_->Fill(reco::deltaR(*reco::GenParticleRef(pGenParticles, iTau->getTauIndex()), 
								   *reco::GenParticleRef(pGenParticles, iTau->getSisterIndex())));
		      // - plot decay mode of sister
		      tauSisterDecayModeGenMatch_->Fill(sisterDecay.second);
		      // - plot pT of tau_mu
		      
		      tauMuPTGenMatch_->Fill(iTau->getVisibleTauP4().Pt());
		      // - plot pT of sister
		      tauSisterPTGenMatch_->Fill(iTau->getVisibleTauSisterP4().Pt());
		      // - plot pT(gen tau_sister) vs decay mode of tau_sister
		      tauSisterPTVsDecayMode_->Fill(sisterDecay.second, iTau->getVisibleTauSisterP4().Pt());
		      recoMuEtaGenMatch_->Fill((*recoMuPtrs.at(muMatch)).eta());
		      tauSisterPTVsdR_->Fill(reco::deltaR(*reco::GenParticleRef(pGenParticles, iTau->getTauIndex()), 
							  *reco::GenParticleRef(pGenParticles, iTau->getSisterIndex())), iTau->getVisibleTauSisterP4().Pt());
		      // - plot PFRelIso vs pT of reco mu
		      double recoMuRelIso = Common::getMuonCombPFIso((*recoMuPtrs.at(muMatch)), 0.5)/(*recoMuPtrs.at(muMatch)).pt();
		      recoMuPFRelIso_->Fill(recoMuRelIso);
		      recoMuPFRelIsoVsdRA1TauDaughters_->Fill(reco::deltaR(*reco::GenParticleRef(pGenParticles, iTau->getTauIndex()), 
									   *reco::GenParticleRef(pGenParticles, iTau->getSisterIndex())), recoMuRelIso);
		      recoMuPFRelIsoVsTauSisterPT_->Fill(iTau->getVisibleTauSisterP4().Pt(), recoMuRelIso);
		      recoMuPFRelIsoVsRecoMuPT_->Fill((*recoMuPtrs.at(muMatch)).pt(), recoMuRelIso);
		      recoMuPFRelIsoVsTauSisterDecayMode_->Fill(sisterDecay.second, recoMuRelIso);
		      recoMuPTVsTauSisterPT_->Fill(iTau->getVisibleTauSisterP4().Pt(),(*recoMuPtrs.at(muMatch)).pt());
		      tauMuPTVsTauSisterPT_->Fill(iTau->getVisibleTauSisterP4().Pt(),iTau->getVisibleTauP4().Pt());
		    }
		}
	    }
	} // if this tau decayed to a mu
	
    }
    catch (std::string& ex) { throw cms::Exception("GenAnalyzer") << ex; }
  }

  //plot a2 decay type vs. a1 decay type
  int val1 = -1;
  int val2 = -1;
  if (aDecay.size() == 2) {
    for (std::vector<std::pair<GenTauDecayID::DecayType, 
	   GenTauDecayID::DecayType> >::const_iterator iADecay = aDecay.begin(); 
	 iADecay != aDecay.end(); ++iADecay) {
      int val = -1;
      if ((iADecay->first == GenTauDecayID::MU) && (iADecay->second == GenTauDecayID::MU)) val = 0;
      if (((iADecay->first == GenTauDecayID::MU) && (iADecay->second == GenTauDecayID::E)) || 
	  ((iADecay->first == GenTauDecayID::E) && (iADecay->second == GenTauDecayID::MU))) val = 1;
      if (((iADecay->first == GenTauDecayID::MU) && (iADecay->second == GenTauDecayID::HAD)) || 
	  ((iADecay->first == GenTauDecayID::HAD) && (iADecay->second == GenTauDecayID::MU))) {
	val = 2;
      }
      if ((iADecay->first == GenTauDecayID::E) && (iADecay->second == GenTauDecayID::E)) val = 3;
      if (((iADecay->first == GenTauDecayID::E) && (iADecay->second == GenTauDecayID::HAD)) || 
	  ((iADecay->first == GenTauDecayID::HAD) && (iADecay->second == GenTauDecayID::E))) {
	val = 4;
      }
      if ((iADecay->first == GenTauDecayID::HAD) && (iADecay->second == GenTauDecayID::HAD)) {
	val = 5;
      }
      if ((iADecay - aDecay.begin()) == 0) val1 = val;
      if ((iADecay - aDecay.begin()) == 1) val2 = val;
    }
  }
  a2TauPairDecayVsA1TauPairDecay_->Fill(val1, val2);

  //plot distribution of true no. in-time interactions
  float trueNInt = -1;
  std::vector<PileupSummaryInfo>::const_iterator iPU = pPU->begin();
  int BX = 0;
  while ((iPU != pPU->end()) && (BX == 0)) {
    int BX = iPU->getBunchCrossing();
    if (BX == 0) { 
      trueNInt = iPU->getTrueNumInteractions();
      BX = -1;
    }
    ++iPU;
  }
  trueNInt_->Fill(trueNInt);
}


// ------------ method called once each job just before starting event loop  ------------
void GenAnalyzer::beginJob()
{
  //open output file
  out_ = new TFile(outFileName_.c_str(), "RECREATE");

  //book histograms
  dRA1TauDaughters_ = new TH1F("dRA1TauDaughters", "", 60, 0.0, 3.0);
  dRA1TauDaughtersGenMatch_ = new TH1F("dRA1TauDaughtersGenMatch", "", 60, 0.0, 3.0);
  aMass_ = new TH1F("aMass", "", 100, 0.0, 25.0);
  HPT_ = new TH1F("HPT", "", 100, 0.0, 200.0);
  tauMuPT_ = new TH1F("tauMuPT", "", 50, 0.0, 100.0);
  tauMuPTGenMatch_ = new TH1F("tauMuPTGenMatch", "", 50, 0.0, 100.0);
  tauSisterPTGenMatch_ = new TH1F("tauSisterPTGenMatch", "", 50, 0.0, 100.0);
  tauSisterDecayModeGenMatch_ = new TH1F("tauSisterDecayModeGenMatch", "",6, -0.5, 5.5);
  //recoMuEtaGenMatch_ = new TH1F("recoMuEtaGenMatch", "", 23, -2.3, 2.3);
  recoMuEtaGenMatch_ = new TH1F("recoMuEtaGenMatch", "", recoMuEtaBins_.size() - 1, &recoMuEtaBins_[0]);
  tauHadPT_ = new TH1F("tauHadPT", "", 50, 0.0, 100.0);
  recoMuPFRelIso_ = new TH1F("recoMuPFRelIso", "", 2000, 0.0, 40.0);
  trueNInt_ = new TH1D("trueNInt", "", 60, 0.0, 60.0);
  a2TauPairDecayVsA1TauPairDecay_ = new TH2F("a2TauPairDecayVsA1TauPairDecay", 
					     ";a_{1} di-tau decay;a_{2} di-tau decay", 
					     6, -0.5, 5.5, 6, -0.5, 5.5);
  tauMuPTVsdR_ = new TH2F("tauMuPTVsdR", ";#DeltaR(#tau_{#mu},#tau_{sister});#tau_{#mu} p_{T} (GeV)", 60, 0., 3.0, 50, 0.0, 100.0);
  tauSisterPTVsdR_ = new TH2F("tauSisterPTVsdR", ";#DeltaR(#tau_{#mu},#tau_{sister});#tau_{sister} p_{T} (GeV)", 60, 0., 3.0, 50, 0.0, 100.0);
  tauSisterPTVsDecayMode_ = new TH2F("tauSisterPTVsDecayMode", ";#tau_{sister} decay mode;#tau_{sister} p_{T} (GeV)", 6, -0.5, 5.5, 50, 0.0, 100.0);
  recoMuPFRelIsoVsdRA1TauDaughters_ = new TH2F("recoMuPFRelIsoVsdRA1TauDaughters", ";#DeltaR;reco #mu PFRelIso", 60, 0., 3., 2000, 0.0, 40.0);
  recoMuPFRelIsoVsTauSisterPT_ = new TH2F("recoMuPFRelIsoVsTauSisterPT", ";#tau_{sister} p_{T} (GeV);reco #mu PFRelIso", 50, 0., 100., 2000, 0.0, 40.0);
  recoMuPTVsTauSisterPT_ = new TH2F("recoMuPTVsTauSisterPT", ";#tau_{sister} p_{T} (GeV);reco #mu p_{T} (GeV)", 50, 0., 100., 50, 0., 100.);
  tauMuPTVsTauSisterPT_ = new TH2F("tauMuPTVsTauSisterPT", ";#tau_{sister} p_{T} (GeV);gen #tau_{#mu} p_{T} (GeV)", 50, 0., 100., 50, 0., 100.);
  recoMuPFRelIsoVsRecoMuPT_ = new TH2F("recoMuPFRelIsoVsRecoMuPT", ";reco #mu p_{T} (GeV);reco #mu PFRelIso", 50, 0., 100., 2000, 0.0, 40.0);
  recoMuPFRelIsoVsTauSisterDecayMode_ = new TH2F("recoMuPFRelIsoVsTauSisterDecayMode", ";#tau_{sister} decay mode;reco #mu PFRelIso", 6, -0.5, 5.5, 2000, 0.0, 40.0);

  //set bin labels where appropriate
  a2TauPairDecayVsA1TauPairDecay_->GetXaxis()->SetBinLabel(1, "#tau_{#mu}#tau_{#mu}");
  a2TauPairDecayVsA1TauPairDecay_->GetXaxis()->SetBinLabel(2, "#tau_{#mu}#tau_{e}");
  a2TauPairDecayVsA1TauPairDecay_->GetXaxis()->SetBinLabel(3, "#tau_{#mu}#tau_{had}");
  a2TauPairDecayVsA1TauPairDecay_->GetXaxis()->SetBinLabel(4, "#tau_{e}#tau_{e}");
  a2TauPairDecayVsA1TauPairDecay_->GetXaxis()->SetBinLabel(5, "#tau_{e}#tau_{had}");
  a2TauPairDecayVsA1TauPairDecay_->GetXaxis()->SetBinLabel(6, "#tau_{had}#tau_{had}");
  a2TauPairDecayVsA1TauPairDecay_->GetYaxis()->SetBinLabel(1, "#tau_{#mu}#tau_{#mu}");
  a2TauPairDecayVsA1TauPairDecay_->GetYaxis()->SetBinLabel(2, "#tau_{#mu}#tau_{e}");
  a2TauPairDecayVsA1TauPairDecay_->GetYaxis()->SetBinLabel(3, "#tau_{#mu}#tau_{had}");
  a2TauPairDecayVsA1TauPairDecay_->GetYaxis()->SetBinLabel(4, "#tau_{e}#tau_{e}");
  a2TauPairDecayVsA1TauPairDecay_->GetYaxis()->SetBinLabel(5, "#tau_{e}#tau_{had}");
  a2TauPairDecayVsA1TauPairDecay_->GetYaxis()->SetBinLabel(6, "#tau_{had}#tau_{had}");

  tauSisterPTVsDecayMode_->GetXaxis()->SetBinLabel(1, "#tau_{had}");
  tauSisterPTVsDecayMode_->GetXaxis()->SetBinLabel(2, "#tau_{#mu}");
  tauSisterPTVsDecayMode_->GetXaxis()->SetBinLabel(3, "#tau_{e}");
  tauSisterPTVsDecayMode_->GetXaxis()->SetBinLabel(4, "#tau_{unknown}");
  tauSisterPTVsDecayMode_->GetXaxis()->SetBinLabel(5, "not #tau");

  tauSisterDecayModeGenMatch_->GetXaxis()->SetBinLabel(1, "#tau_{had}");
  tauSisterDecayModeGenMatch_->GetXaxis()->SetBinLabel(2, "#tau_{#mu}");
  tauSisterDecayModeGenMatch_->GetXaxis()->SetBinLabel(3, "#tau_{e}");
  tauSisterDecayModeGenMatch_->GetXaxis()->SetBinLabel(4, "#tau_{unknown}");
  tauSisterDecayModeGenMatch_->GetXaxis()->SetBinLabel(5, "not #tau");

  recoMuPFRelIsoVsTauSisterDecayMode_->GetXaxis()->SetBinLabel(1, "#tau_{had}");
  recoMuPFRelIsoVsTauSisterDecayMode_->GetXaxis()->SetBinLabel(2, "#tau_{#mu}");
  recoMuPFRelIsoVsTauSisterDecayMode_->GetXaxis()->SetBinLabel(3, "#tau_{e}");
  recoMuPFRelIsoVsTauSisterDecayMode_->GetXaxis()->SetBinLabel(4, "#tau_{unknown}");
  recoMuPFRelIsoVsTauSisterDecayMode_->GetXaxis()->SetBinLabel(5, "not #tau");
}

// ------------ method called once each job just after ending the event loop  ------------
void GenAnalyzer::endJob() 
{
  //make the canvases
  TCanvas dRA1TauDaughtersCanvas("dRA1TauDaughtersCanvas", "", 600, 600);
  Common::setCanvasOptions(dRA1TauDaughtersCanvas, 1, 0, 0);
  TCanvas dRA1TauDaughtersGenMatchCanvas("dRA1TauDaughtersGenMatchCanvas", "", 600, 600);
  Common::setCanvasOptions(dRA1TauDaughtersGenMatchCanvas, 1, 0, 0);
  TCanvas aMassCanvas("aMassCanvas", "", 600, 600);
  Common::setCanvasOptions(aMassCanvas, 1, 0, 0);
  TCanvas HPTCanvas("HPTCanvas", "", 600, 600);
  Common::setCanvasOptions(HPTCanvas, 1, 0, 0);
  TCanvas tauMuPTCanvas("tauMuPTCanvas", "", 600, 600);
  Common::setCanvasOptions(tauMuPTCanvas, 1, 0, 0);
  TCanvas tauMuPTGenMatchCanvas("tauMuPTGenMatchCanvas", "", 600, 600);
  Common::setCanvasOptions(tauMuPTGenMatchCanvas, 1, 0, 0);
  TCanvas tauSisterPTGenMatchCanvas("tauSisterPTGenMatchCanvas", "", 600, 600);
  Common::setCanvasOptions(tauSisterPTGenMatchCanvas, 1, 0, 0);
  TCanvas tauSisterDecayModeGenMatchCanvas("tauSisterDecayModeGenMatchCanvas", "", 600, 600);
  Common::setCanvasOptions(tauSisterDecayModeGenMatchCanvas, 1, 0, 0);
  TCanvas recoMuEtaGenMatchCanvas("recoMuEtaGenMatchCanvas", "", 600, 600);
  Common::setCanvasOptions(recoMuEtaGenMatchCanvas, 1, 0, 0);
  TCanvas tauHadPTCanvas("tauHadPTCanvas", "", 600, 600);
  Common::setCanvasOptions(tauHadPTCanvas, 1, 0, 0);
  TCanvas recoMuPFRelIsoCanvas("recoMuPFRelIsoCanvas", "", 600, 600);
  Common::setCanvasOptions(recoMuPFRelIsoCanvas, 1, 0, 0);
  TCanvas trueNIntCanvas("trueNIntCanvas", "", 600, 600);
  Common::setCanvasOptions(trueNIntCanvas, 1, 0, 0);
  TCanvas a2TauPairDecayVsA1TauPairDecayCanvas("a2TauPairDecayVsA1TauPairDecayCanvas", "", 
					       600, 600);
  TCanvas tauMuPTVsdRCanvas("tauMuPTVsdRCanvas","", 600, 600);
  TCanvas tauSisterPTVsdRCanvas("tauSisterPTVsdRCanvas","", 600, 600);
  TCanvas tauSisterPTVsDecayModeCanvas("tauSisterPTVsDecayModeCanvas", "", 600, 600);
  TCanvas recoMuPFRelIsoVsdRA1TauDaughtersCanvas("recoMuPFRelIsoVsdRA1TauDaughtersCanvas", "", 600, 600);
  TCanvas recoMuPFRelIsoVsTauSisterPTCanvas("recoMuPFRelIsoVsTauSisterPTCanvas", "", 600, 600);
  TCanvas recoMuPTVsTauSisterPTCanvas("recoMuPTVsTauSisterPTCanvas", "", 600, 600);
  TCanvas tauMuPTVsTauSisterPTCanvas("tauMuPTVsTauSisterPTCanvas", "", 600, 600);
  TCanvas recoMuPFRelIsoVsRecoMuPTCanvas("recoMuPFRelIsoVsRecoMuPTCanvas", "", 600, 600);
  TCanvas recoMuPFRelIsoVsTauSisterDecayModeCanvas("recoMuPFRelIsoVsTauSisterDecayModeCanvas", "", 600, 600);
  //format the plots
  Common::setHistogramOptions(dRA1TauDaughters_, kBlack, 0.7, 20, 1.0, "#DeltaR", "", 0.05);
  dRA1TauDaughters_->SetLineWidth(2);
  Common::setHistogramOptions(dRA1TauDaughtersGenMatch_, kBlack, 0.7, 20, 1.0, "#DeltaR", "", 0.05);
  dRA1TauDaughtersGenMatch_->SetLineWidth(2);
  Common::setHistogramOptions(aMass_, kBlack, 0.7, 20, 1.0, "m_{a} (GeV)", "", 0.05);
  aMass_->SetLineWidth(2);
  Common::setHistogramOptions(HPT_, kBlack, 0.7, 20, 1.0, "Higgs pT (GeV)", "", 0.05);
  HPT_->SetLineWidth(2);
  Common::setHistogramOptions(tauMuPT_, kBlack, 0.7, 20, 1.0, "tau_{mu} pT (GeV)", "", 0.05);
  tauMuPT_->SetLineWidth(2);
  Common::setHistogramOptions(tauMuPTGenMatch_, kBlack, 0.7, 20, 1.0, "tau_{mu} pT (GeV)", "", 0.05);
  tauMuPTGenMatch_->SetLineWidth(2);
  Common::setHistogramOptions(tauSisterPTGenMatch_, kBlack, 0.7, 20, 1.0, "tau_{sister} pT (GeV)", "", 0.05);
  tauSisterPTGenMatch_->SetLineWidth(2);
  Common::setHistogramOptions(tauSisterDecayModeGenMatch_, kBlack, 0.7, 20, 1.0, "tau_{sister} decay mode", "", 0.05);
  tauSisterDecayModeGenMatch_->SetLineWidth(2);
  //tauSisterDecayModeGenMatch_->Scale(1./tauSisterDecayModeGenMatch_->Integral());
  Common::setHistogramOptions(recoMuEtaGenMatch_, kBlack, 0.7, 20, 1.0, "reco #mu #eta", "", 0.05);
  recoMuEtaGenMatch_->SetLineWidth(2);
  Common::setHistogramOptions(tauHadPT_, kBlack, 0.7, 20, 1.0, "tau_{had} pT (GeV)", "", 0.05);
  tauHadPT_->SetLineWidth(2);
  Common::setHistogramOptions(recoMuPFRelIso_, kBlack, 0.7, 20, 1.0, "reco #mu PFRelIso", "", 0.05);
  recoMuPFRelIso_->SetLineWidth(2);
  Common::setHistogramOptions(trueNInt_, kBlack, 0.7, 20, 1.0, "No. interactions", "", 0.05);
  trueNInt_->SetLineWidth(2);

  //draw plots
  dRA1TauDaughtersCanvas.cd();
  dRA1TauDaughters_->Draw();
  dRA1TauDaughtersGenMatchCanvas.cd();
  dRA1TauDaughtersGenMatch_->Draw();
  aMassCanvas.cd();
  aMass_->Draw();
  HPTCanvas.cd();
  HPT_->Draw();
  tauMuPTCanvas.cd();
  tauMuPT_->Draw();
  tauMuPTGenMatchCanvas.cd();
  tauMuPTGenMatch_->Draw();
  tauSisterPTGenMatchCanvas.cd();
  tauSisterPTGenMatch_->Draw();
  tauSisterDecayModeGenMatchCanvas.cd();
  tauSisterDecayModeGenMatch_->Draw();
  recoMuEtaGenMatchCanvas.cd();
  recoMuEtaGenMatch_->Draw();
  tauHadPTCanvas.cd();
  tauHadPT_->Draw();
  recoMuPFRelIsoCanvas.cd();
  recoMuPFRelIso_->Draw();
  trueNIntCanvas.cd();
  trueNInt_->Draw();
  Common::draw2DHistograms(a2TauPairDecayVsA1TauPairDecayCanvas, a2TauPairDecayVsA1TauPairDecay_);
  Common::draw2DHistograms(tauMuPTVsdRCanvas, tauMuPTVsdR_);
  Common::draw2DHistograms(tauSisterPTVsdRCanvas, tauSisterPTVsdR_);
  Common::draw2DHistograms(tauSisterPTVsDecayModeCanvas, tauSisterPTVsDecayMode_);
  Common::draw2DHistograms(recoMuPFRelIsoVsdRA1TauDaughtersCanvas, recoMuPFRelIsoVsdRA1TauDaughters_);
  Common::draw2DHistograms(recoMuPFRelIsoVsTauSisterPTCanvas, recoMuPFRelIsoVsTauSisterPT_);
  Common::draw2DHistograms(recoMuPTVsTauSisterPTCanvas, recoMuPTVsTauSisterPT_);
  Common::draw2DHistograms(tauMuPTVsTauSisterPTCanvas, tauMuPTVsTauSisterPT_);
  Common::draw2DHistograms(recoMuPFRelIsoVsRecoMuPTCanvas, recoMuPFRelIsoVsRecoMuPT_);
  Common::draw2DHistograms(recoMuPFRelIsoVsTauSisterDecayModeCanvas, recoMuPFRelIsoVsTauSisterDecayMode_);
  //write output file
  out_->cd();
  dRA1TauDaughtersCanvas.Write();
  dRA1TauDaughtersGenMatchCanvas.Write();
  aMassCanvas.Write();
  HPTCanvas.Write();
  tauMuPTCanvas.Write();
  tauMuPTGenMatchCanvas.Write();
  tauSisterPTGenMatchCanvas.Write();
  tauSisterDecayModeGenMatchCanvas.Write();
  recoMuEtaGenMatchCanvas.Write();
  tauHadPTCanvas.Write();
  recoMuPFRelIsoCanvas.Write();
  trueNIntCanvas.Write();
  a2TauPairDecayVsA1TauPairDecayCanvas.Write();
  tauMuPTVsdRCanvas.Write();
  tauSisterPTVsdRCanvas.Write();
  tauSisterPTVsDecayModeCanvas.Write();
  recoMuPFRelIsoVsdRA1TauDaughtersCanvas.Write();
  recoMuPFRelIsoVsTauSisterPTCanvas.Write();
  recoMuPTVsTauSisterPTCanvas.Write();
  tauMuPTVsTauSisterPTCanvas.Write();
  recoMuPFRelIsoVsRecoMuPTCanvas.Write();
  recoMuPFRelIsoVsTauSisterDecayModeCanvas.Write();
  out_->Write();
  out_->Close();
}

// ------------ method called when starting to processes a run  ------------
void GenAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
  std::cout << "beginRun has run" << std::endl;
  bool changed_ = true;
  if ( !hltConfig_.init(iRun,iSetup,hltTags_[0].process(),changed_) ){
    edm::LogError("TriggerObjectFilter") << 
      "Error! Can't initialize HLTConfigProvider";
    throw cms::Exception("HLTConfigProvider::init() returned non 0");
  }
  //  return true;
}

// ------------ method called when ending the processing of a run  ------------
void GenAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
  //  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
void GenAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, 
						    edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void GenAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, 
						  edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  -----------
void GenAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void GenAnalyzer::reset(const bool doDelete)
{
  if ((doDelete) && (out_ != NULL)) delete out_;
  out_ = NULL;
  if ((doDelete) && (dRA1TauDaughters_ != NULL)) delete dRA1TauDaughters_;
  dRA1TauDaughters_ = NULL;
  if ((doDelete) && (HPT_ != NULL)) delete HPT_;
  HPT_ = NULL;
  if ((doDelete) && (tauMuPT_ != NULL)) delete tauMuPT_;
  tauMuPT_ = NULL;
  if ((doDelete) && (recoMuEtaGenMatch_ != NULL)) delete recoMuEtaGenMatch_;
  recoMuEtaGenMatch_ = NULL;
  if ((doDelete) && (tauHadPT_ != NULL)) delete tauHadPT_;
  tauHadPT_ = NULL;
  if ((doDelete) && (recoMuPFRelIso_ != NULL)) delete recoMuPFRelIso_;
  recoMuPFRelIso_ = NULL;
  if ((doDelete) && (trueNInt_ != NULL)) delete trueNInt_;
  trueNInt_ = NULL;
  if (doDelete && (a2TauPairDecayVsA1TauPairDecay_ != NULL)) {
    delete a2TauPairDecayVsA1TauPairDecay_;
  }
  a2TauPairDecayVsA1TauPairDecay_ = NULL;
  if (doDelete && (tauMuPTVsdR_ != NULL)) {
    delete tauMuPTVsdR_;
  }
  tauMuPTVsdR_ = NULL;
  if (doDelete && (tauSisterPTVsdR_ != NULL)) {
    delete tauSisterPTVsdR_;
  }
  tauSisterPTVsdR_ = NULL;
  if (doDelete && (tauSisterPTVsDecayMode_ != NULL)) {
    delete tauSisterPTVsDecayMode_;
  }
  tauSisterPTVsDecayMode_ = NULL;
  if (doDelete && (recoMuPFRelIsoVsdRA1TauDaughters_ != NULL)) {
    delete recoMuPFRelIsoVsdRA1TauDaughters_;
  }
  recoMuPFRelIsoVsdRA1TauDaughters_ = NULL;
  if (doDelete && (recoMuPFRelIsoVsTauSisterPT_ != NULL)) {
    delete recoMuPFRelIsoVsTauSisterPT_;
  }
  recoMuPFRelIsoVsTauSisterPT_ = NULL;
  if (doDelete && (recoMuPTVsTauSisterPT_ != NULL)) {
    delete recoMuPTVsTauSisterPT_;
  }
  recoMuPTVsTauSisterPT_ = NULL;
  if (doDelete && (tauMuPTVsTauSisterPT_ != NULL)) {
    delete tauMuPTVsTauSisterPT_;
  }
  tauMuPTVsTauSisterPT_ = NULL;
  if (doDelete && (recoMuPFRelIsoVsRecoMuPT_ != NULL)) {
    delete recoMuPFRelIsoVsRecoMuPT_;
  }
  recoMuPFRelIsoVsRecoMuPT_ = NULL;
  if (doDelete && (recoMuPFRelIsoVsTauSisterDecayMode_ != NULL)) {
    delete recoMuPFRelIsoVsTauSisterDecayMode_;
  }
  recoMuPFRelIsoVsTauSisterDecayMode_ = NULL;
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenAnalyzer);
