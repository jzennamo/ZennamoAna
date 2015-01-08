/**
 * \file RecoShower_Hunt.h
 *
 * \ingroup RecoShower_Hunt
 * 
 * \brief Class def header for a class RecoShower_Hunt
 *
 * @author jzennamo
 */

/** \addtogroup RecoShower_Hunt

    @{*/

#ifndef LARLITE_RECOSHOWER_HUNT_H
#define LARLITE_RECOSHOWER_HUNT_H

#include "Analysis/ana_base.h"
#include "GeoAlgo/GeoAABox.h"
#include "LArUtil/Geometry.h"
#include "SPTBase/SPAlgoBase.h"
#include "SPAlgo/SPAlgoEMPart.h"

namespace larlite {
  /**
     \class RecoShower_Hunt
     User custom analysis class made by jzennamo
   */
  class RecoShower_Hunt : public ana_base{
  
  public:

    /// Default constructor
    RecoShower_Hunt(){ _name="RecoShower_Hunt"; _fout=0;};

    /// Default destructor
    virtual ~RecoShower_Hunt(){};

    /** IMPLEMENT in RecoShower_Hunt.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in RecoShower_Hunt.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in RecoShower_Hunt.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void ctr_reset();

    geoalgo::AABox TPC;


    protected:

    ::sptool::SPAlgoEMPart _alg_emp;

    int event_num;
    int shower_num;
    int ina;
    int e_like;
    int N_e;

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
