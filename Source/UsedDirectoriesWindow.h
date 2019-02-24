/*
  ==============================================================================

    UsedDirectoriesWindow.h
    Created: 28 Jan 2019 4:07:26pm
    Author:  Ashley

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MixingKnobPluginAudioProcessor;

class UsedDirectoriesComponent : public Component,
	public TableListBoxModel, public ChangeListener
{
public:
	UsedDirectoriesComponent(MixingKnobPluginAudioProcessor& processor);
	void resized() override;
	int getNumRows() override;
	void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

	void changeListenerCallback(ChangeBroadcaster* changeBroadcaster) override;
private:
	MixingKnobPluginAudioProcessor& processor_;
	TableListBox table{ {}, this };
	Font font{ 14.0f };
	TextButton addButton;
	TextButton removeButton;
	TextButton rescanButton;
	TextButton closeButton;

	void addButtonClicked();
	void removeButtonClicked();
	void rescanButtonClicked();
	void closeButtonClicked();
	void selectedRowsChanged(int lastRowSelected) override;
};

class UsedDirectoriesWindow : public DocumentWindow {
public:
	UsedDirectoriesWindow(MixingKnobPluginAudioProcessor& parent);
	~UsedDirectoriesWindow();

	void closeButtonPressed() override;
private:
	MixingKnobPluginAudioProcessor& parent_;
};
