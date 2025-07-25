#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <exception>

#include <nlohmann/json.hpp>


struct Config
{
    static Config Get(const char* file_path);

    std::string Address = "127.0.0.1:25565";
    std::string LoginName = "";
    std::vector<std::string> TargetBlocks = {
        "minecraft:wheat",
        "minecraft:beetroots",
        "minecraft:carrots",
        "minecraft:potatoes",
        "minecraft:nether_wart"
    };
};
