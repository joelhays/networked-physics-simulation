#include <catch/catch.hpp>

#include "../Network/PacketFactory.h"


TEST_CASE("PacketFactory creates a packet correctly", "[PacketFactory]") {
    //ARRANGE
    PacketFactory packetFactory;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 1;
    addr.sin_port = 36000;

    uint8_t data[] = {1};

    //ACT
    auto packet = packetFactory.Create(addr, data, 1);

    //ASSERT
    REQUIRE(packet != nullptr);
    REQUIRE(packet->address.sin_family == addr.sin_family);
    REQUIRE(packet->address.sin_addr.s_addr == addr.sin_addr.s_addr);
    REQUIRE(packet->address.sin_port == addr.sin_port);
    REQUIRE(packet->data_length == 1);
    REQUIRE(packet->data[0] == 1);

    //ACT
    packetFactory.Destroy(packet);
}

TEST_CASE("PacketFactory destroys a packet correctly", "[PacketFactory]") {
    //ARRANGE
    PacketFactory packetFactory;
    uint8_t data[] = {1};

    //ACT
    auto packet = packetFactory.Create(sockaddr_in(), data, 1);

    //ASSERT
    REQUIRE(packet != nullptr);

    //ACT
    packetFactory.Destroy(packet);

    //ASSERT
    REQUIRE(packet == nullptr);
}