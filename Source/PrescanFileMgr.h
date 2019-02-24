/*
  ==============================================================================

    PrescanFileMgr.h
    Created: 2 Feb 2019 4:29:09pm
    Author:  Ashley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ScannedDirectoriesMap.h"

class MixingKnobPluginAudioProcessor;

class PrescanFileMgr : public ChangeListener
{
public:
	PrescanFileMgr(File file, ScannedDirectoriesMap& map, MixingKnobPluginAudioProcessor& processor);

	void changeListenerCallback(ChangeBroadcaster* changeBroadcaster) override;
private:
	File file_;
	ScannedDirectoriesMap& map_;
	MixingKnobPluginAudioProcessor& processor_;
};