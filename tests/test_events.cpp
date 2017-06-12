
#include <gtest/gtest.h>

#include <EventListener.h>

TEST(EventListener, Single)
{
	int val = 0;
	Event<int> event;
	EventConnection conn = event.Add([&](int i) {
		val = i;
	});
	event.Raise(1);
	ASSERT_EQ(val, 1);
}

TEST(EventListener, Multiple)
{
	int val = 0;
	Event<int> event;
	EventConnection conn1 = event.Add([&](int i) {
		val += i;
	});
	EventConnection conn2 = event.Add([&](int i) {
		val += i;
	});
	event.Raise(1);
	ASSERT_EQ(val, 2);
}

TEST(EventListener, Expired)
{
	int val = 0;
	Event<int> event;
	{
		EventConnection conn = event.Add([&](int i) {
			val = i;
		});
	}
	event.Raise(1);
	ASSERT_EQ(val, 0);
}

TEST(EventListener, Disconnect)
{
	int val = 0;
	Event<int> event;
	EventConnection conn = event.Add([&](int i) {
		val = i;
	});
	conn.Disconnect();
	event.Raise(1);
	ASSERT_EQ(val, 0);
}
