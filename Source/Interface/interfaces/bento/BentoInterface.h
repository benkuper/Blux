/*
  ==============================================================================

    BentoInterface.h
    Created: 29 Sep 2020 3:56:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class BentoInterface :
    public Interface
{
public:
    BentoInterface();
    ~BentoInterface();

    BoolParameter* multiStripMode;

    const int remoteLedPort = 8888;
    const int remoteOSCPort = 9000;
    
    DatagramSocket ledSender;
    OSCSender oscSender;

    void sendValuesForObjectInternal(Object* o) override;

    class BentoInterfaceParams : public ControllableContainer
    {
    public:
        BentoInterfaceParams();

        StringParameter* remoteHost;
        //IntParameter* sendRate;
        IntParameter* stripIndex;
    };

    ControllableContainer* getInterfaceParams() override { return new BentoInterfaceParams(); }

    String getTypeString() const override { return "Bento"; }
    static BentoInterface* create(var params) { return new BentoInterface(); };
};