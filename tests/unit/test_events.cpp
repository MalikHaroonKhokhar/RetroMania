#include <gtest/gtest.h>
#include "core/Event.h"
#include "core/EventDispatcher.h"

using namespace Forge;

class TestEventA : public Event {
public:
    int value = 0;
};

class TestEventB : public Event {};

TEST(EventDispatcherTest, SubscribeAndDispatch) {
    EventDispatcher dispatcher;
    int receivedValue = 0;

    dispatcher.Subscribe<TestEventA>([&](TestEventA& e) {
        receivedValue = e.value;
    });

    TestEventA eventA;
    eventA.value = 42;
    dispatcher.Dispatch(eventA);

    EXPECT_EQ(receivedValue, 42);
}

TEST(EventDispatcherTest, EventConsumption) {
    EventDispatcher dispatcher;
    int callCount = 0;

    dispatcher.Subscribe<TestEventA>([&](TestEventA& e) {
        callCount++;
        e.Handled = true;
    });

    dispatcher.Subscribe<TestEventA>([&](TestEventA& e) {
        callCount++; // Should not be reached
    });

    TestEventA eventA;
    dispatcher.Dispatch(eventA);

    EXPECT_EQ(callCount, 1);
    EXPECT_TRUE(eventA.Handled);
}

TEST(EventDispatcherTest, MultipleEventTypes) {
    EventDispatcher dispatcher;
    bool aReceived = false;
    bool bReceived = false;

    dispatcher.Subscribe<TestEventA>([&](TestEventA& e) {
        aReceived = true;
    });

    dispatcher.Subscribe<TestEventB>([&](TestEventB& e) {
        bReceived = true;
    });

    TestEventA eventA;
    dispatcher.Dispatch(eventA);

    EXPECT_TRUE(aReceived);
    EXPECT_FALSE(bReceived);
}