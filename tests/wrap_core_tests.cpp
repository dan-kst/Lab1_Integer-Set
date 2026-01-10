#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ui/common/WrapCore.hpp"
#include "mocks/MockSetRepository.hpp"

using ::testing::Return;
using ::testing::ByMove;

class WrapCoreTest : public ::testing::Test {
protected:
    std::shared_ptr<MockSetRepository> mockRepo = std::make_shared<MockSetRepository>();
    std::shared_ptr<IntegerSet> currentSet = std::make_shared<IntegerSet>();
    std::unique_ptr<WrapCore> core;

    void SetUp() override {
        core = std::make_unique<WrapCore>(mockRepo, currentSet);
    }
};

TEST_F(WrapCoreTest, RemoveSetClearsLocalIdIfMatched) {
    // 1. Simulate loading a set with ID 5
    auto fakeSet = std::make_unique<IntegerSet>(std::initializer_list<int>{1, 2});
    EXPECT_CALL(*mockRepo, load(5)).WillOnce(Return(ByMove(std::move(fakeSet))));
    core->loadSet(5);
    ASSERT_EQ(core->getId(), 5);

    // 2. Expect a removal call
    EXPECT_CALL(*mockRepo, remove(5)).WillOnce(Return(true));

    // 3. Action
    bool result = core->removeSet(5);

    EXPECT_TRUE(result);
    EXPECT_EQ(core->getId(), 0); // Core should have cleared the ID
}
TEST_F(WrapCoreTest, LoadSetSuccessfullyUpdatesId) {
    auto fakeSet = std::make_unique<IntegerSet>(std::initializer_list<int>{1, 2, 3});
    
    // Test that core correctly handles a successful load
    EXPECT_CALL(*mockRepo, load(10)).WillOnce(Return(ByMove(std::move(fakeSet))));

    bool success = core->loadSet(10);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(core->getId(), 10);
    EXPECT_EQ(currentSet->size(), 3);
}
TEST_F(WrapCoreTest, LoadSetReturnsFalseIfNotFound) {
    // Tell the mock to return a nullptr (simulating a missing ID)
    EXPECT_CALL(*mockRepo, load(999)).WillOnce(Return(ByMove(nullptr)));

    bool success = core->loadSet(999);
    
    EXPECT_FALSE(success);
    // Ensure the current set wasn't cleared or corrupted by a failed load
}
TEST_F(WrapCoreTest, StringToSetParserWorks) {
    std::string input = "[1, 2, 3]";
    auto set = core->stringToSet(input);
    
    EXPECT_EQ(set->size(), 3);
    EXPECT_TRUE(set->contains(1));
    EXPECT_TRUE(set->contains(2));
    EXPECT_TRUE(set->contains(3));
}
TEST_F(WrapCoreTest, BatchUnionWorks) {
    std::vector<std::string> sets = {"[1, 2]", "[2, 3]", "[4]"};
    
    std::string result = core->performBatchOperation(sets, SetOperationType::Union);
    
    // Union of {1,2}, {2,3}, {4} is {1,2,3,4}
    EXPECT_EQ(result, "[1,2,3,4]");
}
TEST_F(WrapCoreTest, BatchIntersectionWorks) {
    std::vector<std::string> sets = {"[1, 2, 3]", "[2, 3, 4]", "[3, 5]"};
    
    std::string result = core->performBatchOperation(sets, SetOperationType::Intersect);
    
    // Intersection of {1,2,3}, {2,3,4}, {3,5} is {3}
    EXPECT_EQ(result, "[3]");
}
TEST_F(WrapCoreTest, BatchDifferenceWorks) {
    std::vector<std::string> sets = {"[1, 2, 3, 4]", "[1]", "[2]"};
    
    std::string result = core->performBatchOperation(sets, SetOperationType::Difference);
    
    // Difference: ({1,2,3,4} - {1}) - {2} is {3,4}
    EXPECT_EQ(result, "[3,4]");
}
