/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>


//==============================================================================
ImpressiveDropAudioProcessor::ImpressiveDropAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), controlParam(*this, nullptr, "PARAMETERS", createParameters()) //Inicjalizacja controlParam
                         

#endif
{

    fx.get<0>().setMode(dsp::LadderFilterMode::HPF24);
    fx.get<0>().setEnabled(true); //Zainicjalizowanie filtra górno przepustowego 24 db

    fx.get<3>().functionToUse = [](float x)
    {  
        float y;
        if (x <= 0.7f) {
            y = x;
        }
        else if ((x <= 0.98f) && (x > 0.7f)) {
            y = (3 - std::pow((2 - 3*(x-0.35f)),2))/3;
        }
        else {
            y = 1;
        }
        return y;
    }; //Zainicjalizowanie funkcji do waveshapera
   
    controlParam.state.addListener(this);//Dodanie listenera do controlparam
    
}

ImpressiveDropAudioProcessor::~ImpressiveDropAudioProcessor()
{
}

//==============================================================================
const juce::String ImpressiveDropAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ImpressiveDropAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ImpressiveDropAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ImpressiveDropAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ImpressiveDropAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ImpressiveDropAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ImpressiveDropAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ImpressiveDropAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ImpressiveDropAudioProcessor::getProgramName (int index)
{
    return {};
}

void ImpressiveDropAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ImpressiveDropAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    lastSampleRate = sampleRate;
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    dsp::ProcessSpec spec;                               /*Itbasically allows us to pass on information about the
                                                         plugin itself nto these DSP module objects
                                                         */
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

   

    fx.reset();
    updatefx();
    fx.prepare(spec);
    

}

void ImpressiveDropAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ImpressiveDropAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ImpressiveDropAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    dsp::AudioBlock<float> block(buffer); //we don't needto iterate throu channels andblocks
    
    updatefx();
    fx.process(dsp::ProcessContextReplacing<float>(block));

   
}

//==============================================================================
bool ImpressiveDropAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ImpressiveDropAudioProcessor::createEditor()
{
    return new ImpressiveDropAudioProcessorEditor (*this);
}

//==============================================================================
void ImpressiveDropAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = controlParam.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml()); //Zapisywanie ustawień parametrów
    copyXmlToBinary(*xml, destData);

}

void ImpressiveDropAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(controlParam.state.getType()))             //Odczytywanie ustawień parametrów
            controlParam.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================


AudioProcessorValueTreeState::ParameterLayout ImpressiveDropAudioProcessor::createParameters() {
    //Jest to funkcja której celem jest stworzenie vectora parametrów AudioValueTreeState
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;
    parameters.push_back(std::make_unique<AudioParameterFloat>("EFFECT", "effect", 0, 999, 0));
    return { parameters.begin(), parameters.end() };
}

void ImpressiveDropAudioProcessor::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) {
    //Jest to funkcja której celem jest zareagowanie na zmianê parametru pod³¹czonego do AudioValueTreeState
    controlParamShouldUpdate = true;
    
}

/*void ImpressiveDropAudioProcessor::updateFilter() 
{
    float value = controlParam.getRawParameterValue("EFFECT")->load();
    stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass; //Przypisanie że to jest lowPAssfilter
    float control_1 = controlParam.getRawParameterValue("EFFECT")->load();
    float array[100] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50 };
    int control_1_place = floor(control_1 * 100);
    stateVariableFilter.state->setCutOffFrequency(lastSampleRate, 50 *array[control_1_place], static_cast<float> (1.0 / MathConstants<double>::sqrt2));
}*/

void ImpressiveDropAudioProcessor::updatefx()
{
 
    float control_1 = controlParam.getRawParameterValue("EFFECT")->load(); //Załadowanie parametru z pokrętła
    int control = floor(control_1);
    //===============FILTR==========================
    fx.setBypassed<0>(false);

    fx.get<0>().setCutoffFrequencyHz(fx_automation.get_fltr_Frequency(control));
    fx.get<0>().setDrive(fx_automation.get_fltr_Drive(control));
    fx.get<0>().setResonance(fx_automation.get_fltr_Resonance(control));

    //=============Chorus==========================
    fx.setBypassed<1>(false);

    fx.get<1>().setRate(fx_automation.get_chrs_Rate(control));
    fx.get<1>().setDepth(fx_automation.get_chrs_Depth(control));
    fx.get<1>().setCentreDelay(fx_automation.get_chrs_CentreDelay(control));
    fx.get<1>().setFeedback(fx_automation.get_chrs_Feedback(control));
    fx.get<1>().setMix(fx_automation.get_chrs_Mix(control));
  

    //=============REVERB=============================
    fx.setBypassed<2>(false);

    dsp::Reverb::Parameters reverbParameters;
    reverbParameters.damping = fx_automation.get_rvrb_Damping(control);
    reverbParameters.dryLevel = fx_automation.get_rvrb_DryLevel(floor(control/10));//Tablica Dry ma 100 elementów
    reverbParameters.wetLevel = fx_automation.get_rvrb_WetLevel(floor(control/10));//TablicaWet ma  100 elementów
    reverbParameters.roomSize = fx_automation.get_rvrb_RoomSize(control);
    reverbParameters.width = fx_automation.get_rvrb_Width(control);
    fx.get<2>().setParameters(reverbParameters);
    //==============Waveshaper======================
    fx.setBypassed<3>(true);
   

 

    

}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{ 
    return new ImpressiveDropAudioProcessor();
}