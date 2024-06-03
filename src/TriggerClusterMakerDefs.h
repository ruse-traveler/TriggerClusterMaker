// ----------------------------------------------------------------------------
/*! \file    TriggerClusterMakerDefs.h'
 *  \authors Derek Anderson
 *  \date    05.29.2024
 *
 *  Miscellaneous definitions for the TriggerClusterMaker
 *  fun4all module
 */
// ----------------------------------------------------------------------------

#ifndef TRIGGERCLUSTERMAKERDEFS_H
#define TRIGGERCLUSTERMAKERDEFS_H

// c++ utilities
#include <limits>
// calo base
#include <calobase/TowerInfoDefs.h>
// trigger libraries
//   - TODO use local paths when ready
#include <calotrigger/TriggerDefs.h>



// ----------------------------------------------------------------------------
//! Miscellaneous tools for the TriggerClusterMaker module
// ----------------------------------------------------------------------------
namespace TriggerClusterMakerDefs {

  // enums --------------------------------------------------------------------

  // eta vs. phi bin
  enum Bin {
    Eta,
    Phi
  };

  // trigger info type
  enum Type {
    Prim,
    LL1
  };



  // constants ----------------------------------------------------------------

  // --------------------------------------------------------------------------
  //! No. of HCal towers (EMCal retowers) along eta of jet patch
  // --------------------------------------------------------------------------
  inline uint32_t NEtaInJet() {
    static const uint32_t nEtaInJet = 4;
    return nEtaInJet;
  }

  // --------------------------------------------------------------------------
  //! No. of HCal towers (EMCal retowers) along phi of jet patch
  // --------------------------------------------------------------------------
  inline uint32_t NPhiInJet() {
    static const uint32_t nPhiInJet = 4;
    return nPhiInJet;
  }



  // methods ------------------------------------------------------------------

  // --------------------------------------------------------------------------
  //! Calculate eta/phi bin manually based on sum key
  // --------------------------------------------------------------------------
  uint32_t GetBinManually(const uint32_t sumkey, const int bin = Bin::Eta, const int type = Type::Prim) {

    // get relevant sum and primitive IDs
    uint32_t sumID;
    uint32_t primID;
    switch (bin) {
      case Bin::Eta:
        sumID  = TriggerDefs::getSumEtaId(sumkey);
        primID = TriggerDefs::getPrimitiveEtaId_from_TriggerSumKey(sumkey);
        break;
      case Bin::Phi:
        sumID  = TriggerDefs::getSumPhiId(sumkey);
        primID = TriggerDefs::getPrimitivePhiId_from_TriggerSumKey(sumkey);
        break;
      default:
        sumID  = std::numeric_limits<uint32_t>::max();
        primID = std::numeric_limits<uint32_t>::max();
        break;
    }

    // get relevant segmentation
    uint32_t segment;
    switch (type) {
      case Type::LL1:
        segment = 2;
        break;
      case Type::Prim:
        [[fallthrough]];
      default:
        segment = 4;
        break;
    }
    return sumID + (segment * primID);

  }  // end 'GetEtaBin(uint32_t, int, int)'



  // --------------------------------------------------------------------------
  //! Get tower key based on provided eta, phi indices
  // --------------------------------------------------------------------------
  uint32_t GetKeyFromEtaPhiIndex(
    const uint32_t eta,
    const uint32_t phi,
    const TriggerDefs::DetectorId detector
  ) {

    uint32_t key;
    switch (detector) {

      // get emcal tower index
      case TriggerDefs::DetectorId::emcalDId:
        key = TowerInfoDefs::encode_emcal(eta, phi);
        std::cout << "    TEST index = " << TowerInfoDefs::decode_emcal(key);
        break;

      // get hcal tower index
      case TriggerDefs::DetectorId::hcalinDId:
        [[fallthrough]];
      case TriggerDefs::DetectorId::hcaloutDId:
        [[fallthrough]];
      case TriggerDefs::DetectorId::hcalDId:
        key = TowerInfoDefs::encode_hcal(eta, phi);
        std::cout << "    TEST index = " << TowerInfoDefs::decode_hcal(key);
        break;

      // otherwise return dummy value
      default:
        key = std::numeric_limits<uint32_t>::max();
        break;
    }
    return key;

  } //  end 'GetKeyFromEtaPhiIndex(uint32_t, uint32_t, TriggerDefs::DetectorId)'


}  // end TriggerClusterMakerDefs namespace

#endif

// end ------------------------------------------------------------------------
