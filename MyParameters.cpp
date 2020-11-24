/*
  ==============================================================================

    Parameters.cpp
    Created: 21 Nov 2020 6:55:55pm
    Author:  Michał

  ==============================================================================
*/

#include "MyParameters.h"
MyParameters::MyParameters(){


}
MyParameters::~MyParameters() {
}

void MyParameters::paint(juce::Graphics&)
{
}

void MyParameters::resized()
{
}

float MyParameters::get_rvrb_RoomSize(int controlParameter)
{
    return rvrb_RoomSize[controlParameter];
}

float MyParameters::get_rvrb_Damping(int controlParameter)
{
    return rvrb_Damping[controlParameter];
}

float MyParameters::get_rvrb_WetLevel(int controlParameter)
{
    return rvrb_WetLevel[controlParameter];
}

float MyParameters::get_rvrb_DryLevel(int controlParameter)
{
    return rvrb_DryLevel[controlParameter];
}

float MyParameters::get_rvrb_Width(int controlParameter)
{
    return rvrb_WetLevel[controlParameter];
}


