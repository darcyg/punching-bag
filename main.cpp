#include <vector>
#include <string>
#include <raknet/RakPeerInterface.h>
#include <raknet/NatPunchthroughServer.h>
#include <cmdparser.hpp>
#include <signal.h>
#include <memory>

namespace {

class DebugPrint :
  public RakNet::NatPunchthroughServerDebugInterface
{
    void OnServerMessage(const char *msg) override
    {
        std::cout << msg << std::endl;
    }
};

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
    sigaction(SIGINT, &action, NULL);

    std::cout << "Registering SIGTERM/SIGINT handler" << std::endl;
}

std::shared_ptr<RakNet::Packet> receive(RakNet::RakPeerInterface* rakPeer)
{
    auto packet = rakPeer->Receive();
    return {packet, [=](RakNet::Packet* packet) {rakPeer->DeallocatePacket(packet);}};
}

void run(int argn, char** argv)
{
    cli::Parser parser(argn, argv);
    parser.set_required<unsigned short>("p", "port", "Port to listen on");
    parser.set_required<std::string>("a", "address", "Address to bind to");

    parser.run_and_exit_if_error();

    register_sigterm();

    auto port = parser.get<unsigned short>("p");
    auto MaximumConnections = 128;

    std::cout << "Starting server on port " << port << std::endl;

    RakNet::RakPeerInterface* rakPeer=RakNet::RakPeerInterface::GetInstance();

    RakNet::NatPunchthroughServer natPunchthroughServer;
    rakPeer->AttachPlugin(&natPunchthroughServer);

    DebugPrint debugPrint;
    natPunchthroughServer.SetDebugInterface(&debugPrint);

    std::vector<RakNet::SocketDescriptor> socketDescriptorList = {
        RakNet::SocketDescriptor(port, parser.get<std::string>("a").c_str())};

    if (rakPeer->Startup(MaximumConnections, socketDescriptorList.data(),
                         socketDescriptorList.size())!=RakNet::RAKNET_STARTED)
    {
        RakNet::RakPeerInterface::DestroyInstance(rakPeer);
        throw std::runtime_error("Unable to start RakNet");
    }

    rakPeer->SetMaximumIncomingConnections(MaximumConnections);
    std::cout << "Server started on " << rakPeer->GetMyBoundAddress().ToString(true) << std::endl;

    while(!stop_running)
    {
        while (auto packet = receive(rakPeer))
        {
            std::cout << "Received packet from " <<
                         packet->systemAddress.ToString(true) << std::endl;
        }

        sleep(1);
    }

    std::cout << "\nStopping.." << std::endl;

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
