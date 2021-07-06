#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DualBandFilterAudioProcessor::DualBandFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
						.withInput("Input", AudioChannelSet::discreteChannels(64), true)
                      #endif
						.withOutput("Output", AudioChannelSet::discreteChannels(64), true)
                     #endif
                       )
#endif
{
}

DualBandFilterAudioProcessor::~DualBandFilterAudioProcessor()
{
}

//==============================================================================
const String DualBandFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DualBandFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else 
    return false;
   #endif
}

bool DualBandFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DualBandFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DualBandFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DualBandFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DualBandFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DualBandFilterAudioProcessor::setCurrentProgram (int index)
{
}

const String DualBandFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void DualBandFilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================


void DualBandFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_dbFilter.init(sampleRate, m_dbFilter.getCurrentOrder());
}

void DualBandFilterAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DualBandFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
	return true;
}
#endif

void DualBandFilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // process here
    m_dbFilter.process(buffer);

    //// clear the buffer for channels above the processed number of ambisonic channels
    //int numberOfChannelsProcessed = (int)pow(m_ambisonicOrder + 1, 2);
    //for (auto i = numberOfChannelsProcessed; i < totalNumOutputChannels; ++i)
    //    buffer.clear(i, 0, buffer.getNumSamples());
}

//==============================================================================
bool DualBandFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DualBandFilterAudioProcessor::createEditor()
{
    return new DualBandFilterAudioProcessorEditor (*this);
}

//==============================================================================
void DualBandFilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	String pluginSettings;
	pluginSettings = String(m_dbFilter.getCurrentOrder());
	pluginSettings += "\n";
	MemoryOutputStream stream(destData, false);
	stream.writeText(pluginSettings, true, true, nullptr);
}

void DualBandFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	StringArray loadedData;
	loadedData.addLines(String::createStringFromData(data, sizeInBytes));
    m_dbFilter.init(getSampleRate(), loadedData[0].getIntValue());
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DualBandFilterAudioProcessor();
}
