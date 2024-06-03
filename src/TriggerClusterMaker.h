// ----------------------------------------------------------------------------
/*! \file    TriggerClusterMaker.h'
 *  \authors Derek Anderson
 *  \date    05.15.2024
 *
 *  A Fun4All QA module to construct trigger clusters,
 *  jet patches stored in RawCluster objects, for
 *  downstream analysis
 */
// ----------------------------------------------------------------------------

#ifndef TRIGGERCLUSTERMAKER_H
#define TRIGGERCLUSTERMAKER_H

// c++ utilities
#include <array>
#include <string>
#include <utility>
#include <vector>
// calo base
#include <calobase/RawClusterContainer.h>
// f4a libraries
#include <fun4all/SubsysReco.h>
// module utilities
#include "TriggerClusterMakerDefs.h"

// forward declarations
class LL1Out;
class PHCompositeNode;
class RawCluster;
class TowerInfoContainer;
class TriggerPrimitiveContainer;



// ----------------------------------------------------------------------------
//! Options for TriggerClusterMaker module
// ----------------------------------------------------------------------------
struct TriggerClusterMakerConfig {

  // general options
  bool debug = true;

  // output options
  std::string outNodeName = "TriggerClusters";

  // input nodes
  std::vector<std::string> inTowerNodes = {
    "TOWERINFO_CALIB_CEMC",
    "TOWERINFO_CALIB_HCALIN",
    "TOWERINFO_CALIB_HCALOUT"
  };
  std::vector<std::string> inLL1Nodes = {
    "LL1OUT_JET"
  };
  std::vector<std::string> inPrimNodes = {
    "TRIGGERPRIMITIVES_JET",
    "TRIGGERPRIMITIVES_EMCAL",
    "TRIGGERPRIMITIVES_EMCAL_LL1",
    "TRIGGERPRIMITIVES_HCAL_LL1",
    "TRIGGERPRIMITIVES_HCALIN",
    "TRIGGERPRIMITIVES_HCALOUT"
  };

};



// ----------------------------------------------------------------------------
//! Makes Trigger Cluster
// ----------------------------------------------------------------------------
/*! This Fun4all modules ingests calorimeter triggers and
 *  trigger primitives to turn them into RawCluster objects,
 *  i.e. "Trigger Clusters", for downstream analysis. Output
 *  clusters can be placed on the node tree, or saved to
 *  a TTree in a specified output file.
 */
class TriggerClusterMaker : public SubsysReco {

  public:

    // ctor
    TriggerClusterMaker(const std::string& name = "TriggerClusterMaker");
    ~TriggerClusterMaker() override;

    // setters
    void SetConfig(const TriggerClusterMakerConfig& config) {m_config = config;}

    // getters
    TriggerClusterMakerConfig GetConfig() {return m_config;}

    // f4a methods
    int Init(PHCompositeNode* topNode)          override;
    int process_event(PHCompositeNode* topNode) override;
    int End(PHCompositeNode* topNode)           override;

  private:

    // private methods
    void InitOutNode(PHCompositeNode* topNode);
    void GrabNodes(PHCompositeNode* topNode);
    void MakeCluster(LL1Out* trigger);
    void MakeCluster(TriggerPrimitive* trigger, TriggerDefs::DetectorId detector);

    // input nodes
    std::vector<TowerInfoContainer*>        m_inTowerNodes;
    std::vector<LL1Out*>                    m_inLL1Nodes;
    std::vector<TriggerPrimitiveContainer*> m_inPrimNodes;

    // output node
    RawClusterContainer* m_outClustNode = NULL;

    // module configuration
    TriggerClusterMakerConfig m_config;

};

#endif

// end ------------------------------------------------------------------------
