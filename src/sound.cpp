#include <iostream>
#include <fstream>
#include <cstdlib>

#include <AL/al.h>
#include <AL/alc.h>

#include "miniaudio.h"
#include "soundlib.h"

Soundlib::Sound::~Sound()
{
    alDeleteBuffers(1, &buffer_);
}

Soundlib::Sound::Sound()
{
    ALenum error;

    // Create buffer
    alGenBuffers(1, &buffer_);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        alDeleteBuffers(1, &buffer_);
        return;
    }
}

Soundlib::Sound::Sound(const std::string& filepath)
{
    ALenum error;

    // Create buffer
    alGenBuffers(1, &buffer_);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        alDeleteBuffers(1, &buffer_);
        return;
    }

    LoadSound(filepath);
}

Soundlib::Sound::Sound(const std::string& filepath, SoundFormat format, float sampleRate)
{
    ALenum error;

    // Create buffer
    alGenBuffers(1, &buffer_);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        alDeleteBuffers(1, &buffer_);
        return;
    }

    LoadSoundRaw(filepath, format, sampleRate);
}

void Soundlib::Sound::LoadSound(const std::string& filepath)
{
    ALenum error;
    
    ma_decoder decoder;
    ma_result result;
    ma_decoder_config config = ma_decoder_config_init(ma_format_s16, 0, 0);
    result = ma_decoder_init_file(filepath.c_str(), &config, &decoder);
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
        free(pcmData);
        return;
    }
    ma_decoder_uninit(&decoder);

    // Load audio data into buffer
    ALenum alFormat = (decoder.outputChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16; // Data was converted to signed 16-bit PCM (by sf_readf_short())
    alBufferData(buffer_, alFormat, (void*)pcmData, dataSize, decoder.outputSampleRate);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        alDeleteBuffers(1, &buffer_);
        free(pcmData);
        return;
    }

    free(pcmData);
}

void Soundlib::Sound::LoadSoundRaw(const std::string& filepath, SoundFormat format, float sampleRate)
{
    ALenum error;

    std::ifstream inFile(filepath, std::ios::ate | std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Could not open input file.");
    }

    inFile.seekg (0, inFile.end);
    int length = inFile.tellg();
    inFile.seekg (0, inFile.beg);

    char* pcmData = new char[length];
    inFile.read(reinterpret_cast<char*>(pcmData), length);

    ALenum alFormat;
    switch (format)
    {
        case SoundFormat::MONO8:   alFormat = AL_FORMAT_MONO8;
        case SoundFormat::MONO16:  alFormat = AL_FORMAT_MONO16;
        case SoundFormat::STEREO8: alFormat = AL_FORMAT_STEREO8;
        default:                   alFormat = AL_FORMAT_STEREO16;
    }
    alBufferData(buffer_, alFormat, (void*)pcmData, length, sampleRate);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        alDeleteBuffers(1, &buffer_);
        return;
    }
}
