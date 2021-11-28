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
class BassOnboardAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                         public juce::Timer
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
    void sliderSetup(juce::Slider& sliderInstance, juce::Slider::SliderStyle style, bool showTextBox);
    
    /// Sets up Label for the Slider instances. Takes the labelInstance and the text for setText
    void sliderLabelSetup(juce::Label& labelInstance, juce::String labelText, float fontSize);
    
    /// Sets up an On/Off combo box
    void onOffBoxSetup(juce::ComboBox& boxInstance);
    
    /// Interface between Filter Parameters and physical devices
    void filterController();
    
    /// Turns Sensor Parameters On & Off
    void sensorOnOffController();
    
    /**
    encoderButton1 Presses cycle through rotary ecoder mappings to parameters
    Encoder Maps compare the incoming encoder value to the current saved (previous) encoder value
    and update the parameter value by the difference up or down, only if that mapping is
    active. Some paramters are also controlled by sensor values. If a sensor is mapped to
    the same parameter as an active rotary encoder, the sensor takes priority and the rotary
    encoder is locked out via the corresponding Override bool.
    */
    void encoderMapping();
    
    /**
    Sets values for mapped encoders. encVal is enc1Val or enc2Val; paramVal is a reference to the parameter value you are controlling; encoder is a reference to the current stored encoder1Val or encoder2Val
    multFactor multiplies the value for each encoder value tick. limitMin is the parameter minimum. limitMax is the parameter maximum. sliderInstance is a reference to the Slider for the parameter.
    */
    void encoderMapValueSet(float encVal,
                            float& paramVal,
                            float& encoder,
                            float multFactor,
                            float limitMin,
                            float limitMax,
                            juce::Slider& sliderInstance);
    
    
    /// encoderButton2 Presse cycle through sensor mappings to parameters
    void sensorMapping();
    
    void sensorMap0();
    void sensorMap1();
    void sensorMap2();
    
    /**
    Sets values for mapped sensors. onOffBox is the ComboBox controlling the sensor on/off. paramOverride is the bool that controls the sensor overriding the rotary encoder.
    sliderInstance is the parameter slider. sensorVal is the current value of the sensor coming over OSC. sensorMin and sensorMax are the min and max values expected from
    the sensor. mapMin and mapMax are the min and max of the parameter that the sensor values will be mapped to.
    */
    void sensorMapValueSet(juce::ComboBox& onOffBox,
                           bool&           paramOverride,
                           juce::Slider&   sliderInstance,
                           float           sensorVal,
                           float           sensorMin,
                           float           sensorMax,
                           float           mapMin,
                           float           mapMax);
    
    BassOnboardAudioProcessor& audioProcessor;
    
    // Sliders
    juce::Slider inGainSlider;
    
    juce::Slider wsAmtSlider;
    juce::Slider wsDryWetSlider;
    
    juce::Slider fbAmtSlider;
    juce::Slider fbDryWetSlider;
    
    juce::Slider bcAmtSlider;
    juce::Slider bcDryWetSlider;
    
    juce::Slider formMorphSlider;
    juce::Slider formDryWetSlider;
    
    juce::Slider delayTimeSlider;
    juce::Slider delayFeedbackSlider;
    juce::Slider delayDryWetSlider;
    
    juce::Slider filtCutoffSlider;
    juce::Slider filtResSlider;
    
    juce::Slider haasWidthSlider;
    
    juce::Slider outGainSlider;
    
    // ComboBox
    juce::ComboBox accelXOnOffBox;
    juce::ComboBox accelYOnOffBox;
    juce::ComboBox accelZOnOffBox;
    
    juce::ComboBox gyroXOnOffBox;
    juce::ComboBox gyroYOnOffBox;
    juce::ComboBox gyroZOnOffBox;
    
    juce::ComboBox touchXOnOffBox;
    juce::ComboBox touchYOnOffBox;
    juce::ComboBox touchZOnOffBox;
    
    juce::ComboBox distanceOnOffBox;
    
    juce::ComboBox filtTypeBox;
    juce::ComboBox filtPolesBox;
    
    // Labels
    juce::Label inLabel;
    juce::Label inGainLabel;
    
    juce::Label wsLabel;
    juce::Label wsAmtLabel;
    juce::Label wsDryWetLabel;
    
    juce::Label fbLabel;
    juce::Label fbAmtLabel;
    juce::Label fbDryWetLabel;
    
    juce::Label bcLabel;
    juce::Label bcAmtLabel;
    juce::Label bcDryWetLabel;
    
    juce::Label formLabel;
    juce::Label formMorphLabel;
    juce::Label formDryWetLabel;
    
    juce::Label delayLabel;
    juce::Label delayTimeLabel;
    juce::Label delayFeedbackLabel;
    juce::Label delayDryWetLabel;
    
    juce::Label filtLabel;
    juce::Label filtCutoffLabel;
    juce::Label filtResLabel;
    
    juce::Label haasLabel;
    juce::Label haasWidthLabel;
    
    juce::Label outLabel;
    juce::Label outGainLabel;
    
    juce::Label accelXOnOffLabel;
    juce::Label accelYOnOffLabel;
    juce::Label accelZOnOffLabel;
    juce::Label gyroXOnOffLabel;
    juce::Label gyroYOnOffLabel;
    juce::Label gyroZOnOffLabel;
    juce::Label touchXOnOffLabel;
    juce::Label touchYOnOffLabel;
    juce::Label touchZOnOffLabel;
    juce::Label distanceOnOffLabel;
    
    
    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inGainAttachment; 
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wsAmtAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wsDryWetAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fbAmtAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fbDryWetAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bcAmtAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bcDryWetAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> formMorphAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> formDryWetAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayFeedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayDryWetAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filtCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filtResAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> haasWidthAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outGainAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> accelXOnOffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> accelYOnOffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> accelZOnOffAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> gyroXOnOffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> gyroYOnOffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> gyroZOnOffAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> touchXOnOffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> touchYOnOffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> touchZOnOffAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> distanceOnOffAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filtTypeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filtPolesAttachment;
    
    // Colors
    juce::Colour onyx;
    juce::Colour lightSlateGray;
    juce::Colour magicMint;
    juce::Colour fieryRose;
    juce::Colour orangePeel;
    juce::Colour transparent;
    
    // Rectangles
    juce::Rectangle<float> inAreaBG;
    juce::Rectangle<float> ngAreaBG;
    juce::Rectangle<float> compAreaBG;
    juce::Rectangle<float> wsAreaBG;
    juce::Rectangle<float> fbAreaBG;
    juce::Rectangle<float> bcAreaBG;
    juce::Rectangle<float> formAreaBG;
    juce::Rectangle<float> delayAreaBG;
    juce::Rectangle<float> filtAreaBG;
    juce::Rectangle<float> haasAreaBG;
    juce::Rectangle<float> outAreaBG;
    
    // Sensor Instances
    //Accelerometer accelerometer;
    //Gyroscope     gyroscope;
    //DistanceMeter distanceMeter;
    //TouchScreen   touchScreen;
    
    // OSC
    std::unique_ptr<OSCHandler> osc;
//    OSCHandler osc;
    
    // Header/Footer
    std::unique_ptr<TitleHeader> titleHeader;
    std::unique_ptr<TitleFooter> titleFooter;
    
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BassOnboardAudioProcessorEditor)
};
