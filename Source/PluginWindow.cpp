/*
  ==============================================================================

    PluginWindow.cpp
    Created: 25 Jan 2019 11:18:59am
    Author:  Ashley

  ==============================================================================
*/

#include "PluginWindow.h"
#include "MixingKnobPluginProcessor.h"

//inu chci to resit tak, ze se kazdeho pluginu budu na editor ptat nejvys jednou
//tj. musim si vracenou hodnotu zapamatovat, aby bylo mozne editor znovu otevrit,
//i kdyz uz byl jednou zavren

//vidim dve moznosti: ukladat primo ten editor
//nebo ukladat cele okno (instanci zdehle PluginWindow)
//budiz to primo editor, okno vytvarejme vzdy znovu

/*PluginWindow::PluginWindow(AudioProcessor* plugin, MixingKnobPluginAudioProcessor& parent) : 
	DocumentWindow(plugin->getName(),
		LookAndFeel::getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
		DocumentWindow::minimiseButton | DocumentWindow::closeButton),
	parent_{parent}
{
	setSize(400, 300);

	if (auto* ui = plugin->createEditorIfNeeded()) {
		setContentNonOwned(ui, true);
	}

	setTopLeftPosition(100, 100);
	setVisible(true);
}*/

PluginWindow::PluginWindow(String pluginName, Component* pluginEditor, MixingKnobPluginAudioProcessor& parent) : 
	DocumentWindow(pluginName,
		LookAndFeel::getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
		DocumentWindow::minimiseButton | DocumentWindow::closeButton),
	parent_{parent}
{
	setSize(400, 300);
  setContentNonOwned(pluginEditor, true);
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
