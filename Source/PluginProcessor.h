/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "BinScrambler.h"
class ModelViewController;

//==============================================================================
/**
*/
class SpectralRandomiserAudioProcessor  : public AudioProcessor,
                                          public ChangeListener
{
public:
    //==============================================================================
    SpectralRandomiserAudioProcessor();
    ~SpectralRandomiserAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;



    
    //change listener functions
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void openButtonClicked();
    

private:
    AudioFormatManager m_formatManager;
    std::unique_ptr<AudioFormatReaderSource> m_readerSource;
    AudioTransportSource m_transportSource;
    
    AudioSampleBuffer currentFileBuffer;
    
    BinScrambler m_testScramble;
    bool doOnce = true;
    bool m_isPlaying;
	bool m_canProcess;
	bool m_isProcessed;
    bool m_playOriginal;
    
    friend class ModelViewController;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralRandomiserAudioProcessor)
};
