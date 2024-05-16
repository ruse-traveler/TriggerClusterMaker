// ----------------------------------------------------------------------------
// 'TriggerClusterMaker.cc'
// Derek Anderson
// 05.15.2024
//
// A Fun4All QA module to construct trigger clusters,
// jet patches stored in RawCluster objects, for
// downstream analysis
// ----------------------------------------------------------------------------

#define TRIGGERCLUSTERMAKER_CC

// f4a libraries
#include <fun4all/Fun4AllReturnCodes.h>
// phool libraries
#include <phool/PHCompositeNode.h>

// module definition
#include "TriggerClusterMaker.h"



// ctor/dtor ------------------------------------------------------------------

TriggerClusterMaker::TriggerClusterMaker(const std::string &name) : SubsysReco(name) {

  // print debug message
  if (m_config.doDebug && (Verbosity() > 0)) {
    std::cout << "TriggerClusterMaker::TriggerClusterMaker(const std::string &name) Calling ctor" << std::endl;
  }

  /* nothing to do */

}  // end ctor



TriggerClusterMaker::~TriggerClusterMaker() {

  // print debug message
  if (m_config.doDebug && (Verbosity() > 0)) {
    std::cout << "TriggerClusterMaker::~TriggerClusterMaker() Calling dtor" << std::endl;
  }

  /* nothing to do */

}  // end dtor



// fun4all methods ------------------------------------------------------------

int TriggerClusterMaker::Init(PHCompositeNode* topNode) {

  if (m_config.doDebug) {
    std::cout << "TriggerClusterMaker::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  }

  /* TODO fill in */

  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'Init(PHCompositeNode*)'



int TriggerClusterMaker::process_event(PHCompositeNode* topNode) {

  if (m_config.doDebug) {
    std::cout << "TriggerClusterMaker::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
  }

  /* TODO fill in */

  return Fun4AllReturnCodes::EVENT_OK;


}  // end 'process_event(PHCompositeNode*)'



int TriggerClusterMaker::End(PHCompositeNode *topNode) {

  if (m_config.doDebug) {
    std::cout << "TriggerClusterMaker::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  }

  /* TODO fill in */

  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'End(PHCompositeNode*)'



// private methods ------------------------------------------------------------

void TriggerClusterMaker::GrabNodes(PHCompositeNode* topNode) {

  // print debug message
  if (m_config.doDebug && (Verbosity() > 0)) {
    std::cout << "TriggerClusterMaker::GrabNodes(PHCompositeNode*) Grabbing input nodes" << std::endl;
  }

  /* TODO fill in */
  return;

}  // end 'GrabNodes(PHCompositeNode*)'

// end ------------------------------------------------------------------------
