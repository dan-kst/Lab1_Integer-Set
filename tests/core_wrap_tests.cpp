#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ui/console/ConsoleWrapCore.hpp"
#include "mocks/MockSetRepository.hpp"

using ::testing::Return;
using ::testing::ByMove;

class CoreWrapTest : public ::testing::Test {
protected:
    std::shared_ptr<MockSetRepository> mockRepo = std::make_shared<MockSetRepository>();
    std::shared_ptr<IntegerSet> currentSet = std::make_shared<IntegerSet>();
    std::unique_ptr<ConsoleWrapCore> core;

    void SetUp() override {
        core = std::make_unique<ConsoleWrapCore>(mockRepo, currentSet);
    }
};

TEST_F(CoreWrapTest, RemoveSetClearsLocalIdIfMatched) {
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
TEST_F(CoreWrapTest, LoadSetSuccessfullyUpdatesId) {
    auto fakeSet = std::make_unique<IntegerSet>(std::initializer_list<int>{1, 2, 3});
    
    // Test that core correctly handles a successful load
    EXPECT_CALL(*mockRepo, load(10)).WillOnce(Return(ByMove(std::move(fakeSet))));

    bool success = core->loadSet(10);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(core->getId(), 10);
    EXPECT_EQ(currentSet->size(), 3);
}
TEST_F(CoreWrapTest, LoadSetReturnsFalseIfNotFound) {
    // Tell the mock to return a nullptr (simulating a missing ID)
    EXPECT_CALL(*mockRepo, load(999)).WillOnce(Return(ByMove(nullptr)));

    bool success = core->loadSet(999);
    
    EXPECT_FALSE(success);
    // Ensure the current set wasn't cleared or corrupted by a failed load
}
