#ifndef MOCK_SET_REPOSITORY_HPP
#define MOCK_SET_REPOSITORY_HPP
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "./storage/ISetRepository.hpp"

// Create the Mock class
class MockSetRepository : public ISetRepository {
public:
    MOCK_METHOD(size_t, save, (const IntegerSet& set), (override));
    MOCK_METHOD(std::unique_ptr<IntegerSet>, load, (size_t id), (override));
    MOCK_METHOD(bool, update, (const IntegerSet& setUpdate, size_t id), (override));
    MOCK_METHOD(bool, remove, (size_t id), (override));
    MOCK_METHOD(std::vector<size_t>, getIdAll, (), (override));
};
#endif
