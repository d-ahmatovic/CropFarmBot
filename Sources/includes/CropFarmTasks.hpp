#include "botcraft/AI/BehaviourClient.hpp"
#include "botcraft/AI/Status.hpp"


Botcraft::Status FindAllCrops(Botcraft::BehaviourClient& client);
Botcraft::Status FarmAndReplantCrops(Botcraft::BehaviourClient& client);
Botcraft::Status UnloadInventoryToChest(Botcraft::BehaviourClient& client);


struct CropData
{
    Botcraft::Position Position;
    std::string_view CropName;

    bool operator==(const CropData& other) const
    {
        return this->Position == other.Position && this->CropName == other.CropName;
    }
};

namespace std
{
    template<>
    struct hash<CropData>
    {
        size_t operator()(const CropData& data) const
        {
            size_t h1 = std::hash<Botcraft::Position>{}(data.Position);
            size_t h2 = std::hash<std::string_view>{}(data.CropName);
            return h1 ^ (h2 << 1);
        }
    };

    template<>
    struct hash<Botcraft::Position>
    {
        size_t operator()(const Botcraft::Position& data) const
        {
            size_t h1 = std::hash<double>{}(data.x);
            size_t h2 = std::hash<double>{}(data.y);
            size_t h3 = std::hash<double>{}(data.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}
