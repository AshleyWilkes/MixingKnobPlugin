/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "MixingKnobPluginProcessor.h"
#include "MixingKnobPluginEditor.h"

//==============================================================================
MixingKnobPluginAudioProcessor::MixingKnobPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                       ),
	processorGraph(new AudioProcessorGraph())
{
}

MixingKnobPluginAudioProcessor::~MixingKnobPluginAudioProcessor()
{
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

void MixingKnobPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String MixingKnobPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void MixingKnobPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MixingKnobPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	processorGraph->setPlayConfigDetails(
		getMainBusNumInputChannels(),
		getMainBusNumOutputChannels(), 
		sampleRate, samplesPerBlock);
	processorGraph->prepareToPlay(sampleRate, samplesPerBlock);

	initializeGraph();
}

void MixingKnobPluginAudioProcessor::releaseResources()
{
	processorGraph->releaseResources();
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
    ScopedNoDenormals noDenormals;
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
	processorGraph->processBlock(buffer, midiMessages);
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MixingKnobPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MixingKnobPluginAudioProcessor();
}

void MixingKnobPluginAudioProcessor::scanDirectory(String path)
{
	//stalo by mozna za to resit, jestli je path jina nez predtim, a pokud ne, nedelat nic
	knownPluginList.clearBlacklistedFiles();
	VST3PluginFormat format;
	PluginDirectoryScanner scanner{ knownPluginList, format, path, true, File{} };
	String scannedFileName;
	while (scanner.scanNextFile(true, scannedFileName));
}

const KnownPluginList& MixingKnobPluginAudioProcessor::getKnownPluginList() 
{
	return knownPluginList;
}

void MixingKnobPluginAudioProcessor::loadPluginWithIndex(int index)
{
	if (processorGraph->getNumNodes() > 0) {
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error", "Changing loaded plugin not supported", "Close");
		return;
	}
}

void MixingKnobPluginAudioProcessor::setGain(double newGainValue)
{
	if (newGainValue < 0) newGainValue = 0;
	if (newGainValue > 100) newGainValue = 100;
	double val = newGainValue / 50;
	leftGain->setGain(val);
	rightGain->setGain(2 - val);
}

void MixingKnobPluginAudioProcessor::connect(NodeId in, NodeId out)
{
	for (int channel = 0; channel < getMainBusNumInputChannels(); ++channel) {
		processorGraph->addConnection({
			{ in, channel },
			{ out, channel }
		});
	}
}

void MixingKnobPluginAudioProcessor::initializeGraph()
{
	processorGraph->clear();//pro jistotu a taky proto, abych to nemohl zapomenout, az budu implementovat zmenu vlozeneho pluginu

							//vytvorim input/output
	audioInputNode = processorGraph->addNode(new GraphIO(GraphIO::audioInputNode), INPUT);
	audioOutputNode = processorGraph->addNode(new GraphIO(GraphIO::audioOutputNode), OUTPUT);

	auto left = std::make_unique<GainProcessor>();
	left->setPlayConfigDetails(
		getMainBusNumInputChannels(), getMainBusNumOutputChannels(),
		getSampleRate(), getBlockSize());
	processorGraph->addNode(left.get(), LEFT_GAIN);
	leftGain = left.release();

	auto right = std::make_unique<GainProcessor>();
	right->setPlayConfigDetails(
		getMainBusNumInputChannels(), getMainBusNumOutputChannels(),
		getSampleRate(), getBlockSize());
	processorGraph->addNode(right.get(), RIGHT_GAIN);
	rightGain = right.release();

	auto inner = std::make_unique<GainProcessor>();
	inner->setGain(-100);
	inner->setPlayConfigDetails(
		getMainBusNumInputChannels(), getMainBusNumOutputChannels(),
		getSampleRate(), getBlockSize());
	processorGraph->addNode(inner.get(), INNER_PLUGIN);
	innerPlugin = inner.release();

	connect(INPUT, LEFT_GAIN);
	connect(LEFT_GAIN, OUTPUT);
	connect(INPUT, RIGHT_GAIN);
	connect(RIGHT_GAIN, INNER_PLUGIN);
	connect(INNER_PLUGIN, OUTPUT);


	//vytvorit VST3PluginInstance

	//1.faze: proted propojim input s outputem a smitec -- OK, tohle funguje
	/*for (int channel = 0; channel < 2; ++channel) {
		processorGraph->addConnection({
			{ audioInputNode->nodeID, channel },
			{ audioOutputNode->nodeID, channel }
			});
	}*/
}