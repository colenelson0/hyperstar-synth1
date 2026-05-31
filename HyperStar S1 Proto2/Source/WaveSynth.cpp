//
//  WaveSynth.cpp
//  HyperStar S1 Proto2
//
//  Created by Cole Nelson on 5/20/26.
//  Copyright © 2026 Flash Memory. All rights reserved.
//

#include "WaveSynth.h"

void WaveSynth::prepareToPlay(double sampleRate, std::string tableType)
{
    this->sampleRate = sampleRate;
    
    initializeOscillators(tableType);
}

void WaveSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    int currentSample = 0;
    
    for (const auto midiMessage : midiMessages)
    {
        const auto midiEvent = midiMessage.getMessage();
        const int midiEventSample = static_cast<int>(midiEvent.getTimeStamp());
        
        render(buffer, currentSample, midiEventSample);
        handleMidiEvent(midiEvent);
        
        currentSample = midiEventSample;
    }
    
    render(buffer, currentSample, buffer.getNumSamples());
}

void WaveSynth::initializeOscillators(std::string tableType)
{
    constexpr int OSC_COUNT = 128;
    
    const std::vector<float> wavetable = generateWavetable(tableType);
    
    oscillators.clear();
    for (int i = 0; i < OSC_COUNT; ++i)
    {
        oscillators.emplace_back(wavetable, sampleRate);
    }
}

std::vector<float> WaveSynth::generateWavetable(std::string tableType)
{
    constexpr int TABLE_LENGTH = 128;
    std::vector<float> wavetable(TABLE_LENGTH);
    
    const float PI = std::atanf(1.0) * 4.0f;
    const float TL_FLOAT = static_cast<float>(TABLE_LENGTH);
    
    if (tableType == "saw")
    {
        for (int i = 0; i < TABLE_LENGTH; ++i)
        {
            wavetable[i] = (2.0f * static_cast<float>(i) / TL_FLOAT) - 1.0f;
        }
    }
    else if (tableType == "square")
    {
        for (int i = 0; i < TABLE_LENGTH; ++i)
        {
            wavetable[i] = (static_cast<float>(i) < TL_FLOAT / 2.0f) ? 1.0f : -1.0f;
        }
    }
    else // sine will be the default shape
    {
        for (int i = 0; i < TABLE_LENGTH; ++i)
        {
            wavetable[i] = std::sinf(2 * PI * static_cast<float>(i) / TL_FLOAT);
        }
    }
    
    return wavetable;
}

void WaveSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
    if (midiEvent.isNoteOn())
    {
        const float oscillatorId = static_cast<float>(midiEvent.getNoteNumber());
        const float frequency = midiNoteNumberToFrequency(oscillatorId);
        oscillators[oscillatorId].setFrequency(frequency);
    }
    else if (midiEvent.isNoteOff())
    {
        const float oscillatorId = static_cast<float>(midiEvent.getNoteNumber());
        oscillators[oscillatorId].stop();
    }
    else if (midiEvent.isAllNotesOff())
    {
        for (auto& oscillator : oscillators)
        {
            oscillator.stop();
        }
    }
}

float WaveSynth::midiNoteNumberToFrequency(const float noteNum)
{
    constexpr float A4_FREQUENCY = 440.0f;
    constexpr float A4_NOTE_NUM = 69.0f;
    
    return A4_FREQUENCY * std::powf(2.0f, (noteNum - A4_NOTE_NUM) / 12.0f);
}

void WaveSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    auto* firstChannel = buffer.getWritePointer(0);
    
    for (auto& oscillator : oscillators)
    {
        if (oscillator.isPlaying())
        {
            for (int sample = startSample; sample < endSample; ++sample)
            {
                firstChannel[sample] += (oscillator.getSample() * amplitude * 0.5);
            }
        }
    }
    
    for (int channel = 1; channel < buffer.getNumChannels(); ++channel)
    {
        std::copy(firstChannel + startSample, firstChannel + endSample, buffer.getWritePointer(channel) + startSample);
    }
}
