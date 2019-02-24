/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GraphMixingImplementation.h"
#include "PluginWindow.h"
#include "PrescanFileMgr.h"
#include "UsedDirectoriesWindow.h"
#include "ScannedDirectoriesMap.h"

//==============================================================================
/**
*/
class MixingKnobPluginAudioProcessor : public AudioProcessor,
	public ChangeBroadcaster
{
public:
	//==============================================================================
	MixingKnobPluginAudioProcessor();
	~MixingKnobPluginAudioProcessor();

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

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
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;

	//==============================================================================
	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	//==============================================================================
	//void scanDirectory(String path);
	//const KnownPluginList& getKnownPluginList();
	using PluginsList = ScannedDirectoriesMap::PluginsList;
	PluginsList getAvailablePluginsList();
	void loadPluginWithIndex(int index);
	void loadPluginWithDescription(const PluginDescription& descrition);
	void openPluginWindow();
	void closePluginWindow();
	void openUsedDirectoriesWindow();
	void closeUsedDirectoriesWindow();
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
	double getGain();

	void addUsedDirectory(File directory);
	void removeUsedDirectories(Array<File> directories);
	void scanDirectory(File directory);
	void rescanUsedDirectories();
	void updateAvailablePluginsList();
	bool isInnerPluginSet();
	PluginDescription getInnerPluginDescription();

	static File findPrescanFile();
	static Array<File> getPrescanDirectories();

	const Array<File>& getUsedDirectories() const;
private:
	std::unique_ptr<MixingImplementation> impl;

	VST3PluginFormat format;
	//HashMap<String, Array<PluginDescription>> knownPluginsMap;
	ScannedDirectoriesMap scannedDirectoriesMap;
	//KnownPluginList knownPluginList;
	PluginsList availablePluginsList;
	AudioPluginInstance* innerPlugin{ nullptr };
	std::unique_ptr<PluginWindow> innerPluginWindow;

	//StringArray usedDirectories{};
	Array<File> usedDirectories;
	std::unique_ptr<UsedDirectoriesWindow> usedDirectoriesWindow;

	PrescanFileMgr prescanFileMgr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixingKnobPluginAudioProcessor)
};
