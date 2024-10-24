/*
  ==============================================================================

	OSCInterface.h
	Created: 26 Sep 2020 1:52:46pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#pragma once
using namespace servus;

class OSCOutput :
	public BaseItem,
	public Thread
{
public:
	OSCOutput();
	~OSCOutput();

	bool forceDisabled;
	bool senderIsConnected;

	//SEND
	BoolParameter* useLocal;
	StringParameter* remoteHost;
	IntParameter* remotePort;

	void setForceDisabled(bool value);

	virtual void setupSender();
	void sendOSC(const OSCMessage& m);
	void sendOSC(const OSCBundle& m);

	virtual void run() override;

	void onContainerParameterChangedInternal(Parameter* p) override;

	virtual InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;

private:
	OSCSender sender;
	std::queue<std::unique_ptr<OSCMessage>> messageQueue;
	std::queue<std::unique_ptr<OSCBundle>> bundleQueue;
	CriticalSection queueLock;
};

class OSCInterface :
	public Interface,
	public OSCReceiver::Listener<OSCReceiver::RealtimeCallback>,
	public Thread, //for zeroconf async creation (smoother when creating an OSC module)
	public BaseManager<OSCOutput>::ManagerListener
{
public:
	OSCInterface(const String &name = "OSC", bool canHaveScript = false);
	~OSCInterface();

	//RECEIVE
	IntParameter* localPort;
	BoolParameter* isConnected;
	BoolParameter* useBundles;

	OSCReceiver receiver;
	OSCSender genericSender;

	struct BundleTarget
	{
		String ip = "";
		int port = 0;
		OSCBundle bundle;
	};

	OwnedArray<BundleTarget> bundles;
	HashMap<String, BundleTarget *> bundleMap;

	//ZEROCONF

	Servus servus;

	std::unique_ptr<EnablingControllableContainer> receiveCC;
	std::unique_ptr<BaseManager<OSCOutput>> outputManager;

	//Script
	const Identifier oscEventId = "oscEvent";

	//RECEIVE
	virtual void setupReceiver();

	void processMessage(const OSCMessage& msg);
	virtual void processMessageInternal(const OSCMessage&);

	void itemAdded(OSCOutput* output) override;
	void itemsAdded(Array<OSCOutput*> output) override;

	//SEND
	virtual void setupSenders();
	virtual void sendOSC(const OSCMessage& msg, String ip = "", int port = 0);
	virtual void finishSendValues() override;

	//ZEROCONF
	void setupZeroConf();

	//Script
	static var sendOSCFromScript(const var::NativeFunctionArgs& args);
	static var sendOSCToFromScript(const var::NativeFunctionArgs& args);
	static var matchOSCAddrFromScript(const var::NativeFunctionArgs& args);
	static var registerOSCCallbackFromScript(const var::NativeFunctionArgs& args);


	virtual void loadJSONDataInternal(var data) override;

	virtual void onContainerNiceNameChanged() override;
	virtual void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	virtual void oscMessageReceived(const OSCMessage& message) override;
	virtual void oscBundleReceived(const OSCBundle& bundle) override;

	virtual void run() override;

	String getTypeString() const override { return "Generic OSC"; }
	static OSCInterface* create(var params) { return new OSCInterface(); };

private:
	Array<std::tuple<OSCAddressPattern, Identifier>> scriptCallbacks;
};