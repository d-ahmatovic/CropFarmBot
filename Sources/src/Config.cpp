#include "Config.hpp"

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config, Address, LoginName, TargetBlocks)

Config Config::Get(const char* file_path)
{
    std::ifstream input(file_path);
    if (input.good())
    {
        nlohmann::json data = nlohmann::json::parse(input);
        return data.template get<Config>();
    }
    else
    {
        return Config{};
    }
}