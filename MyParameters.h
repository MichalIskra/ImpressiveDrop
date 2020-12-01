/*
  ==============================================================================

    MyPrameters.h
    Created: 24 Nov 2020 5:54:59pm
    Author:  Michał

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MyParameters  : public juce::Component
{
public:
    MyParameters()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

    }

    ~MyParameters() override
    {
    }

    void paint (juce::Graphics& g) override
    {
    }

    void resized() override
    {
    }

    //==============================REVERB==========================================

        int get_rvrb_RoomSize(int controlParameter) {
            return rvrb_RoomSize[controlParameter];
        }
        int get_rvrb_Damping(int controlParameter) {
            return rvrb_Damping[controlParameter];
        }
        int get_rvrb_WetLevel(int controlParameter) {
            return rvrb_WetLevel[controlParameter];
        }
        int get_rvrb_DryLevel(int controlParameter) {
            return rvrb_DryLevel[controlParameter];
        }
        int get_rvrb_Width(int controlParameter) {
            return rvrb_WetLevel[controlParameter];
        }
   //==============================FILTER==========================================
        int get_fltr_Frequency(int controlParameter) {
            return rvrb_DryLevel[controlParameter];
        }
        int get_fltr_Resonance(int controlParameter) {
            return rvrb_WetLevel[controlParameter];
        }
    
private:

//==============================REVERB==========================================
        //Room size, 0 to 1.0, where 1.0 is big, 0 is small.
        int rvrb_RoomSize[100] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100 };
        //Damping, 0 to 1.0, where 0 is not damped, 1.0 is fully damped.
        int rvrb_Damping[100] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100 };
        //Wet level, 0 to 1.0.       
        int rvrb_WetLevel[2] = {2, 3};
        //Dry level, 0 to 1.0.
        int rvrb_DryLevel[2] = { 2, 3 };
        //	Reverb width, 0 to 1.0, where 1.0 is very wide.
        int rvrb_Width[2] = { 2, 3 };
//===============================FILTER============================================
        //=========parametry do setCutOffFrequency=============
        //Frequency
        int fltr_Frequency[100]={ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,390,391,392,393,394,395,396,397,398,399,400 };
        //Resonance
        int fltr_Resonance[100]= { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,390,391,392,393,394,395,396,397,398,399,400 };
        //
        int fltr_Drive[100] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,390,391,392,393,394,395,396,397,398,399,400 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyParameters)
};






