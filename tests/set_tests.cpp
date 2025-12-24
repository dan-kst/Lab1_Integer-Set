#include <gtest/gtest.h>
#include "core/IntegerSet.hpp"

TEST(IntegerSetTest, AddElementIncreasesSize) {
    IntegerSet mySet;
    mySet.add(10);
    EXPECT_EQ(mySet.size(), 1);
    EXPECT_TRUE(mySet.contains(10));
}
TEST(IntegerSetTest, IntersectionWorks) {
    IntegerSet setA;
    setA.add(1); setA.add(2);
    
    IntegerSet setB;
    setB.add(2); setB.add(3);
    
    auto result = setA.intersect(setB); // Returns a unique_ptr<IntegerSet>
    EXPECT_TRUE(result->contains(2));
    EXPECT_FALSE(result->contains(1));
}
