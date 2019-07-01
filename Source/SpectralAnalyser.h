/*
  ==============================================================================

    SpectralAnalyser.h
    Created: 3 Dec 2018 4:08:11pm
    Author:  asmprog

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <complex>
#include <math.h>

typedef std::vector<float> AudioVector;
typedef std::complex<float> Complex;
typedef std::vector<Complex> ComplexVector;


//==============================================================================
/*
*/
class SpectralAnalyser    : public Component, public Timer
{
public:
    SpectralAnalyser(int numBins = 512);
    ~SpectralAnalyser();

    void paint (Graphics&) override;
    void resized() override;
    void processSample(float sample);
    void timerCallback() override;
    void analyse();

private:
    int m_totalBinNumber, m_currentAudioWritePosition;
    AudioVector m_audioData;
    bool m_readyToUpdate;
    
    ComplexVector m_spectralData;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralAnalyser)
};
