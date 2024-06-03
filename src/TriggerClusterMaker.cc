// ----------------------------------------------------------------------------
/*! \file    TriggerClusterMaker.cc'
 *  \authors Derek Anderson
 *  \date    05.15.2024
 *
 *  A Fun4All QA module to construct trigger clusters,
 *  jet patches stored in RawCluster objects, for
 *  downstream analysis
 */
// ----------------------------------------------------------------------------

#define TRIGGERCLUSTERMAKER_CC

// c++ utiilites
#include <algorithm>
#include <cassert>
#include <iostream>
// calo base
#include <calobase/RawClusterv1.h>
#include <calobase/TowerInfo.h>
#include <calobase/TowerInfoContainer.h>
// trigger libraries
#include <calotrigger/LL1Out.h>
#include <calotrigger/LL1Outv1.h>
#include <calotrigger/TriggerPrimitiveContainer.h>
#include <calotrigger/TriggerPrimitiveContainerv1.h>
// f4a libraries
#include <fun4all/Fun4AllReturnCodes.h>
// phool libraries
#include <phool/getClass.h>
#include <phool/phool.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHObject.h>

// module definition
#include "TriggerClusterMaker.h"



// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Module constructor
// ----------------------------------------------------------------------------
TriggerClusterMaker::TriggerClusterMaker(const std::string &name) : SubsysReco(name) {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "TriggerClusterMaker::TriggerClusterMaker(const std::string &name) Calling ctor" << std::endl;
  }

  // make sure vectors are clear
  m_inTowerNodes.clear();
  m_inLL1Nodes.clear();
  m_inPrimNodes.clear();

}  // end ctor



// ----------------------------------------------------------------------------
//! Module destructor
// ----------------------------------------------------------------------------
TriggerClusterMaker::~TriggerClusterMaker() {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "TriggerClusterMaker::~TriggerClusterMaker() Calling dtor" << std::endl;
  }

  /* nothing to do */

}  // end dtor



// fun4all methods ============================================================

// ----------------------------------------------------------------------------
//! Initialize module
// ----------------------------------------------------------------------------
int TriggerClusterMaker::Init(PHCompositeNode* topNode) {

  if (m_config.debug) {
    std::cout << "TriggerClusterMaker::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  }

  // initialize outputs
  InitOutNode(topNode);
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'Init(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Grab inputs and build trigger clusters
// ----------------------------------------------------------------------------
int TriggerClusterMaker::process_event(PHCompositeNode* topNode) {

  if (m_config.debug) {
    std::cout << "TriggerClusterMaker::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
  }

  // grab input nodes
  GrabNodes(topNode);

  // loop over LL1 nodes
  LL1Outv1::Range lloWordRange;
  for (auto inLL1Node : m_inLL1Nodes) {

    // loop over trigger words
    lloWordRange = inLL1Node -> getTriggerWords();
    for (
      LL1Outv1::Iter itTrgWord = lloWordRange.first;
      itTrgWord != lloWordRange.second;
      ++itTrgWord
    ) {

      // FIXME figure out how to get trigger
      //   - primitives from LL1Out
      //auto word = (*itTrgWord).second;
      //MakeCluster(word);

    }  // end trigger word loop
  }  // end LL1 node loop


  // loop over trigger primitive nodes
  TriggerPrimitiveContainerv1::Range trgPrimStoreRange;
  for (auto inPrimNode : m_inPrimNodes) {

    // loop over primitives
    trgPrimStoreRange = inPrimNode -> getTriggerPrimitives();
    for (
      TriggerPrimitiveContainerv1::Iter itTrgPrim = trgPrimStoreRange.first;
      itTrgPrim != trgPrimStoreRange.second;
      ++itTrgPrim
    ) {

      // grab trigger primitve
      TriggerPrimitive* primitive = (*itTrgPrim).second;
      MakeCluster(primitive, TriggerDefs::DetectorId::hcaloutDId);

    }  // end trigger primitive loop
  }  // end trigger primitive node loop

  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'process_event(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Run final calculations
// ----------------------------------------------------------------------------
int TriggerClusterMaker::End(PHCompositeNode *topNode) {

  if (m_config.debug) {
    std::cout << "TriggerClusterMaker::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  }

  /* TODO fill in */

  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'End(PHCompositeNode*)'



// private methods ============================================================

// ----------------------------------------------------------------------------
//! Create output node on node tree
// ----------------------------------------------------------------------------
void TriggerClusterMaker::InitOutNode(PHCompositeNode* topNode) {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "TriggerClusterMaker::InitOutNode(PHCompositeNode*) Creating output node" << std::endl;
  }

  // find dst node
  //   - if missing, abort
  PHNodeIterator   itNode(topNode);
  PHCompositeNode* dstNode = static_cast<PHCompositeNode*>(itNode.findFirst("PHCompositeNode", "DST"));
  if (!dstNode) {
    std::cerr << PHWHERE << ": PANIC! DST node missing! Aborting!" << std::endl;
    assert(dstNode);
  }

  // find or add LL1 node
  PHNodeIterator   itTrg(dstNode);
  PHCompositeNode* trgNode = static_cast<PHCompositeNode*>(itNode.findFirst("PHCompositeNode", "LL1"));
  if (!trgNode) {
    PHCompositeNode* trgNodeToAdd = new PHCompositeNode("LL1");
    dstNode -> addNode(trgNodeToAdd);
    trgNode =  trgNodeToAdd;
  }

  // create container for clusters
  m_outClustNode = new RawClusterContainer();

  // and add node to tree
  PHIODataNode<PHObject>* clustNode = new PHIODataNode<PHObject>(m_outClustNode, m_config.outNodeName, "PHObject");
  if (!clustNode) {
    std::cerr << PHWHERE << ": PANIC! Couldn't create cluster node! Aborting!" << std::endl;
    assert(clustNode);
  } else {
    trgNode -> addNode(clustNode);
  }
  return;

}  // end 'InitOutNode(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Grab input nodes
// ----------------------------------------------------------------------------
void TriggerClusterMaker::GrabNodes(PHCompositeNode* topNode) {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "TriggerClusterMaker::GrabNodes(PHCompositeNode*) Grabbing input nodes" << std::endl;
  }

  // get tower info nodes
  for (const std::string& inTowerNode : m_config.inTowerNodes) {
    m_inTowerNodes.push_back(
      findNode::getClass<TowerInfoContainer>(topNode, inTowerNode)
    );
    if (!m_inTowerNodes.back()) {
      std::cerr<< PHWHERE << ": PANIC! Couldn't grab input TowerInfoContainer node '" << inTowerNode << "'!" << std::endl;
      assert(m_inTowerNodes.back());
    }
  }

  // get LL1 nodes
  for (const std::string& inLL1Node : m_config.inLL1Nodes) {
    m_inLL1Nodes.push_back(
      findNode::getClass<LL1Out>(topNode, inLL1Node)
    );
    if (!m_inLL1Nodes.back()) {
      std::cerr << PHWHERE << ": PANIC! Couldn't grab LL1Out node '" << inLL1Node << "'!" << std::endl;
      assert(m_inLL1Nodes.back());
    }
  }

  // get trigger primitive nodes
  for (const std::string& inPrimNode : m_config.inPrimNodes) {
    m_inPrimNodes.push_back(
      findNode::getClass<TriggerPrimitiveContainer>(topNode, inPrimNode)
    );
    if (!m_inPrimNodes.back()) {
      std::cerr << PHWHERE << ": PANIC! Couldn't grab TriggerPrimitive node '" << inPrimNode << "'!" << std::endl;
      assert(m_inPrimNodes.back());
    }
  }
  return;

}  // end 'GrabNodes(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Make cluster from an LL1Out object
// ----------------------------------------------------------------------------
//   - FIXME it might be good to make this return a
//     RawClusterv1...
void TriggerClusterMaker::MakeCluster(LL1Out* trigger) {

  // print debug message
  if (m_config.debug && (Verbosity() > 1)) {
    std::cout << "TriggerClusterMaker::MakeCluster(LL1Out*) Making cluster from LL1Out object" << std::endl;
  }

/* FIXME this isn't the right way of doing this...

  // loop through trigger word
  for (
    auto itWord = trigger -> begin();
    itWord != trigger -> end();
    ++itWord
  ) {

    // get eta, phi bin of LL1
    const uint64_t iEta = TriggerClusterMakerDefs::GetBinManually(
      (*itTrgWord).first,
       TriggerClusterMakerDefs::Bin::Eta,
       TriggerClusterMakerDefs::Type::LL1
    );
    const uint64_t iPhi = TriggerClusterMakerDefs::GetBinManually(
      (*itTrgWord).first,
       TriggerClusterMakerDefs::Bin::Phi,
       TriggerClusterMakerDefs::Type::LL1
    );

  }  // end trigger loop

  // TODO things will be done here... //
*/
  return;

}  // end 'MakeCluster(LL1Out*)'



// ----------------------------------------------------------------------------
//! Make cluster from a TriggerPrimitive object
// ----------------------------------------------------------------------------
//   - FIXME it might be good to make this return a
//     RawClusterv1...
void TriggerClusterMaker::MakeCluster(TriggerPrimitive* trigger, TriggerDefs::DetectorId detector) {

  // print debug message
  if (m_config.debug && (Verbosity() > 1)) {
    std::cout << "TriggerClusterMaker::MakeCluster(TriggerPrimitive*) Making cluster from TriggerPrimitive object" << std::endl;
  }

  // loop over sums
  TriggerPrimitivev1::Range trgPrimSumRange = trigger -> getSums();
  for (
    TriggerPrimitive::Iter itPrimSum = trgPrimSumRange.first;
    itPrimSum != trgPrimSumRange.second;
    ++itPrimSum
  ) {

    // create new cluster
    RawClusterv1* cluster = new RawClusterv1();

    // loop over towers in sum
    auto sum = (*itPrimSum).second;
    for (
      auto itSum = sum -> begin();
      itSum != sum -> end();
      ++itSum
    ) {

      // get eta, phi bin of sum
      const uint32_t iEta = TriggerClusterMakerDefs::GetBinManually(
        (*itPrimSum).first,
         TriggerClusterMakerDefs::Bin::Eta,
         TriggerClusterMakerDefs::Type::Prim
      );
      const uint32_t iPhi = TriggerClusterMakerDefs::GetBinManually(
        (*itPrimSum).first,
         TriggerClusterMakerDefs::Bin::Phi,
         TriggerClusterMakerDefs::Type::Prim
      );

      // then get tower index
      //   - TODO iterate through all tower indices in range
      const uint32_t key = TriggerClusterMakerDefs::GetKeyFromEtaPhiIndex(iEta, iPhi, detector);
      std::cout << "    CHECK0 (eta, phi) = (" << iEta << ", " << iPhi << "), key = " << key
                << std::endl;

    }  // end tower loop

    // put cluster in node
    m_outClustNode -> AddCluster(cluster);

  }  // end primitive sum loop
  return;

}  // end 'MakeCluster(TriggerPrimitive*, TriggerDefs::DetectorId)'

// end ------------------------------------------------------------------------
