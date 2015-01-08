/**
 * \file MCShower_Hunt.h
 *
 * \ingroup MCShower_Hunt
 * 
 * \brief Class def header for a class MCShower_Hunt
 *
 * @author jzennamo
 */

/** \addtogroup MCShower_Hunt

    @{*/

#ifndef LARLITE_MCSHOWER_HUNT_H
#define LARLITE_MCSHOWER_HUNT_H

#include "Analysis/ana_base.h"
#include "../../BasicTool/GeoAlgo/GeoAABox.h"
#include "LArUtil/Geometry.h"

namespace larlite {
  /**
     \class MCShower_Hunt
     User custom analysis class made by jzennamo
   */
  class MCShower_Hunt : public ana_base{
  
  public:

    /// Default constructor
    MCShower_Hunt(){ _name="MCShower_Hunt"; _fout=0;};

    /// Default destructor
    virtual ~MCShower_Hunt(){};

    /** IMPLEMENT in MCShower_Hunt.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShower_Hunt.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in MCShower_Hunt.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void ctr_reset();
    
    /** TPC Definition

	Define TPC
	
    */
    geoalgo::AABox TPC;

    protected:

    /** Variable Definitions

	;lshkt
	
    */

    int nu_CC;
    int nu_pdg;
    int nu_ina;
    int mcs_pdg;
    int mcs_ina;
    int mcs_anc; 

    int mctruth;
    int mcshower;
    int N_e;

    int N_shower = 0, N_truth = 0; 
    
    TTree* tree;
    

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
