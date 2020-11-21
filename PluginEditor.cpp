/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ImpressiveDropAudioProcessorEditor::ImpressiveDropAudioProcessorEditor (ImpressiveDropAudioProcessor& p)
    : AudioProcessorEditor (&p), 
      audioProcessor (p), 
      controlKnob(p)//nie ma default konstruktora wiêc daje mu adres procesora
{
    
    setSize (300, 300);
    addAndMakeVisible(&controlKnob);
  
}

ImpressiveDropAudioProcessorEditor::~ImpressiveDropAudioProcessorEditor()
{
}

//==============================================================================
void ImpressiveDropAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::black);
}

void ImpressiveDropAudioProcessorEditor::resized()
{
    Rectangle<int> area = getLocalBounds();
    int width = area.getWidth();
    int height = area.getHeight();
    controlKnob.setBounds(area);

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
