#include <gtest/gtest.h>
#include "ui/console/ConsoleWrapUI.hpp"
#include "mocks/MockSetRepository.hpp"

TEST(UIInputTest, HandleReadCapturesValidInteger) {
    auto mockRepo = std::make_shared<MockSetRepository>();
    ConsoleWrapUI ui(mockRepo);
    
    std::stringstream inputData("42\n");
    size_t outValue = 0;

    bool success = ui.handleRead(inputData, outValue);

    EXPECT_TRUE(success);
    EXPECT_EQ(outValue, 42);
}

TEST(UIInputTest, HandleReadFailsOnNonNumeric) {
    auto mockRepo = std::make_shared<MockSetRepository>();
    ConsoleWrapUI ui(mockRepo);
    
    std::stringstream inputData("not_a_number\n");
    size_t outValue = 100;

    bool success = ui.handleRead(inputData, outValue);

    EXPECT_FALSE(success);
    EXPECT_EQ(outValue, 100); // Value should remain unchanged on failure
}
