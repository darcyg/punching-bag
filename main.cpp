#include <vector>
#include <string>
#include <raknet/RakPeerInterface.h>
#include <raknet/NatPunchthroughServer.h>
#include <cmdparser.hpp>
#include <signal.h>

namespace {
volatile sig_atomic_t stop_running = 0;
}

void sigterm_handler(int)
{
    stop_running = 1;
}

void register_sigterm()
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = sigterm_handler;
    sigaction(SIGTERM, &action, NULL);

    std::cout << "Registering SIGTERM handler" << std::endl;
}

void run(int argn, char** argv)
{
    cli::Parser parser(argn, argv);
    parser.set_required<unsigned short>("p", "port", "Port to listen on");

    parser.run_and_exit_if_error();

    register_sigterm();

    auto port = parser.get<unsigned short>("p");
    auto MaximumConnections = 128;

    std::cout << "Starting server on port " << port << std::endl;

    RakNet::RakPeerInterface* rakPeer=RakNet::RakPeerInterface::GetInstance();

    RakNet::NatPunchthroughServer natPunchthroughServer;
    rakPeer->AttachPlugin(&natPunchthroughServer);

    std::vector<RakNet::SocketDescriptor> socketDescriptorList = {
        RakNet::SocketDescriptor(port, "")};

    rakPeer->Startup(MaximumConnections, socketDescriptorList.data(), socketDescriptorList.size());

    std::cout << "Server started" << std::endl;

    while(!stop_running)
    {
        sleep(1);
    }

    std::cout << "Stopping.." << std::endl;

    rakPeer->Shutdown(100);

    RakNet::RakPeerInterface::DestroyInstance(rakPeer);

    std::cout << "Server shut down!" << std::endl;
}

int main(int argn, char** argv)
{
    try
    {
        run(argn, argv);
        return 0;
    }
    catch(std::exception const& error)
    {
        std::cout << error.what() << std::endl;
        return -1;
    }
}
