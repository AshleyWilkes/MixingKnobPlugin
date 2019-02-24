/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "MixingKnobPluginProcessor.h"
#include "MixingKnobPluginEditor.h"
#include "GraphMixingImplementation.h"
#include "MixerMixingImplementation.h"

//==============================================================================
MixingKnobPluginAudioProcessor::MixingKnobPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                       ),
	impl(std::make_unique<GraphMixingImplementation>()),
	//impl(std::make_unique<MixerMixingImplementation>()),
	scannedDirectoriesMap(),
	prescanFileMgr(MixingKnobPluginAudioProcessor::findPrescanFile(), scannedDirectoriesMap, *this)
{
	//AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Error", "Using prescan file '" + prescanFile.getFullPathName() + "'", "Close");
	usedDirectories = scannedDirectoriesMap.keys();
	updateAvailablePluginsList();
	sendChangeMessage();
}

MixingKnobPluginAudioProcessor::~MixingKnobPluginAudioProcessor()
{
	closePluginWindow();
	closeUsedDirectoriesWindow();
}

//==============================================================================
const String MixingKnobPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MixingKnobPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MixingKnobPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MixingKnobPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MixingKnobPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MixingKnobPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MixingKnobPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MixingKnobPluginAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String MixingKnobPluginAudioProcessor::getProgramName (int /*index*/)
{
    return {};
}

void MixingKnobPluginAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void MixingKnobPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	impl->prepareToPlay(sampleRate, samplesPerBlock,
		getMainBusNumInputChannels(),
		getMainBusNumOutputChannels(),
		getBlockSize());
}

void MixingKnobPluginAudioProcessor::releaseResources()
{
	impl->releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MixingKnobPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MixingKnobPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	impl->processBlock(buffer, midiMessages);
}

//==============================================================================
bool MixingKnobPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MixingKnobPluginAudioProcessor::createEditor()
{
    return new MixingKnobPluginAudioProcessorEditor (*this);
}

//==============================================================================
void MixingKnobPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	MemoryOutputStream mos{ destData, false };
	//je treba ulozit
	//jestli mam aktivni plugin
	bool activePluginExists = ( innerPlugin != nullptr );
	mos.writeBool(activePluginExists);
	if (activePluginExists) {
		//kterej plugin je aktivni
		auto pluginDescription = innerPlugin->getPluginDescription();
		auto pluginDescriptionXmlPtr = pluginDescription.createXml();
		auto pluginDescriptionString = pluginDescriptionXmlPtr->createDocument(String());
		mos.writeString(pluginDescriptionString);
		//jaka je hodnota mixer knobu
		auto sliderValue = impl->getGain();
		mos.writeFloat(sliderValue);
		//jestli ma aktivni plugin otevreny okno
		bool isActivePluginWindowOpen = (innerPluginWindow != nullptr);
		mos.writeBool(isActivePluginWindowOpen);
	}
}

void MixingKnobPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	MemoryInputStream mis{ data, static_cast<size_t> (sizeInBytes), false };
	bool activePluginExists = mis.readBool();
	if (activePluginExists) {
		auto pluginDescriptionString = mis.readString();
		XmlDocument pluginDescriptionXmlDocument{ pluginDescriptionString };
		auto pluginDescriptionXmlPtr = pluginDescriptionXmlDocument.getDocumentElement();
		PluginDescription pluginDescription;
		pluginDescription.loadFromXml(*pluginDescriptionXmlPtr);
		loadPluginWithDescription(pluginDescription);

		auto sliderValue = mis.readFloat();
		impl->setGain(sliderValue);

		bool isActivePluginWindowOpen = mis.readBool();
		if (isActivePluginWindowOpen) {
			openPluginWindow();
		}
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	File logFile = File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory).getChildFile("Documents\\workspace\\JUCE_Plugins\\MixingKnobPlugin\\log.txt");
	logFile.replaceWithText("");
	FileLogger log(logFile, "");
	Logger::setCurrentLogger(&log);
    return new MixingKnobPluginAudioProcessor();
}

/*void MixingKnobPluginAudioProcessor::scanDirectory(String path)
{
	//stalo by mozna za to resit, jestli je path jina nez predtim, a pokud ne, nedelat nic
	knownPluginList.clearBlacklistedFiles();
	PluginDirectoryScanner scanner{ knownPluginList, format, path, true, File{} };
	String scannedFileName;
	while (scanner.scanNextFile(true, scannedFileName));
}*/

/*const KnownPluginList& MixingKnobPluginAudioProcessor::getKnownPluginList() 
{
	return knownPluginList;
}*/

MixingKnobPluginAudioProcessor::PluginsList
MixingKnobPluginAudioProcessor::getAvailablePluginsList()
{
	return availablePluginsList;
}

void MixingKnobPluginAudioProcessor::loadPluginWithIndex(int index)
{
	auto pluginDescription = availablePluginsList[index];
	loadPluginWithDescription(pluginDescription);
}

void MixingKnobPluginAudioProcessor::loadPluginWithDescription(const PluginDescription& pluginDescription)
{
	//prisla zadost o loadnuti plugin, ktery uz loadnuty je; nedelej nic
	if (innerPlugin && pluginDescription.isDuplicateOf(innerPlugin->getPluginDescription())) {
		return;
	}
	//loadujeme novy plugin (jiny nez pripadny stavajici), je treba zavrit okno jiz loadnuteho pluginu
	innerPluginWindow.reset(nullptr);
	innerPlugin = format.createInstanceFromDescription(pluginDescription, 0, 0);
	if (!innerPlugin) {
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Error", "CreatePlugin failed!", "Close");
		return;
	}

	impl->takeOwnershipOfPluginAndUseIt(innerPlugin);
}

void MixingKnobPluginAudioProcessor::openPluginWindow()
{
	if (!innerPlugin) {
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Error", "No plugin is selected!", "Close");
		return;
	}
	if (!innerPlugin->hasEditor()) {
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Error", "Plugin has no editor!", "Close");
		return;
	}
	innerPluginWindow.reset(new PluginWindow(innerPlugin, *this));
}

void MixingKnobPluginAudioProcessor::closePluginWindow()
{
	innerPluginWindow.reset(nullptr);
}

void MixingKnobPluginAudioProcessor::openUsedDirectoriesWindow()
{
	usedDirectoriesWindow.reset(new UsedDirectoriesWindow(*this));
}

void MixingKnobPluginAudioProcessor::closeUsedDirectoriesWindow()
{
	usedDirectoriesWindow.reset(nullptr);
}

void MixingKnobPluginAudioProcessor::setGain(double newGainValue)
{
	impl->setGain(static_cast<float>(newGainValue));
}

double MixingKnobPluginAudioProcessor::getGain()
{
	return impl ? impl->getGain() : 50;
}

void MixingKnobPluginAudioProcessor::addUsedDirectory(File directory)
{
	if (!scannedDirectoriesMap.contains(directory)) {
		scanDirectory(directory);
	}
	usedDirectories.add(directory);
	updateAvailablePluginsList();
	sendChangeMessage();
}

void MixingKnobPluginAudioProcessor::removeUsedDirectories(Array<File> directories)
{
	for (auto dir : directories) {
		usedDirectories.removeFirstMatchingValue(dir);
	}
	updateAvailablePluginsList();
	sendChangeMessage();
}

void MixingKnobPluginAudioProcessor::scanDirectory(File directory)
{
	Label scanningLabel{ "", "Scanning directory '" + directory.getFullPathName() + "'" };
	scanningLabel.setSize(500, 250);
	DialogWindow::showDialog("Scanning", &scanningLabel, nullptr, LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId), true);
	AlertWindow scanningWindow{ "Scanning", "Scanning directory '" + directory.getFullPathName() + "'", AlertWindow::AlertIconType::InfoIcon };
	KnownPluginList pathPluginList;
	PluginDirectoryScanner scanner{ pathPluginList, format, directory.getFullPathName(), true, File{} };
	String scannedFileName;
	try {
		while (scanner.scanNextFile(true, scannedFileName));
	}
	catch (std::exception& e) {}

	scannedDirectoriesMap.set(directory, pathPluginList);
	if (DialogWindow* dw = scanningLabel.findParentComponentOfClass<DialogWindow>()) {
		dw->exitModalState(1234);
	}
}

void MixingKnobPluginAudioProcessor::rescanUsedDirectories()
{
	scannedDirectoriesMap.clear();

	for (auto usedDir : usedDirectories) {
		scanDirectory(usedDir);
	}
	updateAvailablePluginsList();
	sendChangeMessage();
}

void MixingKnobPluginAudioProcessor::updateAvailablePluginsList()
{
	availablePluginsList = scannedDirectoriesMap.getPluginsListForDirectories(usedDirectories);
}

bool
MixingKnobPluginAudioProcessor::isInnerPluginSet() {
	return innerPlugin != nullptr;
}

PluginDescription
MixingKnobPluginAudioProcessor::getInnerPluginDescription() {
	return innerPlugin->getPluginDescription();
}

File MixingKnobPluginAudioProcessor::findPrescanFile()
{
	Logger::writeToLog("Find prescan file!");
	for (File dir : getPrescanDirectories()) {
		Logger::writeToLog("dir: " + dir.getFullPathName());
		if (!dir.exists() || !dir.isDirectory()) {
			/*AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Error",
				"Couldn't search for prescan.xml in '" + dir.getFullPathName() + "': Not an existing directory", "Close");*/
			Logger::writeToLog("ERROR: not an existing directory");
			continue;
		}

		//zjistit, jestli existuje prescan.xml
		File file = dir.getChildFile("MixingKnobPlugin" + File::getSeparatorString() + "prescan.xml");
		//pokud jo, vratit
		if (file.exists()) {
			Logger::writeToLog("Using existing file '" + file.getFullPathName() + "'");
			return file;
		}
		//pokud ne, zkusit vytvorit a vratit
		else if (file.create()) {
			Logger::writeToLog("Using newly created '" + file.getFullPathName() + "'");
			return file;
		}
		//file se nepovedlo vytvorit, pokracujeme nasledujicim adresarem
		Logger::writeToLog("Couldn't create '" + file.getFullPathName() + "'");
	}
	return File();
}

Array<File> MixingKnobPluginAudioProcessor::getPrescanDirectories()
{
	Array<File> result;
	result.add(File::getSpecialLocation(File::SpecialLocationType::currentExecutableFile).getParentDirectory());
	result.add(File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory));
	return result;
}

const Array<File>& MixingKnobPluginAudioProcessor::getUsedDirectories() const
{
	return usedDirectories;
}