/*
  ==============================================================================

    FoldbackDistortion.cpp
    Created: 8 Feb 2021 3:50:17pm
    Author:  Robert Fullum

  ==============================================================================
*/

#include "FoldbackDistortion.h"

FoldbackDistortion::FoldbackDistortion() :
    dryWet ( std::make_unique<DryWet>() )
{}


FoldbackDistortion::~FoldbackDistortion() {}


void FoldbackDistortion::setSampleRate(float SR)
{
    foldbackAmtSmooth.reset ( SR, 0.01f );
    dryWetSmooth.reset      ( SR, 0.01f );
    
    foldbackAmtSmooth.setTargetValue ( 0.0f );
    dryWetSmooth.setTargetValue      ( 0.0f );
}

juce::AudioBuffer<float> FoldbackDistortion::processFoldback(juce::AudioBuffer<float> &bufferIn, float foldbackAmount, float dryWetVal)
{
    auto* chanL = bufferIn.getWritePointer ( 0 );
    auto* chanR = bufferIn.getWritePointer ( 1 );
    
    foldbackAmtSmooth.setTargetValue ( foldbackAmount );
    dryWetSmooth.setTargetValue      ( dryWetVal      );
    
    for (int sample = 0; sample < bufferIn.getNumSamples(); sample++)
    {
        float foldbackAmtSmoothVal = foldbackAmtSmooth.getNextValue();
        float dryWetSmoothVal      = dryWetSmooth.getNextValue();
        
        float wetSampleL = juce::dsp::FastMathApproximations::sin( chanL[sample] * foldbackAmtSmoothVal );
        float wetSampleR = juce::dsp::FastMathApproximations::sin( chanR[sample] * foldbackAmtSmoothVal );
        
        chanL[sample] = dryWet->dryWetMixEqualPowerBySample( chanL[sample], wetSampleL, dryWetSmoothVal );
        chanR[sample] = dryWet->dryWetMixEqualPowerBySample( chanR[sample], wetSampleR, dryWetSmoothVal );
    }
    
    return bufferIn;
}


void FoldbackDistortion::processFoldbackBuffer(juce::AudioBuffer<float> &bufferIn, float foldbackAmount, float dryWetVal)
{
    auto* chanL = bufferIn.getWritePointer ( 0 );
    auto* chanR = bufferIn.getWritePointer ( 1 );
    
    foldbackAmtSmooth.setTargetValue ( foldbackAmount );
    dryWetSmooth.setTargetValue      ( dryWetVal      );
    
    for (int sample = 0; sample < bufferIn.getNumSamples(); sample++)
    {
        float foldbackAmtSmoothVal = foldbackAmtSmooth.getNextValue();
        float dryWetSmoothVal      = dryWetSmooth.getNextValue();
        
        float wetSampleL = juce::dsp::FastMathApproximations::sin( chanL[sample] * foldbackAmtSmoothVal );
        float wetSampleR = juce::dsp::FastMathApproximations::sin( chanR[sample] * foldbackAmtSmoothVal );
        
        chanL[sample] = dryWet->dryWetMixEqualPowerBySample( chanL[sample], wetSampleL, dryWetSmoothVal );
        chanR[sample] = dryWet->dryWetMixEqualPowerBySample( chanR[sample], wetSampleR, dryWetSmoothVal );
    }
}
