#ifndef RECOSHOWER_HUNT_CXX
#define RECOSHOWER_HUNT_CXX

#include "RecoShower_Hunt.h"

namespace larlite {

  bool RecoShower_Hunt::initialize() {

    TPC.Min(0,
            -(::larutil::Geometry::GetME()->DetHalfHeight()),
            0);

    TPC.Max(2*(::larutil::Geometry::GetME()->DetHalfWidth()),
            ::larutil::Geometry::GetME()->DetHalfHeight(),
            ::larutil::Geometry::GetME()->DetLength());

    tree = new TTree("tree","");
    tree->Branch("event_num",&event_num,"event_num/I");
    tree->Branch("shower_num",&shower_num,"shower_num/I");
    tree->Branch("ina",&ina,"ina/I");
    tree->Branch("e_like",&e_like,"e_like/I");
    tree->Branch("N_e",&N_e,"N_e/I");

    // load dEdx training results
    _alg_emp.LoadParams();

    return true;
  }
  
  void RecoShower_Hunt::ctr_reset(){


    shower_num = 0; 
    ina = 0;
    e_like = 0; 
    N_e = 0;

  }

  bool RecoShower_Hunt::analyze(storage_manager* storage) {
  
    std::cout << " yo yo yo bitches" << std::endl;


    auto ev_shower  = storage->get_data<event_shower>("showerreco");
    if (!ev_shower) { std::cout << "FAIL!" << std::endl; return false; }

    event_num++;
    shower_num = 0;
    
    for(auto const& shower : *ev_shower){
      
      shower_num++;

      if(TPC.Contain(shower.ShowerStart()))
	ina = 1;
      else
	ina = 0;


      std::cout << "DO I Kill you ?!" << std::endl; 

      std::cout << "shr dEdx: " << shower.dEdx()[2] << std::endl;
      
      e_like = (_alg_emp.LL(true, shower.dEdx()[2], -1.) >
		_alg_emp.LL(false,shower.dEdx()[2], -1.)) ? 1 : 0; 

      std::cout << "no I don't." << std::endl;
       
      for(auto const& shower2 : *ev_shower){
	
	int e_like2 = (_alg_emp.LL(true, shower2.dEdx()[2], -1.) >
		       _alg_emp.LL(false,shower2.dEdx()[2], -1.)) ? 1 : 0;
	
	if(TPC.Contain(shower2.ShowerStart()) && e_like2) N_e++;
	
      }
      
      std::cout << "NUMBER OF ELECTRONS " << std::endl;
      tree->Fill();
      
    }
    
    return true;
  }

  bool RecoShower_Hunt::finalize() {

    if(_fout)
      tree->Write();

    return true;
  }

}
#endif
