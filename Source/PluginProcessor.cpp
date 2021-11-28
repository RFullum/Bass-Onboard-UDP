/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BassOnboardAudioProcessor::BassOnboardAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif

//
// ParameterFloats:
// id, description, min, max, default
// ~OR~
// id, description, normalisableRange(min, max, increment, skew, symmetric),
//                 default, param label, param category, string from value, string to value
//
// ParameterChoices:
// id, description, StringArray( {"choice1", "choice2", ... } ), default index of StringArray
//
parameters(*this, nullptr, "ParameterTree",
{
    // Gain params
    std::make_unique<juce::AudioParameterFloat> ( "inGain", "Input Gain",
                                                  juce::NormalisableRange<float>( -100.0f, 12.0f, 0.01f, 2.0f, false),
                                                  0.0f, "dB" ),
    std::make_unique<juce::AudioParameterFloat> ( "outGain", "Output Gain",
                                                  juce::NormalisableRange<float>( -100.0f, 12.0f, 0.01f, 2.0f, false ),
                                                  0.0f, "dB" ),
    // Distortion Params
    std::make_unique<juce::AudioParameterFloat> ( "wsAmt", "Waveshape Dry/Wet",
                                                  juce::NormalisableRange<float>( 1.0f, 200.0f, 0.01f, 1.0f, false ),
                                                  0.0f, "" ),
    std::make_unique<juce::AudioParameterFloat> ( "wsDryWet", "Waveshape Dry/Wet",
                                                  juce::NormalisableRange<float>( 0.0f, 1.0f, 0.01f, 1.0f, false ),
                                                  0.0f, "" ),
    std::make_unique<juce::AudioParameterFloat> ( "foldbackAmt", "Foldback Amount",
                                                  juce::NormalisableRange<float>( 1.0f, 200.0f, 0.01f, 0.325f, false ),
                                                  1.0f, "" ),
    std::make_unique<juce::AudioParameterFloat> ( "foldbackDryWet", "Foldback Dry/Wet",
                                                  juce::NormalisableRange<float>( 0.0f, 1.0f, 0.01f, 1.0f, false ),
                                                  0.0f, "" ),
    std::make_unique<juce::AudioParameterFloat> ( "bitcrushAmt", "Bitcrush Amount",
                                                  juce::NormalisableRange<float>( 0.00001f, 0.0225f, 0.0001f, 0.375f, false ),
                                                  1.0f, "" ),
    std::make_unique<juce::AudioParameterFloat> ( "bitcrushDryWet", "Bitcrush Dry/Wet",
                                                  juce::NormalisableRange<float>( 0.0f, 1.0f, 0.01f, 1.0f, false ),
                                                  0.0f, "" ),
    // Formant Params
    std::make_unique<juce::AudioParameterFloat> ( "formantMorph", "Formant Morph",
                                                  juce::NormalisableRange<float>( 5.0f, 9.0f, 0.001f, 1.0f, false ),
                                                  0, "" ),
    std::make_unique<juce::AudioParameterFloat> ( "formantDryWet", "Formant Dry/Wet",
                                                  juce::NormalisableRange<float>( 0.0f, 1.0f, 0.01f, 1.0f, false ),
                                                  0.0f, "" ),
    // Delay FX
    std::make_unique<juce::AudioParameterFloat> ( "delayFXTime", "Delay FX Time",
                                                  juce::NormalisableRange<float>( 0.0f, 1.0f, 0.01f, 1.0f, false ),
                                                  0.0f, "" ),
    std::make_unique<juce::AudioParameterFloat> ( "delayFXFdbck", "Delay FX Feedback",
                                                  juce::NormalisableRange<float>( 0.0f, 1.0f, 0.01f, 1.0f, false ),
                                                  0.0f, "" ),
    std::make_unique<juce::AudioParameterFloat> ( "delayFXDryWet", "Delay FX Dry/Wet",
                                                  juce::NormalisableRange<float>( 0.0f, 1.0f, 0.01f, 1.0f, false ),
                                                  0.0f, "" ),
    std::make_unique<juce::AudioParameterFloat> ( "haasWidth", "Width Amount",
                                                  juce::NormalisableRange<float>( 0.0f, 1.0f, 0.01f, 1.0f, false ),
                                                  0.0f, "" ),
    // Filtering Params
    std::make_unique<juce::AudioParameterFloat> ( "svFiltCutoff", "Filter Cutoff",
                                                  juce::NormalisableRange<float>(20.0f, 18000.0f, 0.01f, 0.25f, false),
                                                  18000.0f, "" ),
    std::make_unique<juce::AudioParameterFloat> ( "svFiltRes", "Filter Resonance",
                                                  juce::NormalisableRange<float>(0.70f, 2.5f, 0.01, 0.75f, false),
                                                  0.70, "" ),
    std::make_unique<juce::AudioParameterChoice> ( "svFiltType", "Filter Type",
                                                   juce::StringArray( {"LPF", "BPF", "HPF"} ), 0 ),
    std::make_unique<juce::AudioParameterChoice> ( "svFiltPoles", "Filter Poles",
                                                   juce::StringArray( {"1 pole : -12dB", "2 pole : -24dB"} ), 0 ),
    // Sensor On/Off Params
    std::make_unique<juce::AudioParameterChoice> ( "accelXOnOff", "Accel X On/Off",        juce::StringArray ( {"Off", "On"} ), 0 ),
    std::make_unique<juce::AudioParameterChoice> ( "accelYOnOff", "Accel Y On/Off",        juce::StringArray ( {"Off", "On"} ), 0 ),
    std::make_unique<juce::AudioParameterChoice> ( "accelZOnOff", "Accel Z On/Off",        juce::StringArray ( {"Off", "On"} ), 0 ),
    std::make_unique<juce::AudioParameterChoice> ( "gyroXOnOff",  "Gyro X On/Off",         juce::StringArray ( {"Off", "On"} ), 0 ),
    std::make_unique<juce::AudioParameterChoice> ( "gyroYOnOff",  "Gyro Y On/Off",         juce::StringArray ( {"Off", "On"} ), 0 ),
    std::make_unique<juce::AudioParameterChoice> ( "gyroZOnOff",  "Gyro Z On/Off",         juce::StringArray ( {"Off", "On"} ), 0 ),
    std::make_unique<juce::AudioParameterChoice> ( "touchXOnOff", "Touch Screen X On/Off", juce::StringArray ( {"Off", "On"} ), 0 ),
    std::make_unique<juce::AudioParameterChoice> ( "touchYOnOff", "Touch Screen Y On/Off", juce::StringArray ( {"Off", "On"} ), 0 ),
    std::make_unique<juce::AudioParameterChoice> ( "touchZOnOff", "Touch Screen Z On/Off", juce::StringArray ( {"Off", "On"} ), 0 ),
    std::make_unique<juce::AudioParameterChoice> ( "distOnOff",   "TDistance On/Off",      juce::StringArray ( {"Off", "On"} ), 0 )
}),
    waveShaper    ( std::make_unique<WaveShaper>()         ),
    foldback      ( std::make_unique<FoldbackDistortion>() ),
    bitCrush      ( std::make_unique<BitCrusher>()         ),
    formant       ( std::make_unique<FormantFilter>()      ),
    delayFXDryWet ( std::make_unique<DryWet>()             )
// Constructor
{
    // Gain Params
    inGainDBParam  = parameters.getRawParameterValue ( "inGain"  );
    outGainDBParam = parameters.getRawParameterValue ( "outGain" );
    
    // Distortion Params
    waveShapeAmountParam = parameters.getRawParameterValue ( "wsAmt"          );
    waveShapeDryWetParam = parameters.getRawParameterValue ( "wsDryWet"       );
    foldbackAmountParam  = parameters.getRawParameterValue ( "foldbackAmt"    );
    foldbackDryWetParam  = parameters.getRawParameterValue ( "foldbackDryWet" );
    bitCrushAmountParam  = parameters.getRawParameterValue ( "bitcrushAmt"    );
    bitCrushDryWetParam  = parameters.getRawParameterValue ( "bitcrushDryWet" );
    
    // Formant Params
    formantMorphParam = parameters.getRawParameterValue  ( "formantMorph"  );
    formantDryWetParam = parameters.getRawParameterValue ( "formantDryWet" );
    
    // Delay Params
    delayFXTimeParam   = parameters.getRawParameterValue ( "delayFXTime"   );
    delayFXFdbckParam  = parameters.getRawParameterValue ( "delayFXFdbck"  );
    delayFXDryWetParam = parameters.getRawParameterValue ( "delayFXDryWet" );
    
    haasWidthParam = parameters.getRawParameterValue ( "haasWidth" );
    
    // Filter Params
    svFilterCutoffParam = parameters.getRawParameterValue ( "svFiltCutoff" );
    svFilterResParam    = parameters.getRawParameterValue ( "svFiltRes"    );
    svFilterTypeParam   = parameters.getRawParameterValue ( "svFiltType"   );
    svFilterPolesParam  = parameters.getRawParameterValue ( "svFiltPoles"  );
    
    // Sensor On/Off Params
    accelXOnOffParam = parameters.getRawParameterValue ( "accelXOnOff" );
    accelYOnOffParam = parameters.getRawParameterValue ( "accelYOnOff" );
    accelZOnOffParam = parameters.getRawParameterValue ( "accelZOnOff" );
    
    gyroXOnOffParam = parameters.getRawParameterValue ( "gyroXOnOff" );
    gyroYOnOffParam = parameters.getRawParameterValue ( "gyroYOnOff" );
    gyroZOnOffParam = parameters.getRawParameterValue ( "gyroZOnOff" );
    
    touchXOnOffParam = parameters.getRawParameterValue ( "touchXOnOff" );
    touchYOnOffParam = parameters.getRawParameterValue ( "touchYOnOff" );
    touchZOnOffParam = parameters.getRawParameterValue ( "touchZOnOff" );
    
    distanceOnOffParam = parameters.getRawParameterValue ( "distOnOff" );
}

BassOnboardAudioProcessor::~BassOnboardAudioProcessor()
{
}

//==============================================================================
const juce::String BassOnboardAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BassOnboardAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BassOnboardAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BassOnboardAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BassOnboardAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BassOnboardAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BassOnboardAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BassOnboardAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BassOnboardAudioProcessor::getProgramName (int index)
{
    return {};
}

void BassOnboardAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BassOnboardAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // DSP
    juce::dsp::ProcessSpec spec;
    
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate       = sampleRate;
    
    // Gains
    inGain.prepare                ( spec );
    inGain.reset                  ( );
    inGain.setRampDurationSeconds ( 0.01f );
    
    outGain.prepare                ( spec );
    outGain.reset                  ( );
    outGain.setRampDurationSeconds ( 0.01f );
    
    // Distortion
    waveShaper->setSampleRate ( sampleRate );
    foldback->setSampleRate   ( sampleRate );
    bitCrush->prepare         ( sampleRate );
    
    // Formant
    formant->prepare ( spec );
    
    // Delays
    delayFX.prepare   ( spec );
    haasDelay.prepare ( spec );
    delayFX.reset   ();
    haasDelay.reset ();
    
    delayFXTimeSmooth.reset  ( sampleRate, 0.1f  );
    delayFXFdbckSmooth.reset ( sampleRate, 0.01f );
    haasSmooth.reset         ( sampleRate, 0.01f );
    
    delayFXTimeSmooth.setTargetValue  ( 0.0f );
    delayFXFdbckSmooth.setTargetValue ( 0.0f );
    haasSmooth.setTargetValue         ( 0.0f );
    
    // Filter
    svFilter1.prepare ( spec );
    svFilter2.prepare ( spec );
    svFilter1.reset   ();
    svFilter2.reset   ();
    
    svFilterCutoffSmooth.reset ( sampleRate, 0.01f );
    svFilterResSmooth.reset    ( sampleRate, 0.01f );
    
    svFilterCutoffSmooth.setTargetValue ( 18000.0f );
    svFilterResSmooth.setTargetValue    ( 0.70f    );
}

void BassOnboardAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BassOnboardAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return true;
}
#endif

void BassOnboardAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Audio input to buffer
//    auto* leftChannel  = buffer.getWritePointer ( 0 );
//    auto* rightChannel = buffer.getWritePointer ( 1 );
    
    // Create the AudioBlock for DSP widgets
    juce::dsp::AudioBlock<float> sampleBlock ( buffer );
    
    // Apply Input Gain
    inGain.setGainDecibels ( *inGainDBParam );
    inGain.process         ( juce::dsp::ProcessContextReplacing<float>( sampleBlock ) );
    
    // Create Effects buffer
    juce::AudioBuffer<float> effectsBuffer;
    effectsBuffer.clear();
    effectsBuffer.makeCopyOf ( buffer );
    
//    int effectsSize = effectsBuffer.getNumSamples ();
    
    //auto* writeL = effectsBuffer.getWritePointer ( 0 );
    //auto* writeR = effectsBuffer.getWritePointer ( 1 );
    
    // Distortions
    waveShaper->processWaveshapeBuffer ( effectsBuffer, *waveShapeAmountParam, *waveShapeDryWetParam );
    foldback->processFoldbackBuffer    ( effectsBuffer, *foldbackAmountParam,  *foldbackDryWetParam  );
    bitCrush->processBuffer            ( effectsBuffer, *bitCrushAmountParam,  *bitCrushDryWetParam  );
    
    // Formant
    formant->processBuffer ( effectsBuffer, *formantMorphParam, *formantDryWetParam );
    
    // Delay
    delayFXTimeSmooth.setTargetValue  ( *delayFXTimeParam  );
    delayFXFdbckSmooth.setTargetValue ( *delayFXFdbckParam );

    for (int sample = 0; sample < effectsBuffer.getNumSamples(); sample++)
    {
        auto* leftChan  = effectsBuffer.getWritePointer ( 0 );
        auto* rightChan = effectsBuffer.getWritePointer ( 1 );

        float delayTimeSamples = delayFXTimeSmooth.getNextValue() * getSampleRate();   // 1 sec max delay (((Increase?)))
        float feedbackAmt      = delayFXFdbckSmooth.getNextValue();

        float delaySampleL = delayFX.popSample ( 0, delayTimeSamples, true );
        float delaySampleR = delayFX.popSample ( 1, delayTimeSamples, true );

        delayFX.pushSample ( 0, leftChan[sample]  + (delaySampleL * feedbackAmt) );
        delayFX.pushSample ( 1, rightChan[sample] + (delaySampleR * feedbackAmt) );

        leftChan[sample]  = delayFXDryWet->dryWetMixEqualPowerBySample ( leftChan[sample],
                                                                         delaySampleL,
                                                                        *delayFXDryWetParam );
        rightChan[sample] = delayFXDryWet->dryWetMixEqualPowerBySample ( rightChan[sample],
                                                                         delaySampleR,
                                                                        *delayFXDryWetParam );
    }
    
    
    // Filtering
    
    switch ( static_cast<int>(*svFilterTypeParam) )
    {
        case 0 :
            svFilter1.setType ( juce::dsp::StateVariableTPTFilterType::lowpass );
            svFilter2.setType ( juce::dsp::StateVariableTPTFilterType::lowpass );
            break;
        case 1 :
            svFilter1.setType ( juce::dsp::StateVariableTPTFilterType::bandpass );
            svFilter2.setType ( juce::dsp::StateVariableTPTFilterType::bandpass );
            break;
        case 2 :
            svFilter1.setType ( juce::dsp::StateVariableTPTFilterType::highpass );
            svFilter2.setType ( juce::dsp::StateVariableTPTFilterType::highpass );
            break;
        default:
            svFilter1.setType ( juce::dsp::StateVariableTPTFilterType::lowpass );
            svFilter2.setType ( juce::dsp::StateVariableTPTFilterType::lowpass );
            break;
    }

    svFilterCutoffSmooth.setTargetValue ( *svFilterCutoffParam );
    svFilterResSmooth.setTargetValue    ( *svFilterResParam    );
    
    
    for (int sample = 0; sample < effectsBuffer.getNumSamples(); sample++)
    {
        auto* leftChan  = effectsBuffer.getWritePointer ( 0 );
        auto* rightChan = effectsBuffer.getWritePointer ( 1 );

        float cutoffFreq = svFilterCutoffSmooth.getNextValue();
        float cutoffRes  = svFilterResSmooth.getNextValue();

        svFilter1.setCutoffFrequency ( cutoffFreq );
        svFilter2.setCutoffFrequency ( cutoffFreq );
        svFilter1.setResonance       ( cutoffRes  );
        svFilter2.setResonance       ( cutoffRes  );

        switch ( static_cast<int>(*svFilterPolesParam) )
        {
            case 0 :
                leftChan[sample]  = svFilter1.processSample ( 0, leftChan[sample]  );
                rightChan[sample] = svFilter1.processSample ( 1, rightChan[sample] );
                break;
            case 1 :
                leftChan[sample]  = svFilter2.processSample ( 0, svFilter1.processSample ( 0, leftChan[sample]  ) );
                rightChan[sample] = svFilter2.processSample ( 1, svFilter1.processSample ( 1, rightChan[sample] ) );
                break;
            default:
                leftChan[sample]  = svFilter1.processSample ( 0, leftChan[sample]  );
                rightChan[sample] = svFilter1.processSample ( 1, rightChan[sample] );
                break;
        }
    }
    
    svFilter1.snapToZero();
    svFilter2.snapToZero();
    
    // Haas Widener
    haasSmooth.setTargetValue ( *haasWidthParam );
    
    for (int i = 0; i < effectsBuffer.getNumSamples(); i++)
    {
        float delayInSamples = haasSmooth.getNextValue() * ( 0.03f * getSampleRate() );
        auto* leftChan       = effectsBuffer.getWritePointer ( 0 );

        haasDelay.pushSample ( 0, leftChan[i] );

        leftChan[i] = haasDelay.popSample ( 0, delayInSamples, true );
    }
    
    // Copy FX buffer onto buffer
    buffer.makeCopyOf ( effectsBuffer );
    
    // Apply Output Gain
    outGain.setGainDecibels ( *outGainDBParam );
    outGain.process         ( juce::dsp::ProcessContextReplacing<float>( sampleBlock ) );
}

//==============================================================================
bool BassOnboardAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BassOnboardAudioProcessor::createEditor()
{
    return new BassOnboardAudioProcessorEditor ( *this );
    //return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void BassOnboardAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // getStateInformation
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void BassOnboardAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // setStateInformation
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (parameters.state.getType()))
        {
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BassOnboardAudioProcessor();
}
