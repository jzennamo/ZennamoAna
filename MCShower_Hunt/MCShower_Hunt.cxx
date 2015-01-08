#ifndef MCSHOWER_HUNT_CXX
#define MCSHOWER_HUNT_CXX

#include "MCShower_Hunt.h"

namespace larlite {

  bool MCShower_Hunt::initialize() {

    TPC.Min(0,
	    -(::larutil::Geometry::GetME()->DetHalfHeight()),
	    0);

    TPC.Max(2*(::larutil::Geometry::GetME()->DetHalfWidth()),
	    ::larutil::Geometry::GetME()->DetHalfHeight(),
	    ::larutil::Geometry::GetME()->DetLength());

    tree = new TTree("tree","");
    //    tree->Branch("",&,"/I");
    tree->Branch("mctruth",&mctruth,"mctruth/I");
    tree->Branch("nu_CC",&nu_CC,"nu_CC/I");
    tree->Branch("nu_pdg",&nu_pdg,"nu_pdg/I");
    tree->Branch("nu_ina",&nu_ina,"nu_ina/I");

    tree->Branch("mcshower",&mcshower,"mcshower/I");
    tree->Branch("mcs_pdg",&mcs_pdg,"mcs_pdg/I");
    tree->Branch("mcs_ina",&mcs_ina,"mcs_ina/I");

    tree->Branch("mcs_anc",&mcs_anc,"mcs_anc/I");

    tree->Branch("N_e",&N_e,"N_e/I");

    N_truth = 0;
    N_shower = 0;

    return true;
  }

  void MCShower_Hunt::ctr_reset() {

    nu_CC = 0;
    nu_pdg = 0;
    nu_ina = 0;
    mcs_pdg = 0;
    mcs_ina = 0;
    mcs_anc = 0; 
    N_e = 0;

  }
  
  bool MCShower_Hunt::analyze(storage_manager* storage) {

    //Super MC Truth-iness  
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");
    if (!ev_mctruth) { std::cout << "FAIL!" << std::endl; return false; }

    //MCShower
    auto ev_mcs = storage->get_data<event_mcshower>("mcreco");
    if (!ev_mcs) { std::cout << "FAIL!" << std::endl; return false; }

    //Find how many are really cool nu_e CC events in MC Truth
    for(auto const& event : *ev_mctruth){

      if(ev_mctruth->size() > 1) std::cout << "JOSEPH CONFUSED" << std::endl;

      ctr_reset();

      mctruth = 1;
      mcshower = 0;

      auto neutrino = event.GetNeutrino();

      if(neutrino.CCNC() == 0){nu_CC = 1;}
      else{nu_CC = 0;}

      nu_pdg = neutrino.Nu().PdgCode();

      if(TPC.Contain(neutrino.Nu().Trajectory().at(0).Position()))
	nu_ina = 1;
      else
	nu_ina = 0;      

      if(nu_ina && ((nu_CC && abs(nu_pdg) == 12) || (neutrino.InteractionType() == 1098))) N_truth++;
      
      tree->Fill();
      
    }


    //Look at MCShowers and see if I can select these events
    int erm = 0;
    double x = 0, y = 0, z = 0, E = 0, id = 0, n_dau = 0, id_mom = 0, pdg_mom = 0, pdg_anc = 0, id_anc= 0;
    std::string proc ="", proc1 = ""; 
    double x1 = 0, y1 = 0, z1 = 0, E1 = 0, id1 = 0, n_dau1 = 0, id_mom1 = 0, pdg_mom1 = 0, pdg_anc1 = 0, id_anc1 = 0;
  
    int event_ccnc = 0, event_IntType = 0, event_nuPDG = 0;
    

    for(auto const& shower : *ev_mcs){

      ctr_reset();

      for(auto const& shower2 : *ev_mcs){	  
	if(TPC.Contain(shower2.Start().Position()) &&  abs(shower2.PdgCode()) == 11 && abs(shower2.AncestorPdgCode()) == 11)
	  N_e++;
      }
      
      mctruth = 0;
      mcshower = 1;

      mcs_pdg = shower.PdgCode(); 

      mcs_anc = shower.AncestorPdgCode();
      
      if(TPC.Contain(shower.Start().Position()))
	mcs_ina = 1;
      else
	mcs_ina = 0;

      if(mcs_ina && abs(mcs_pdg) == 11 && abs(mcs_anc) == 11 && N_e == 1){ N_shower++; erm++;      
      
	if(erm == 1){
	  
	  x = shower.Start().Position().X();
	  y = shower.Start().Position().Y();
	  z = shower.Start().Position().Z();
	  E = shower.Start().Momentum().E();
	  id = shower.TrackID();
	  n_dau = shower.DaughterTrackID().size();
	  id_mom = shower.MotherTrackID();
	  pdg_mom = shower.MotherPdgCode();	
	  pdg_anc = shower.AncestorPdgCode();
	  id_anc = shower.AncestorTrackID();
	  proc = shower.Process();
	}
	
	if(erm == 2){
	  
	  x1 = shower.Start().Position().X();
	  y1 = shower.Start().Position().Y();
	  z1 = shower.Start().Position().Z();
	  E1 = shower.Start().Momentum().E();
	  id1 = shower.TrackID();
	  n_dau1 =shower.DaughterTrackID().size();
	  id_mom1 =shower.MotherTrackID();	
	  pdg_mom1 = shower.MotherPdgCode();
	  pdg_anc1 = shower.AncestorPdgCode();
	  id_anc1 = shower.AncestorTrackID();  
	  proc1 = shower.Process();	  



	  
	  for(auto const& event : *ev_mctruth){
	    	    
	      auto neutrino = event.GetNeutrino();	      
	      event_ccnc = neutrino.CCNC(); 
	      event_IntType = neutrino.InteractionType(); 
	      event_nuPDG = neutrino.Nu().PdgCode();

	      for(auto const& part : event.GetParticles()){
		

		if(abs(part.PdgCode()) == 11){
		  std::cout << "Event ID : " << ev_mctruth->event_id() << std::endl;
		  std::cout << "electron E : " << part.Trajectory().at(0).Momentum().E() << std::endl;
		  std::cout << "Mother ID  : " << event.GetParticles().at(part.Mother()).PdgCode() << std::endl;

		 //		  std::cout << "Electron Matched!" << std::endl;

		}
		
	      }

	      std::cout << "\n\t Event Dump : " << std::endl;
	      std::cout << "\t \t  CC or NC : " << event_ccnc << std::endl;
	      std::cout << "\t \t  nu PDG ID: " << event_nuPDG << std::endl;
	      std::cout << "\t \t  Int Type : " << event_IntType << std::endl;

	  }

	}
	
      }
      tree->Fill();

    }

      if(erm > 1){ 
	std::cout << " Number of  " << erm <<  std::endl;
	
      	std::cout << "\n\t First Shower : " << std::endl;
	std::cout << "\t \t  Start X: " << x << std::endl; 
	std::cout << "\t \t  Start Y: " << y << std::endl; 
	std::cout << "\t \t  Start Z: " << z << std::endl; 
	std::cout << "\t \t  Energy : " << E << std::endl; 
	std::cout << "\t \t  Trk ID : " << id << std::endl; 
	std::cout << "\t \t  N Daug : " << n_dau << std::endl;
	std::cout << "\t \t  Mom ID : " << id_mom << std::endl;
	std::cout << "\t \t  Mom PDG: " << pdg_mom << std::endl;
	std::cout << "\t \t  Anc ID : " << id_anc << std::endl;
	std::cout << "\t \t  Anc PDG: " << pdg_anc << std::endl;
	std::cout << "\t \t  Process: " << proc << std::endl;

	std::cout << "\n\t Second Shower : " << std::endl;
	std::cout << "\t \t  Start X: " << x1 << std::endl; 
	std::cout << "\t \t  Start Y: " << y1 << std::endl; 
	std::cout << "\t \t  Start Z: " << z1 << std::endl; 
	std::cout << "\t \t  Energy : " << E1 << std::endl; 
	std::cout << "\t \t  Trk ID : " << id1 << std::endl;        
	std::cout << "\t \t  N Daug : " << n_dau1 << std::endl;
	std::cout << "\t \t  Mom ID : " << id_mom1 << std::endl;
	std::cout << "\t \t  Mom PDG: " << pdg_mom1 << std::endl;
	std::cout << "\t \t  Anc ID : " << id_anc1 << std::endl;
	std::cout << "\t \t  Anc PDG: " << pdg_anc1 << std::endl;
	std::cout << "\t \t  Process: " << proc1 << std::endl;

      }
       
    return true;
  }

  bool MCShower_Hunt::finalize() {

    std::cout << " Number of MCTruth  (#nu_e CC + #nu-e NC) interactions : " << N_truth  << std::endl;
    std::cout << " Number of MCShower (#nu_e CC + #nu-e NC) interactions : " << N_shower << std::endl;
    
    if(_fout)
      tree->Write();  
    return true;
  }

}
#endif
