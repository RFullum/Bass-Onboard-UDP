/*
  ==============================================================================

    WaveShaper.cpp
    Created: 8 Feb 2021 3:02:31pm
    Author:  Robert Fullum

  ==============================================================================
*/

#include "WaveShaper.h"


WaveShaper::WaveShaper() :
    dryWet ( std::make_unique<DryWet>() )
{}

WaveShaper::~WaveShaper() {}

void WaveShaper::setSampleRate(float SR)
{
    wsAmountSmooth.reset ( SR, 0.01f );
    dryWetSmooth.reset   ( SR, 0.01f );
    
    wsAmountSmooth.setTargetValue ( 0.0f );
    dryWetSmooth.setTargetValue   ( 0.0f );
}


juce::AudioBuffer<float> WaveShaper::processWaveshape(juce::AudioBuffer<float>& bufferIn, float wsAmount, float dryWetVal)
{
    int numSamples = bufferIn.getNumSamples();
    
    auto* leftChan = bufferIn.getWritePointer  ( 0 );
    auto* rightChan = bufferIn.getWritePointer ( 1 );
    
    wsAmountSmooth.setTargetValue ( wsAmount );
    dryWetSmooth.setTargetValue   ( dryWetVal );
    
    for (int sample = 0; sample < numSamples; sample++)
    {
        float wsAmtSmoothedVal  = wsAmountSmooth.getNextValue();
        float dryWetSmoothedVal = dryWetSmooth.getNextValue();
        
        float wetSampleL = juce::dsp::FastMathApproximations::tanh ( leftChan[sample]  * wsAmtSmoothedVal );
        float wetSampleR = juce::dsp::FastMathApproximations::tanh ( rightChan[sample] * wsAmtSmoothedVal );
        
        leftChan[sample]  = dryWet->dryWetMixEqualPowerBySample ( leftChan[sample],  wetSampleL, dryWetSmoothedVal );
        rightChan[sample] = dryWet->dryWetMixEqualPowerBySample ( rightChan[sample], wetSampleR, dryWetSmoothedVal );
    }
    
    return bufferIn;
}


void WaveShaper::processWaveshapeBuffer(juce::AudioBuffer<float> &bufferIn, float wsAmount, float dryWetVal)
{
    int numSamples = bufferIn.getNumSamples();
    
    auto* leftChan  = bufferIn.getWritePointer ( 0 );
    auto* rightChan = bufferIn.getWritePointer ( 1 );
    
    wsAmountSmooth.setTargetValue ( wsAmount  );
    dryWetSmooth.setTargetValue   ( dryWetVal );
    
    for (int sample = 0; sample < numSamples; sample++)
    {
        float wsAmtSmoothedVal  = wsAmountSmooth.getNextValue();
        float dryWetSmoothedVal = dryWetSmooth.getNextValue();
        
        float wetSampleL = juce::dsp::FastMathApproximations::tanh ( leftChan[sample]  * wsAmtSmoothedVal );
        float wetSampleR = juce::dsp::FastMathApproximations::tanh ( rightChan[sample] * wsAmtSmoothedVal );
        
        leftChan[sample]  = dryWet->dryWetMixEqualPowerBySample ( leftChan[sample],  wetSampleL, dryWetSmoothedVal );
        rightChan[sample] = dryWet->dryWetMixEqualPowerBySample ( rightChan[sample], wetSampleR, dryWetSmoothedVal );
    }
}
