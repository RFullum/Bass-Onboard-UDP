/*
  ==============================================================================

    FoldbackDistortion.h
    Created: 8 Feb 2021 3:50:17pm
    Author:  Robert Fullum

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DryWet.h"

class FoldbackDistortion
{
public:
    FoldbackDistortion();
    ~FoldbackDistortion();
    
    void setSampleRate(float SR);
    
    /// Returns an audio buffer of the dry/wet mixed Foldback signal
    juce::AudioBuffer<float> processFoldback(juce::AudioBuffer<float>& bufferIn, float foldbackAmount, float dryWetVal);
    
    void processFoldbackBuffer(juce::AudioBuffer<float>& bufferIn, float foldbackAmount, float dryWetVal);
    
private:
    // DryWet class instance
    std::unique_ptr<DryWet> dryWet;
    
    // Member Variables
    juce::SmoothedValue<float> foldbackAmtSmooth;
    juce::SmoothedValue<float> dryWetSmooth;
};
