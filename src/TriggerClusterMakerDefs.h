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



// ----------------------------------------------------------------------------
//! Miscellaneous tools for the TriggerClusterMaker module
// ----------------------------------------------------------------------------
namespace TriggerClusterMakerDefs {

  // LL1 accessors
  enum LL1 {
    Raw,
    Jet
  };

  // trigger primitive accessors
  enum Primitive {
    RawEM,
    RawEMLL1,
    RawTrg,
    EMCal,
    EMCalLL1,
    IHCal,
    OHCal,
    HCalLL1
  };

}  // end TriggerClusterMakerDefs namespace

#endif

// end ------------------------------------------------------------------------
