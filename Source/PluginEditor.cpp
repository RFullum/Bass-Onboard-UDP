/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BassOnboardAudioProcessorEditor::BassOnboardAudioProcessorEditor (BassOnboardAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    inGainAttachment        ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "inGain",         inGainSlider        ) ),
    wsAmtAttachment         ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "wsAmt",          wsAmtSlider         ) ),
    wsDryWetAttachment      ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "wsDryWet",       wsDryWetSlider      ) ),
    fbAmtAttachment         ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "foldbackAmt",    fbAmtSlider         ) ),
    fbDryWetAttachment      ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "foldbackDryWet", fbDryWetSlider      ) ),
    bcAmtAttachment         ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "bitcrushAmt",    bcAmtSlider         ) ),
    bcDryWetAttachment      ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "bitcrushDryWet", bcDryWetSlider      ) ),
    formMorphAttachment     ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "formantMorph",   formMorphSlider     ) ),
    formDryWetAttachment    ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "formantDryWet",  formDryWetSlider    ) ),
    delayTimeAttachment     ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "delayFXTime",    delayTimeSlider     ) ),
    delayFeedbackAttachment ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "delayFXFdbck",   delayFeedbackSlider ) ),
    delayDryWetAttachment   ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "delayFXDryWet",  delayDryWetSlider   ) ),
    filtCutoffAttachment    ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "svFiltCutoff",   filtCutoffSlider    ) ),
    filtResAttachment       ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "svFiltRes",      filtResSlider       ) ),
    haasWidthAttachment     ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "haasWidth",      haasWidthSlider     ) ),
    outGainAttachment       ( std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "outGain",        outGainSlider       ) ),
    filtTypeAttachment      ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "svFiltType",  filtTypeBox      ) ),
    filtPolesAttachment     ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "svFiltPoles", filtPolesBox     ) ),
    accelXOnOffAttachment   ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "accelXOnOff", accelXOnOffBox   ) ),
    accelYOnOffAttachment   ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "accelYOnOff", accelYOnOffBox   ) ),
    accelZOnOffAttachment   ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "accelZOnOff", accelZOnOffBox   ) ),
    gyroXOnOffAttachment    ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "gyroXOnOff",  gyroXOnOffBox    ) ),
    gyroYOnOffAttachment    ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "gyroYOnOff",  gyroYOnOffBox    ) ),
    gyroZOnOffAttachment    ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "gyroZOnOff",  gyroZOnOffBox    ) ),
    touchXOnOffAttachment   ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "touchXOnOff", touchXOnOffBox   ) ),
    touchYOnOffAttachment   ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "touchYOnOff", touchYOnOffBox   ) ),
    touchZOnOffAttachment   ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "touchZOnOff", touchZOnOffBox   ) ),
    distanceOnOffAttachment ( std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "distOnOff",   distanceOnOffBox ) ),
    onyx           ( juce::Colour( 0xFF353B3C ) ),
    lightSlateGray ( juce::Colour( 0xFF829298 ) ),
    magicMint      ( juce::Colour( 0xFFAEFFD8 ) ),
    fieryRose      ( juce::Colour( 0xFFFF6872 ) ),
    orangePeel     ( juce::Colour( 0xFFFC9800 ) ),
    transparent    ( juce::Colour( 0x00000000 ) ),
    osc         ( std::make_unique<OSCHandler>()  ),
    titleHeader ( std::make_unique<TitleHeader>() ),
    titleFooter ( std::make_unique<TitleFooter>() ),
    currentEncoderMapping ( 0 ),
    encoder1Val ( 0.0f ),
    encoder2Val ( 0.0f ),
    outGainVal  ( 0.0f ),
    haasVal     ( 0.0f ),
    wsDWVal     ( 0.0f ),
    wsAmtVal    ( 1.0f ),
    fbDWVal     ( 0.0f ),
    fbAmtVal    ( 1.0f ),
    bcDWVal     ( 0.0f ),
    bcAmtVal    ( 0.0f ),
    formDWVal   ( 0.0f ),
    formAmtVal  ( 0.0f ),
    dlyDWVal    ( 0.0f ),
    dlyTVal     ( 0.0f ),
    dlyFBVal    ( 0.0f ),
    wsAmtOverride     ( false ),
    fbAmtOverride     ( false ),
    bcAmtOverride     ( false ),
    formMorphOverride ( false ),
    delayTimeOverride ( false )
{
    setSize( 1000, 600 );
     
    juce::Timer::startTimerHz(60);
    
    //
    // Sliders Setup
    //
    juce::Slider::SliderStyle vertSlider = juce::Slider::SliderStyle::LinearVertical;
    
    // In Gain
    sliderSetup ( inGainSlider, vertSlider, true );
    
    // Waveshaper
    sliderSetup ( wsAmtSlider,    vertSlider, true );
    sliderSetup ( wsDryWetSlider, vertSlider, true );
    
    // Foldback
    sliderSetup ( fbAmtSlider,    vertSlider, true );
    sliderSetup ( fbDryWetSlider, vertSlider, true );
    
    // Bitcrusher
    sliderSetup ( bcAmtSlider,    vertSlider, true );
    sliderSetup ( bcDryWetSlider, vertSlider, true );
    
    // Formant
    sliderSetup ( formMorphSlider,  vertSlider, true );
    sliderSetup ( formDryWetSlider, vertSlider, true );
    
    // Delay
    sliderSetup ( delayTimeSlider,     vertSlider, true );
    sliderSetup ( delayFeedbackSlider, vertSlider, true );
    sliderSetup ( delayDryWetSlider,   vertSlider, true );
    
    // Filter
    sliderSetup ( filtCutoffSlider, vertSlider, true );
    sliderSetup ( filtResSlider,    vertSlider, true );
    
    // Haas Width
    sliderSetup ( haasWidthSlider, vertSlider, true );
    
    // Out Gain
    sliderSetup ( outGainSlider, vertSlider, true );
    
    //
    // Combo Box Setup
    //
    
    // On/Off boxes
    onOffBoxSetup ( accelXOnOffBox );
    onOffBoxSetup ( accelYOnOffBox );
    onOffBoxSetup ( accelZOnOffBox );
    
    onOffBoxSetup ( gyroXOnOffBox );
    onOffBoxSetup ( gyroYOnOffBox );
    onOffBoxSetup ( gyroZOnOffBox );
    
    onOffBoxSetup ( touchXOnOffBox );
    onOffBoxSetup ( touchYOnOffBox );
    onOffBoxSetup ( touchZOnOffBox );
    
    onOffBoxSetup( distanceOnOffBox );
    
    // Filter Type
    filtTypeBox.addItem              ( "LPF", 1 );
    filtTypeBox.addItem              ( "BPF", 2 );
    filtTypeBox.addItem              ( "HPF", 3 );
    filtTypeBox.setJustificationType ( juce::Justification::centred );
    filtTypeBox.setSelectedItemIndex ( 0 );
    
    filtTypeBox.setColour ( juce::ComboBox::backgroundColourId, onyx           );
    filtTypeBox.setColour ( juce::ComboBox::outlineColourId,    lightSlateGray );
    filtTypeBox.setColour ( juce::ComboBox::textColourId,       orangePeel     );
    filtTypeBox.setColour ( juce::ComboBox::arrowColourId,      magicMint      );
    
    addAndMakeVisible ( filtTypeBox );
    
    // Filter Poles
    filtPolesBox.addItem              ( "-12dB", 1 );
    filtPolesBox.addItem              ( "-24dB", 2 );
    filtPolesBox.setJustificationType ( juce::Justification::centred );
    filtPolesBox.setSelectedItemIndex ( 0 );
    
    filtPolesBox.setColour ( juce::ComboBox::backgroundColourId, onyx           );
    filtPolesBox.setColour ( juce::ComboBox::outlineColourId,    lightSlateGray );
    filtPolesBox.setColour ( juce::ComboBox::textColourId,       orangePeel     );
    filtPolesBox.setColour ( juce::ComboBox::arrowColourId,      magicMint      );
    
    addAndMakeVisible ( filtPolesBox );
    
    //
    // Label Setup
    //
    
    float bigLabelSize   = 16.0f;
    float smallLabelSize = 12.0f;
    
    // In Gain
    sliderLabelSetup ( inLabel,     "In",   bigLabelSize   );
    sliderLabelSetup ( inGainLabel, "Gain", smallLabelSize );
    
    // Waveshaper
    sliderLabelSetup ( wsLabel,       "Waveshaper", bigLabelSize   );
    sliderLabelSetup ( wsAmtLabel,    "Amount",     smallLabelSize );
    sliderLabelSetup ( wsDryWetLabel, "Dry/Wet",    smallLabelSize );
    
    // Foldback
    sliderLabelSetup ( fbLabel,       "Foldback", bigLabelSize   );
    sliderLabelSetup ( fbAmtLabel,    "Amount",   smallLabelSize );
    sliderLabelSetup ( fbDryWetLabel, "Dry/Wet",  smallLabelSize );
    
    // Bitcursher
    sliderLabelSetup ( bcLabel,       "Bitcrusher", bigLabelSize   );
    sliderLabelSetup ( bcAmtLabel,    "Amount",     smallLabelSize );
    sliderLabelSetup ( bcDryWetLabel, "Dry/Wet",    smallLabelSize );
    
    // Formant
    sliderLabelSetup ( formLabel,       "Formant", bigLabelSize   );
    sliderLabelSetup ( formMorphLabel,  "Morph",   smallLabelSize );
    sliderLabelSetup ( formDryWetLabel, "Dry/Wet", smallLabelSize );
    
    // Delay
    sliderLabelSetup ( delayLabel,         "Delay",    bigLabelSize   );
    sliderLabelSetup ( delayTimeLabel,     "Time",     smallLabelSize );
    sliderLabelSetup ( delayFeedbackLabel, "Feedback", smallLabelSize );
    sliderLabelSetup ( delayDryWetLabel,   "Dry/Wet",  smallLabelSize );
    
    // Filter
    sliderLabelSetup ( filtLabel,       "Filter",    bigLabelSize   );
    sliderLabelSetup ( filtCutoffLabel, "Cutoff",    smallLabelSize );
    sliderLabelSetup ( filtResLabel,    "Resonance", smallLabelSize );
    
    // Haas Width
    sliderLabelSetup ( haasLabel,      "Width",  bigLabelSize   );
    sliderLabelSetup ( haasWidthLabel, "Amount", smallLabelSize );
    
    // Out Gain
    sliderLabelSetup ( outLabel,     "Out",  bigLabelSize   );
    sliderLabelSetup ( outGainLabel, "Gain", smallLabelSize );
    
    // On Off ComboBox Labels
    sliderLabelSetup ( accelXOnOffLabel,   "AX",   smallLabelSize );
    sliderLabelSetup ( accelYOnOffLabel,   "AY",   smallLabelSize );
    sliderLabelSetup ( accelZOnOffLabel,   "AZ",   smallLabelSize );
    sliderLabelSetup ( gyroXOnOffLabel,    "GX",   smallLabelSize );
    sliderLabelSetup ( gyroYOnOffLabel,    "GY",   smallLabelSize );
    sliderLabelSetup ( gyroZOnOffLabel,    "GZ",   smallLabelSize );
    sliderLabelSetup ( touchXOnOffLabel,   "TX",   smallLabelSize );
    sliderLabelSetup ( touchYOnOffLabel,   "TY",   smallLabelSize );
    sliderLabelSetup ( touchZOnOffLabel,   "TZ",   smallLabelSize );
    sliderLabelSetup ( distanceOnOffLabel, "Dist", smallLabelSize );
    
    // Header/Footer
    titleHeader->setTextColor ( orangePeel );
    titleFooter->setTextColor ( orangePeel );
    
    addAndMakeVisible ( titleHeader.get() );
    addAndMakeVisible ( titleFooter.get() );
}
// End Constructor

BassOnboardAudioProcessorEditor::~BassOnboardAudioProcessorEditor()
{
    juce::Timer::stopTimer();
}

//==============================================================================
void BassOnboardAudioProcessorEditor::paint (juce::Graphics& g)
{
    float rounding = 2.0f;
    
    g.fillAll( lightSlateGray );
    
    // Subsections
    g.setColour            ( onyx );
    g.fillRoundedRectangle ( inAreaBG,    rounding );
    g.fillRoundedRectangle ( wsAreaBG,    rounding );
    g.fillRoundedRectangle ( fbAreaBG,    rounding );
    g.fillRoundedRectangle ( bcAreaBG,    rounding );
    g.fillRoundedRectangle ( formAreaBG,  rounding );
    g.fillRoundedRectangle ( delayAreaBG, rounding );
    g.fillRoundedRectangle ( filtAreaBG,  rounding );
    g.fillRoundedRectangle ( haasAreaBG,  rounding );
    g.fillRoundedRectangle ( outAreaBG,   rounding );
}

void BassOnboardAudioProcessorEditor::resized()
{
    // Padding around each effect section to visually separate
    float areaPadding = 2.0f;
    
    // Total area split to top & bottom halves
    juce::Rectangle<int> totalArea  = getLocalBounds().reduced( areaPadding );
    
    juce::Rectangle<int> headerArea = totalArea.removeFromTop    ( 55.0f ).reduced ( areaPadding );
    juce::Rectangle<int> footerArea = totalArea.removeFromBottom ( 30.0f ).reduced ( areaPadding );
    
    titleHeader->setBounds ( headerArea );
    titleFooter->setBounds ( footerArea );
    
    juce::Rectangle<int> onOffStrip = totalArea.removeFromBottom( totalArea.getHeight() * 0.1f ); // Space to put the Sensor On/Off until redesign
    juce::Rectangle<int> topHalf    = totalArea.removeFromTop( totalArea.getHeight() * 0.5f );
    juce::Rectangle<int> bottomHalf = totalArea;
    
    // Sensor On/Off until the redesign
    float onOffButtonWidth = onOffStrip.getWidth() * 0.1f;
    float onOffLabelHeight = onOffStrip.getHeight() * 0.33f;
    
    juce::Rectangle<int> accelXOnOffArea   = onOffStrip.removeFromLeft ( onOffButtonWidth );
    juce::Rectangle<int> accelYOnOffArea   = onOffStrip.removeFromLeft ( onOffButtonWidth );
    juce::Rectangle<int> accelZOnOffArea   = onOffStrip.removeFromLeft ( onOffButtonWidth );
    juce::Rectangle<int> gyroXOnOffArea    = onOffStrip.removeFromLeft ( onOffButtonWidth );
    juce::Rectangle<int> gyroYOnOffArea    = onOffStrip.removeFromLeft ( onOffButtonWidth );
    juce::Rectangle<int> gyroZOnOffArea    = onOffStrip.removeFromLeft ( onOffButtonWidth );
    juce::Rectangle<int> touchXOnOffArea   = onOffStrip.removeFromLeft ( onOffButtonWidth );
    juce::Rectangle<int> touchYOnOffArea   = onOffStrip.removeFromLeft ( onOffButtonWidth );
    juce::Rectangle<int> touchZOnOffArea   = onOffStrip.removeFromLeft ( onOffButtonWidth );
    juce::Rectangle<int> distanceOnOffArea = onOffStrip;
    
    juce::Rectangle<int> accelXOnOffLabelArea   = accelXOnOffArea.removeFromTop   ( onOffLabelHeight );
    juce::Rectangle<int> accelYOnOffLabelArea   = accelYOnOffArea.removeFromTop   ( onOffLabelHeight );
    juce::Rectangle<int> accelZOnOffLabelArea   = accelZOnOffArea.removeFromTop   ( onOffLabelHeight );
    juce::Rectangle<int> gyroXOnOffLabelArea    = gyroXOnOffArea.removeFromTop    ( onOffLabelHeight );
    juce::Rectangle<int> gyroYOnOffLabelArea    = gyroYOnOffArea.removeFromTop    ( onOffLabelHeight );
    juce::Rectangle<int> gyroZOnOffLabelArea    = gyroZOnOffArea.removeFromTop    ( onOffLabelHeight );
    juce::Rectangle<int> touchXOnOffLabelArea   = touchXOnOffArea.removeFromTop   ( onOffLabelHeight );
    juce::Rectangle<int> touchYOnOffLabelArea   = touchYOnOffArea.removeFromTop   ( onOffLabelHeight );
    juce::Rectangle<int> touchZOnOffLabelArea   = touchZOnOffArea.removeFromTop   ( onOffLabelHeight );
    juce::Rectangle<int> distanceOnOffLabelArea = distanceOnOffArea.removeFromTop ( onOffLabelHeight );
    
    accelXOnOffLabel.setBounds   ( accelXOnOffLabelArea   );
    accelYOnOffLabel.setBounds   ( accelYOnOffLabelArea   );
    accelZOnOffLabel.setBounds   ( accelZOnOffLabelArea   );
    gyroXOnOffLabel.setBounds    ( gyroXOnOffLabelArea    );
    gyroYOnOffLabel.setBounds    ( gyroYOnOffLabelArea    );
    gyroZOnOffLabel.setBounds    ( gyroZOnOffLabelArea    );
    touchXOnOffLabel.setBounds   ( touchXOnOffLabelArea   );
    touchYOnOffLabel.setBounds   ( touchYOnOffLabelArea   );
    touchZOnOffLabel.setBounds   ( touchZOnOffLabelArea   );
    distanceOnOffLabel.setBounds ( distanceOnOffLabelArea );
    
    accelXOnOffBox.setBounds   ( accelXOnOffArea   );
    accelYOnOffBox.setBounds   ( accelYOnOffArea   );
    accelZOnOffBox.setBounds   ( accelZOnOffArea   );
    gyroXOnOffBox.setBounds    ( gyroXOnOffArea    );
    gyroYOnOffBox.setBounds    ( gyroYOnOffArea    );
    gyroZOnOffBox.setBounds    ( gyroZOnOffArea    );
    touchXOnOffBox.setBounds   ( touchXOnOffArea   );
    touchYOnOffBox.setBounds   ( touchYOnOffArea   );
    touchZOnOffBox.setBounds   ( touchZOnOffArea   );
    distanceOnOffBox.setBounds ( distanceOnOffArea );
    
    // Top & Bottom Halves split into effect sections, width based on number of sliders
    float topSpacing    = topHalf.getWidth() / 9.0f;
    float bottomSpacing = bottomHalf.getWidth() / 9.0f;
    
    juce::Rectangle<int> inArea   = topHalf.removeFromLeft ( topSpacing ).reduced        ( areaPadding );
    juce::Rectangle<int> wsArea   = topHalf.removeFromLeft ( topSpacing * 2.0f ).reduced ( areaPadding );
    juce::Rectangle<int> fbArea   = topHalf.removeFromLeft ( topSpacing * 2.0f ).reduced ( areaPadding );
    juce::Rectangle<int> bcArea   = topHalf.removeFromLeft ( topSpacing * 2.0f ).reduced ( areaPadding );
    juce::Rectangle<int> formArea = topHalf.reduced                                      ( areaPadding );
    
    juce::Rectangle<int> delayArea = bottomHalf.removeFromLeft ( bottomSpacing * 3.0f ).reduced ( areaPadding );
    juce::Rectangle<int> filtArea  = bottomHalf.removeFromLeft ( bottomSpacing * 4.0f ).reduced ( areaPadding );
    juce::Rectangle<int> haasArea  = bottomHalf.removeFromLeft ( bottomSpacing ).reduced        ( areaPadding );
    juce::Rectangle<int> outArea   = bottomHalf.reduced                                         ( areaPadding );
    
    inAreaBG.setBounds    ( inArea.getX(),    inArea.getY(),    inArea.getWidth(),    inArea.getHeight()    );
    wsAreaBG.setBounds    ( wsArea.getX(),    wsArea.getY(),    wsArea.getWidth(),    wsArea.getHeight()    );
    fbAreaBG.setBounds    ( fbArea.getX(),    fbArea.getY(),    fbArea.getWidth(),    fbArea.getHeight()    );
    bcAreaBG.setBounds    ( bcArea.getX(),    bcArea.getY(),    bcArea.getWidth(),    bcArea.getHeight()    );
    formAreaBG.setBounds  ( formArea.getX(),  formArea.getY(),  formArea.getWidth(),  formArea.getHeight()  );
    delayAreaBG.setBounds ( delayArea.getX(), delayArea.getY(), delayArea.getWidth(), delayArea.getHeight() );
    filtAreaBG.setBounds  ( filtArea.getX(),  filtArea.getY(),  filtArea.getWidth(),  filtArea.getHeight()  );
    haasAreaBG.setBounds  ( haasArea.getX(),  haasArea.getY(),  haasArea.getWidth(),  haasArea.getHeight()  );
    outAreaBG.setBounds   ( outArea.getX(),   outArea.getY(),   outArea.getWidth(),   outArea.getHeight()   );
    
    float labelHeight = 50.0f;
    float boxReduce   = 12.0f;
    
    
    // Top Half Areas
    
    // In Gain
    juce::Rectangle<int> inLabelArea     = inArea.removeFromTop    ( labelHeight );
    juce::Rectangle<int> inGainLabelArea = inArea.removeFromTop    ( labelHeight );
    
    inLabel.setBounds      ( inLabelArea );
    inGainLabel.setBounds  ( inGainLabelArea );
    inGainSlider.setBounds ( inArea );
    
    // Waveshaper
    juce::Rectangle<int> wsLabelArea      = wsArea.removeFromTop    ( labelHeight );
    juce::Rectangle<int> wsParamLabelArea = wsArea.removeFromTop    ( labelHeight );
    
    wsLabel.setBounds    ( wsLabelArea );
    
    float wsWidth = wsArea.getWidth() * 0.5f;
    
    juce::Rectangle<int> wsAmtLabelArea    = wsParamLabelArea.removeFromLeft ( wsWidth );
    juce::Rectangle<int> wsDryWetLabelArea = wsParamLabelArea;
    
    wsAmtLabel.setBounds    ( wsAmtLabelArea    );
    wsDryWetLabel.setBounds ( wsDryWetLabelArea );
    
    juce::Rectangle<int> wsAmtArea    = wsArea.removeFromLeft ( wsWidth );
    juce::Rectangle<int> wsDryWetArea = wsArea;
    
    wsAmtSlider.setBounds    ( wsAmtArea    );
    wsDryWetSlider.setBounds ( wsDryWetArea );
    
    
    // Foldback
    juce::Rectangle<int> fbLabelArea      = fbArea.removeFromTop    ( labelHeight );
    juce::Rectangle<int> fbParamLabelArea = fbArea.removeFromTop    ( labelHeight );
    
    fbLabel.setBounds    ( fbLabelArea );
    
    float fbWidth = fbArea.getWidth() * 0.5f;
    
    juce::Rectangle<int> fbAmtLabelArea    = fbParamLabelArea.removeFromLeft ( fbWidth );
    juce::Rectangle<int> fbDryWetLabelArea = fbParamLabelArea;
    
    fbAmtLabel.setBounds    ( fbAmtLabelArea    );
    fbDryWetLabel.setBounds ( fbDryWetLabelArea );
    
    juce::Rectangle<int> fbAmtArea    = fbArea.removeFromLeft ( fbWidth );
    juce::Rectangle<int> fbDryWetArea = fbArea;
    
    fbAmtSlider.setBounds    ( fbAmtArea    );
    fbDryWetSlider.setBounds ( fbDryWetArea );
    
    
    // Bitcrusher
    juce::Rectangle<int> bcLabelArea      = bcArea.removeFromTop    ( labelHeight );
    juce::Rectangle<int> bcParamLabelArea = bcArea.removeFromTop    ( labelHeight );
    
    bcLabel.setBounds    ( bcLabelArea );
    
    float bcWidth = bcArea.getWidth() * 0.5f;
    
    juce::Rectangle<int> bcAmtLabelArea    = bcParamLabelArea.removeFromLeft ( bcWidth );
    juce::Rectangle<int> bcDryWetLabelArea = bcParamLabelArea;
    
    bcAmtLabel.setBounds    ( bcAmtLabelArea    );
    bcDryWetLabel.setBounds ( bcDryWetLabelArea );
    
    juce::Rectangle<int> bcAmtArea    = bcArea.removeFromLeft ( bcWidth );
    juce::Rectangle<int> bcDryWetArea = bcArea;
    
    bcAmtSlider.setBounds    ( bcAmtArea    );
    bcDryWetSlider.setBounds ( bcDryWetArea );
    
    
    // Bottom Half Areas
    
    // Formant
    juce::Rectangle<int> formLabelArea      = formArea.removeFromTop    ( labelHeight );
    juce::Rectangle<int> formParamLabelArea = formArea.removeFromTop    ( labelHeight );
    
    formLabel.setBounds    ( formLabelArea );
    
    float formWidth = formArea.getWidth() * 0.5f;
    
    juce::Rectangle<int> formMorphLabelArea  = formParamLabelArea.removeFromLeft ( formWidth );
    juce::Rectangle<int> formDryWetLabelArea = formParamLabelArea;
    
    formMorphLabel.setBounds  ( formMorphLabelArea  );
    formDryWetLabel.setBounds ( formDryWetLabelArea );
    
    juce::Rectangle<int> formMorphArea  = formArea.removeFromLeft ( formWidth );
    juce::Rectangle<int> formDryWetArea = formArea;
    
    formMorphSlider.setBounds  ( formMorphArea  );
    formDryWetSlider.setBounds ( formDryWetArea );
    
    
    // Delay
    juce::Rectangle<int> delayLabelArea      = delayArea.removeFromTop    ( labelHeight );
    juce::Rectangle<int> delayParamLabelArea = delayArea.removeFromTop    ( labelHeight );
    
    delayLabel.setBounds    ( delayLabelArea );
    
    float delayWidth = delayArea.getWidth() * 0.33f;
    
    juce::Rectangle<int> delayTimeLabelArea   = delayParamLabelArea.removeFromLeft ( delayWidth );
    juce::Rectangle<int> delayFdbkLabelArea   = delayParamLabelArea.removeFromLeft ( delayWidth );
    juce::Rectangle<int> delayDryWetLabelArea = delayParamLabelArea;
    
    delayTimeLabel.setBounds     ( delayTimeLabelArea  );
    delayFeedbackLabel.setBounds ( delayFdbkLabelArea  );
    delayDryWetLabel.setBounds   (delayDryWetLabelArea );
    
    juce::Rectangle<int> delayTimeArea   = delayArea.removeFromLeft ( delayWidth );
    juce::Rectangle<int> delayFdbkArea   = delayArea.removeFromLeft ( delayWidth );
    juce::Rectangle<int> delayDryWetArea = delayArea;
    
    delayTimeSlider.setBounds     ( delayTimeArea   );
    delayFeedbackSlider.setBounds ( delayFdbkArea   );
    delayDryWetSlider.setBounds   ( delayDryWetArea );
    
    
    // Filter
    juce::Rectangle<int> filtLabelArea      = filtArea.removeFromTop    ( labelHeight );
    juce::Rectangle<int> filtParamLabelArea = filtArea.removeFromTop    ( labelHeight );
    
    filtLabel.setBounds    ( filtLabelArea );
    
    float filtWidth = filtArea.getWidth() * 0.25f;
    
    juce::Rectangle<int> filtCutoffLabelArea = filtParamLabelArea.removeFromLeft  ( filtWidth );
    juce::Rectangle<int> filtParamSpacerArea = filtParamLabelArea.removeFromRight ( filtWidth * 2.0f );
    juce::Rectangle<int> filtResLabelArea    = filtParamLabelArea;
    
    filtCutoffLabel.setBounds ( filtCutoffLabelArea );
    filtResLabel.setBounds    ( filtResLabelArea    );
    
    juce::Rectangle<int> filtCutoffArea = filtArea.removeFromLeft ( filtWidth );
    juce::Rectangle<int> filtResArea    = filtArea.removeFromLeft ( filtWidth );
    juce::Rectangle<int> filtTypeArea   = filtArea.removeFromTop  ( filtArea.getHeight() * 0.5f );
    juce::Rectangle<int> filtPoleArea   = filtArea;
    
    filtCutoffSlider.setBounds ( filtCutoffArea );
    filtResSlider.setBounds    ( filtResArea    );
    filtTypeBox.setBounds      ( filtTypeArea.reduced(boxReduce, boxReduce) );
    filtPolesBox.setBounds     ( filtPoleArea.reduced(boxReduce, boxReduce) );
    
    
    // Haas Width
    juce::Rectangle<int> haasLabelArea      = haasArea.removeFromTop    ( labelHeight );
    juce::Rectangle<int> haasParamLabelArea = haasArea.removeFromTop    ( labelHeight );
    
    haasLabel.setBounds       ( haasLabelArea );
    haasWidthLabel.setBounds  ( haasParamLabelArea );
    haasWidthSlider.setBounds ( haasArea );
    
    
    // Out Gain
    juce::Rectangle<int> outLabelArea      = outArea.removeFromTop    ( labelHeight );
    juce::Rectangle<int> outParamLabelArea = outArea.removeFromTop    ( labelHeight );
    
    outLabel.setBounds      ( outLabelArea      );
    outGainLabel.setBounds  ( outParamLabelArea );
    outGainSlider.setBounds ( outArea           );
    
    
    
}



void BassOnboardAudioProcessorEditor::timerCallback()
{
    sensorOnOffController();    // Update Sensor On/Off Parameters
    sensorMapping();            // Send sensor values to mapped parameters
    encoderMapping();           // Send encoder values to mapped parameters
    filterController();         // Update filter parameters
}


/// Sets up Slider object instances in constructor. sliderInstance is the slider to set up, suffix is textValueSuffix, sliderFillColor is the slider color below the thumb
void BassOnboardAudioProcessorEditor::sliderSetup(juce::Slider &sliderInstance, juce::Slider::SliderStyle style, bool showTextBox)
{
    sliderInstance.setSliderStyle(style);
        
    // If slider has a textbox, draw it, otherwise, don't
    if (showTextBox)
    {
        sliderInstance.setTextBoxStyle ( juce::Slider::TextBoxBelow, false, 50, 20 );
        sliderInstance.setColour       ( juce::Slider::textBoxOutlineColourId, juce::Colour( (juce::uint8)0, (juce::uint8)0, (juce::uint8)0, (juce::uint8)0 ) );
        sliderInstance.setColour       ( juce::Slider::textBoxTextColourId, magicMint );
    }
    else
    {
        sliderInstance.setTextBoxStyle( juce::Slider::NoTextBox, false, 0, 0 );
    }
    
    sliderInstance.setColour( juce::Slider::trackColourId, fieryRose );
    sliderInstance.setColour( juce::Slider::thumbColourId, magicMint );
    
    addAndMakeVisible ( sliderInstance );
}


/// Sets up Label for the Slider instances. Takes the labelInstance and the text for setText
void BassOnboardAudioProcessorEditor::sliderLabelSetup(juce::Label &labelInstance, juce::String labelText, float fontSize)
{
    labelInstance.setText              ( labelText, juce::dontSendNotification );
    labelInstance.setJustificationType ( juce::Justification::centred );
    labelInstance.setColour            ( juce::Label::textColourId, orangePeel );
    labelInstance.setFont              ( juce::Font("helvetica", fontSize, 1) );
    
    addAndMakeVisible ( labelInstance );
}


/// Sets up an On/Off combo box
void BassOnboardAudioProcessorEditor::onOffBoxSetup(juce::ComboBox &boxInstance)
{
    boxInstance.addItem              ( "Off", 1 );
    boxInstance.addItem              ( "On",  2 );
    boxInstance.setJustificationType ( juce::Justification::centred );
    boxInstance.setSelectedItemIndex ( 0 );
    
    boxInstance.setColour ( juce::ComboBox::backgroundColourId, onyx        );
    boxInstance.setColour ( juce::ComboBox::outlineColourId, lightSlateGray );
    boxInstance.setColour ( juce::ComboBox::textColourId, orangePeel        );
    boxInstance.setColour ( juce::ComboBox::arrowColourId, magicMint        );
    
    addAndMakeVisible ( boxInstance );
}


/// Interface between Filter Parameters and physical devices
void BassOnboardAudioProcessorEditor::filterController()
{
    // Change filter type LPF/HPF/BPF
    if (filtTypeBox.getSelectedId() != osc->getFiltType() + 1.0f)
        filtTypeBox.setSelectedId( osc->getFiltType() + 1.0f );
    
    // Change Filter Poles -12dB/-24dB
    float pole = osc->getFiltPole();
    
    if (pole == -1.0f)
        pole = 2.0f;
    
    if (filtPolesBox.getSelectedId() != pole)
        filtPolesBox.setSelectedId( pole );
    
    // Filter Cutoff and Resonance
    float cutoffOn = touchYOnOffBox.getSelectedId();
    float resOn    = touchXOnOffBox.getSelectedId();
    
    if (cutoffOn == 2.0f)
    {
        float cutoffMap = juce::jmap   ( osc->getTouchY(), 300.0f, 800.0f, 45.0f, 15000.0f );   // Map touchscreen values to parameter values
        
        cutoffMap = juce::jlimit( 20.0f, 18000.0f, cutoffMap ); // Limit values to parameter range
        
        // Set cutoff value
        if (filtCutoffSlider.getValue() != cutoffMap)
            filtCutoffSlider.setValue( cutoffMap );
    }
    
    if (resOn == 2.0f)
    {
        float resMap = juce::jmap ( osc->getTouchX(), 300.0f, 800.0f, 0.9f,  2.4f     );  // Map touchscreen values to parameter values
        
        resMap = juce::jlimit( 0.7f, 2.5f, resMap );  // Limit values to parameter range
        
        // Set resonance value
        if (filtResSlider.getValue() != resMap)
            filtResSlider.setValue( resMap );
    }
}

/// Turns Sensor Parameters On & Off
void BassOnboardAudioProcessorEditor::sensorOnOffController()
{
    // Converts -1 Off/1 On values from Arduino to match 1 Off & 2 On values of ComboBox
    float aX = ( osc->getAccelXOnOff() == 1.0f ) ? 2.0f : 1.0f;
    float aY = ( osc->getAccelYOnOff() == 1.0f ) ? 2.0f : 1.0f;
    float aZ = ( osc->getAccelZOnOff() == 1.0f ) ? 2.0f : 1.0f;
    
    float gX = ( osc->getGyroXOnOff() == 1.0f ) ? 2.0f : 1.0f;
    float gY = ( osc->getGyroYOnOff() == 1.0f ) ? 2.0f : 1.0f;
    float gZ = ( osc->getGyroZOnOff() == 1.0f ) ? 2.0f : 1.0f;
    
    float tX = ( osc->getTouchXOnOff() == 1.0f ) ? 2.0f : 1.0f;
    float tY = ( osc->getTouchYOnOff() == 1.0f ) ? 2.0f : 1.0f;
    float tZ = ( osc->getTouchZOnOff() == 1.0f ) ? 2.0f : 1.0f;
    
    float dis = (osc->getDistanceOnOff() == 1.0f ) ? 2.0f : 1.0f;
    
    // If the button flipped the value, change the ComboBox value
    if (accelXOnOffBox.getSelectedId() != aX)
        accelXOnOffBox.setSelectedId( aX );
    
    if (accelYOnOffBox.getSelectedId() != aY)
        accelYOnOffBox.setSelectedId( aY );
    
    if (accelZOnOffBox.getSelectedId() != aZ)
        accelZOnOffBox.setSelectedId( aZ );
    
    if (gyroXOnOffBox.getSelectedId() != gX)
        gyroXOnOffBox.setSelectedId( gX );
    
    if (gyroYOnOffBox.getSelectedId() != gY)
        gyroYOnOffBox.setSelectedId( gY );
    
    if (gyroZOnOffBox.getSelectedId() != gZ)
        gyroZOnOffBox.setSelectedId( gZ );
    
    if (touchXOnOffBox.getSelectedId() != tX)
        touchXOnOffBox.setSelectedId( tX );
    
    if (touchYOnOffBox.getSelectedId() != tY)
        touchYOnOffBox.setSelectedId( tY );
    
    if (touchZOnOffBox.getSelectedIdAsValue() != tZ)
        touchZOnOffBox.setSelectedId( tZ );
    
    if (distanceOnOffBox.getSelectedId() != dis)
        distanceOnOffBox.setSelectedId( dis );
}

/**
 encoderButton1 Presses cycle through rotary ecoder mappings to parameters
 Encoder Maps compare the incoming encoder value to the current saved (previous) encoder value
 and update the parameter value by the difference up or down, only if that mapping is
 active. Some paramters are also controlled by sensor values. If a sensor is mapped to
 the same parameter as an active rotary encoder, the sensor takes priority and the rotary
 encoder is locked out via the corresponding Override bool.
 */
void BassOnboardAudioProcessorEditor::encoderMapping()
{
    // Get value from Rotary Encoders
    float enc1Val = osc->getEncoder1();
    float enc2Val = osc->getEncoder2();
    
    // Encoder1 Button presses cycle through Rotary Encoder Mappings 0 - 6
    switch ((int)osc->getEncButton1())
    {
        // Enc1 = outGain; Enc2 = Haas;
        case 0:
            // outGain
            if (enc1Val != encoder1Val)
                encoderMapValueSet( enc1Val, outGainVal, encoder1Val, 1.0f, -100.0f, 12.0f, outGainSlider );
            
            // Haas
            if (enc2Val != encoder2Val)
                encoderMapValueSet( enc2Val, haasVal, encoder2Val, 0.1f, 0.0f, 1.0f, haasWidthSlider );
            break;
            
        // Enc1 = WS D/W; Enc2 = WS Amt
        case 1:
            // D/W
            if (enc1Val != encoder1Val)
                encoderMapValueSet( enc1Val, wsDWVal, encoder1Val, 0.1f, 0.0f, 1.0f, wsDryWetSlider );
            
            // Amt
            if (enc2Val != encoder2Val && !wsAmtOverride)
                encoderMapValueSet( enc2Val, wsAmtVal, encoder2Val, 1.0f, 1.0f, 200.0f, wsAmtSlider );
            break;
            
        // Enc1 = FB D/W; Enc2 = FB Amt
        case 2:
            // D/W
            if (enc1Val != encoder1Val)
                encoderMapValueSet( enc1Val, fbDWVal, encoder1Val, 0.1f, 0.0f, 1.0f, fbDryWetSlider );
            
            // Amt
            if (enc2Val != encoder2Val && !fbAmtOverride)
                encoderMapValueSet( enc2Val, fbAmtVal, encoder2Val, 1.0f, 1.0f, 200.0f, fbAmtSlider );
            break;
            
        // Enc1 = BC D/W; Enc2 = BC Amt
        case 3:
            // D/W
            if (enc1Val != encoder1Val)
                encoderMapValueSet( enc1Val, bcDWVal, encoder1Val, 0.1f, 0.0f, 1.0f, bcDryWetSlider );
            
            // Amt
            if (enc2Val != encoder2Val && !bcAmtOverride)
                encoderMapValueSet( enc2Val, bcAmtVal, encoder2Val, 0.1f, 0.0f, 1.0f, bcAmtSlider );
            break;
            
        // Enc1 = Form D/W; Enc2 = Form Morph
        case 4:
            // D/W
            if (enc1Val != encoder1Val)
                encoderMapValueSet( enc1Val, formDWVal, encoder1Val, 0.1f, 0.0f, 1.0f, formDryWetSlider );
            
            // Amt
            if (enc2Val != encoder2Val && !formMorphOverride)
                encoderMapValueSet( enc2Val, formAmtVal, encoder2Val, 1.0f, 0.0f, 9.0f, formMorphSlider );
            break;
            
        // Enc1 = Delay D/W; Enc2 = Delay Time
        case 5:
            // D/W
            if (enc1Val != encoder1Val)
                encoderMapValueSet( enc1Val, dlyDWVal, encoder1Val, 0.1f, 0.0f, 1.0f, delayDryWetSlider );
            
            // Time
            if (enc2Val != encoder2Val && !delayTimeOverride)
                encoderMapValueSet( enc2Val, dlyTVal, encoder2Val, 0.1f, 0.0f, 1.0f, delayTimeSlider );
            break;
            
        // Enc2 = Delay D/W; Enc2 = Delay FB
        case 6:
            // D/W
            if (enc1Val != encoder1Val)
                encoderMapValueSet( enc1Val, dlyDWVal, encoder1Val, 0.1f, 0.0f, 1.0f, delayDryWetSlider );
            
            // Feedback
            if (enc2Val != encoder2Val)
                encoderMapValueSet( enc2Val, dlyFBVal, encoder2Val, 0.1f, 0.0f, 1.0f, delayFeedbackSlider );
            break;
            
        // Same as case 0: Enc1 = OutGain; Enc2 = Haas
        default:
            if (enc1Val != encoder1Val)
                encoderMapValueSet( enc1Val, outGainVal, encoder1Val, 1.0f, -100.0f, 12.0f, outGainSlider );
            
            // Haas
            if (enc2Val != encoder2Val)
                encoderMapValueSet( enc2Val, haasVal, encoder2Val, 0.1f, 0.0f, 1.0f, haasWidthSlider );
            break;
    }
    
    // update current encoder value
    encoder1Val = enc1Val;
    encoder2Val = enc2Val;
}

/**
 Sets values for mapped encoders. encVal is enc1Val or enc2Val; paramVal is a reference to the parameter value you are controlling; encoder is a reference to the current stored encoder1Val or encoder2Val
 multFactor multiplies the value for each encoder value tick. limitMin is the parameter minimum. limitMax is the parameter maximum. sliderInstance is a reference to the Slider for the parameter.
 */
void BassOnboardAudioProcessorEditor::encoderMapValueSet(float encVal, float& paramVal, float& encoder, float multFactor, float limitMin, float limitMax, juce::Slider &sliderInstance)
{
    paramVal += (encVal - encoder) * multFactor;
    paramVal  = juce::jlimit( limitMin, limitMax, paramVal );
    
    sliderInstance.setValue( paramVal );
}



/// encoderButton2 Presse cycle through sensor mappings to parameters
void BassOnboardAudioProcessorEditor::sensorMapping()
{
    switch ((int)osc->getEncButton2())
    {
        case 0:
            sensorMap0();
            break;
            
        case 1:
            sensorMap1();
            break;
            
        case 2:
            sensorMap2();
            break;
            
        default:
            sensorMap0();
            break;
    }
}

// Sensor Maps send the values of the mappable sensors (Accel/Gyro/Distance) to various
// parameters values. The Encoder2 Button presses cycle through the sensor mappings 0 to 2.
// If a sensor is on and is mapped to a parameter, it triggers the override that prevents
// a Rotary Encoder from controlling the same parameter.
void BassOnboardAudioProcessorEditor::sensorMap0()
{
    // AccelX to Waveshape Amt
    sensorMapValueSet( accelXOnOffBox, wsAmtOverride, wsAmtSlider, osc->getAccelX(), -4.0f, 4.0f, 1.0f, 200.0f );
    
    // AccelY to Foldback Amt
    sensorMapValueSet( accelYOnOffBox, fbAmtOverride, fbAmtSlider, osc->getAccelY(), -4.0f, 4.0f, 1.0f, 200.0f );
    
    // AccelZ to Bitcrush Amt
    sensorMapValueSet( accelZOnOffBox, bcAmtOverride, bcAmtSlider, osc->getAccelZ(), -4.0f, 4.0f, 0.0f, 1.0f );
    
    // Distance to Formant Morph
    sensorMapValueSet( distanceOnOffBox, formMorphOverride, formMorphSlider, osc->getDistance(), 0.0f, 1200.0f, 0.0f, 9.0f );
    
    // GyroX to Delay Time
    sensorMapValueSet( gyroXOnOffBox, delayTimeOverride, delayTimeSlider, osc->getGyroX(), -2000.0f, 2000.0f, 0.0f, 1.0f );
}

// See Sensor Maps comment above
void BassOnboardAudioProcessorEditor::sensorMap1()
{
    // AccelX to Waveshape Amt
    sensorMapValueSet( accelXOnOffBox, wsAmtOverride, wsAmtSlider, osc->getAccelX(), -4.0f, 4.0f, 1.0f, 200.0f );
    
    // AccelY to Foldback Amt
    sensorMapValueSet( accelYOnOffBox, fbAmtOverride, fbAmtSlider, osc->getAccelY(), -4.0f, 4.0f, 1.0f, 200.0f );
    
    // AccelZ to Bitcrush Amt
    sensorMapValueSet( accelZOnOffBox, bcAmtOverride, bcAmtSlider, osc->getAccelZ(), -4.0f, 4.0f, 0.0f, 1.0f );
    
    // GyroX to formant morph
    sensorMapValueSet( gyroXOnOffBox, formMorphOverride, formMorphSlider, osc->getGyroX(), -2000.0f, 2000.0f, 0.0f, 9.0f );
    
    // Distance to delay time
    sensorMapValueSet( distanceOnOffBox, delayTimeOverride, delayTimeSlider, osc->getDistance(), 0.0f, 1200.0f, 0.0f, 1.0f );
}

// See Sensor Maps comment above
void BassOnboardAudioProcessorEditor::sensorMap2()
{
    // GyroX to WS Amt
    sensorMapValueSet( gyroXOnOffBox, wsAmtOverride, wsAmtSlider, osc->getGyroX(), -2000.0f, 2000.0f, 1.0f, 200.0f );
    
    // GyroY to FB Amt
    sensorMapValueSet( gyroYOnOffBox, fbAmtOverride, fbAmtSlider, osc->getGyroY(), -2000.0f, 2000.0f, 1.0f, 200.0f );
    
    // GyroZ to BC Amt
    sensorMapValueSet( gyroZOnOffBox, bcAmtOverride, bcAmtSlider, osc->getGyroZ(), -2000.0f, 2000.0f, 0.0f, 1.0f );
    
    // AccelX to Formant morph
    sensorMapValueSet( accelXOnOffBox, formMorphOverride, formMorphSlider, osc->getAccelX(), -4.0f, 4.0f, 0.0f, 9.0f );
}

/**
 Sets values for mapped sensors. onOffBox is the ComboBox controlling the sensor on/off. paramOverride is the bool that controls the sensor overriding the rotary encoder.
 sliderInstance is the parameter slider. sensorVal is the current value of the sensor coming over OSC. sensorMin and sensorMax are the min and max values expected from
 the sensor. mapMin and mapMax are the min and max of the parameter that the sensor values will be mapped to.
 */
void BassOnboardAudioProcessorEditor::sensorMapValueSet(juce::ComboBox &onOffBox, bool &paramOverride, juce::Slider &sliderInstance, float sensorVal, float sensorMin, float sensorMax, float mapMin, float mapMax)
{
    if (onOffBox.getSelectedId() == 2.0f)
    {
        paramOverride = true;
        sliderInstance.setValue( juce::jmap( sensorVal, sensorMin, sensorMax, mapMin, mapMax ) );
    }
    else paramOverride = false;
}
