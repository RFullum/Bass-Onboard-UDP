/*
  ==============================================================================

    BitCrusher.h
    Created: 9 Feb 2021 10:20:32am
    Author:  Robert Fullum

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DryWet.h"
#include "Wavetable.h"

class BitCrusher
{
public:
    BitCrusher();
    ~BitCrusher();
    
    /// Sets up BitCrusher class
    void prepare(float SR);
    
    /// Takes AudioBuffer and crush amount (0 to 1). Returns bitcrushed AudioBuffer. onOff is 0 or 1 from ParameterChoice
    juce::AudioBuffer<float> process(juce::AudioBuffer<float>& bufferIn, float crushAmt, float dryWetVal);
    
    void processBuffer(juce::AudioBuffer<float>& bufferIn, float crushAmt, float dryWetVal);
    
private:
    /// Populates Wavetable
    void setWavetables();
    
    juce::AudioBuffer<float> crush(juce::AudioBuffer<float> &bufferIn, float dryWetAmt);
    
    void crushBuffer(juce::AudioBuffer<float> &bufferIn, float dryWetAmt);
    
    std::unique_ptr<DryWet>          dryWet;
    std::unique_ptr<SquareWavetable> squareWT;

    // Member Variables
    float sampleRate;
    
    juce::SmoothedValue<float> dryWetSmooth;
    juce::AudioBuffer<float>   sampleBuffer;
};


//=========================================================
//=========================================================
//=========================================================
