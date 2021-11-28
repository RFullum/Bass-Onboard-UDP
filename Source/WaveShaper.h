/*
  ==============================================================================

    WaveShaper.h
    Created: 8 Feb 2021 3:02:31pm
    Author:  Robert Fullum

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DryWet.h"

/// Waveshape Distortion Class
class WaveShaper
{
public:
    WaveShaper();
    ~WaveShaper();
    
    void setSampleRate(float SR);

    /// Returns an audio buffer of the dry/wet mixed Waveshaped signal
    juce::AudioBuffer<float> processWaveshape(juce::AudioBuffer<float>& bufferIn, float wsAmount, float dryWetVal);
    
    void processWaveshapeBuffer(juce::AudioBuffer<float>& bufferIn, float wsAmount, float dryWetVal);
    
private:
    // Dry Wet Class Instance
    std::unique_ptr<DryWet> dryWet;
    
    // Member Variables
    juce::SmoothedValue<float> wsAmountSmooth;
    juce::SmoothedValue<float> dryWetSmooth;
};
