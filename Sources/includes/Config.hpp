#include <string>
#include <vector>


struct Config
{
    static void Read(const char* file_path);
    static std::shared_ptr<Config> GetInstance();

    std::string Address = "127.0.0.1:25565";
    std::string LoginName = "";
    std::vector<std::string> TargetBlocks = {
        "minecraft:wheat",
        "minecraft:beetroots",
        "minecraft:carrots",
        "minecraft:potatoes",
        "minecraft:nether_wart"
    };
    std::array<int, 3> BasePosition = {131, 63, 219};
    std::array<int, 3> Storage = {131, 63, 219};
    int WorkRange = 10;
};
