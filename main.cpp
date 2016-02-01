
#include <raknet/RakPeerInterface.h>
#include <raknet/NatPunchthroughServer.h>

int main()
{
    RakNet::RakPeerInterface* rakPeer=RakNet::RakPeerInterface::GetInstance();
    rakPeer->Shutdown(100);
    RakNet::RakPeerInterface::DestroyInstance(rakPeer);

    return 0;
}

