/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HyperStarS1Proto2AudioProcessorEditor::HyperStarS1Proto2AudioProcessorEditor (HyperStarS1Proto2AudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p), gainKnobAttachment (audioProcessor.getState(), "waveAmp", gainKnob)
{
    gainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    gainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    gainLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible (gainLabel);
    addAndMakeVisible (gainKnob);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

HyperStarS1Proto2AudioProcessorEditor::~HyperStarS1Proto2AudioProcessorEditor()
{
}

//==============================================================================
void HyperStarS1Proto2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

//    g.setColour (juce::Colours::white);
//    g.setFont (juce::FontOptions (15.0f));
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void HyperStarS1Proto2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const int centerX = getWidth() / 2;
    const int centerY = getHeight() / 2;
    gainLabel.setBounds(centerX - 75, centerY - 95, 150, 20);
    gainKnob.setBounds (centerX - 75, centerY - 75, 150, 150);
}
