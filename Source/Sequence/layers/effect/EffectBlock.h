/*
  ==============================================================================

    EffectBlock.h
    Created: 10 Oct 2020 10:46:50am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Effect;

class EffectBlock :
    public LayerBlock
{
public:
    EffectBlock(var params = var());
    ~EffectBlock();

    String effectType;
    std::unique_ptr<Effect> effect;

    FloatParameter* fadeIn;
    FloatParameter* fadeOut;

    virtual void processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier = 1.0f, int id = -1, float time = -1);

    void onContainerParameterChangedInternal(Parameter* p) override;
	virtual void controllableStateChanged(Controllable* c) override;

	class  EffectBlockListener
	{
	public:
		/** Destructor. */
		virtual ~EffectBlockListener() {}
		virtual void effectBlockFadesChanged(EffectBlock*) {}
	};

	ListenerList<EffectBlockListener> effectBlockListeners;
	void addEffectBlockListener(EffectBlockListener* newListener) { effectBlockListeners.add(newListener); }
	void removeEffectBlockListener(EffectBlockListener* listener) { effectBlockListeners.remove(listener); }

	// ASYNC
	class  EffectBlockEvent
	{
	public:
		enum Type { FADES_CHANGED, REGENERATE_PREVIEW };

		EffectBlockEvent(Type t, EffectBlock* p, var v = var()) :
			type(t), effectBlock(p), value(v) {}

		Type type;
		EffectBlock* effectBlock;
		var value;
	};

	QueuedNotifier<EffectBlockEvent> effectBlockNotifier;
	typedef QueuedNotifier<EffectBlockEvent>::Listener AsyncListener;

	void addAsyncEffectBlockListener(AsyncListener* newListener) { effectBlockNotifier.addListener(newListener); }
	void addAsyncCoalescedEffectBlockListener(AsyncListener* newListener) { effectBlockNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncEffectBlockListener(AsyncListener* listener) { effectBlockNotifier.removeListener(listener); }

    String getTypeString() const override { return effectType; }
    static EffectBlock* create(var params) { return new EffectBlock(params); }
};