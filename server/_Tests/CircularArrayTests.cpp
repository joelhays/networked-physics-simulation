#include <catch/catch.hpp>

#include "../Util/CircularArray.h"


TEST_CASE("CircularArray inserts values correctly", "[CircularArray]") {
    //ARRANGE
    CircularArray<int> arr(3);
    int v[] = { 1, 2, 3 };

    //ACT
    arr.insert(v[0]);
    arr.insert(v[1]);
    arr.insert(v[2]);

    //ASSERT
    REQUIRE(arr.getSequence() == 2);
    REQUIRE(*arr.getValue(0) == v[0]);
    REQUIRE(*arr.getValue(1) == v[1]);
    REQUIRE(*arr.getValue(2) == v[2]);
}

TEST_CASE("CircularArray wraps correctly", "[CircularArray]") {
    //ARRANGE
    CircularArray<int> arr(3);
    int v[] = { 1, 2, 3, 4 };

    //ACT
    arr.insert(v[0]);
    arr.insert(v[1]);
    arr.insert(v[2]);
    arr.insert(v[3]);

    //ASSERT
    REQUIRE(arr.getSequence() == 3);
    REQUIRE(arr.getValue(0) == nullptr);
    REQUIRE(*arr.getValue(1) == v[1]);
    REQUIRE(*arr.getValue(2) == v[2]);
    REQUIRE(*arr.getValue(3) == v[3]);
}

TEST_CASE("CircularArray clears correctly", "[CircularArray]") {
    //ARRANGE
    CircularArray<int> arr(3);
    int v1 = 1;

    //ACT
    arr.insert(v1);

    arr.clear();

    //ASSERT
    REQUIRE(arr.getSequence() == -1);
    REQUIRE(arr.getValue(0) == nullptr);
}