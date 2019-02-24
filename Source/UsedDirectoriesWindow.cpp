/*
  ==============================================================================

    UsedDirectoriesWindow.cpp
    Created: 28 Jan 2019 4:07:26pm
    Author:  Ashley

  ==============================================================================
*/

#include "UsedDirectoriesWindow.h"
#include "MixingKnobPluginProcessor.h"

UsedDirectoriesComponent::UsedDirectoriesComponent(MixingKnobPluginAudioProcessor& processor) :
	processor_{ processor }
{
	processor.addChangeListener(this);

	table.getHeader().addColumn("Directory", 1, 320, 30, 400);
	table.getHeader().setSortColumnId(1, true);
	table.setColour(ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness(1);
	table.setMultipleSelectionEnabled(true);
	table.setClickingTogglesRowSelection(true);
	addAndMakeVisible(table);

	addButton.onClick = [this] {addButtonClicked(); };
	addButton.setButtonText("Add directory");
	addAndMakeVisible(addButton);

	removeButton.onClick = [this] {removeButtonClicked(); };
	removeButton.setButtonText("Remove selected");
	removeButton.setEnabled(false);
	addAndMakeVisible(removeButton);

	rescanButton.onClick = [this] { rescanButtonClicked(); };
	rescanButton.setButtonText("Rescan directories");
	addAndMakeVisible(rescanButton);

	closeButton.onClick = [this] { closeButtonClicked(); };
	closeButton.setButtonText("Close");
	addAndMakeVisible(closeButton);

	setSize(410, 300);
}

void UsedDirectoriesComponent::resized()
{
	table.setBounds(10, 10, 380, 200);
	addButton.setBounds(20, 240, 60, 30);
	removeButton.setBounds(120, 240, 60, 30);
	rescanButton.setBounds(220, 240, 60, 30);
	closeButton.setBounds(320, 240, 60, 30);
}

int UsedDirectoriesComponent::getNumRows()
{
	return processor_.getUsedDirectories().size();
}

void UsedDirectoriesComponent::paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
	if (rowNumber >= processor_.getUsedDirectories().size()) return;
	auto alternateColor = getLookAndFeel().findColour(ListBox::backgroundColourId)
		.interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);
	if (rowIsSelected) {
		g.fillAll(Colours::lightblue);
	}
	else {
		if (rowNumber % 2) {
			g.fillAll(alternateColor);
		}
	}
}

void UsedDirectoriesComponent::paintCell(Graphics& g, int rowNumber, int /*colNumber*/, int width, int height, bool rowIsSelected)
{
	if (rowNumber >= processor_.getUsedDirectories().size()) return;
	g.setColour(rowIsSelected ? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId));
	g.setFont(font);
	auto text = processor_.getUsedDirectories().getReference(rowNumber).getFullPathName();
	g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft);

	g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
	g.fillRect(width - 1, 0, 1, height);
}

void UsedDirectoriesComponent::changeListenerCallback(ChangeBroadcaster* changeBroadcatser)
{
	if (changeBroadcatser == &processor_) {
		table.updateContent();
		table.repaint();
	}
}

void UsedDirectoriesComponent::addButtonClicked()
{
	FileChooser chooser("Select a directory to scan...");
	if (chooser.browseForDirectory()) {
		auto dir = chooser.getResult();
		processor_.addUsedDirectory(dir);
	}
}

void UsedDirectoriesComponent::removeButtonClicked()
{
	auto selectedRowsRanges = table.getSelectedRows().getRanges();
	Array<File> selectedDirectories;
	for (auto range : selectedRowsRanges ) {
		for (int i = range.getStart(); i < range.getEnd(); ++i) {
			selectedDirectories.add(processor_.getUsedDirectories()[i]);
		}
	}
	processor_.removeUsedDirectories(selectedDirectories);
}

void UsedDirectoriesComponent::rescanButtonClicked()
{
	processor_.rescanUsedDirectories();
}

void UsedDirectoriesComponent::closeButtonClicked()
{
	processor_.closeUsedDirectoriesWindow();
}

void UsedDirectoriesComponent::selectedRowsChanged(int lastRowSelected)
{
	auto selectedRows = table.getSelectedRows();
	removeButton.setEnabled(selectedRows.size() != 0);
}

UsedDirectoriesWindow::UsedDirectoriesWindow(MixingKnobPluginAudioProcessor& parent) :
	DocumentWindow("Used directories",
		LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
		DocumentWindow::minimiseButton | DocumentWindow::closeButton),
	parent_{ parent }
{
	setSize(400, 300);
	setContentOwned(new UsedDirectoriesComponent(parent), true);

	setTopLeftPosition(150, 150);
	setVisible(true);
}

UsedDirectoriesWindow::~UsedDirectoriesWindow()
{
	clearContentComponent();
}

void UsedDirectoriesWindow::closeButtonPressed()
{
	parent_.closeUsedDirectoriesWindow();
	//delete this;
}