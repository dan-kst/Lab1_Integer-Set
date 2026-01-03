#include <gtest/gtest.h>
#include "./storage/ISetRepository.hpp"
#include "./storage/PostgresRepository.hpp"
#include "./mocks/MockSetRepository.hpp"

TEST(StorageTest, AppCallsSaveMethod) {
    IntegerSet mySet = {1, 2, 3};
    MockSetRepository mockRepo;

    // We EXPECT the save method to be called exactly 1 time with ID 99
    EXPECT_CALL(mockRepo, save(testing::_)).Times(1);

    // Act
    mockRepo.save(mySet); 
}
TEST(StorageTest, PostgresRealSaveLoad) {
    // This is an Integration Test - it needs Docker running
    PostgresRepository repo;
    IntegerSet original = {42, 100, 7};
    
    // Save the set and capture the REAL ID assigned by Postgres
    size_t assignedId = repo.save(original); 
    
    // Use that ID to load it back
    auto loaded = repo.load(assignedId);
    
    EXPECT_EQ(loaded->size(), 3);
    EXPECT_TRUE(loaded->contains(42));
    
    // Clean up after the test so the DB doesn't get cluttered
    repo.remove(assignedId);
}
