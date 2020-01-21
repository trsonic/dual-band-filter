/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <random>

using std::vector;

//==============================================================================
/**
*/
class DualBandFilterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DualBandFilterAudioProcessor();
    ~DualBandFilterAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//public variables 
	int ambisonicOrder, ambisonicOrderSelected = 1;
	Array<double> xOverFreqs = { 690, 1250, 1831, 2423, 3022 };

private:
    //==============================================================================
	OwnedArray<IIRFilter> lowPassFilterArray, highPassFilterArray;		
	AudioBuffer<float> lowPassedBuffer, highPassedBuffer;
	vector<vector<double>> coeffsGainCorrected {
		{-1.41415265612978, -0.816531743649337,	0, 0, 0, 0},
		{-1.58112863177163, -1.22474223817030, -0.632463690644260, 0, 0, 0},
		{-1.66838610529516, -1.43664727526966, -1.02145240045448, -0.508189586539144, 0, 0},
		{-1.72075018733317, -1.55934381976132, -1.25924096053497, -0.862311050887111, -0.423065259648094, 0},
		{-1.75570426339051, -1.63719422561165, -1.41217329137904, -1.10328983994383, -0.741301139024051, -0.361642089896808}
	};

	void setFilterCoeffs(double cutoff, double sampleRate);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DualBandFilterAudioProcessor)
};
