#include <string>
#include <iostream>

#include <botcraft/AI/SimpleBehaviourClient.hpp>
#include <botcraft/Utilities/Logger.hpp>

#include "Config.hpp"

int main(int argc, char* argv[])
{
    try
    {
        // Init logging, log everything >= Info, only to console, no file
        Botcraft::Logger::GetInstance().SetLogLevel(Botcraft::LogLevel::Info);
        Botcraft::Logger::GetInstance().SetFilename("");
        // Add a name to this thread for logging
        Botcraft::Logger::GetInstance().RegisterThread("main");

        Config cfg = Config::Get("config.json");

        auto action_tree = Botcraft::Builder<Botcraft::SimpleBehaviourClient>("empty tree").selector().end();

        Botcraft::SimpleBehaviourClient client(false);
        client.SetAutoRespawn(true);

        client.Connect(cfg.Address, cfg.LoginName);

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