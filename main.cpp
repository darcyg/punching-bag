#include <vector>
#include <string>
#include <raknet/RakPeerInterface.h>
#include <raknet/NatPunchthroughServer.h>
#include <cxxopts.hpp>

namespace {

auto const title = "punching-bag";

}

int main(int argn, char** argv)
{
    cxxopts::Options options(title);
    options.add_options()
            ("p,port", "Port to listen on");


    options.parse(argn, argv);

    auto MaximumConnections = 128;

    RakNet::RakPeerInterface* rakPeer=RakNet::RakPeerInterface::GetInstance();

    RakNet::NatPunchthroughServer natPunchthroughServer;
    rakPeer->AttachPlugin(&natPunchthroughServer);

    std::vector<RakNet::SocketDescriptor> socketDescriptorList = {
        RakNet::SocketDescriptor(options["port"].as<unsigned short>(), "")};

    rakPeer->Startup(MaximumConnections, socketDescriptorList.data(), socketDescriptorList.size());
    rakPeer->Shutdown(100);

    RakNet::RakPeerInterface::DestroyInstance(rakPeer);

    return 0;
}

