#include "botcraft/AI/BehaviourClient.hpp"
#include "botcraft/AI/Status.hpp"


Botcraft::Status FindAllCrops(Botcraft::BehaviourClient& client, const int radius);
Botcraft::Status IdentifyNecessarySeeds(Botcraft::BehaviourClient& client);
Botcraft::Status FarmAndReplantCrops(Botcraft::BehaviourClient& client);