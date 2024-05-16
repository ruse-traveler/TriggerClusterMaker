// ----------------------------------------------------------------------------
// 'TriggerClusterMakerConfig.h'
// Derek Anderson
// 05.16.2024
//
// Configuration struct for 'TriggerClusterMaker' module.
// ----------------------------------------------------------------------------

#ifndef TRIGGERCLUSTERMAKERCONFIG_H
#define TRIGGERCLUSTERMAKERCONFIG_H



struct TriggerClusterMakerConfig {

  // general options
  bool debug = true;

  // output options
  bool        saveToNode  = false;
  bool        saveToFile  = true;
  std::string outNodeName = "TriggerCluster";
  std::string outFileName = "test.root";

  // input nodes
  std::array<std::string, 2> inLL1Nodes = {
    "LL1OUT_RAW_JET",
    "LL1OUT_JET"
  };
  std::array<std::string, 8> inPrimNodes = {
    "TRIGGERPRIMITIVES_RAW_EMCAL",
    "TRIGGERPRIMITIVES_RAW_EMCAL_LL1",
    "TRIGGERPRIMITIVES_RAW_JET",
    "TRIGGERPRIMITIVES_EMCAL",
    "TRIGGERPRIMITIVES_EMCAL_LL1",
    "TRIGGERPRIMITIVES_HCALIN",
    "TRIGGERPRIMITIVES_HCALOUT",
    "TRIGGERPRIMITIVES_HCAL_LL1"
  };

};

#endif

// end ------------------------------------------------------------------------
