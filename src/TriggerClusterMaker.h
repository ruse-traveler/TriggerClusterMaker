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
#include <string>
#include <utility>
#include <vector>
// f4a libraries
#include <fun4all/SubsysReco.h>

// forward declarations
class Fun4AllHistoManager;
class JetContainer;
class PHCompositeNode;
class TH1;



// TriggerClusterMaker definition ---------------------------------------------

class TriggerClusterMaker : public SubsysReco {

  public:

    // ctor
    TriggerClusterMaker(const std::string &name = "TriggerClusterMaker");
    ~TriggerClusterMaker() override;

    // inline public methods
    void Configure(TriggerClusterMakerConfig config) {m_config = config;}

    // f4a methods
    int Init(PHCompositeNode *topNode)          override;
    int process_event(PHCompositeNode *topNode) override;
    int End(PHCompositeNode *topNode)           override;

  private:

    // private methods
    void GrabNodes(PHCompositeNode* topNode);

    // f4a memebers
    std::shared_ptr<Fun4AllHistManager> m_manager = NULL;
    std::shared_ptr<JetContainer>       m_jetMap  = NULL;

    // output histograms
    std::vector<std::unique_ptr<TH1D>> m_vecHist;

};

#endif

// end ------------------------------------------------------------------------
