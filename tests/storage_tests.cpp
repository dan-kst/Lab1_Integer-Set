#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "./storage/ISetRepository.hpp"
#include "./storage/PostgresRepository.hpp"

// Create the Mock class
class MockSetRepository : public ISetRepository {
public:
    MOCK_METHOD(void, save, (const IntegerSet& set, int id), (override));
    MOCK_METHOD(std::unique_ptr<IntegerSet>, load, (int id), (override));
};

TEST(StorageTest, AppCallsSaveMethod) {
    IntegerSet mySet = {1, 2, 3};
    MockSetRepository mockRepo;

    // We EXPECT the save method to be called exactly 1 time with ID 99
    EXPECT_CALL(mockRepo, save(testing::_, 99)).Times(1);

    // Act
    mockRepo.save(mySet, 99); 
}
TEST(StorageTest, PostgresRealSaveLoad) {
    // This test requires Docker to be running!
    PostgresRepository repo;
    IntegerSet original = {42, 100, 7};
    
    repo.save(original, 1);
    auto loaded = repo.load(1);
    
    EXPECT_EQ(loaded->size(), 3);
    EXPECT_TRUE(loaded->contains(42));
}
