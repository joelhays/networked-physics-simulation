cmake_minimum_required(VERSION 3.5.1)
project(NetworkedPhysics)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(/usr/local/include/bullet)

set(MAIN_FILE main.cpp)
set(SOURCE_FILES
        MainConfig.h
        Flatbuffer/message_generated.h
        Network/UdpClient.cpp
        Network/UdpClient.h
        Server/NetworkManager.cpp
        Server/NetworkManager.h
        Server/NetworkManagerConfig.h
        Server/Server.cpp
        Server/Server.h
        Server/Player.h
        Server/PlayerManager.cpp
        Server/PlayerManager.h
        Server/MessageProcessor.cpp
        Server/MessageProcessor.h
        Network/PacketFactory.cpp
        Network/PacketFactory.h
        Network/Packet.h
        Simulation/Simulation.cpp
        Simulation/Simulation.h
        Simulation/Snapshot.h
        Util/CircularArray.h
        Util/FlatbufferUtil.h
        Network/ISocketClient.h
        injection.h)
set(TEST_SOURCE_FILES
        _Tests/CatchMain.cpp
        _Tests/NetworkManagerTests.cpp
        _Tests/Mocks/MockSocketClient.h
        _Tests/Mocks/MockPacketFactory.h
        _Tests/CircularArrayTests.cpp
        _Tests/PacketFactoryTests.cpp)
set(LIBRARIES
        libflatbuffers.a
        libBulletDynamics.so
        libBulletCollision.so
        libLinearMath.so
        libfruit.so)

add_executable(NetworkedPhysics ${MAIN_FILE} ${SOURCE_FILES})
add_executable(NetworkedPhysicsTests ${TEST_SOURCE_FILES} ${SOURCE_FILES})

target_link_libraries(NetworkedPhysics ${LIBRARIES})
target_link_libraries(NetworkedPhysicsTests ${LIBRARIES})


