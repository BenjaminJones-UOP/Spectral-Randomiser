/*
  ==============================================================================

    BinScrambler.h
    Created: 3 Apr 2019 5:31:15pm
    Author:  asmprog

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//=================================================================================

class BinScrambler
{
	dsp::FFT perform_fft;

	static constexpr int fft_order = 10;
	static constexpr int fft_size = 1 << fft_order;

	std::vector<std::shared_ptr<float>> processed_buffer;

public:

	BinScrambler(float fftOrder) :
		perform_fft(fftOrder)
	{
	}

	~BinScrambler()
	{
	}

	void process(AudioBuffer<float>& bufferFill)
	{
		std::vector<float> buffer_data; //Data from audio file
		std::vector<float> fft_data; //copy buffer to this for processing

		int buffer_size = bufferFill.getNumSamples();

		for (int i = 0; i < buffer_size; i++) //Fill vector(s) with nothing (set up size)
		{
			buffer_data.push_back(0.0);
		}

		for (int i = 0; i < buffer_size * 2; i++)
		{
			fft_data.push_back(0.0);
		}

		for (int i = 0; i < buffer_size; i++)
		{
			processed_buffer.push_back(std::make_shared<float>(0.0));
		}

		for (int i = 0; i < buffer_size; i++) //Fill buffer data vector with the audio buffer
		{
			buffer_data[i] = *bufferFill.getReadPointer(0, i);
		}

		memcpy(fft_data.data(), buffer_data.data(), sizeof(fft_data.size()));
		perform_fft.performRealOnlyForwardTransform(fft_data.data());

		//Do processing/binscrambling here

		perform_fft.performRealOnlyInverseTransform(fft_data.data());
		memcpy(processed_buffer.data(), fft_data.data(), sizeof(processed_buffer.size()));

	};

	AudioSourceChannelInfo* getProcessedBuffer() 
	{
		AudioSourceChannelInfo* buffer;
		AudioBuffer<float> tempBuffer(2, fft_size);

		for (int i = 0; i < fft_size; i++) {
			tempBuffer.addSample(0, i, *processed_buffer[i]);
			//std::cout << "Line number in processing buffer = " << i << std::endl;
		}

		buffer = new AudioSourceChannelInfo(&tempBuffer, 0, tempBuffer.getNumSamples());

		return buffer;
	}
};
