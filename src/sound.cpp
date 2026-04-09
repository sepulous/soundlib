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

Soundlib::Sound::Sound(const std::string& filepath, SoundFormat format, float sample_rate)
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

    LoadSoundRaw(filepath, format, sample_rate);
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

    ma_uint64 frame_count;
    ma_decoder_get_length_in_pcm_frames(&decoder, &frame_count);

    int data_size = frame_count * decoder.outputChannels * sizeof(short);
    short *pcm_data = (short *)malloc(data_size);

    ma_uint64 frames_read;
    ma_decoder_read_pcm_frames(&decoder, (void *)pcm_data, frame_count, &frames_read);
    if (frames_read != frame_count)
    {
        std::cerr << "Failed to read all frames.\n";
        ma_decoder_uninit(&decoder);
        free(pcm_data);
        return;
    }
    ma_decoder_uninit(&decoder);

    // Load audio data into buffer
    ALenum al_format = (decoder.outputChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16; // Data was converted to signed 16-bit PCM (by sf_readf_short())
    alBufferData(buffer_, al_format, (void *)pcm_data, data_size, decoder.outputSampleRate);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        alDeleteBuffers(1, &buffer_);
    }

    free(pcm_data);
}

void Soundlib::Sound::LoadSoundRaw(const std::string& filepath, SoundFormat format, float sample_rate)
{
    ALenum error;

    std::ifstream file(filepath, std::ios::ate | std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open input file.");
    }

    file.seekg (0, file.end);
    int length = file.tellg();
    file.seekg (0, file.beg);

    char *pcm_data = new char[length];
    file.read(reinterpret_cast<char *>(pcm_data), length);

    ALenum al_format;
    switch (format)
    {
        case SoundFormat::MONO8:   al_format = AL_FORMAT_MONO8;
        case SoundFormat::MONO16:  al_format = AL_FORMAT_MONO16;
        case SoundFormat::STEREO8: al_format = AL_FORMAT_STEREO8;
        default:                   al_format = AL_FORMAT_STEREO16;
    }
    alBufferData(buffer_, al_format, (void *)pcm_data, length, sample_rate);

    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        alDeleteBuffers(1, &buffer_);
    }

    free(pcm_data);
}
