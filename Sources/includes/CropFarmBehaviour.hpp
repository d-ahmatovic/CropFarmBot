#ifndef CFB_BEHAVIOUR_HEADER
#define CFB_BEHAVIOUR_HEADER

#include <memory>

#include <botcraft/AI/SimpleBehaviourClient.hpp>

#include "CropFarmTasks.hpp"


std::shared_ptr<Botcraft::BehaviourTree<Botcraft::SimpleBehaviourClient>> CreateSleepTree();
std::shared_ptr<Botcraft::BehaviourTree<Botcraft::SimpleBehaviourClient>> SearchAndOptimizeCropsTree();
std::shared_ptr<Botcraft::BehaviourTree<Botcraft::SimpleBehaviourClient>> HarvestCropsTree();
std::shared_ptr<Botcraft::BehaviourTree<Botcraft::SimpleBehaviourClient>> StoreCropsTree();


#endif
