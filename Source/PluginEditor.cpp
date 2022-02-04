#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DualBandFilterAudioProcessorEditor::DualBandFilterAudioProcessorEditor(DualBandFilterAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	orderList.add("1st Order");
	orderList.add("2nd Order");
	orderList.add("3rd Order");
	orderList.add("4th Order");
	orderList.add("5th Order");
	orderList.add("6th Order");
	orderList.add("7th Order");

	// combobox
	orderListComboBox.setEditableText(false);
	orderListComboBox.setJustificationType(Justification::centred);
	orderListComboBox.addItemList(orderList, 1);
	orderListComboBox.setSelectedItemIndex(processor.getOrder() - 1, dontSendNotification);
	orderListComboBox.addListener(this);
	addAndMakeVisible(orderListComboBox);

    setSize (400, 300);
}

DualBandFilterAudioProcessorEditor::~DualBandFilterAudioProcessorEditor()
{
}

//==============================================================================
void DualBandFilterAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText (orderList[processor.getOrder() - 1] + ", crossover freq: " + String(processor.getCurrentXFreq()) + " Hz", getLocalBounds(), Justification::centred, 1);
}

void DualBandFilterAudioProcessorEditor::resized()
{
	orderListComboBox.setBounds(10,10,150,25);
}

void DualBandFilterAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	processor.setOrder(orderListComboBox.getSelectedItemIndex() + 1);
	repaint();
}