/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DualBandFilterAudioProcessorEditor  : public AudioProcessorEditor,
											public ComboBox::Listener
{
public:
    DualBandFilterAudioProcessorEditor (DualBandFilterAudioProcessor&);
    ~DualBandFilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DualBandFilterAudioProcessor& processor;

	ComboBox orderListComboBox;
	StringArray orderList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DualBandFilterAudioProcessorEditor)
};
