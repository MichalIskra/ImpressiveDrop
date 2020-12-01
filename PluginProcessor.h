/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MyParameters.h"

using namespace juce;

//==============================================================================


class ImpressiveDropAudioProcessor  : public juce::AudioProcessor,
                                      public ValueTree::Listener
{
public:
    //==============================================================================
    ImpressiveDropAudioProcessor();
    ~ImpressiveDropAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    ///========================== moje funkcje itd
    AudioProcessorValueTreeState& getControlParam() { return controlParam; } //funkcja zwracająca controlParam, które jest zmienną prywatną
   
    ///==========================
   
    void updatefx();

    void reset() noexcept
    {
        fx.reset();     // [3]
    }
   
private:
    AudioProcessorValueTreeState controlParam; // Struktura trzymajaca parametry 
    AudioProcessorValueTreeState::ParameterLayout createParameters(); //funkcja tworzaca liste parametrow użytych w AudioValueTreeState 
    void valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property) override; //nadpisanie funkcji z ValueTree::Listener
    std::atomic<bool> controlParamShouldUpdate = false; //Stworzenie kontrolki bool do wywoływania odpowiedniej reakcji na zmianę parametru pokrętła
    //==============================================================================
    //_______________________________FILTR___________________________________________________________________________________________
    //dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>> stateVariableFilter;
    float lastSampleRate;//Zmienna potrzebna aby się odnieść w dsp  , dsp::DelayLine<float, dsp::DelayLineInterpolationTypes::Lagrange3rd>
    //dsp::ProcessorChain<dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>>, dsp::Reverb, dsp::LadderFilter<float>, dsp::WaveShaper<float>> fx;
 
    //
    dsp::ProcessorChain<dsp::LadderFilter<float>,dsp::Chorus<float>, dsp::Reverb, dsp::WaveShaper<float, std::function<float(float)>>> fx;

    MyParameters fx_automation;

    // now you can process float arrays l & r thus:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImpressiveDropAudioProcessor)
};