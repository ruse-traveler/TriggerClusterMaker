// ----------------------------------------------------------------------------
// 'Fun4All_TestTriggerClusterMaker.C'
// Derek Anderson
// 05.16.2024
//
// A small Fun4All macro to test the 'TriggerClusterMaker' module.
// ----------------------------------------------------------------------------

#define FUN4ALL_TESTTRIGGERCLUSTERMAKER_C

// c++ utilities
#include <string>
// calotrigger utilities
#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloWaveformProcessing.h>
#include <calotrigger/LL1PacketGetter.h>
#include <calotrigger/CaloTriggerEmulator.h>
// ffa modules
#include <ffamodules/FlagHandler.h>
#include <ffamodules/CDBInterface.h>
// fun4all libraries
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
// phool utilities
#include <phool/recoConsts.h>
// module definitions
#include </sphenix/user/danderson/install/include/triggerclustermaker/TriggerClusterMaker.h>

R__LOAD_LIBRARY(libcalo_io.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(/sphenix/user/danderson/install/lib/libtriggerclustermaker.so)



// macro body -----------------------------------------------------------------

void Fun4All_TestTriggerClusterMaker(
  const int runnumber = 41725,
  const int nEvents = 10,
  const int verbosity = 5,
  const std::string inFile = "input/DST_PRDF-00041725-0000.root"
) {

  // options ------------------------------------------------------------------

  // tower builder options
  const uint32_t    nSamples = 12;
  const std::string prefix   = "WAVEFORM_";

  // twer builder type: need waveforms for trigger primitives
  CaloTowerDefs::BuilderType build = CaloTowerDefs::kPRDFWaveform;

  // trigger cluster maker options
  TriggerClusterMakerConfig cfg_maker {
    .debug      = true,
    .saveToNode = false,
    .saveToFile = false
  };

  // initialize f4a -----------------------------------------------------------

  Fun4AllServer* f4a = Fun4AllServer::instance();
  CDBInterface*  cdb = CDBInterface::instance();
  recoConsts*    rc  = recoConsts::instance();
  f4a -> Verbosity(verbosity);
  cdb -> Verbosity(verbosity);

  // grab lookup tables
  rc -> set_StringFlag("CDB_GLOBALTAG", "ProdA_2023");
  rc -> set_uint64Flag("TIMESTAMP", runnumber);

  // register inputs/outputs --------------------------------------------------

  Fun4AllPrdfInputManager* input = new Fun4AllPrdfInputManager("InputPrdfManager");
  input -> fileopen(inFile);
  f4a   -> registerInputManager(input);

  // register subsystem reco modules ------------------------------------------

  // place LL1 onto NodeTree for jet LL1 board
  LL1PacketGetter* lloJetGetter = new LL1PacketGetter("LL1PACKETGETTER_JET", "JET", "HCAL");
  lloJetGetter -> Verbosity(verbosity);
  f4a          -> registerSubsystem(lloJetGetter);

  // likewise for emcal LL1 boards
  LL1PacketGetter* lloEMCalGetter = new LL1PacketGetter("LL1PACKETGETTER_EMCAL", "NONE", "EMCAL");
  lloEMCalGetter -> Verbosity(verbosity);
  f4a            -> registerSubsystem(lloEMCalGetter);

  // build emcal towers  
  CaloTowerBuilder* emBuilder = new CaloTowerBuilder("EMCALBUILDER");
  emBuilder -> set_detector_type(CaloTowerDefs::CEMC);
  emBuilder -> set_processing_type(CaloWaveformProcessing::FAST);
  emBuilder -> set_builder_type(build);
  emBuilder -> set_outputNodePrefix(prefix);
  emBuilder -> set_nsamples(nSamples);
  f4a       -> registerSubsystem(emBuilder);

  // build inner hcal towers
  CaloTowerBuilder* ihBuilder = new CaloTowerBuilder("HCALINBUILDER");
  ihBuilder -> set_detector_type(CaloTowerDefs::HCALIN);
  ihBuilder -> set_processing_type(CaloWaveformProcessing::FAST);
  ihBuilder -> set_builder_type(build);
  ihBuilder -> set_outputNodePrefix(prefix);
  ihBuilder -> set_nsamples(nSamples);
  f4a       -> registerSubsystem(ihBuilder);

  // build outer hcal towers
  CaloTowerBuilder* ohBuilder = new CaloTowerBuilder("HCALOUTBUILDER");
  ohBuilder -> set_detector_type(CaloTowerDefs::HCALOUT);
  ohBuilder -> set_processing_type(CaloWaveformProcessing::FAST);
  ohBuilder -> set_builder_type(build);
  ohBuilder -> set_outputNodePrefix(prefix);
  ohBuilder -> set_nsamples(nSamples);
  f4a       -> registerSubsystem(ohBuilder);

  // finally, make trigger clusters
  TriggerClusterMaker* maker = new TriggerClusterMaker("TriggerClusterMaker");
  maker -> SetConfig(cfg_maker);
  f4a   -> registerSubsystem(maker);

  // run modules and exit -----------------------------------------------------

  // run4all
  f4a -> run(nEvents);
  f4a -> End();
  delete f4a;

  // exit
  gSystem -> Exit(0);
  return;

}

// end ------------------------------------------------------------------------
