#include <vector>
#include <string>
#include <raknet/RakPeerInterface.h>
#include <raknet/NatPunchthroughServer.h>

int main()
{
    auto MaximumConnections = 128;

    RakNet::RakPeerInterface* rakPeer=RakNet::RakPeerInterface::GetInstance();

    /*std::vector<std::string> addressList;
    for (int i=0, ie=rakPeer->GetNumberOfAddresses(); i!=ie; ++i)
    {
        auto IP = rakPeer->GetLocalIP(i);
        if (IP!=RakNet::UNASSIGNED_SYSTEM_ADDRESS)
            addressList.push_back(IP);
    }*/

    RakNet::NatPunchthroughServer natPunchthroughServer;
    rakPeer->AttachPlugin(&natPunchthroughServer);

    std::vector<RakNet::SocketDescriptor> socketDescriptorList = {
        RakNet::SocketDescriptor(9000, "")
    };

    rakPeer->Startup(MaximumConnections, socketDescriptorList.data(), socketDescriptorList.size());
    rakPeer->Shutdown(100);

    RakNet::RakPeerInterface::DestroyInstance(rakPeer);

    return 0;
}

