#ifndef TESTRESOLUTION_CC
#define TESTRESOLUTION_CC

#include "TestResolution.h"

#include "TCanvas.h"
#include "TLegend.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TestResolution::TestResolution(const edm::ParameterSet& iConfig) :
  theMuonLabel_( iConfig.getParameter<edm::InputTag>( "MuonLabel" ) ),
  theMuonType_( iConfig.getParameter<int>( "MuonType" ) ),
  theRootFileName_( iConfig.getUntrackedParameter<string>("OutputFileName") )
{
  //now do what ever initialization is needed
  outputFile_ = new TFile(theRootFileName_.c_str(), "RECREATE");
  outputFile_->cd();
  sigmaPt_ = new TProfile("sigmaPtOverPt", "sigmaPt/Pt vs muon Pt", 1000, 0, 100);
  eventCounter_ = 0;
  // Create the corrector and set the parameters
  resolutionFunction_.reset(new ResolutionFunction( iConfig.getUntrackedParameter<string>("ResolutionsIdentifier") ) );
  cout << "resolutionFunction_ = " << &*resolutionFunction_ << endl;
}


TestResolution::~TestResolution()
{
  outputFile_->cd();
  TCanvas canvas("sigmaPtOverPt","sigmaPt/Pt vs muon Pt", 1000, 800);
  canvas.cd();
  sigmaPt_->GetXaxis()->SetTitle("Pt(GeV)");
//   TLegend * legend = new TLegend(0.7,0.71,0.98,1.);
//   legend->SetTextSize(0.02);
//   legend->SetFillColor(0); // Have a white background
//   legend->AddEntry(uncorrectedPt_, "original pt");
//   legend->AddEntry(correctedPt_, "corrected pt");
  sigmaPt_->Draw();
//   legend->Draw("same");

  canvas.Write();
  sigmaPt_->Write();
  outputFile_->Close();

  cout << "Total analyzed events = " << eventCounter_ << endl;
}


//
// member functions
//

// ------------ method called to for each event  ------------
void TestResolution::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;

  ++eventCounter_;
  if ( eventCounter_%100 == 0 ) {
    std::cout << "Event number " << eventCounter_ << std::endl;
  }

  // Take the reco-muons, depending on the type selected in the cfg
  // --------------------------------------------------------------

  vector<reco::LeafCandidate> muons;

  if (theMuonType_==1) { // GlobalMuons
    Handle<reco::MuonCollection> glbMuons;
    iEvent.getByLabel (theMuonLabel_, glbMuons);
    muons = fillMuonCollection(*glbMuons);
  }
  else if (theMuonType_==2) { // StandaloneMuons
    Handle<reco::TrackCollection> saMuons;
    iEvent.getByLabel (theMuonLabel_, saMuons);
    muons = fillMuonCollection(*saMuons);
  }
  else if (theMuonType_==3) { // Tracker tracks
    Handle<reco::TrackCollection> tracks;
    iEvent.getByLabel (theMuonLabel_, tracks);
    muons = fillMuonCollection(*tracks);
  }

  // Loop on the recMuons
  vector<reco::LeafCandidate>::const_iterator recMuon = muons.begin();
  for ( ; recMuon!=muons.end(); ++recMuon ) {  

    // Fill the histogram with uncorrected pt values
    sigmaPt_->Fill(resolutionFunction_->sigmaPt(*recMuon), recMuon->pt());

  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(TestResolution);

#endif // TESTRESOLUTION_CC
