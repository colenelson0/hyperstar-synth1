//
//  Oscillator.h
//  HyperStar S1 Proto2
//
//  Created by Cole Nelson on 5/20/26.
//  Copyright © 2026 Flash Memory. All rights reserved.
//

#pragma once
#include <JuceHeader.h>
#include <vector>

class Oscillator
{
public:
    Oscillator(std::vector<float> wavetable, double sampleRate);
    void setFrequency(float frequency);
    float getSample();
    void stop();
    bool isPlaying();
private:
    std::vector<float> wavetable;
    double sampleRate;
    float index = 0.0f;
    float indexIncrement = 0.0f;
    
    float linearInterpolation();
};
