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



  // methods ------------------------------------------------------------------

  // FIXME deprecate once mapping is correct
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

  }  // end 'GetEtaBin(uint32_t, int)'

}  // end TriggerClusterMakerDefs namespace

#endif

// end ------------------------------------------------------------------------
