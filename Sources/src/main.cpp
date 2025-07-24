#include <string>
#include <iostream>

#include "botcraft/AI/SimpleBehaviourClient.hpp"
#include "botcraft/Utilities/Logger.hpp"

struct Arguments
{
    bool Help = false;
    std::string Address = "127.0.0.1:25565";
    std::string LoginName = "";

    int ReturnCode = 0;
};


void ShowHelp(const char* argv0)
{
    std::cout << "Usage: " << argv0 << " <options>\n"
        << "Options:\n"
        << "\t-h, --help\tShow this help message\n"
        << "\t--address\tAddress of the server you want to connect to, default: 127.0.0.1:25565\n"
        << "\t--login\t\tPlayer name in offline mode, empty for Microsoft account, default: empty\n"
        << std::endl;
}

Arguments ParseCommandLine(int argc, char* argv[])
{
    Arguments args;
    for (int i = 1; i < argc; i++)
    {
        std::string data = argv[i];

        if (data == "-h" || data == "--help")
        {
            args.Help = true;
            return args;
        }
        else if (data == "--address")
        {
            if ((i + 1) < argc)
            {
                args.Address = argv[i + 1];
                i++;
            }
            else
            {
                LOG_FATAL("--address requires an argument!");
                args.ReturnCode = 1;
            }
        }
    }

    return args;
}

int main(int argc, char* argv[])
{
    try
    {
        // Init logging, log everything >= Info, only to console, no file
        Botcraft::Logger::GetInstance().SetLogLevel(Botcraft::LogLevel::Info);
        Botcraft::Logger::GetInstance().SetFilename("");
        // Add a name to this thread for logging
        Botcraft::Logger::GetInstance().RegisterThread("main");

        Arguments args;
        if (argc > 1)
        {
            args = ParseCommandLine(argc, argv);
            if (args.Help)
            {
                ShowHelp(argv[0]);
                return 0;
            }

            if (args.ReturnCode != 0)
            {
                return args.ReturnCode;
            }
        }

        auto action_tree = Botcraft::Builder<Botcraft::SimpleBehaviourClient>("empty tree").selector().end();

        Botcraft::SimpleBehaviourClient client(false);
        client.SetAutoRespawn(true);

        client.Connect(args.Address, args.LoginName);

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