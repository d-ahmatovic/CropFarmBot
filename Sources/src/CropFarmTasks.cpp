#include <vector>
#include <algorithm>
#include <map>

#include "CropFarmTasks.hpp"

#include "botcraft/Game/Entities/LocalPlayer.hpp"
#include "botcraft/Game/World/World.hpp"

using namespace Botcraft;


/*
    Note, these crops can't be farmed without adding too much logic (ingame and in code) on top:
    - Mushrooms (https://minecraft.wiki/w/Mushrooms)
    - Sea Pickle (https://minecraft.wiki/w/Sea_Pickle)
    - Fungi (https://minecraft.wiki/w/Fungi)
    - Chorus Flower (https://minecraft.wiki/w/Chorus_Flower)
*/

static constexpr std::array<std::string_view, 5> g_ReplantableCrop = {
    "minecraft:wheat",
    "minecraft:beetroots",
    "minecraft:carrots",
    "minecraft:potatoes",
    "minecraft:nether_wart"
};

static constexpr std::array<std::string_view, 3> g_RegrowableInteractionCrop = {
    "minecraft:sweet_berry_bush",
    "minecraft:cave_vines_body_with_berries",
    "minecraft:cave_vines_head_with_berries"
};

static constexpr std::array<std::string_view, 2> g_RegrowableCrop = {
    "minecraft:melon",
    "minecraft:pumpkin"
};

static constexpr std::array<std::string_view, 4> g_RegrowableAboveStem = {
    "minecraft:bamboo",
    "minecraft:sugar_cane",
    "minecraft:cactus",
    "minecraft:kelp_plant"
};

static constexpr std::array<std::string_view, 3> g_GrowsOnceCrop = {
    "minecraft:torchflower_crop",
    "minecraft:pitcher_crop",
    "minecraft:cocoa"
};

static const std::map<std::string_view, std::string_view> g_ReplantableCropSeedsMap = {
    {"minecraft:wheat", "minecraft:wheat_seeds"},
    {"minecraft:beetroots", "minecraft:beetroot_seeds"},
    {"minecraft:carrots", "minecraft:carrot"},
    {"minecraft:potatoes", "minecraft:potato"},
    {"minecraft:nether_wart", "minecraft:nether_wart"}
};

static const std::map<std::string_view, int> g_CropMaxAge = {
    {"minecraft:wheat", 7},
    {"minecraft:beetroots", 3},
    {"minecraft:carrots", 7},
    {"minecraft:potatoes", 7},
    {"minecraft:sweet_berry_bush", 3},
    {"minecraft:cocoa", 2},
    {"minecraft:nether_wart", 3}
};

static const std::map<std::string_view, std::vector<std::string_view>> g_CropOutput = {
    {"minecraft:wheat", {"minecraft:wheat_seeds", ""}}
};

static bool CanBeFarmed(std::string_view block_name)
{
}

static bool IsReplantableCrop(std::string_view block_name)
{

}

static std::string_view GetCropSeed(std::string_view block_name)
{
    auto it = g_ReplantableCropSeedsMap.find(block_name);
    if (it != g_ReplantableCropSeedsMap.end())
    {
        return it->second;
    }

    return "";
}

static int GetCropMaxAge(std::string_view block_name)
{
    auto it = g_CropMaxAge.find(block_name);
    if (it != g_CropMaxAge.end())
    {
        return it->second;
    }

    return -1;
}


Status FindAllCrops(BehaviourClient& client, const int radius)
{
    std::vector<Position> crop_positions;
    std::shared_ptr<World> world = client.GetWorld();

    Position client_pos = client.GetLocalPlayer()->GetPosition();

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

                const Blockstate* block = world->GetBlock(current_pos);

                if (block == nullptr)
                {
                    continue;
                }

                std::string_view block_name = block->GetName();
                
            }
        }
    }

}

Status FindOptimalCropPath(BehaviourClient& client)
{

}