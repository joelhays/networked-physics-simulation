#ifndef NETWORKEDPHYSICS_INJECTION_H
#define NETWORKEDPHYSICS_INJECTION_H

#include <fruit/fruit.h>
#include "Network/ISocketClient.h"
#include "Network/UdpClient.h"
#include "Network/IPacketFactory.h"
#include "Server/Server.h"

fruit::Component<ISocketClient> getSocketClientComponent() {
    return fruit::createComponent()
            .registerConstructor<UdpClient()>()
            .bind<ISocketClient, UdpClient>();
}

fruit::Component<IPacketFactory> getPacketFactoryComponent() {
    return fruit::createComponent()
            .registerConstructor<PacketFactory()>();
}

fruit::Component<NetworkManager> getNetworkManagerComponent() {
    return fruit::createComponent()
            .registerConstructor<NetworkManager(ISocketClient&, PacketFactory&)>()
            .install(getSocketClientComponent)
            .install(getPacketFactoryComponent);
}

fruit::Component<Server> getServerComponent() {
    return fruit::createComponent()
            .registerConstructor<Server(NetworkManager&)>()
            .install(getNetworkManagerComponent);
}

fruit::Injector<Server> injector(getServerComponent);
Server *server = injector.get<Server*>();



#endif //NETWORKEDPHYSICS_INJECTION_H
