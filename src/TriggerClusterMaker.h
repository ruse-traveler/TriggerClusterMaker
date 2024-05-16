// ----------------------------------------------------------------------------
// 'TriggerClusterMaker.h'
// Derek Anderson
// 05.15.2024
//
// A Fun4All QA module to construct trigger clusters,
// jet patches stored in RawCluster objects, for
// downstream analysis
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
// root libraries
#include <TFile.h>
#include <TTree.h>

// module configuration
#include "TriggerClusterMakerConfig.h"

// forward declarations
class LL1Out;
class PHCompositeNode;
class RawCluster;
class TriggerPrimitiveContainer;



// TriggerClusterMaker definition ---------------------------------------------

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

    // private methods
    void InitOutFile();
    void InitOutTree();
    void InitOutNode(PHCompositeNode* topNode);
    void GrabNodes(PHCompositeNode* topNode);

    // f4a members
    std::vector<LL1Out*>                    m_inLL1Nodes;
    std::vector<TriggerPrimitiveContainer*> m_inPrimNodes;

    // output members
    std::unique_ptr<TFile>               m_outFile       = NULL;
    std::unique_ptr<TTree>               m_outTree       = NULL;
/* TODO implement
    std::unique_ptr<RawClusterContainer> m_outClustStore = NULL;
*/

    // module configuration
    TriggerClusterMakerConfig m_config;

};

#endif

// end ------------------------------------------------------------------------
