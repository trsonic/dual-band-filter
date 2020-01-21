/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

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
void DualBandFilterAudioProcessor::setFilterCoeffs(double cutoff, double sampleRate)
{
	double k, k2, a0, denominator, a1, a2, bLp0, bLp1, bLp2, bHp0, bHp1, bHp2;
	k = tan((MathConstants<double>::pi * cutoff) / (sampleRate));
	k2 = 2 * k;
	a0 = 1;
	denominator = pow(k, 2) + k2 + 1;
	a1 = (2 * (pow(k, 2) - 1)) / denominator;
	a2 = (pow(k, 2) - k2 + 1) / denominator;
	bLp0 = pow(k, 2) / denominator;
	bLp1 = 2 * bLp0;
	bLp2 = bLp0;
	bHp0 = 1 / denominator;
	bHp1 = -2 * bHp0;
	bHp2 = bHp0;

	int numberOfChannelsProcessed = (int)pow(ambisonicOrder + 1, 2);

	lowPassFilterArray.clear();
	highPassFilterArray.clear();

	for (int i = 0; i < numberOfChannelsProcessed; ++i)
	{
		lowPassFilterArray.add(new IIRFilter);
		highPassFilterArray.add(new IIRFilter);
		lowPassFilterArray[i]->setCoefficients(IIRCoefficients(bLp0, bLp1, bLp2, a0, a1, a2));
		highPassFilterArray[i]->setCoefficients(IIRCoefficients(bHp0, bHp1, bHp2, a0, a1, a2));
	}
}

void DualBandFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	ambisonicOrder = ambisonicOrderSelected;

	// Calculate and set dual band filters coefficients
	setFilterCoeffs(xOverFreqs[ambisonicOrder - 1], sampleRate);

}

void DualBandFilterAudioProcessor::releaseResources()
{
	lowPassFilterArray.clear();
	highPassFilterArray.clear();
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
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	// check the current selected ambisonic order
	if (ambisonicOrder != ambisonicOrderSelected)
	{
		ambisonicOrder = ambisonicOrderSelected;
		setFilterCoeffs(xOverFreqs[ambisonicOrder - 1], getSampleRate());
	}

	// clear the buffer for channels above the processed number of ambisonic channels
	int numberOfChannelsProcessed = (int)pow(ambisonicOrder + 1, 2);
	for (auto i = numberOfChannelsProcessed; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// get the buffer length and duplicate buffer for dual band processing
	int bufferLength = buffer.getNumSamples();
	lowPassedBuffer.makeCopyOf(buffer);
	highPassedBuffer.makeCopyOf(buffer);

    for (int ch = 0; ch < numberOfChannelsProcessed && numberOfChannelsProcessed <= totalNumInputChannels; ++ch)
    {
		lowPassFilterArray[ch]->processSamples(lowPassedBuffer.getWritePointer(ch), bufferLength);
		highPassFilterArray[ch]->processSamples(highPassedBuffer.getWritePointer(ch), bufferLength);

		// Get the Ambisonic order of the current channel
		int currentOrder = floor(sqrt(ch));

		// Apply gain correction and combine two bands
		FloatVectorOperations::multiply(highPassedBuffer.getWritePointer(ch), coeffsGainCorrected[ambisonicOrder - 1][currentOrder], bufferLength);
		FloatVectorOperations::add(buffer.getWritePointer(ch), lowPassedBuffer.getReadPointer(ch), highPassedBuffer.getReadPointer(ch), bufferLength);
    }
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
	pluginSettings = String(ambisonicOrderSelected);
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
	ambisonicOrderSelected = loadedData[0].getIntValue();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DualBandFilterAudioProcessor();
}
