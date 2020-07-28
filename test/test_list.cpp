extern "C" {
#include <stdio.h>
#include <stdarg.h>
#include "../list.h"
}

#include "mock.hpp"
#include "hook.hpp"


using namespace testing;

TEST(Sample, Test)
{
    list_t* listTester = initList();
    char *name = "Hakitsy", *type = "Directory";
    EXPECT_NE(listTester, nullptr);
    EXPECT_EQ(addElement(listTester, name, 0, type), 0);
    EXPECT_EQ(listTester->counter, 1);
    EXPECT_EQ(printList(listTester), 0);
    EXPECT_EQ(removeElement(listTester, listTester->first), 0);
    EXPECT_EQ(listTester->counter, 0);
    EXPECT_EQ(deleteList(listTester), 0);
}
