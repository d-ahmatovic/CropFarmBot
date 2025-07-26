#include <string>
#include <iostream>

#include <botcraft/AI/SimpleBehaviourClient.hpp>
#include <botcraft/Utilities/Logger.hpp>
#include <botcraft/Game/Vector3.hpp>

#include "Config.hpp"
#include "CropFarmBehaviour.hpp"


using namespace Botcraft;


int main(int argc, char* argv[])
{
    try
    {
        // Init logging, log everything >= Info, only to console, no file
        Logger::GetInstance().SetLogLevel(LogLevel::Info);
        Logger::GetInstance().SetFilename("");
        // Add a name to this thread for logging
        Logger::GetInstance().RegisterThread("main");


        // Read config file
        Config::Read("config.json");
        std::shared_ptr<Config> cfg = Config::GetInstance();

        // Create client
        SimpleBehaviourClient client(false);
        client.SetAutoRespawn(true);

        // Set blackboard values that are used by the bot
        {
            Botcraft::Blackboard& blackboard = client.GetBlackboard();

            blackboard.Set<Position>(CLIENT_BLACKBOARD_BASE, Position(cfg->BasePosition));
            blackboard.Set<Position>(CLIENT_BLACKBOARD_CHEST, Position(cfg->StoragePosition));
            blackboard.Set<Position>(CLIENT_BLACKBOARD_BED, Position(cfg->BedPosition));
            blackboard.Set<int>(CLIENT_BLACKBOARD_RADIUS, cfg->WorkRange);
        }

        client.Connect(cfg->Address, cfg->LoginName);

        auto action_tree = Botcraft::Builder<Botcraft::SimpleBehaviourClient>("empty tree").selector().end();

        // Wait 10 seconds and start the flow afterwards
        Botcraft::Utilities::SleepFor(std::chrono::seconds(10));
        client.SetBehaviourTree(action_tree);
        client.RunBehaviourUntilClosed();

        client.Disconnect();
    }
    catch (std::exception& e)
    {
        LOG_FATAL("Exception: " << e.what());
        return 2;
    }
    catch (...)
    {
        LOG_FATAL("Unknown exception occured!");
        return 3;
    }
}