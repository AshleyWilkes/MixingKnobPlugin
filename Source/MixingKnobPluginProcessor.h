/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GainProcessor.h"

//==============================================================================
/**
*/
class MixingKnobPluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    MixingKnobPluginAudioProcessor();
    ~MixingKnobPluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//==============================================================================
	void scanDirectory(String path);
	const KnownPluginList& getKnownPluginList();
	void loadPluginWithIndex(int index);
	//nezrejme.
	//hodnota parametru je 0 - 100. reprezentuje procentualni silu druhe (delsi) vetve graphu
	//jinak receno pro hodnotu 0 ma gain prave vetve vstup zcela ztlumit a gain leve vetve nejspis
	//dvakrat zesilit, aby vysledkem bylo totez co nezkresleny signal
	//hodnota 50 by asi mela byt, ze oba gainy jsou 0
	//
	//lepsi cesta na uvod je jina:
	//leva cesta dostane jako gain hodnotu newGainValue/50
	//prava cesta dostane 2 - leva cesta
	//tyto hodnoty se v processBlocku pouzijou jako parametr metody AudioBuffer.applyGain
	void setGain(double newGainValue);

private:
	KnownPluginList knownPluginList;

	using GraphIO = AudioProcessorGraph::AudioGraphIOProcessor;
	using NodePtr = AudioProcessorGraph::Node::Ptr;
	using NodeId = AudioProcessorGraph::NodeID;
	std::unique_ptr<AudioProcessorGraph> processorGraph;
	NodePtr audioInputNode;
	NodePtr audioOutputNode;
	GainProcessor* leftGain;
	GainProcessor* rightGain;
	AudioProcessor* innerPlugin;

	NodeId INPUT{ 100 };
	NodeId OUTPUT{ 101 };
	NodeId LEFT_GAIN{ 102 };
	NodeId RIGHT_GAIN{ 103 };
	NodeId INNER_PLUGIN{ 104 };

	void initializeGraph();
	void connect(NodeId in, NodeId out);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixingKnobPluginAudioProcessor)
};
