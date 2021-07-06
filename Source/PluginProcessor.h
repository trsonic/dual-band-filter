#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DualBandFilter.h"


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

    void setOrder(int order)
    {
        m_dbFilter.init(getSampleRate(), order);
    }

    int getOrder()
    {
        return m_dbFilter.getCurrentOrder();
    }

    double getCurrentXFreq()
    {
        return m_dbFilter.getCurrentXFreq();
    }

private:
    //==============================================================================

    DualBandFilter m_dbFilter;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DualBandFilterAudioProcessor)
};
