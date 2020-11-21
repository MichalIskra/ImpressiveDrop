/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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

   // stateVariableFilter.reset(); //gets outall garbage values
   //updateFilter();
   //stateVariableFilter.prepare(spec);

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

    dsp::AudioBlock<float> block(buffer); //wedon't needto iteratethrou channels andblocks
    //updateFilter();
    updatefx();

    //stateVariableFilter.process(dsp::ProcessContextReplacing<float> (block));
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
    parameters.push_back(std::make_unique<AudioParameterFloat>("EFFECT", "effect", 0.0f, 0.99f, 0.0f));
    return { parameters.begin(), parameters.end() };
}

void ImpressiveDropAudioProcessor::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) {
    //Jest to funkcja której celem jest zareagowanie na zmianê parametru pod³¹czonego do AudioValueTreeState
    controlParamShouldUpdate = true;
    mGain = controlParam.getRawParameterValue("EFFECT")->load();
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

    fx.get<0>().state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass; //Przypisanie że to jest lowPAssfilter
    float control = controlParam.getRawParameterValue("EFFECT")->load();

    float array[100] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,390,391,392,393,394,395,396,397,398,399,400 };
    int control_0_place = floor(control * 100);
    fx.get<0>().state->setCutOffFrequency(lastSampleRate, 50 * array[control_0_place], static_cast<float> (1.0 / MathConstants<double>::sqrt2));

    dsp::Reverb::Parameters reverbParameters;
    float damping[100] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100 };
    int control_damping_place = floor(control * 100);
    reverbParameters.damping = damping[control_damping_place]/1000;

    float dryLevel[100] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100 };
    int control_dryLevel_place = floor(control * 100);
    reverbParameters.dryLevel = abs(dryLevel[control_damping_place]-100)/100*0.5;

    float roomSize[100] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100 };
    int control_roomSize_place = floor(control * 100);
    reverbParameters.roomSize = roomSize[control_damping_place] / 100;


    fx.get<1>().setParameters(reverbParameters);



}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{ 
    return new ImpressiveDropAudioProcessor();
}

