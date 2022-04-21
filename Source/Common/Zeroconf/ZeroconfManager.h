/*
  ==============================================================================

    ZeroconfManager.h
    Created: 1 Mar 2019 9:15:55am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "servus/servus.h"

using namespace servus;

class ZeroconfManager :
	public Thread,
	public Timer
{
public:
	juce_DeclareSingleton(ZeroconfManager, true);
	ZeroconfManager();
	~ZeroconfManager();

	struct ServiceInfo
	{
		String name;
		String host;
		String ip;
		int port;
	};

	class ZeroconfSearcher
	{
	public:
		ZeroconfSearcher(StringRef name, StringRef serviceName);
		~ZeroconfSearcher();

		String name;
		String serviceName;
		Servus servus;
		OwnedArray<ServiceInfo> services;

		bool search();
		String getIPForHostAndPort(String host, int port);

		ServiceInfo * getService(StringRef name, StringRef host, int port);
		void addService(StringRef name, StringRef host, StringRef ip, int port);
		void removeService(ServiceInfo * service);
		void updateService(ServiceInfo * service, StringRef host, StringRef ip, int port);

		class SearcherListener
		{
		public:
			virtual ~SearcherListener() {}
			virtual void serviceAdded(ServiceInfo* service) {};
			virtual void serviceUpdated(ServiceInfo* service) {};
			virtual void serviceRemoved(ServiceInfo* service) {};
		};

		ListenerList<SearcherListener> listeners;
		void addSearcherListener(SearcherListener* newListener) { listeners.add(newListener); }
		void removeSearcherListener(SearcherListener* listener) { listeners.remove(listener); }


	};

	OwnedArray<ZeroconfSearcher, CriticalSection> searchers;

	ZeroconfSearcher * addSearcher(StringRef name, StringRef serviceName);
	void removeSearcher(StringRef name);

	ZeroconfSearcher * getSearcher(StringRef name);

	void showMenuAndGetService(StringRef service, std::function<void(ServiceInfo*)> returnFunc, bool showLocal = true, bool showRemote = true, bool separateLocalAndRemote = true, bool excludeInternal = true, const String& nameFilter = "");

	void search();

	virtual void timerCallback() override;
	virtual void run() override;

	class ZeroconfEvent {
	public:
		enum Type { SERVICES_CHANGED };
		ZeroconfEvent(Type type) : type(type) {}
		Type type;
	};

	QueuedNotifier<ZeroconfEvent> zeroconfAsyncNotifier;
	typedef QueuedNotifier<ZeroconfEvent>::Listener AsyncListener;
	void addAsyncZeroconfListener(AsyncListener* newListener) { zeroconfAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedZeroconfListener(AsyncListener* newListener) { zeroconfAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncZeroconfListener(AsyncListener* listener) { zeroconfAsyncNotifier.removeListener(listener); }

};
