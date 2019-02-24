/*
  ==============================================================================

    PluginWindow.cpp
    Created: 25 Jan 2019 11:18:59am
    Author:  Ashley

  ==============================================================================
*/

#include "PluginWindow.h"
#include "MixingKnobPluginProcessor.h"

PluginWindow::PluginWindow(AudioProcessor* plugin, MixingKnobPluginAudioProcessor& parent) : 
	DocumentWindow(plugin->getName(),
		LookAndFeel::getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
		DocumentWindow::minimiseButton | DocumentWindow::closeButton),
	parent_{parent}
{
	setSize(400, 300);

	if (auto* ui = plugin->createEditor()) {
		setContentNonOwned(ui, true);
	}

	setTopLeftPosition(100, 100);
	setVisible(true);
}

PluginWindow::~PluginWindow()
{
	clearContentComponent();
}

void PluginWindow::closeButtonPressed()
{
	parent_.closePluginWindow();
	//delete this;
}