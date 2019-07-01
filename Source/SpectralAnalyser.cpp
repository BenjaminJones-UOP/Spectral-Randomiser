/*
  ==============================================================================

    SpectralAnalyser.cpp
    Created: 3 Dec 2018 4:08:11pm
    Author:  asmprog

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SpectralAnalyser.h"

//==============================================================================
SpectralAnalyser::SpectralAnalyser(int numBins):
    m_totalBinNumber(numBins),
    m_currentAudioWritePosition(0),
    m_audioData(numBins),
    m_readyToUpdate(false),
    m_spectralData(numBins)
{

    startTimerHz(60);
}

SpectralAnalyser::~SpectralAnalyser()
{
}

void SpectralAnalyser::paint (Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    
    //Random r;
    //Colour col( r.nextInt(255),r.nextInt(255),r.nextInt(255)  );
    
    
    g.setColour (Colours::grey);
    g.drawRect(getLocalBounds(), 1);
    float halfBinNumber(m_totalBinNumber * 0.5);
    float binWidth = ((float)getWidth()/(float)halfBinNumber);
    float mag,mag2;
    float height = getHeight();
    
    
    g.setColour(Colours::white);
    
    AffineTransform t = AffineTransform::verticalFlip(height);
    g.addTransform(t);
    
    for (int bin = 0; bin < halfBinNumber -1; ++bin)
    {
        Complex current = m_spectralData[bin];
        Complex next = m_spectralData[bin+1];
        
        mag = sqrt( ( current.real() * current.real() ) + ( current.imag() * current.imag() ) );
        mag2 = sqrt( ( next.real() * next.real() ) + ( next.imag() * next.imag() ) );
        
        
        //g.fillRect(bin * binWidth, 0.0f, binWidth, mag*height);
        g.drawLine(bin * binWidth, mag*height, (bin+1)*binWidth, mag2*height);
        
    
    }
    
    //DBG("test");
    
    
    
    
}

void SpectralAnalyser::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void SpectralAnalyser::processSample(float sample)
{
    m_audioData[m_currentAudioWritePosition] = sample;
    
    ++m_currentAudioWritePosition;

    
    if (m_currentAudioWritePosition >= m_totalBinNumber)
    {
        m_readyToUpdate = true;
        
        m_currentAudioWritePosition = 0;
        
    }
    //DBG(m_currentAudioWritePosition);
    
}

void SpectralAnalyser::timerCallback()
{
    
    
    if (m_readyToUpdate == true)
    {
        //do some analysis here
        analyse();
        
        //repaint
        repaint();
        
        //set ready to update to false
        m_readyToUpdate = false;
        
    }
}


void SpectralAnalyser::analyse()
{
    for (int bin = 0; bin < m_totalBinNumber; ++bin)
    {
        Complex currentComplexNumber(0,0);
        float real, imag;
        
        
        for (int sample = 0; sample < m_totalBinNumber /2; ++sample)
        {
            //real = cos((bin * sample) / (float)m_totalBinNumber);
            //imag = (sin((bin * sample) / (float)m_totalBinNumber)) * -1.0f;
            
            real = m_audioData[sample] * cos(bin * sample / (float)m_totalBinNumber);
            imag = (m_audioData[sample] * sin(bin * sample / (float)m_totalBinNumber))*  -1.0f;
            
            currentComplexNumber += Complex(real, imag);
        }
        
        currentComplexNumber /= (float)m_totalBinNumber;
        
        m_spectralData[bin] = currentComplexNumber;
        
    }
    
    
    
    
    
}
