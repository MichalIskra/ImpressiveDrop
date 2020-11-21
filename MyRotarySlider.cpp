/*
  ==============================================================================

    MyRotarySlider.cpp
    Created: 5 Nov 2020 12:15:08pm
    Author:  Michał

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MyRotarySlider.h"
using namespace juce;

//==============================================================================
MyRotarySlider::MyRotarySlider(ImpressiveDropAudioProcessor& p) : //Przyjmuje adres do audio processora, aby wprowadzić przy tworzeniu obiektu
processor(p)//Inicjalizacja zmiennej processor
{
   
    knob.setSliderStyle(Slider::SliderStyle::Rotary); //Stworzenie pokrętła
    addAndMakeVisible(&knob);//Dodanie go do widoku
    controlKnobAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getControlParam(), "EFFECT", knob);/*Dołączenie pokrętła do audiovaluetreestate poprzez użycie 
                                                                                                                                          funkcji make_unique zwracającej inteligentny wskaźnik std::unique_ptr<T> 
                                                                                                                                         zaalokowanym obiektem lub tablicą obiektów danego typu.*/
    knob.setLookAndFeel(&otherLookAndFeel);

}

MyRotarySlider::~MyRotarySlider()
{
}

void MyRotarySlider::paint (juce::Graphics& g)
{
 
}

void MyRotarySlider::resized()
{
     //____________________________Pokretlo____________________________________________________________
    Rectangle<int> area = getLocalBounds(); //Stworzenie pierwszego obszaru obejmujacego wtyczke
    knob.setBounds(area);//Ustawienie granic pokretla

}

