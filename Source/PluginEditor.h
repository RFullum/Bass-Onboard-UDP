/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OSCHandling.h"
#include "TitleHeader.h"

//==============================================================================
/**
*/
class BassOnboardAudioProcessorEditor  : public juce::AudioProcessorEditor, public Timer
{
public:
    BassOnboardAudioProcessorEditor (BassOnboardAudioProcessor&);
    ~BassOnboardAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    /// Sets up Slider object instances in constructor. sliderInstance is the slider to set up, suffix is textValueSuffix, sliderFillColor is the slider color below the thumb
    void sliderSetup(Slider& sliderInstance, Slider::SliderStyle style, bool showTextBox);
    
    /// Sets up Label for the Slider instances. Takes the labelInstance and the text for setText
    void sliderLabelSetup(Label& labelInstance, String labelText, float fontSize);
    
    /// Sets up an On/Off combo box
    void onOffBoxSetup(ComboBox& boxInstance);
    
    /// Interface between Filter Parameters and physical devices
    void filterController();
    
    /// Turns Sensor Parameters On & Off
    void sensorOnOffController();
    
    /// encoderButton1 Presses cycle through rotary ecoder mappings to parameters
    void encoderMapping();
    
    void encoderMap0(float enc1Val, float enc2Val);
    void encoderMap1(float enc1Val, float enc2Val);
    void encoderMap2(float enc1Val, float enc2Val);
    void encoderMap3(float enc1Val, float enc2Val);
    void encoderMap4(float enc1Val, float enc2Val);
    void encoderMap5(float enc1Val, float enc2Val);
    void encoderMap6(float enc1Val, float enc2Val);
    
    /// encoderButton2 Presse cycle through sensor mappings to parameters
    void sensorMapping();
    
    void sensorMap0();
    void sensorMap1();
    void sensorMap2();
    
    // Sliders
    Slider inGainSlider;
    
    /*
     
     Might want to re-introduce the compressor so I'm just commenting it out.
     
    Slider compThreshSlider;
    Slider compRatioSlider;
    Slider compAttackSlider;
    Slider compReleaseSlider;
    */
    
    
    Slider wsAmtSlider;
    Slider wsDryWetSlider;
    
    Slider fbAmtSlider;
    Slider fbDryWetSlider;
    
    Slider bcAmtSlider;
    Slider bcDryWetSlider;
    
    Slider formMorphSlider;
    Slider formDryWetSlider;
    
    Slider delayTimeSlider;
    Slider delayFeedbackSlider;
    Slider delayDryWetSlider;
    
    Slider filtCutoffSlider;
    Slider filtResSlider;
    
    Slider haasWidthSlider;
    
    Slider outGainSlider;
    
    // ComboBox
    ComboBox accelXOnOffBox;
    ComboBox accelYOnOffBox;
    ComboBox accelZOnOffBox;
    
    ComboBox gyroXOnOffBox;
    ComboBox gyroYOnOffBox;
    ComboBox gyroZOnOffBox;
    
    ComboBox touchXOnOffBox;
    ComboBox touchYOnOffBox;
    ComboBox touchZOnOffBox;
    
    ComboBox distanceOnOffBox;
    
    // ComboBox compOnOffBox;   // Might want to re-introduce the compressor so I'm just commenting it out.
    // ComboBox wsOnOffBox;
    // ComboBox fbOnOffBox;
    // ComboBox bcOnOffBox;
    // ComboBox formOnOffBox;
    // ComboBox delayOnOffBox;
    ComboBox filtTypeBox;
    ComboBox filtPolesBox;
    // ComboBox filtOnOffBox;
    // ComboBox haasOnOffBox;
    
    // Labels
    Label inLabel;
    Label inGainLabel;
    
    /*
     
     Might want to re-introduce the compressor so I'm just commenting it out.
     
    Label compLabel;
    Label compThreshLabel;
    Label compRatioLabel;
    Label compAttackLabel;
    Label compReleaseLabel;
    */
    
    Label wsLabel;
    Label wsAmtLabel;
    Label wsDryWetLabel;
    
    Label fbLabel;
    Label fbAmtLabel;
    Label fbDryWetLabel;
    
    Label bcLabel;
    Label bcAmtLabel;
    Label bcDryWetLabel;
    
    Label formLabel;
    Label formMorphLabel;
    Label formDryWetLabel;
    
    Label delayLabel;
    Label delayTimeLabel;
    Label delayFeedbackLabel;
    Label delayDryWetLabel;
    
    Label filtLabel;
    Label filtCutoffLabel;
    Label filtResLabel;
    
    Label haasLabel;
    Label haasWidthLabel;
    
    Label outLabel;
    Label outGainLabel;
    
    Label accelXOnOffLabel;
    Label accelYOnOffLabel;
    Label accelZOnOffLabel;
    Label gyroXOnOffLabel;
    Label gyroYOnOffLabel;
    Label gyroZOnOffLabel;
    Label touchXOnOffLabel;
    Label touchYOnOffLabel;
    Label touchZOnOffLabel;
    Label distanceOnOffLabel;
    
    
    // Attachments
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> inGainAttachment;
    
    /*
     
     Might want to re-introduce the compressor so I'm just commenting it out.
     
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> compThreshAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> compRatioAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> compAttackAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> compReleaseAttachment;
    */
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> wsAmtAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> wsDryWetAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> fbAmtAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> fbDryWetAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> bcAmtAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> bcDryWetAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> formMorphAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> formDryWetAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> delayFeedbackAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> delayDryWetAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> filtCutoffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> filtResAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> haasWidthAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> outGainAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> accelXOnOffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> accelYOnOffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> accelZOnOffAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> gyroXOnOffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> gyroYOnOffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> gyroZOnOffAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> touchXOnOffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> touchYOnOffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> touchZOnOffAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> distanceOnOffAttachment;
    
    // std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> compOnOffAttachment;    // Might want to re-introduce the compressor so I'm just commenting it out.
    
    /*
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> wsOnOffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> fbOnOffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> bcOnOffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> formOnOffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> delayOnOffAttachment;
     */
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> filtTypeAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> filtPolesAttachment;
    /*
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> filtOnOffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> haasOnOffAttachment;
    */
    
    // Colors
    Colour onyx;
    Colour lightSlateGray;
    Colour magicMint;
    Colour fieryRose;
    Colour orangePeel;
    
    // Rectangles
    Rectangle<float> inAreaBG;
    Rectangle<float> ngAreaBG;
    Rectangle<float> compAreaBG;
    Rectangle<float> wsAreaBG;
    Rectangle<float> fbAreaBG;
    Rectangle<float> bcAreaBG;
    Rectangle<float> formAreaBG;
    Rectangle<float> delayAreaBG;
    Rectangle<float> filtAreaBG;
    Rectangle<float> haasAreaBG;
    Rectangle<float> outAreaBG;
    
    // Sensor Instances
    //Accelerometer accelerometer;
    //Gyroscope     gyroscope;
    //DistanceMeter distanceMeter;
    //TouchScreen   touchScreen;
    
    // OSC
    OSCHandler osc;
    
    // Header/Footer
    TitleHeader titleHeader;
    TitleFooter titleFooter;
    
    int currentEncoderMapping;
    
    float encoder1Val;
    float encoder2Val;
    float outGainVal;
    float haasVal;
    float wsDWVal;
    float wsAmtVal;
    float fbDWVal;
    float fbAmtVal;
    float bcDWVal;
    float bcAmtVal;
    float formDWVal;
    float formAmtVal;
    float dlyDWVal;
    float dlyTVal;
    float dlyFBVal;
    
    bool wsAmtOverride;
    bool fbAmtOverride;
    bool bcAmtOverride;
    bool formMorphOverride;
    bool delayTimeOverride;
    
    
    BassOnboardAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BassOnboardAudioProcessorEditor)
};
