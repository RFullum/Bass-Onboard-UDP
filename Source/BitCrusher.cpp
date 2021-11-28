/*
  ==============================================================================

    BitCrusher.cpp
    Created: 9 Feb 2021 10:20:32am
    Author:  Robert Fullum

  ==============================================================================
*/

#include "BitCrusher.h"

//=============== Public ======================================

BitCrusher::BitCrusher() :
    dryWet   ( std::make_unique<DryWet>()          ),
    squareWT ( std::make_unique<SquareWavetable>() ),
    sampleRate ( 44100.0f )
{}

BitCrusher::~BitCrusher() {}

/// Sets up BitCrusher class
void BitCrusher::prepare(float SR)
{
    sampleRate = SR;
    
    dryWetSmooth.reset          ( sampleRate, 0.01f );
    dryWetSmooth.setTargetValue ( 0.0f );
    
    setWavetables();
}

/// Takes AudioBuffer and crush amount (0 to 1). Returns bitcrushed AudioBuffer. onOff is 0 or 1 from ParameterChoice
juce::AudioBuffer<float> BitCrusher::process(juce::AudioBuffer<float> &bufferIn, float crushAmt, float dryWetVal)
{
    float crushMapped = juce::jmap ( crushAmt, 1.0f, sampleRate );
    float crushFreq   = juce::jmap ( sampleRate / crushMapped, 1.0f, sampleRate, 20.0f, sampleRate );    // in Hz
    
    squareWT->setIncrement ( crushFreq );
    
    return crush ( bufferIn, dryWetVal );
}


void BitCrusher::processBuffer(juce::AudioBuffer<float> &bufferIn, float crushAmt, float dryWetVal)
{
    float crushMapped = juce::jmap ( crushAmt, 1.0f, sampleRate );
    float crushFreq   = juce::jmap ( sampleRate / crushMapped, 1.0f, sampleRate, 20.0f, sampleRate );    // in Hz
    
    squareWT->setIncrement ( crushFreq );
    
    crushBuffer ( bufferIn, dryWetVal);
}

//================= Private ===================================

/// Populates Wavetable
void BitCrusher::setWavetables()
{
    squareWT->setSampleRate     ( sampleRate );
    squareWT->populateWavetable ();
}



juce::AudioBuffer<float> BitCrusher::crush(juce::AudioBuffer<float> &bufferIn, float dryWetAmt)
{
    auto* leftChan  = bufferIn.getWritePointer ( 0 );
    auto* rightChan = bufferIn.getWritePointer ( 1 );
    
    dryWetSmooth.setTargetValue ( dryWetAmt );
    
    for (int chan = 0; chan < bufferIn.getNumChannels(); chan++)
    {
        for (int sample = 0; sample < bufferIn.getNumSamples(); sample++)
        {
            float outVal;
            float crushVal  = squareWT->process();
            float sampleVal = bufferIn.getSample( chan, sample );
            float holdVal   = 0.0f;
            
            if (crushVal >= 0.0f)
            {
                outVal  = sampleVal;
                holdVal = outVal;
            }
            else
                outVal = holdVal;
            
            float mixedSample = dryWet->dryWetMixEqualPowerBySample( sampleVal, outVal, dryWetSmooth.getNextValue() );
            
            if (chan == 0)
                leftChan[sample]  = mixedSample;
            else if (chan == 1)
                rightChan[sample] = mixedSample;
        }
    }
    
    return  bufferIn;
}


void BitCrusher::crushBuffer(juce::AudioBuffer<float> &bufferIn, float dryWetAmt)
{
    auto* leftChan  = bufferIn.getWritePointer ( 0 );
    auto* rightChan = bufferIn.getWritePointer ( 1 );
    
    dryWetSmooth.setTargetValue ( dryWetAmt );
    
    for (int chan = 0; chan < bufferIn.getNumChannels(); chan++)
    {
        for (int sample = 0; sample < bufferIn.getNumSamples(); sample++)
        {
            float outVal    = 0.0f;
            float crushVal  = squareWT->process();
            float sampleVal = bufferIn.getSample( chan, sample );
            float holdVal   = 0.0f;
            
            if (crushVal >= 0.0f)
            {
                outVal  = sampleVal;
                holdVal = outVal;
            }
            else
                outVal = holdVal;
            
            float mixedSample = dryWet->dryWetMixEqualPowerBySample( sampleVal, outVal, dryWetSmooth.getNextValue() );
            
            if (chan == 0)
                leftChan[sample]  = mixedSample;
            else if (chan == 1)
                rightChan[sample] = mixedSample;
        }
    }
}

