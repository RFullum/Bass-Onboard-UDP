/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DryWet.h"
#include "WaveShaper.h"
#include "FoldbackDistortion.h"
#include "BitCrusher.h"
#include "FormantFilter.h"

//==============================================================================
/**
*/
class BassOnboardAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BassOnboardAudioProcessor();
    ~BassOnboardAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // APVTS Parameters instalnce
    juce::AudioProcessorValueTreeState parameters;

private:
    // Gain Params
    std::atomic<float>* inGainDBParam;
    std::atomic<float>* outGainDBParam;
    
    // Distortion Params
    std::atomic<float>* waveShapeAmountParam;
    std::atomic<float>* waveShapeDryWetParam;
    std::atomic<float>* foldbackAmountParam;
    std::atomic<float>* foldbackDryWetParam;
    std::atomic<float>* bitCrushAmountParam;
    std::atomic<float>* bitCrushDryWetParam;
    
    // Formant Params
    std::atomic<float>* formantMorphParam;
    std::atomic<float>* formantDryWetParam;
    
    // Delay Params
    std::atomic<float>* delayFXTimeParam;
    std::atomic<float>* delayFXFdbckParam;
    std::atomic<float>* delayFXDryWetParam;
    std::atomic<float>* haasWidthParam;
    
    juce::SmoothedValue<float> delayFXTimeSmooth;
    juce::SmoothedValue<float> delayFXFdbckSmooth;
    juce::SmoothedValue<float> haasSmooth;
    
    // Filter Params
    std::atomic<float>* svFilterCutoffParam;
    std::atomic<float>* svFilterResParam;
    std::atomic<float>* svFilterTypeParam;
    std::atomic<float>* svFilterPolesParam;
    
    juce::SmoothedValue<float> svFilterCutoffSmooth;
    juce::SmoothedValue<float> svFilterResSmooth;
    
    // Sensor On/Off
    std::atomic<float>* accelXOnOffParam;
    std::atomic<float>* accelYOnOffParam;
    std::atomic<float>* accelZOnOffParam;
    
    std::atomic<float>* gyroXOnOffParam;
    std::atomic<float>* gyroYOnOffParam;
    std::atomic<float>* gyroZOnOffParam;
    
    std::atomic<float>* touchXOnOffParam;
    std::atomic<float>* touchYOnOffParam;
    std::atomic<float>* touchZOnOffParam;
    
    std::atomic<float>* distanceOnOffParam;
    
    // Gain
    juce::dsp::Gain<float> inGain;
    juce::dsp::Gain<float> outGain;
    
    // FX
    std::unique_ptr<WaveShaper>         waveShaper;
    std::unique_ptr<FoldbackDistortion> foldback;
    std::unique_ptr<BitCrusher>         bitCrush;
    std::unique_ptr<FormantFilter>      formant;
    
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> delayFX   { 192000 };
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> haasDelay { 192000 };

    
    std::unique_ptr<DryWet> delayFXDryWet;
    
    // Filter
    juce::dsp::StateVariableTPTFilter<float> svFilter1;
    juce::dsp::StateVariableTPTFilter<float> svFilter2;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BassOnboardAudioProcessor)
};
