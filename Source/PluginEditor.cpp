/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BassOnboardAudioProcessorEditor::BassOnboardAudioProcessorEditor (BassOnboardAudioProcessor& p)
    : AudioProcessorEditor (&p),
    onyx                   ( Colour( (uint8)53,  (uint8)59,  (uint8)60  ) ),
    lightSlateGray         ( Colour( (uint8)130, (uint8)146, (uint8)152 ) ),
    magicMint              ( Colour( (uint8)174, (uint8)255, (uint8)216 ) ),
    fieryRose              ( Colour( (uint8)255, (uint8)104, (uint8)114 ) ),
    orangePeel             ( Colour( (uint8)252, (uint8)152, (uint8)0   ) ),
    audioProcessor (p)
{
    setSize (1000, 600);
    
    Timer::startTimerHz(60);
    
    //
    // Sliders Setup
    //
    Slider::SliderStyle vertSlider = Slider::SliderStyle::LinearVertical;
    
    // In Gain
    sliderSetup ( inGainSlider, vertSlider, true );
    
    // Compressor
    sliderSetup ( compThreshSlider,  vertSlider, true );
    sliderSetup ( compRatioSlider,   vertSlider, true );
    sliderSetup ( compAttackSlider,  vertSlider, true );
    sliderSetup ( compReleaseSlider, vertSlider, true );
    
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
    onOffBoxSetup ( compOnOffBox  );
    onOffBoxSetup ( wsOnOffBox    );
    onOffBoxSetup ( fbOnOffBox    );
    onOffBoxSetup ( bcOnOffBox    );
    onOffBoxSetup ( formOnOffBox  );
    onOffBoxSetup ( delayOnOffBox );
    onOffBoxSetup ( filtOnOffBox  );
    onOffBoxSetup ( haasOnOffBox  );
    
    // Filter Type
    filtTypeBox.addItem              ( "LPF", 1 );
    filtTypeBox.addItem              ( "BPF", 2 );
    filtTypeBox.addItem              ( "HPF", 3 );
    filtTypeBox.setJustificationType ( Justification::centred );
    filtTypeBox.setSelectedItemIndex ( 0 );
    
    filtTypeBox.setColour ( ComboBox::backgroundColourId, onyx        );
    filtTypeBox.setColour ( ComboBox::outlineColourId, lightSlateGray );
    filtTypeBox.setColour ( ComboBox::textColourId, orangePeel        );
    filtTypeBox.setColour ( ComboBox::arrowColourId, magicMint        );
    
    addAndMakeVisible                ( filtTypeBox );
    
    // Filter Poles
    filtPolesBox.addItem              ( "-12dB", 1 );
    filtPolesBox.addItem              ( "-24dB", 2 );
    filtPolesBox.setJustificationType ( Justification::centred );
    filtPolesBox.setSelectedItemIndex ( 0 );
    
    filtPolesBox.setColour ( ComboBox::backgroundColourId, onyx        );
    filtPolesBox.setColour ( ComboBox::outlineColourId, lightSlateGray );
    filtPolesBox.setColour ( ComboBox::textColourId, orangePeel        );
    filtPolesBox.setColour ( ComboBox::arrowColourId, magicMint        );
    
    addAndMakeVisible                 ( filtPolesBox );
    
    //
    // Label Setup
    //
    
    float bigLabelSize   = 16.0f;
    float smallLabelSize = 12.0f;
    
    // In Gain
    sliderLabelSetup ( inLabel,     "In",   bigLabelSize   );
    sliderLabelSetup ( inGainLabel, "Gain", smallLabelSize );
    
    // Compressor
    sliderLabelSetup ( compLabel,        "Compressor", bigLabelSize   );
    sliderLabelSetup ( compThreshLabel,  "Thresh",     smallLabelSize );
    sliderLabelSetup ( compRatioLabel,   "Ratio",      smallLabelSize );
    sliderLabelSetup ( compAttackLabel,  "Attack",     smallLabelSize );
    sliderLabelSetup ( compReleaseLabel, "Release",    smallLabelSize );
    
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
    
    //
    // Slider Attachments
    //
    
    // In Gain
    inGainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "inGain", inGainSlider );
    
    // Compressor
    compThreshAttachment  = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,"compThresh",  compThreshSlider  );
    compRatioAttachment   = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,"compRatio",   compRatioSlider   );
    compAttackAttachment  = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,"compAttack",  compAttackSlider  );
    compReleaseAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,"compRelease", compReleaseSlider );
    
    // Waveshaper
    wsAmtAttachment    = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "wsAmt",    wsAmtSlider    );
    wsDryWetAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "wsDryWet", wsDryWetSlider );
    
    // Foldback
    fbAmtAttachment    = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "foldbackAmt",    fbAmtSlider    );
    fbDryWetAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "foldbackDryWet", fbDryWetSlider );
    
    // Bitcrusher
    bcAmtAttachment    = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "bitcrushAmt",    bcAmtSlider    );
    bcDryWetAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "bitcrushDryWet", bcDryWetSlider );
    
    // Formant
    formMorphAttachment  = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "formantMorph",  formMorphSlider  );
    formDryWetAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "formantDryWet", formDryWetSlider );
    
    // Delay
    delayTimeAttachment     = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "delayFXTime",   delayTimeSlider     );
    delayFeedbackAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "delayFXFdbck",  delayFeedbackSlider );
    delayDryWetAttachment   = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "delayFXDryWet", delayDryWetSlider   );
    
    // Filter
    filtCutoffAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "svFiltCutoff", filtCutoffSlider );
    filtResAttachment    = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "svFiltRes",    filtResSlider    );
    
    // Haas Width
    haasWidthAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "haasWidth", haasWidthSlider );
    
    // Out Gain
    outGainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters, "outGain", outGainSlider );
    
    //
    // ComboBox Attachments
    //
    compOnOffAttachment  = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "compOnOff",     compOnOffBox  );
    wsOnOffAttachment    = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "wsOnOff",       wsOnOffBox    );
    fbOnOffAttachment    = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "foldbackOnOff", fbOnOffBox    );
    bcOnOffAttachment    = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "bitcrushOnOff", bcOnOffBox    );
    formOnOffAttachment  = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "formantOnOff",  formOnOffBox  );
    delayOnOffAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "delayFXOnOff",  delayOnOffBox );
    filtTypeAttachment   = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "svFiltType",    filtTypeBox   );
    filtPolesAttachment  = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "svFiltPoles",   filtPolesBox  );
    filtOnOffAttachment  = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "svFiltOnOff",   filtOnOffBox  );
    haasOnOffAttachment  = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.parameters, "haasOnOff",     haasOnOffBox  );
    
}

BassOnboardAudioProcessorEditor::~BassOnboardAudioProcessorEditor()
{
    Timer::stopTimer();
}

//==============================================================================
void BassOnboardAudioProcessorEditor::paint (juce::Graphics& g)
{
    float rounding = 2.0f;
    
    g.fillAll( lightSlateGray );
    
    // Subsections
    g.setColour            ( onyx );
    g.fillRoundedRectangle ( inAreaBG,    rounding );
    g.fillRoundedRectangle ( ngAreaBG,    rounding ); // Not needed after re-design layout
    g.fillRoundedRectangle (compAreaBG,   rounding );
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
    Rectangle<int> totalArea  = getLocalBounds().reduced( areaPadding );
    Rectangle<int> topHalf    = totalArea.removeFromTop( totalArea.getHeight() * 0.5f );
    Rectangle<int> bottomHalf = totalArea;
    
    // Top & Bottom Halves split into effect sections, width based on number of sliders
    float topSpacing    = topHalf.getWidth() / 15.0f;
    float bottomSpacing = bottomHalf.getWidth() / 11.0f;
    
    Rectangle<int> inArea   = topHalf.removeFromLeft ( topSpacing ).reduced        ( areaPadding );
    Rectangle<int> ngArea   = topHalf.removeFromLeft ( topSpacing * 4.0f ).reduced ( areaPadding );
    Rectangle<int> compArea = topHalf.removeFromLeft ( topSpacing * 4.0f ).reduced ( areaPadding );
    Rectangle<int> wsArea   = topHalf.removeFromLeft ( topSpacing * 2.0f ).reduced ( areaPadding );
    Rectangle<int> fbArea   = topHalf.removeFromLeft ( topSpacing * 2.0f ).reduced ( areaPadding );
    Rectangle<int> bcArea   = topHalf.reduced                                      ( areaPadding );
    
    Rectangle<int> formArea  = bottomHalf.removeFromLeft ( bottomSpacing * 2.0f ).reduced ( areaPadding );
    Rectangle<int> delayArea = bottomHalf.removeFromLeft ( bottomSpacing * 3.0f ).reduced ( areaPadding );
    Rectangle<int> filtArea  = bottomHalf.removeFromLeft ( bottomSpacing * 4.0f ).reduced ( areaPadding );
    Rectangle<int> haasArea  = bottomHalf.removeFromLeft ( bottomSpacing ).reduced        ( areaPadding );
    Rectangle<int> outArea   = bottomHalf.reduced                                         ( areaPadding );
    
    inAreaBG.setBounds    ( inArea.getX(),    inArea.getY(),    inArea.getWidth(),    inArea.getHeight()    );
    ngAreaBG.setBounds    ( ngArea.getX(),    ngArea.getY(),    ngArea.getWidth(),    ngArea.getHeight()    );  // Not needed after re-design layout
    compAreaBG.setBounds  ( compArea.getX(),  compArea.getY(),  compArea.getWidth(),  compArea.getHeight()  );
    wsAreaBG.setBounds    ( wsArea.getX(),    wsArea.getY(),    wsArea.getWidth(),    wsArea.getHeight()    );
    fbAreaBG.setBounds    ( fbArea.getX(),    fbArea.getY(),    fbArea.getWidth(),    fbArea.getHeight()    );
    bcAreaBG.setBounds    ( bcArea.getX(),    bcArea.getY(),    bcArea.getWidth(),    bcArea.getHeight()    );
    formAreaBG.setBounds  ( formArea.getX(),  formArea.getY(),  formArea.getWidth(),  formArea.getHeight()  );
    delayAreaBG.setBounds ( delayArea.getX(), delayArea.getY(), delayArea.getWidth(), delayArea.getHeight() );
    filtAreaBG.setBounds  ( filtArea.getX(),  filtArea.getY(),  filtArea.getWidth(),  filtArea.getHeight()  );
    haasAreaBG.setBounds  ( haasArea.getX(),  haasArea.getY(),  haasArea.getWidth(),  haasArea.getHeight()  );
    outAreaBG.setBounds   ( outArea.getX(),   outArea.getY(),   outArea.getWidth(),   outArea.getHeight()   );
    
    float labelHeight = 50.0f;
    float boxHeight   = 50.0f;
    float boxReduceX  = 75.0f;  // Only works on wider effects section
    float boxReduceY  = 12.0f;
    
    
    // Top Half Areas
    
    // In Gain
    Rectangle<int> inLabelArea     = inArea.removeFromTop    ( labelHeight );
    Rectangle<int> inGainLabelArea = inArea.removeFromTop    ( labelHeight );
    Rectangle<int> inBoxSpacer     = inArea.removeFromBottom ( boxHeight   );
    
    inLabel.setBounds      ( inLabelArea );
    inGainLabel.setBounds  ( inGainLabelArea );
    inGainSlider.setBounds ( inArea );
    
    
    // Noise Gate
    Rectangle<int> ngLabelArea      = ngArea.removeFromTop    ( labelHeight );  // Not needed after re-design layout
      
    
    // Compressor
    Rectangle<int> compLabelArea      = compArea.removeFromTop    ( labelHeight );
    Rectangle<int> compParamLabelArea = compArea.removeFromTop    ( labelHeight );
    Rectangle<int> compBoxArea        = compArea.removeFromBottom ( boxHeight   );
    
    compLabel.setBounds    ( compLabelArea );
    compOnOffBox.setBounds ( compBoxArea.reduced(boxReduceX, boxReduceY) );
    
    float compWidth = compArea.getWidth() * 0.25f;
    
    Rectangle<int> compThreshLabelArea  = compParamLabelArea.removeFromLeft ( compWidth );
    Rectangle<int> compRatioLabelArea   = compParamLabelArea.removeFromLeft ( compWidth );
    Rectangle<int> compAttackLabelArea  = compParamLabelArea.removeFromLeft ( compWidth );
    Rectangle<int> compReleaseLabelArea = compParamLabelArea;
    
    compThreshLabel.setBounds  ( compThreshLabelArea  );
    compRatioLabel.setBounds   ( compRatioLabelArea   );
    compAttackLabel.setBounds  ( compAttackLabelArea  );
    compReleaseLabel.setBounds ( compReleaseLabelArea );
    
    Rectangle<int> compThreshArea  = compArea.removeFromLeft ( compWidth );
    Rectangle<int> compRatioArea   = compArea.removeFromLeft ( compWidth );
    Rectangle<int> compAttackArea  = compArea.removeFromLeft ( compWidth );
    Rectangle<int> compReleaseArea = compArea;
    
    compThreshSlider.setBounds  ( compThreshArea  );
    compRatioSlider.setBounds   ( compRatioArea   );
    compAttackSlider.setBounds  ( compAttackArea  );
    compReleaseSlider.setBounds ( compReleaseArea );
    
    
    // Waveshaper
    Rectangle<int> wsLabelArea      = wsArea.removeFromTop    ( labelHeight );
    Rectangle<int> wsParamLabelArea = wsArea.removeFromTop    ( labelHeight );
    Rectangle<int> wsBoxArea        = wsArea.removeFromBottom ( boxHeight   );
    
    wsLabel.setBounds    ( wsLabelArea );
    wsOnOffBox.setBounds ( wsBoxArea.reduced(boxReduceY, boxReduceY) );
    
    float wsWidth = wsArea.getWidth() * 0.5f;
    
    Rectangle<int> wsAmtLabelArea    = wsParamLabelArea.removeFromLeft ( wsWidth );
    Rectangle<int> wsDryWetLabelArea = wsParamLabelArea;
    
    wsAmtLabel.setBounds    ( wsAmtLabelArea    );
    wsDryWetLabel.setBounds ( wsDryWetLabelArea );
    
    Rectangle<int> wsAmtArea    = wsArea.removeFromLeft ( wsWidth );
    Rectangle<int> wsDryWetArea = wsArea;
    
    wsAmtSlider.setBounds    ( wsAmtArea    );
    wsDryWetSlider.setBounds ( wsDryWetArea );
    
    
    // Foldback
    Rectangle<int> fbLabelArea      = fbArea.removeFromTop    ( labelHeight );
    Rectangle<int> fbParamLabelArea = fbArea.removeFromTop    ( labelHeight );
    Rectangle<int> fbBoxArea        = fbArea.removeFromBottom ( boxHeight   );
    
    fbLabel.setBounds    ( fbLabelArea );
    fbOnOffBox.setBounds ( fbBoxArea.reduced(boxReduceY, boxReduceY) );
    
    float fbWidth = fbArea.getWidth() * 0.5f;
    
    Rectangle<int> fbAmtLabelArea    = fbParamLabelArea.removeFromLeft ( fbWidth );
    Rectangle<int> fbDryWetLabelArea = fbParamLabelArea;
    
    fbAmtLabel.setBounds    ( fbAmtLabelArea    );
    fbDryWetLabel.setBounds ( fbDryWetLabelArea );
    
    Rectangle<int> fbAmtArea    = fbArea.removeFromLeft ( fbWidth );
    Rectangle<int> fbDryWetArea = fbArea;
    
    fbAmtSlider.setBounds    ( fbAmtArea    );
    fbDryWetSlider.setBounds ( fbDryWetArea );
    
    
    // Bitcrusher
    Rectangle<int> bcLabelArea      = bcArea.removeFromTop    ( labelHeight );
    Rectangle<int> bcParamLabelArea = bcArea.removeFromTop    ( labelHeight );
    Rectangle<int> bcBoxArea        = bcArea.removeFromBottom ( boxHeight   );
    
    bcLabel.setBounds    ( bcLabelArea );
    bcOnOffBox.setBounds ( bcBoxArea.reduced(boxReduceY, boxReduceY)   );
    
    float bcWidth = bcArea.getWidth() * 0.5f;
    
    Rectangle<int> bcAmtLabelArea    = bcParamLabelArea.removeFromLeft ( bcWidth );
    Rectangle<int> bcDryWetLabelArea = bcParamLabelArea;
    
    bcAmtLabel.setBounds    ( bcAmtLabelArea    );
    bcDryWetLabel.setBounds ( bcDryWetLabelArea );
    
    Rectangle<int> bcAmtArea    = bcArea.removeFromLeft ( bcWidth );
    Rectangle<int> bcDryWetArea = bcArea;
    
    bcAmtSlider.setBounds    ( bcAmtArea    );
    bcDryWetSlider.setBounds ( bcDryWetArea );
    
    
    // Bottom Half Areas
    
    // Formant
    Rectangle<int> formLabelArea      = formArea.removeFromTop    ( labelHeight );
    Rectangle<int> formParamLabelArea = formArea.removeFromTop    ( labelHeight );
    Rectangle<int> formBoxArea        = formArea.removeFromBottom ( boxHeight   );
    
    formLabel.setBounds    ( formLabelArea );
    formOnOffBox.setBounds ( formBoxArea.reduced(boxReduceY * 2.0f, boxReduceY) );
    
    float formWidth = formArea.getWidth() * 0.5f;
    
    Rectangle<int> formMorphLabelArea  = formParamLabelArea.removeFromLeft ( formWidth );
    Rectangle<int> formDryWetLabelArea = formParamLabelArea;
    
    formMorphLabel.setBounds  ( formMorphLabelArea  );
    formDryWetLabel.setBounds ( formDryWetLabelArea );
    
    Rectangle<int> formMorphArea  = formArea.removeFromLeft ( formWidth );
    Rectangle<int> formDryWetArea = formArea;
    
    formMorphSlider.setBounds  ( formMorphArea  );
    formDryWetSlider.setBounds ( formDryWetArea );
    
    
    // Delay
    Rectangle<int> delayLabelArea      = delayArea.removeFromTop    ( labelHeight );
    Rectangle<int> delayParamLabelArea = delayArea.removeFromTop    ( labelHeight );
    Rectangle<int> delayBoxArea        = delayArea.removeFromBottom ( boxHeight   );
    
    delayLabel.setBounds    ( delayLabelArea );
    delayOnOffBox.setBounds ( delayBoxArea.reduced(boxReduceX, boxReduceY) );
    
    float delayWidth = delayArea.getWidth() * 0.33f;
    
    Rectangle<int> delayTimeLabelArea   = delayParamLabelArea.removeFromLeft ( delayWidth );
    Rectangle<int> delayFdbkLabelArea   = delayParamLabelArea.removeFromLeft ( delayWidth );
    Rectangle<int> delayDryWetLabelArea = delayParamLabelArea;
    
    delayTimeLabel.setBounds     ( delayTimeLabelArea  );
    delayFeedbackLabel.setBounds ( delayFdbkLabelArea  );
    delayDryWetLabel.setBounds   (delayDryWetLabelArea );
    
    Rectangle<int> delayTimeArea   = delayArea.removeFromLeft ( delayWidth );
    Rectangle<int> delayFdbkArea   = delayArea.removeFromLeft ( delayWidth );
    Rectangle<int> delayDryWetArea = delayArea;
    
    delayTimeSlider.setBounds     ( delayTimeArea   );
    delayFeedbackSlider.setBounds ( delayFdbkArea   );
    delayDryWetSlider.setBounds   ( delayDryWetArea );
    
    
    // Filter
    Rectangle<int> filtLabelArea      = filtArea.removeFromTop    ( labelHeight );
    Rectangle<int> filtParamLabelArea = filtArea.removeFromTop    ( labelHeight );
    Rectangle<int> filtBoxArea        = filtArea.removeFromBottom ( boxHeight   );
    
    filtLabel.setBounds    ( filtLabelArea );
    filtOnOffBox.setBounds ( filtBoxArea.reduced(boxReduceX * 1.5f, boxReduceY) );
    
    float filtWidth = filtArea.getWidth() * 0.25f;
    
    Rectangle<int> filtCutoffLabelArea = filtParamLabelArea.removeFromLeft  ( filtWidth );
    Rectangle<int> filtParamSpacerArea = filtParamLabelArea.removeFromRight ( filtWidth * 2.0f );
    Rectangle<int> filtResLabelArea    = filtParamLabelArea;
    
    filtCutoffLabel.setBounds ( filtCutoffLabelArea );
    filtResLabel.setBounds    ( filtResLabelArea    );
    
    Rectangle<int> filtCutoffArea = filtArea.removeFromLeft ( filtWidth );
    Rectangle<int> filtResArea    = filtArea.removeFromLeft ( filtWidth );
    Rectangle<int> filtTypeArea   = filtArea.removeFromTop  ( filtArea.getHeight() * 0.5f );
    Rectangle<int> filtPoleArea   = filtArea;
    
    filtCutoffSlider.setBounds ( filtCutoffArea );
    filtResSlider.setBounds    ( filtResArea    );
    filtTypeBox.setBounds      ( filtTypeArea.reduced(boxReduceY, boxReduceY) );
    filtPolesBox.setBounds     ( filtPoleArea.reduced(boxReduceY, boxReduceY) );
    
    
    // Haas Width
    Rectangle<int> haasLabelArea      = haasArea.removeFromTop    ( labelHeight );
    Rectangle<int> haasParamLabelArea = haasArea.removeFromTop    ( labelHeight );
    Rectangle<int> haasBoxArea        = haasArea.removeFromBottom ( boxHeight   );
    
    haasLabel.setBounds       ( haasLabelArea );
    haasWidthLabel.setBounds  ( haasParamLabelArea );
    haasOnOffBox.setBounds    ( haasBoxArea.reduced( 4.0f, boxReduceY) );
    haasWidthSlider.setBounds ( haasArea );
    
    
    // Out Gain
    Rectangle<int> outLabelArea      = outArea.removeFromTop    ( labelHeight );
    Rectangle<int> outParamLabelArea = outArea.removeFromTop    ( labelHeight );
    Rectangle<int> outBoxSpacer      = outArea.removeFromBottom ( boxHeight   );
    
    outLabel.setBounds      ( outLabelArea      );
    outGainLabel.setBounds  ( outParamLabelArea );
    outGainSlider.setBounds ( outArea           );
    
    
    
}



void BassOnboardAudioProcessorEditor::timerCallback()
{
    // Generic Sensor Map
    float accelXMapped = jmap( osc.getAccelX(), -4.0f, 4.0f, 1.0f, 200.0f );
    float accelYMapped = jmap( osc.getAccelY(), -4.0f, 4.0f, 1.0f, 200.0f );
    float accelZMapped = jmap( osc.getAccelZ(), -4.0f, 4.0f, 0.0f, 1.0f   );
    
    float distanceMapped = jmap( osc.getDistance(), 0.0f, 1200.0f, 0.0f, 9.0f );
    
    float touchXMapped = jmap( osc.getTouchX(), 300.0f, 800.0f, 0.7f,  2.5f     );
    float touchYMapped = jmap( osc.getTouchY(), 300.0f, 800.0f, 20.0f, 18000.0f );
    
    // Control Parameter with mapped Sensor values
    wsAmtSlider.setValue ( accelXMapped );
    fbAmtSlider.setValue ( accelYMapped );
    bcAmtSlider.setValue ( accelZMapped );
    
    formMorphSlider.setValue ( distanceMapped );
    
    filtCutoffSlider.setValue ( touchYMapped );
    filtResSlider.setValue    ( touchXMapped );
    
    
    
    
        
}


/// Sets up Slider object instances in constructor. sliderInstance is the slider to set up, suffix is textValueSuffix, sliderFillColor is the slider color below the thumb
void BassOnboardAudioProcessorEditor::sliderSetup(Slider &sliderInstance, Slider::SliderStyle style, bool showTextBox)
{
    sliderInstance.setSliderStyle(style);
        
    // If slider has a textbox, draw it, otherwise, don't
    if (showTextBox)
    {
        sliderInstance.setTextBoxStyle ( Slider::TextBoxBelow, false, 50, 20 );
        sliderInstance.setColour       ( Slider::textBoxOutlineColourId, Colour( (uint8)0, (uint8)0, (uint8)0, (uint8)0 ) );
        sliderInstance.setColour       ( Slider::textBoxTextColourId, magicMint );
    }
    else
    {
        sliderInstance.setTextBoxStyle( Slider::NoTextBox, false, 0, 0 );
    }
    
    sliderInstance.setColour( Slider::trackColourId, fieryRose );
    sliderInstance.setColour( Slider::thumbColourId, magicMint );
    
    addAndMakeVisible(sliderInstance);
}


/// Sets up Label for the Slider instances. Takes the labelInstance and the text for setText
void BassOnboardAudioProcessorEditor::sliderLabelSetup(Label &labelInstance, String labelText, float fontSize)
{
    labelInstance.setText              ( labelText, dontSendNotification );
    labelInstance.setJustificationType ( Justification::centred );
    labelInstance.setColour            ( Label::textColourId, orangePeel );
    labelInstance.setFont              ( Font("helvetica", fontSize, 1) );
    addAndMakeVisible                  ( labelInstance );
}


/// Sets up an On/Off combo box
void BassOnboardAudioProcessorEditor::onOffBoxSetup(ComboBox &boxInstance)
{
    boxInstance.addItem              ( "Off", 1 );
    boxInstance.addItem              ( "On",  2 );
    boxInstance.setJustificationType ( Justification::centred );
    boxInstance.setSelectedItemIndex ( 0 );
    
    boxInstance.setColour ( ComboBox::backgroundColourId, onyx        );
    boxInstance.setColour ( ComboBox::outlineColourId, lightSlateGray );
    boxInstance.setColour ( ComboBox::textColourId, orangePeel        );
    boxInstance.setColour ( ComboBox::arrowColourId, magicMint        );
    
    addAndMakeVisible ( boxInstance );
}
