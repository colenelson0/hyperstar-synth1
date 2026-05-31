//
//  WaveSynth.h
//  HyperStar S1 Proto2
//
//  Created by Cole Nelson on 5/20/26.
//  Copyright © 2026 Flash Memory. All rights reserved.
//

#pragma once
#include <JuceHeader.h>
#include "Oscillator.h"

class WaveSynth
{
public:
    void prepareToPlay(double sampleRate, std::string tableType);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    
    [[nodiscard]] float getAmplitude() const { return amplitude; };
    void setAmplitude(const float newAmplitude) { amplitude = newAmplitude; };
    
private:
    double sampleRate;
    float amplitude = 0.0f;
    std::vector<Oscillator> oscillators;
    
    // Helper functions
    void initializeOscillators(std::string tableType);
    std::vector<float> generateWavetable(std::string tableType);
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    float midiNoteNumberToFrequency(const float noteNum);
    void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
    
};
