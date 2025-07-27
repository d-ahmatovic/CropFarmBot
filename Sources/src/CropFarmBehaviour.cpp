#include <botcraft/AI/Tasks/AllTasks.hpp>

#include "CropFarmBehaviour.hpp"
#include "CropFarmTasks.hpp"

std::shared_ptr<Botcraft::BehaviourTree<Botcraft::SimpleBehaviourClient>> CreateSleepTree()
{
    return Botcraft::Builder<Botcraft::SimpleBehaviourClient>("Go to sleep")
        .selector() // Returns at the first child that reports back true
            .inverter().leaf("Check if night time", Botcraft::IsNightTime)

            .sequence() // Returns true if all childs return true
                .leaf(Botcraft::CopyBlackboardData, CLIENT_BLACKBOARD_BED, "GoTo.goal")
                .leaf(Botcraft::SetBlackboardData<int>, "GoTo.dist_tolerance", 1)
                .leaf(Botcraft::SetBlackboardData<int>, "GoTo.min_end_dist", 0)
                .leaf(Botcraft::SetBlackboardData<int>, "GoTo.min_end_dist_xz", 0)
                .leaf("Walk to bed", Botcraft::GoToBlackboard)

                // Right click bed and wait for day time
                .repeater(0).sequence()
                    .leaf(Botcraft::CopyBlackboardData, CLIENT_BLACKBOARD_BED, "InteractWithBlock.pos")
                    .leaf(Botcraft::SetBlackboardData<bool>, "InteractWithBlock.animation", true)
                    .leaf("Right click the bed", Botcraft::InteractWithBlockBlackboard)

                    // Wait a second
                    .repeater(100).leaf(Botcraft::Yield)
                    
                    // True cancels repeater, a sequence is only true if all leafs are true
                    // so we invert the night time to get day time
                    .inverter().leaf("Check if night time", Botcraft::IsNightTime)
                .end()
            .end()
        .end();
}


std::shared_ptr<Botcraft::BehaviourTree<Botcraft::SimpleBehaviourClient>> SearchAndOptimizeCropsTree()
{
    return Botcraft::Builder<Botcraft::SimpleBehaviourClient>("Search crops and find optimal path")
        .sequence()
            .leaf("Get harvestable crop data", FindAllCrops)
            .leaf("Optimize pathing", OptimizeCropHarvestPath)
        .end();
}


std::shared_ptr<Botcraft::BehaviourTree<Botcraft::SimpleBehaviourClient>> HarvestCropsTree()
{
    return Botcraft::Builder<Botcraft::SimpleBehaviourClient>("Harvest crops")
        .sequence()
        .end();
}


std::shared_ptr<Botcraft::BehaviourTree<Botcraft::SimpleBehaviourClient>> StoreCropsTree()
{
    return Botcraft::Builder<Botcraft::SimpleBehaviourClient>("Put harvested crops into chest")
        .selector()
            .inverter().leaf("Check if there are items present", IsInventoryFilled)
            .sequence()
                // Walk to chest
                .leaf(Botcraft::CopyBlackboardData, CLIENT_BLACKBOARD_CHEST, "GoTo.goal")
                .leaf(Botcraft::SetBlackboardData<int>, "GoTo.dist_tolerance", 3)
                .leaf(Botcraft::SetBlackboardData<int>, "GoTo.min_end_dist", 0)
                .leaf(Botcraft::SetBlackboardData<int>, "GoTo.min_end_dist_xz", 0)

                // Sort items so they stack up (we may have placed some items beforehand)
                .leaf(Botcraft::SortInventory)

                // Open the container
                .leaf(Botcraft::CopyBlackboardData, CLIENT_BLACKBOARD_CHEST, "OpenContainer.pos")
                .selector()
                    .inverter().leaf("Open chest", Botcraft::OpenContainerBlackboard)

                    // Wait one second
                    .repeater(100).inverter().leaf(Botcraft::Yield)

                    .leaf("Unload items to chest", UnloadInventoryToChest)
                .end()

            .end()
        .end();
}

std::shared_ptr<Botcraft::BehaviourTree<Botcraft::SimpleBehaviourClient>> CreateFullTree()
{
    return Botcraft::Builder<Botcraft::SimpleBehaviourClient>("Main tree")
        .sequence()
            .leaf(IsInventoryFilled)
        .end();
}