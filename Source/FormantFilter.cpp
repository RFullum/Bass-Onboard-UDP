/*
  ==============================================================================

    FormantFilter.cpp
    Created: 17 Feb 2021 3:32:50pm
    Author:  Robert Fullum

  ==============================================================================
*/

#include "FormantFilter.h"


FormantFilter::FormantFilter() :
    dryWet ( std::make_unique<DryWet>() )
{
    setQVals();
}

FormantFilter::~FormantFilter() {}


/// Pass in a ProcessSpec to prepare the filters
void FormantFilter::prepare(juce::dsp::ProcessSpec &PS)
{
    // Filter ProcessSpec setup
    filt1.prepare ( PS );
    filt2.prepare ( PS );
    filt3.prepare ( PS );
    
    filt1.reset();
    filt2.reset();
    filt3.reset();
    
    filt1.setType ( juce::dsp::StateVariableTPTFilterType::bandpass );
    filt2.setType ( juce::dsp::StateVariableTPTFilterType::bandpass );
    filt3.setType ( juce::dsp::StateVariableTPTFilterType::bandpass );
    
    // Smoothed Value Setup
    morphSmooth1.reset          ( PS.sampleRate, 0.01f );
    morphSmooth2.reset          ( PS.sampleRate, 0.01f );
    morphSmooth3.reset          ( PS.sampleRate, 0.01f );
    morphSmooth1.setTargetValue ( ow[0] );
    morphSmooth2.setTargetValue ( ow[1] );
    morphSmooth3.setTargetValue ( ow[2] );
    
    qSmooth1.reset          ( PS.sampleRate, 0.01f );
    qSmooth2.reset          ( PS.sampleRate, 0.01f );
    qSmooth3.reset          ( PS.sampleRate, 0.01f );
    qSmooth1.setTargetValue ( owQ[0] );
    qSmooth2.setTargetValue ( owQ[1] );
    qSmooth3.setTargetValue ( owQ[2] );
    
    dryWetSmooth.reset          ( PS.sampleRate, 0.01f );
    dryWetSmooth.setTargetValue ( 0.0f );
}




/**
Takes the AudioBuffer, formant morph value (0.0f to 9.0f), and dry/wet (0.0 to 1.0f)
and returns the formant filtered AudioBuffer
*/
juce::AudioBuffer<float> FormantFilter::process(juce::AudioBuffer<float> &buf, float morph, float dW)
{
    freqMorph ( morph );
    
    return filterAudio ( buf, dW );
}


void FormantFilter::processBuffer(juce::AudioBuffer<float> &buf, float morph, float dW)
{
    freqMorph         ( morph   );
    filterAudioBuffer ( buf, dW );
}



/// Applies filters to audio, sums filters, and blends with dryWet
juce::AudioBuffer<float> FormantFilter::filterAudio(juce::AudioBuffer<float> &bufIn, float dryWet_)
{
    auto* leftChan  = bufIn.getWritePointer ( 0 );
    auto* rightChan = bufIn.getWritePointer ( 1 );
    
    juce::AudioBuffer<float> f1;
    juce::AudioBuffer<float> f2;
    juce::AudioBuffer<float> f3;
    
    f1.makeCopyOf ( bufIn );
    f2.makeCopyOf ( bufIn );
    f3.makeCopyOf ( bufIn );
    
    morphSmooth1.setTargetValue ( outFreqs[0] );
    morphSmooth2.setTargetValue ( outFreqs[1] );
    morphSmooth3.setTargetValue ( outFreqs[2] );
    qSmooth1.setTargetValue     ( outQVals[0] );
    qSmooth2.setTargetValue     ( outQVals[1] );
    qSmooth3.setTargetValue     ( outQVals[2] );
    
    dryWetSmooth.setTargetValue ( dryWet_ );
    
    for (int sample = 0; sample < bufIn.getNumSamples(); sample++)
    {
        filt1.setCutoffFrequency ( morphSmooth1.getNextValue() );
        filt2.setCutoffFrequency ( morphSmooth2.getNextValue() );
        filt3.setCutoffFrequency ( morphSmooth3.getNextValue() );
        filt1.setResonance       ( qSmooth1.getNextValue() );
        filt2.setResonance       ( qSmooth2.getNextValue() );
        filt3.setResonance       ( qSmooth3.getNextValue() );
        
        float smoothedDW = dryWetSmooth.getNextValue();
        
        leftChan[sample] = dryWet->dryWetMixEqualPowerBySample ( leftChan[sample],
                                                                 filt1.processSample ( 0, leftChan[sample] ),           //   0 dB
                                                                 smoothedDW )
                         + dryWet->dryWetMixEqualPowerBySample ( leftChan[sample],
                                                                 filt2.processSample ( 0, leftChan[sample] ) * 0.178f,  // -15 dB
                                                                 smoothedDW )
                         + dryWet->dryWetMixEqualPowerBySample ( leftChan[sample],
                                                                 filt3.processSample ( 0, leftChan[sample] ) * 0.355f,  //  -9 dB
                                                                 smoothedDW );
        
        rightChan[sample] = dryWet->dryWetMixEqualPowerBySample ( rightChan[sample],
                                                                  filt1.processSample ( 0, rightChan[sample] ),
                                                                  smoothedDW )
                          + dryWet->dryWetMixEqualPowerBySample ( rightChan[sample],
                                                                  filt2.processSample ( 0, rightChan[sample] ) * 0.178f,
                                                                  smoothedDW )
                          + dryWet->dryWetMixEqualPowerBySample ( rightChan[sample],
                                                                  filt2.processSample ( 0, rightChan[sample]) * 0.355f,
                                                                  smoothedDW );
    }
    
    filt1.snapToZero();
    filt2.snapToZero();
    filt3.snapToZero();
    
    return bufIn;
}


void FormantFilter::filterAudioBuffer(juce::AudioBuffer<float> &bufIn, float dryWet_)
{
    auto* leftChan  = bufIn.getWritePointer ( 0 );
    auto* rightChan = bufIn.getWritePointer ( 1 );
    
    juce::AudioBuffer<float> f1;
    juce::AudioBuffer<float> f2;
    juce::AudioBuffer<float> f3;
    
    f1.makeCopyOf ( bufIn );
    f2.makeCopyOf ( bufIn );
    f3.makeCopyOf ( bufIn );
    
    morphSmooth1.setTargetValue ( outFreqs[0] );
    morphSmooth2.setTargetValue ( outFreqs[1] );
    morphSmooth3.setTargetValue ( outFreqs[2] );
    qSmooth1.setTargetValue     ( outQVals[0] );
    qSmooth2.setTargetValue     ( outQVals[1] );
    qSmooth3.setTargetValue     ( outQVals[2] );
    
    dryWetSmooth.setTargetValue ( dryWet_ );
    
    for (int sample = 0; sample < bufIn.getNumSamples(); sample++)
    {
        filt1.setCutoffFrequency ( morphSmooth1.getNextValue() );
        filt2.setCutoffFrequency ( morphSmooth2.getNextValue() );
        filt3.setCutoffFrequency ( morphSmooth3.getNextValue() );
        filt1.setResonance       ( qSmooth1.getNextValue() );
        filt2.setResonance       ( qSmooth2.getNextValue() );
        filt3.setResonance       ( qSmooth3.getNextValue() );
        
        float smoothedDW = dryWetSmooth.getNextValue();
        
        leftChan[sample] = dryWet->dryWetMixEqualPowerBySample ( leftChan[sample],
                                                                 filt1.processSample ( 0, leftChan[sample] ),           //   0 dB
                                                                 smoothedDW )
                         + dryWet->dryWetMixEqualPowerBySample ( leftChan[sample],
                                                                 filt2.processSample ( 0, leftChan[sample] ) * 0.178f,  // -15 dB
                                                                 smoothedDW )
                         + dryWet->dryWetMixEqualPowerBySample ( leftChan[sample],
                                                                 filt3.processSample ( 0, leftChan[sample] ) * 0.355f,  //  -9 dB
                                                                 smoothedDW );
        
        rightChan[sample] = dryWet->dryWetMixEqualPowerBySample ( rightChan[sample],
                                                                  filt1.processSample ( 0, rightChan[sample] ),
                                                                  smoothedDW )
                          + dryWet->dryWetMixEqualPowerBySample ( rightChan[sample],
                                                                  filt2.processSample ( 0, rightChan[sample] ) * 0.178f,
                                                                  smoothedDW )
                          + dryWet->dryWetMixEqualPowerBySample ( rightChan[sample],
                                                                  filt2.processSample ( 0, rightChan[sample]) * 0.355f,
                                                                  smoothedDW );
    }
    
    filt1.snapToZero();
    filt2.snapToZero();
    filt3.snapToZero();
}


/// Morphs each formant frequency and Q to the next formant in the series.
void FormantFilter::freqMorph(float morph)
{
    // Morphs through the formant filter freqs
    for (int i = 0; i < 3; i++)
        freqMapping ( morph, i );
}



/// Morphs each formant frequency and Q to the next formant in the series.
void FormantFilter::freqMapping(float MV, int inc)
{
    switch (static_cast<int>(MV))
    {
        case 0 :
            outFreqs[inc] = juce::jmap( MV, 0.0f, 1.0f, ow[inc],  oo[inc]  );
            outQVals[inc] = juce::jmap( MV, 0.0f, 1.0f, owQ[inc], ooQ[inc] );
            break;
        case 1 :
            outFreqs[inc] = juce::jmap( MV, 1.0f, 2.0f, oo[inc],  uu[inc]  );
            outQVals[inc] = juce::jmap( MV, 1.0f, 2.0f, ooQ[inc], uuQ[inc] );
            break;
        case 2 :
            outFreqs[inc] = juce::jmap( MV, 2.0f, 3.0f, uu[inc],  ah[inc]  );
            outQVals[inc] = juce::jmap( MV, 2.0f, 3.0f, uuQ[inc], ahQ[inc] );
            break;
        case 3 :
            outFreqs[inc] = juce::jmap( MV, 3.0f, 4.0f, ah[inc],  uh[inc]  );
            outQVals[inc] = juce::jmap( MV, 3.0f, 4.0f, ahQ[inc], uhQ[inc] );
            break;
        case 4 :
            outFreqs[inc] = juce::jmap( MV, 4.0f, 5.0f, uh[inc],  er[inc]  );
            outQVals[inc] = juce::jmap( MV, 4.0f, 5.0f, uhQ[inc], erQ[inc] );
            break;
        case 5 :
            outFreqs[inc] = juce::jmap( MV, 5.0f, 6.0f, er[inc],  ae[inc]  );
            outQVals[inc] = juce::jmap( MV, 5.0f, 6.0f, erQ[inc], aeQ[inc] );
            break;
        case 6 :
            outFreqs[inc] = juce::jmap( MV, 6.0f, 7.0f, ae[inc],  eh[inc]  );
            outQVals[inc] = juce::jmap( MV, 6.0f, 7.0f, aeQ[inc], ehQ[inc] );
            break;
        case 7 :
            outFreqs[inc] = juce::jmap( MV, 7.0f, 8.0f, eh[inc],  ih[inc]  );
            outQVals[inc] = juce::jmap( MV, 7.0f, 8.0f, ehQ[inc], ihQ[inc] );
            break;
        case 8 :
        case 9 :
            outFreqs[inc] = juce::jmap( MV, 8.0f, 9.0f, ih[inc],  iy[inc]  );
            outQVals[inc] = juce::jmap( MV, 8.0f, 9.0f, ihQ[inc], iyQ[inc] );
            break;
        default:
            outFreqs[inc] = ow[inc];
            outQVals[inc] = owQ[inc];
            break;
    }
}


/// Initialize Q Values based on the resonant frequency
void FormantFilter::setQVals()
{
    for (int i = 0; i < 3; i++)
    {
        float denom = 50.0f;
        
        // Q = Frq / (0.5f * Bandwidth_in_Hz) : where Bandwidth is 100Hz
        owQ[i] = ow[i] / denom;
        ooQ[i] = oo[i] / denom;
        uuQ[i] = uu[i] / denom;
        ahQ[i] = ah[i] / denom;
        uhQ[i] = uh[i] / denom;
        erQ[i] = er[i] / denom;
        aeQ[i] = ae[i] / denom;
        ehQ[i] = eh[i] / denom;
        ihQ[i] = ih[i] / denom;
        iyQ[i] = iy[i] / denom;
    }
}
