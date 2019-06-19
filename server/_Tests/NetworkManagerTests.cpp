#include <catch/catch.hpp>

#include "../Server/NetworkManager.h"
#include "Mocks/MockSocketClient.h"
#include "Mocks/MockPacketFactory.h"


TEST_CASE("ReceivePacket returns nullptr if no packets are queued", "[NetworkManager]") {
    //ARRANGE
    MockSocketClient::ResetMock();
    MockPacketFactory::ResetMock();

    MockSocketClient socketClient{};
    MockPacketFactory packetFactory{};

    NetworkManagerConfig networkManagerConfig{};
    networkManagerConfig.maxPacketQueueSize = 10;
    networkManagerConfig.maxPacketSize = 1024;
    networkManagerConfig.serverPort = 34000;
    networkManagerConfig.serverIpAddress = std::string{"0.0.0.0"};

    NetworkManager nm = NetworkManager(socketClient, packetFactory, networkManagerConfig);

    //ACT
    auto packet = nm.ReceivePacket();

    //ASSERT
    REQUIRE(packet == nullptr);
}

TEST_CASE("ReceivePacket returns packet if one is queued", "[NetworkManager]") {
    //ARRANGE
    MockSocketClient::ResetMock();
    MockPacketFactory::ResetMock();

    MockSocketClient socketClient{};
    MockPacketFactory packetFactory{};

    NetworkManagerConfig networkManagerConfig{};
    networkManagerConfig.maxPacketQueueSize = 10;
    networkManagerConfig.maxPacketSize = 1024;
    networkManagerConfig.serverPort = 34000;
    networkManagerConfig.serverIpAddress = std::string{"0.0.0.0"};

    NetworkManager nm = NetworkManager(socketClient, packetFactory, networkManagerConfig);

    //ACT
    nm.Update();
    auto packet1 = nm.ReceivePacket();
    auto packet2 = nm.ReceivePacket();

    //ASSERT
    REQUIRE(MockSocketClient::NumReceived == 2);
    REQUIRE(MockPacketFactory::NumCreated == 2);
    REQUIRE(packet1 != nullptr);
    REQUIRE(packet2 != nullptr);
}

TEST_CASE("SendPacket calls SocketClient to send packet data", "[NetworkManager]") {
    // ARRANGE
    MockSocketClient::ResetMock();
    MockPacketFactory::ResetMock();

    MockSocketClient socketClient{};
    MockPacketFactory packetFactory{};

    NetworkManagerConfig networkManagerConfig{};
    networkManagerConfig.maxPacketQueueSize = 10;
    networkManagerConfig.maxPacketSize = 1024;
    networkManagerConfig.serverPort = 34000;
    networkManagerConfig.serverIpAddress = std::string{"0.0.0.0"};

    NetworkManager nm = NetworkManager(socketClient, packetFactory, networkManagerConfig);

    Packet packet = Packet(sockaddr_in(), nullptr, 0);

    //ACT
    nm.SendPacket(&packet);
    nm.SendPacket(&packet);
    nm.Update();

    //ASSERT
    REQUIRE(MockSocketClient::NumSent == 2);
    REQUIRE(MockPacketFactory::NumDestroyed == 2);
}


