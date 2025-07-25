#include <vector>
#include <algorithm>
#include <map>

#include <botcraft/Game/Entities/LocalPlayer.hpp>
#include <botcraft/Game/World/World.hpp>
#include <botcraft/AI/Tasks/AllTasks.hpp>

#include "CropFarmTasks.hpp"
#include "Config.hpp"

using namespace Botcraft;


/*
    Note, these crops can't be farmed without adding too much logic (ingame and in code) on top:
    - Cocoa (https://minecraft.wiki/w/Cocoa)
    - Kelp (https://minecraft.wiki/w/Kelp)
    - Mushrooms (https://minecraft.wiki/w/Mushrooms)
    - Sea Pickle (https://minecraft.wiki/w/Sea_Pickle)
    - Fungi (https://minecraft.wiki/w/Fungi)
    - Chorus Flower (https://minecraft.wiki/w/Chorus_Flower)
*/

static bool IsHitAndReplantCrop(std::string_view block_name)
{
    static constexpr std::array<std::string_view, 5> replantable_crops = {
        "minecraft:wheat",
        "minecraft:beetroots",
        "minecraft:carrots",
        "minecraft:potatoes",
        "minecraft:nether_wart"
    };
    for (std::size_t i = 0; i < replantable_crops.size(); i++)
    {
        if (replantable_crops.at(i) == block_name)
        {
            return true;
        }
    }
    return false;
}

static bool IsRightClickableCrop(std::string_view block_name)
{
    static constexpr std::array<std::string_view, 3> interaction_crops = {
        "minecraft:sweet_berry_bush",
        "minecraft:cave_vines_body_with_berries",
        "minecraft:cave_vines_head_with_berries"
    };

    for (std::size_t i = 0; i < interaction_crops.size(); i++)
    {
        if (interaction_crops.at(i) == block_name)
        {
            return true;
        }
    }
    return false;
}

static bool IsOneshotCrop(std::string_view block_name)
{
    static constexpr std::array<std::string_view, 5> oneshot_crops = {
        "minecraft:melon",
        "minecraft:pumpkin",
        "minecraft:torchflower_crop",
        "minecraft:pitcher_crop"
    };

    for (std::size_t i = 0; i < oneshot_crops.size(); i++)
    {
        if (oneshot_crops.at(i) == block_name)
        {
            return true;
        }
    }
    return false;
}

static bool IsHighGrowingCrop(std::string_view block_name)
{
    static const std::array<std::string, 4> stem_based_crops = {
        "minecraft:bamboo",
        "minecraft:sugar_cane",
        "minecraft:cactus",
    };

    for (std::size_t i = 0; i < stem_based_crops.size(); i++)
    {
        if (stem_based_crops.at(i) == block_name)
        {
            return true;
        }
    }
    return false;
}

static int GetCropMaxAge(std::string_view block_name)
{
    static const std::map<std::string_view, int> max_age_map = {
        {"minecraft:wheat", 7},
        {"minecraft:beetroots", 3},
        {"minecraft:carrots", 7},
        {"minecraft:potatoes", 7},
        {"minecraft:sweet_berry_bush", 3},
        {"minecraft:nether_wart", 3},
        {"minecraft:torchflower_crop", 2},
        {"minecraft:pitcher_crop", 4}
    };

    auto it = max_age_map.find(block_name);
    if (it != max_age_map.end())
    {
        return it->second;
    }
    else return -1;
}

static std::string_view GetReplantableCropSeedName(std::string_view block_name)
{
    // std::map<BlockName, ItemName>
    static const std::map<std::string_view, std::string_view> replantable_seed_map = {
        {"minecraft:wheat", "minecraft:wheat_seeds"},
        {"minecraft:beetroots", "minecraft:beetroot_seeds"},
        {"minecraft:carrots", "minecraft:carrot"},
        {"minecraft:potatoes", "minecraft:potato"},
        {"minecraft:nether_wart", "minecraft:nether_wart"}
    };

    auto it = replantable_seed_map.find(block_name);
    if (it != replantable_seed_map.end())
    {
        return it->second;
    }

    return "";
} 

static std::vector<std::string_view> GetPossibleHarvestOutput(std::string_view block_name)
{
    // std::map<BlockName, std::vector<ItemName>>
    static const std::map<std::string_view, std::vector<std::string_view>> crop_outputs = {
        {"minecraft:wheat", {"minecraft:wheat_seeds", "minecraft:wheat"}},
        {"minecraft:beetroots", {"minecraft:beetroot", "minecraft:beetroot_seeds"}},
        {"minecraft:carrots", {"minecraft:carrot"}},
        {"minecraft:potatoes", {"minecraft:potato", "minecraft:poisonous_potato"}},
        {"minecraft:melon", {"minecraft:melon_slice"}},
        {"minecraft:pumpkin", {"minecraft:pumpkin"}},
        {"minecraft:torchflower_crop", {"minecraft:torchflower"}},
        {"minecraft:pitcher_crop", {"minecraft:pitcher_plant"}},
        {"minecraft:bamboo", {"minecraft:bamboo"}},
        {"minecraft:sugar_cane", {"minecraft:sugar_cane"}},
        {"minecraft:sweet_berry_bush", {"minecraft:sweet_berries"}},
        {"minecraft:cactus", {"minecraft:cactus"}},
        {"minecraft:cave_vines_body_with_berries", {"minecraft:glow_berries"}},
        {"minecraft:cave_vines_head_with_berries", {"minecraft:glow_berries"}},
        {"minecraft:nether_wart", {"minecraft:nether_wart"}}
    };

    auto it = crop_outputs.find(block_name);
    if (it != crop_outputs.end())
    {
        return it->second;
    }
    return {};
}


struct CropData
{
    Botcraft::Position Position;
    std::string_view CropName;
};


Status FindAllCrops(BehaviourClient& client)
{
    std::shared_ptr<Config> cfg = Config::GetInstance();
    int radius = cfg->WorkRange;

    std::vector<CropData> crop_positions;
    std::shared_ptr<World> world = client.GetWorld();

    const Position& client_pos = client.GetLocalPlayer()->GetPosition();

    Position current_pos;
    int max_y = std::max(-radius, world->GetMinY());
    int min_y = std::min(radius + 1, world->GetMinY() + world->GetHeight());

    for (int x = -radius; x < radius + 1; x++)
    {
        current_pos.x = client_pos.x + x;
        for (int y = max_y; y < min_y; y++)
        {
            current_pos.y = client_pos.y + y;
            for (int z = -radius; z < radius + 1; z++)
            {
                current_pos.z = client_pos.z + z;

                // Check if blocks are wanted
                const Blockstate* block = world->GetBlock(current_pos);
                if (block == nullptr)
                {
                    continue;
                }

                const std::string& block_name = block->GetName();
                if (std::find(cfg->TargetBlocks.begin(), cfg->TargetBlocks.end(), block_name) == cfg->TargetBlocks.end())
                {
                    continue;
                }
                
                int max_age = GetCropMaxAge(block_name);
                int crop_age = -1;

                auto crop_data = world->GetBlockEntityData(current_pos);
                if (crop_data.HasData() && crop_data.contains("age"))
                {
                    crop_age = crop_data["age"].get<int>();
                }

                if (crop_age != max_age)
                {
                    continue;
                }
                crop_positions.emplace_back(current_pos, block_name);
            }
        }
    }
    if (crop_positions.size() != 0)
    {
        client.GetBlackboard().Set("cropfarm.crops", crop_positions);
        return Status::Success;

    }
    return Status::Failure;
}

void OptimizeCropHarvestPath(std::vector<CropData>& crop_data, const Position& client_pos, const Position& storage_pos)
{
    std::sort(crop_data.begin(), crop_data.end(), [&](const CropData& a, const CropData& b)
    {
        double dist_a_to_player = a.Position.SqrDist(client_pos); 
        double dist_b_to_player = b.Position.SqrDist(client_pos);

        double dist_a_to_storage = a.Position.SqrDist(storage_pos);
        double dist_b_to_storage = b.Position.SqrDist(storage_pos);

        double score_a = dist_a_to_player + 0.3 * dist_a_to_storage;
        double score_b = dist_b_to_player + 0.3 * dist_b_to_storage;

        return score_a < score_b;
    });
}

Status FarmAndReplantCrops(BehaviourClient& client)
{
    if (!client.GetBlackboard().Contains("cropfarm.crops"))
    {
        return Status::Success;
    }

    std::shared_ptr<Config> cfg = Config::GetInstance();

    const Position client_pos = client.GetLocalPlayer()->GetPosition();
    const Position storage_pos = Position(cfg->Storage[0], cfg->Storage[1], cfg->Storage[2]);

    std::vector<CropData> crop_data = client.GetBlackboard().Get<std::vector<CropData>>("cropfarm.crops");
    OptimizeCropHarvestPath(crop_data, client_pos, storage_pos);
    /*
        IsHitAndReplantCrop
        IsRightClickableCrop
        IsOneshotCrop
        IsHighGrowingCrop
        GetCropMaxAge
        GetReplantableCropSeedName
        GetPossibleHarvestOutput
    */
   return Status::Success;
}
