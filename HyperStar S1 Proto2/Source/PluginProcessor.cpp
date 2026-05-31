/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HyperStarS1Proto2AudioProcessor::HyperStarS1Proto2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), state(*this, nullptr, "parameters", createParameters())
#endif
{
}

HyperStarS1Proto2AudioProcessor::~HyperStarS1Proto2AudioProcessor()
{
}

//==============================================================================
const juce::String HyperStarS1Proto2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HyperStarS1Proto2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HyperStarS1Proto2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HyperStarS1Proto2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HyperStarS1Proto2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HyperStarS1Proto2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HyperStarS1Proto2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void HyperStarS1Proto2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HyperStarS1Proto2AudioProcessor::getProgramName (int index)
{
    return {};
}

void HyperStarS1Proto2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HyperStarS1Proto2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    waveSynth.prepareToPlay(sampleRate, "saw");
}

void HyperStarS1Proto2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HyperStarS1Proto2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void HyperStarS1Proto2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    buffer.clear();
    
    float amp = state.getRawParameterValue("waveAmp")->load();
    waveSynth.setAmplitude(amp);
    
    waveSynth.processBlock(buffer, midiMessages);
}

//==============================================================================
bool HyperStarS1Proto2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HyperStarS1Proto2AudioProcessor::createEditor()
{
    return new HyperStarS1Proto2AudioProcessorEditor (*this);
}

//==============================================================================
void HyperStarS1Proto2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HyperStarS1Proto2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HyperStarS1Proto2AudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout HyperStarS1Proto2AudioProcessor::createParameters()
{
    return
    {
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"waveAmp"}, "Osc Amplitude", 0.0f, 1.0f, 0.6f)
    };
}
