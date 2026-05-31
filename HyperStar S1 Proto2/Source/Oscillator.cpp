//
//  Oscillator.cpp
//  HyperStar S1 Proto2
//
//  Created by Cole Nelson on 5/20/26.
//  Copyright © 2026 Flash Memory. All rights reserved.
//

#include "Oscillator.h"

Oscillator::Oscillator(std::vector<float> wavetable, double sampleRate)
: wavetable{std::move(wavetable)}, sampleRate{sampleRate}
{}

void Oscillator::setFrequency(float frequency)
{
    indexIncrement = frequency * static_cast<float>(wavetable.size()) / static_cast<float>(sampleRate);
}

float Oscillator::getSample()
{
    const float sample = linearInterpolation();
    index += indexIncrement;
    // If new index is outside of the wavetable's range, jump back into the range
    index = std::fmod(index, static_cast<float>(wavetable.size()));
    return sample;
}

void Oscillator::stop()
{
    index = 0.0f;
    indexIncrement = 0.0f;
}

bool Oscillator::isPlaying()
{
    return indexIncrement != 0;
}


float Oscillator::linearInterpolation()
{
    // Here, we save the discrete index nearest to the left of the current index as truncatedIndex
    const int truncatedIndex = static_cast<int>(index);
    // Then, we use that to get the very next discrete index, which could at the beginning of the table if we need to loop around
    const int nextIndex = (truncatedIndex + 1) % static_cast<int>(wavetable.size());
    
    // The distance between the current index and truncatedIndex is used to determine the weight that nextIndex has on the return value
    const float nextIndexWeight = index - static_cast<float>(truncatedIndex);
    // The distance between the current index and nextIndex is used to determine the weight that truncatedIndex has on the return value
    const float truncatedIndexWeightB = 1.0f - nextIndexWeight;
    
    return wavetable[truncatedIndex] * truncatedIndexWeightB + wavetable[nextIndex] * nextIndexWeight;
}
