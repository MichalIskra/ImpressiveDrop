/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyRotarySlider.h"

using namespace juce;

//==============================================================================
/**
*/
class ImpressiveDropAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ImpressiveDropAudioProcessorEditor (ImpressiveDropAudioProcessor&);
    ~ImpressiveDropAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    ImpressiveDropAudioProcessor& audioProcessor;
    MyRotarySlider controlKnob;//stworzenie pokretla, ale nie ma defaultowego konstruktora więc wczesniej zainicjowałem(lista inicjalizacyjna)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImpressiveDropAudioProcessorEditor)
};
