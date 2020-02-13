#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SessionManager.h"

class updateMock: public SessionManager::update {
public:
    MOCK_METHOD1

}