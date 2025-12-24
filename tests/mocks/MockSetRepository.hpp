#include <gmock/gmock.h>
#include "storage/ISetRepository.hpp"

class MockSetRepository : public ISetRepository {
public:
    MOCK_METHOD(void, save, (const IntegerSet& set, int id), (override));
    MOCK_METHOD(std::unique_ptr<IntegerSet>, load, (int id), (override));
};
