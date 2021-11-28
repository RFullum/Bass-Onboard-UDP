/*
  ==============================================================================

    TitleHeader.h
    Created: 4 Feb 2021 11:47:30am
    Author:  Robert Fullum

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TitleHeader  : public juce::Component
{
public:
    TitleHeader();
    ~TitleHeader() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /// Sets the background color
    void setBackgroundColor(juce::Colour& bg);
    
    /// Sets the textColor
    void setTextColor(juce::Colour& textC);
    
protected:
    juce::Colour backgroundColor;
    juce::Colour textColor;

private:
    juce::Rectangle<int> fullumMusicArea;
    juce::Rectangle<int> theClockArea;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TitleHeader)
};


//==============================================================================
//==============================================================================
//==============================================================================

class TitleFooter : public TitleHeader
{
public:
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    juce::Rectangle<int> versionArea;
    juce::Rectangle<int> urlArea;
};



