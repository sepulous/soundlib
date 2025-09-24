#include <iostream>
#include <cstdlib>

#include <AL/al.h>
#include <AL/alc.h>

#include "miniaudio.h"
#include "soundlib.h"

Soundlib::Sound::~Sound()
{
    alDeleteBuffers(1, &m_buffer);
}

Soundlib::Sound::Sound()
{
    ALenum error;

    // Create buffer
    alGenBuffers(1, &m_buffer);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        alDeleteBuffers(1, &m_buffer);
        return;
    }
}

Soundlib::Sound::Sound(const char* filepath)
{
    ALenum error;
    
    ma_decoder decoder;
    ma_result result;
    ma_decoder_config config = ma_decoder_config_init(ma_format_s16, 0, 0);
    result = ma_decoder_init_file(filepath, &config, &decoder);
    if (result != MA_SUCCESS)
    {
        std::cerr << "Failed to open file: " << filepath << "\n";
        return;
    }

    ma_uint64 frameCount;
    ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount);

    int dataSize = frameCount * decoder.outputChannels * sizeof(short);
    short* pcmData = (short*)malloc(dataSize);

    ma_uint64 framesRead;
    ma_decoder_read_pcm_frames(&decoder, (void*)pcmData, frameCount, &framesRead);
    if (framesRead != frameCount)
    {
        std::cerr << "Failed to read all frames.\n";
        ma_decoder_uninit(&decoder);
        return;
    }
    ma_decoder_uninit(&decoder);

    // Create buffer
    alGenBuffers(1, &m_buffer);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        alDeleteBuffers(1, &m_buffer);
        return;
    }

    // Load audio data into buffer
    ALenum alFormat = (decoder.outputChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16; // Data was converted to signed 16-bit PCM (by sf_readf_short())
    alBufferData(m_buffer, alFormat, (void*)pcmData, dataSize, decoder.outputSampleRate);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        alDeleteBuffers(1, &m_buffer);
        return;
    }
}

void Soundlib::Sound::LoadSound(const char* filepath)
{
    ALenum error;
    
    ma_decoder decoder;
    ma_result result;
    ma_decoder_config config = ma_decoder_config_init(ma_format_s16, 0, 0);
    result = ma_decoder_init_file(filepath, &config, &decoder);
    if (result != MA_SUCCESS)
    {
        std::cerr << "Failed to open file: " << filepath << "\n";
        return;
    }

    ma_uint64 frameCount;
    ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount);

    int dataSize = frameCount * decoder.outputChannels * sizeof(short);
    short* pcmData = (short*)malloc(dataSize);

    ma_uint64 framesRead;
    ma_decoder_read_pcm_frames(&decoder, (void*)pcmData, frameCount, &framesRead);
    if (framesRead != frameCount)
    {
        std::cerr << "Failed to read all frames.\n";
        ma_decoder_uninit(&decoder);
        return;
    }
    ma_decoder_uninit(&decoder);

    // Load audio data into buffer
    ALenum alFormat = (decoder.outputChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16; // Data was converted to signed 16-bit PCM (by sf_readf_short())
    alBufferData(m_buffer, alFormat, (void*)pcmData, dataSize, decoder.outputSampleRate);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        alDeleteBuffers(1, &m_buffer);
        return;
    }
}
