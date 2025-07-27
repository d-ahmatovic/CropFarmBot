#ifndef CFB_TASKS_HEADER
#define CFB_TASKS_HEADER

#include "botcraft/AI/BehaviourClient.hpp"
#include "botcraft/AI/Status.hpp"


#define CLIENT_BLACKBOARD_BASE      "CropFarmTasks.Base"
#define CLIENT_BLACKBOARD_CHEST     "CropFarmTasks.StorageUnit"
#define CLIENT_BLACKBOARD_BED       "CropFarmTasks.Bed"
#define CLIENT_BLACKBOARD_RADIUS    "CropFarmTasks.Radius"
#define CLIENT_BLACKBOARD_CROPS     "CropFarmTasks.Crops"
#define CLIENT_BLACKBOARD_TARGETS   "CropFarmTasks.TargetCrops"


Botcraft::Status FindAllCrops(Botcraft::BehaviourClient& client);
Botcraft::Status OptimizeCropHarvestPath(Botcraft::BehaviourClient& client);
Botcraft::Status FarmAndReplantCrops(Botcraft::BehaviourClient& client);
Botcraft::Status UnloadInventoryToChest(Botcraft::BehaviourClient& client);
Botcraft::Status IsInventoryFilled(Botcraft::BehaviourClient& client);


struct CropData
{
    Botcraft::Position Position;
    std::string_view CropName;
};

#endif
