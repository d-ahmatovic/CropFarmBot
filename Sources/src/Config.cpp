#include <memory>
#include <fstream>

#include <nlohmann/json.hpp>

#include "Config.hpp"

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config, Address, LoginName, TargetBlocks, BasePosition, Storage, WorkRange)

std::shared_ptr<Config> g_Config;

std::shared_ptr<Config> Config::GetInstance()
{
    return g_Config;
}

void Config::Read(const char* file_path)
{
    std::ifstream input(file_path);
    if (input.good())
    {
        nlohmann::json data = nlohmann::json::parse(input);
        g_Config = std::make_shared<Config>(data.template get<Config>());
    }
    else
    {
        g_Config = std::make_shared<Config>();
    }
}
