/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class HyperStarS1Proto2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HyperStarS1Proto2AudioProcessorEditor (HyperStarS1Proto2AudioProcessor&);
    ~HyperStarS1Proto2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider gainKnob;
    juce::Label gainLabel { "gainLabel", "Gain" };
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HyperStarS1Proto2AudioProcessor& audioProcessor;
    
    juce::AudioProcessorValueTreeState::SliderAttachment gainKnobAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HyperStarS1Proto2AudioProcessorEditor)
};
