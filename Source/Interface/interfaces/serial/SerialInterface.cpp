/*
  ==============================================================================

	SerialInterface.cpp
	Created: 26 Sep 2020 1:53:17pm
	Author:  bkupe

  ==============================================================================
*/

#include "Interface/InterfaceIncludes.h"

SerialInterface::SerialInterface() :
	Interface("Serial", true),
	port(nullptr),
	customParams("Custom Parameters", false, false, false, false)
{

	portParam = new SerialDeviceParameter("Port", "Serial Port to connect", true);
	addParameter(portParam);
	baudRate = addIntParameter("Baud Rate", "The connection speed. Common values are 9600, 57600, 115200", 115200, 9600, 1000000);
	portParam->openBaudRate = baudRate->intValue();

	isConnected = addBoolParameter("Is Connected", "This is checked if a serial port is connected.", false);
	isConnected->setControllableFeedbackOnly(true);
	isConnected->isSavable = false;
	//connectionFeedbackRef = isConnected;

	scriptObject.getDynamicObject()->setMethod(sendId, SerialInterface::sendStringFromScript);
	scriptObject.getDynamicObject()->setMethod(sendBytesId, SerialInterface::sendBytesFromScript);

	addChildControllableContainer(&customParams);
	customParams.addBaseManagerListener(this);
}

SerialInterface::~SerialInterface()
{
	setCurrentPort(nullptr);
}

bool SerialInterface::setPortStatus(bool status)
{
	if (port == nullptr)
	{
		isConnected->setValue(false);
		return false;
	}

	bool shouldOpen = status && enabled->boolValue();

	if (shouldOpen)  //We want to open the port, it's not already opened and the module is enabled
	{
		//port->setMode(streamingType->getValueDataAsEnum<SerialDevice::PortMode>()); //always set mode, port might be already open with default mode
		port->setBaudRate(baudRate->intValue());

		if (port->isOpen()) port->close();
		port->open(baudRate->intValue());
		if (!port->isOpen())
		{
			NLOGERROR(niceName, "Could not open port : " << port->info->port);
			port = nullptr; //Avoid crash if SerialPort is busy
		}
		else
		{
			NLOG(niceName, "Port " << port->info->port << " opened, have fun.");
		}
	}
	else  //We want to close the port or the module is disabled, and it's actually opened
	{
		NLOG(niceName, "Port closed : " << port->info->port);
		if (port->isOpen()) port->close();
	}

	isConnected->setValue(port->isOpen());
	return port->isOpen();
}

void SerialInterface::setCurrentPort(SerialDevice* _port)
{
	if (port == _port) return;

	setPortStatus(false);

	if (port != nullptr)
	{
		port->removeSerialDeviceListener(this);
	}

	port = _port;

	if (port != nullptr)
	{
		port->addSerialDeviceListener(this);
		setPortStatus(true);
		lastOpenedPortID = port->info->deviceID;
	}

}

void SerialInterface::onContainerParameterChangedInternal(Parameter* p)
{
	Interface::onContainerParameterChangedInternal(p);

	if (p == enabled)
	{
		if (port != nullptr)
		{
			setPortStatus(true);

			NLOG(niceName, "Module is " << (enabled->boolValue() ? "enabled" : "disabled") << ", " << (enabled->boolValue() ? "opening" : "closing serial port"));
		}
	}
	else if (p == baudRate)
	{
		portParam->openBaudRate = baudRate->intValue();
		if (port != nullptr && port->isOpen())
		{
			SerialDevice* d = portParam->getDevice();
			if (d != nullptr) d->setBaudRate(portParam->openBaudRate);
		}

	}
	else if (p == portParam)
	{
		SerialDevice* newDevice = portParam->getDevice();
		SerialDevice* prevPort = port;
		setCurrentPort(newDevice);

		if (port == nullptr && prevPort != nullptr)
		{
			DBG("Manually set no ghost port");
			lastOpenedPortID = ""; //forces no ghosting when user chose to manually disable port
		}
	}
}

void SerialInterface::sendValuesForObjectInternal(Object* o)
{
	Array<var> args;
	args.add(o->getScriptObject());

	var valuesData(new DynamicObject());
	for (auto& c : o->componentManager->items)
	{
		if (!c->enabled->boolValue()) continue;

		var compData(new DynamicObject());
		for (auto& p : c->computedParameters) compData.getDynamicObject()->setProperty(p->shortName, p->getValue());
		valuesData.getDynamicObject()->setProperty(c->shortName, compData);
	}
	args.add(valuesData);

	var itfParams = ((CustomSerialParams*)o->interfaceParameters.get())->getParamValues();
	args.add(itfParams);

	scriptManager->callFunctionOnAllItems("sendValuesForObject", args);
}

void SerialInterface::serialDataReceived(const var& data)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Received : " + data.toString());
	}
}

void SerialInterface::sendMessage(const String& message, var)
{
	if (port == nullptr) return;

	if (logOutgoingData->boolValue()) NLOG(niceName, "Sending : " << message);

	port->writeString(message);
}

void SerialInterface::sendBytes(Array<uint8> data, var)
{
	if (port == nullptr) return;

	if (logOutgoingData->boolValue())
	{
		String s = "Sending " + String(data.size()) + " bytes :";
		for (auto& b : data) s += "\n0x" + String::toHexString(b);
		NLOG(niceName, s);
	}

	port->writeBytes(data);
}



var SerialInterface::sendStringFromScript(const var::NativeFunctionArgs& a)
{
	SerialInterface* m = getObjectFromJS<SerialInterface>(a);
	if (!checkNumArgs(m->niceName, a, 1)) return false;
	String s = getStringFromArgs(a, 0);
	m->sendMessage(s);
	return var();
}

var SerialInterface::sendBytesFromScript(const var::NativeFunctionArgs& a)
{
	SerialInterface* m = getObjectFromJS<SerialInterface>(a);
	if (!checkNumArgs(m->niceName, a, 1)) return false;
	Array<uint8> data = getByteFromArgs(a, 0);
	m->sendBytes(data);
	return var();
}



String SerialInterface::getStringFromArgs(const var::NativeFunctionArgs& a, int offset)
{
	String result = "";
	if (a.arguments[offset].isObject()) result = JSON::toString(a.arguments[offset]);
	else
	{
		for (int i = offset; i < a.numArguments; ++i)
		{
			if (a.arguments[i].isArray() && a.arguments[i].size() > 0)
			{
				for (int j = 0; j < a.arguments[i].size(); j++)
				{
					result += (result.isNotEmpty() ? " " : "") + a.arguments[i][j].toString();
				}
			}
			else
			{
				result += (result.isNotEmpty() ? " " : "") + a.arguments[i].toString();
			}
		}
	}

	return result;
}

Array<uint8> SerialInterface::getByteFromArgs(const var::NativeFunctionArgs& a, int offset)
{
	Array<uint8> data;
	for (int i = offset; i < a.numArguments; ++i)
	{
		if (a.arguments[i].isArray())
		{
			Array<var>* aa = a.arguments[i].getArray();
			for (auto& vaa : *aa) data.add((uint8)(int)vaa);
		}
		else if (a.arguments[i].isInt() || a.arguments[i].isDouble() || a.arguments[i].isInt64() || a.arguments[i].isBool())
		{
			data.add((uint8)(int)a.arguments[i]);
		}
	}

	return data;
}



void SerialInterface::itemAdded(GenericControllableItem*)
{
	customSerialListeners.call(&SerialInterfaceListener::customParamsChanged, this);
}

void SerialInterface::itemRemoved(GenericControllableItem*)
{
	customSerialListeners.call(&SerialInterfaceListener::customParamsChanged, this);
}



SerialInterface::CustomSerialParams::CustomSerialParams(SerialInterface* i) :
	ControllableContainer("Interface Parameters"),
	itf(i)
{
	if (itf != nullptr) itf->addSerialInterfaceListener(this);
	rebuildArgsFromInterface();
}


SerialInterface::CustomSerialParams::~CustomSerialParams()
{
	if (itf != nullptr) itf->removeSerialInterfaceListener(this);
	itf = nullptr;
}


void SerialInterface::CustomSerialParams::customParamsChanged(SerialInterface*)
{
	rebuildArgsFromInterface();
}

void SerialInterface::CustomSerialParams::rebuildArgsFromInterface()
{
	if (itf == nullptr) return;

	var oldData = getJSONData();
	clear();

	for (auto& gci : itf->customParams.items)
	{
		if (gci->controllable->type == Controllable::TRIGGER) continue;

		if (Parameter* p = ControllableFactory::createParameterFrom((Parameter*)gci->controllable, true, true))
		{
			p->canBeDisabledByUser = true;
			p->setEnabled(false);
			addParameter(p);
		}
	}

	loadJSONData(oldData);
}

var SerialInterface::CustomSerialParams::getParamValues()
{
	Array<WeakReference<Parameter>> params = getAllParameters();

	var values(new DynamicObject());
	for (auto& p : params)
	{
		Parameter* targetP = p;
		if (!p->enabled)
		{
			if (GenericControllableItem* gci = itf->customParams.getItemWithName(p->shortName)) targetP = (Parameter*)gci->controllable;
			else targetP = nullptr;
		}

		if (targetP != nullptr) values.getDynamicObject()->setProperty(targetP->shortName, targetP->getValue());

	}
	return values;
}
