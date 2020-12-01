/*
  ==============================================================================

    MyRotarySlider.h
    Created: 5 Nov 2020 12:15:08pm
    Author:  Michał

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
using namespace juce;

//==============================================================================
/*Klasa OtherLookAndFeel służąca do nadania customowego wyglądu mojej wtyczki
*/
class OtherLookAndFeel : public LookAndFeel_V4 {

public:
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) {
        float diameter = jmin(width, height); //Sprawdzam który wymiar mniejszy, aby dopasować do tego pokrętło
        float radius = diameter / 2;
        float centreX = x + width / 2;
        float centreY = y + height / 2;
        float rx = centreX - radius;
        float ry = centreY - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        Rectangle<float> dialArea(rx, ry, diameter, diameter);

        g.setColour(Colours::yellow);
        
        g.fillEllipse(dialArea);
        g.setColour(Colours::black);

        Path dialTick;
        dialTick.addRectangle(0, -radius, 10.0f, radius + 0.33);
        g.fillPath(dialTick, AffineTransform::rotation(angle).translated(centreX, centreY));
        
        g.setColour(Colours::black);
        g.drawEllipse(rx, ry, diameter, diameter, 5.0f);


    }

};

//==============================================================================
/*
*/
class MyRotarySlider  : public juce::Component
                        
{
public:
    MyRotarySlider(ImpressiveDropAudioProcessor&);
    ~MyRotarySlider() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Slider knob;
    OtherLookAndFeel otherLookAndFeel; //Stworzenie obiektu do podmiany defaultowego pokrętła 
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> controlKnobAttachment; //Unikalny wskaźnik na obiekt typu SliderAttachment
    /* 
    - może istnieć tylko jeden wskaźnik tego typu pokazujący na dany obiekt,
    - w momencie kiedy przestaje istnieć wskaźnik, usuwany jest także obiekt, 
    na który ten wskaźnik pokazywał, a pamięć, którą zajmował jest zwalniana,
    - obiekt na który wskazuje wskaźnik jest także usuwany kiedy do wskaźnika 
    przypisujemy inny obiekt, 
    - obiekt wskazywany przez unique_ptr nie może być zarządzany przez inny obiekt tego typu ani też shared_ptr.*/
    ImpressiveDropAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyRotarySlider)
};
