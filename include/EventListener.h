#pragma once

#include <functional>
#include <list>
#ifdef _WIN32
#include <memory>
#endif // _WIN32

//
// signal-slot pattern implementation.
//
// Callbacks registered to the event are deregistered when
// the eventconnection handle is destructed.
//

class EventConnection {

public:
	EventConnection() = default;

	// ensure that the connection can not be duplicated.
	EventConnection(const EventConnection &evlc) = delete;
	void operator=(const EventConnection &evlc) = delete;

	// rvalue constructor and assignment only allow moving
	EventConnection(EventConnection &&evlc) = default;
	EventConnection& operator=(EventConnection &&evlc) = default;

	void Disconnect()
	{
		mEventRecord.reset();
	}

	bool IsConnected() const
	{
		const ReferenceRecord *er = mEventRecord.get();
		return er && er->connected;
	}

protected:
	struct ReferenceRecord {
		bool connected = true;
	};

	template <class ...T> friend class Event;
	EventConnection(std::shared_ptr<ReferenceRecord>& er)
		: mEventRecord(er)
	{
	}

private:
	std::shared_ptr<ReferenceRecord> mEventRecord;
};

template<class ...T>
class Event {

public:
	using EventCallback = std::function<void(T...)>;

	Event() = default;
	Event(const Event&) = delete;
	void operator=(const EventConnection &evlc) = delete;

	~Event()
	{
		// mark all the records disconnected
		for (const CallbackRecord& tuple : mListeners) {
			if (auto er = std::get<0>(tuple).lock()) {
				// this happens only if this event is destroyed before the eventconnection
				er->connected = false;
			}
		}
	}

	EventConnection Add(const EventCallback cb, bool front = false)
	{
		auto er = std::make_shared<EventConnection::ReferenceRecord>();
		if (front) {
			mListeners.emplace_front(er, cb);
		} else {
			mListeners.emplace_back(er, cb);
		}
		return EventConnection(er);
	}

	void operator()(T&&... args)
	{
		Raise(std::forward<T...>(args...));
	}

	void Raise(T&&... args)
	{
		// call listeners while purging expired ones
		auto it = mListeners.begin();
		while (it != mListeners.end()) {
			const CallbackRecord& cr = *it;
			if (auto r = std::get<0>(cr).lock()) {
				std::get<1>(cr)(std::forward<T...>(args...)); // call callback function
				it++;
			} else {
				it = mListeners.erase(it);
			}
		}
	}
protected:
	using CallbackRecord = std::tuple<std::weak_ptr<EventConnection::ReferenceRecord>, EventCallback>;
	std::list<CallbackRecord> mListeners;
};
