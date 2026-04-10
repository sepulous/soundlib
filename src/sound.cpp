
#include <fstream>
#include <cstdlib>

#include <AL/al.h>
#include <AL/alc.h>

#include "miniaudio.h"
#include "soundlib.h"

Soundlib::Sound::~Sound()
{
    if (buffer_ != AL_NONE)
        alDeleteBuffers(1, &buffer_);
}

Soundlib::Sound::Sound()
{
    // Create buffer
    alGenBuffers(1, &buffer_);
    if (alGetError() != AL_NO_ERROR)
    {
        error_ = Soundlib::Error::BUFFER_CREATE_FAIL;
        buffer_ = AL_NONE;
    }
}

Soundlib::Sound::Sound(const std::string& filepath)
{
    // Create buffer
    alGenBuffers(1, &buffer_);
    if (alGetError() != AL_NO_ERROR)
    {
        error_ = Soundlib::Error::BUFFER_CREATE_FAIL;
        buffer_ = AL_NONE;
        return;
    }

    LoadSound(filepath);
}

Soundlib::Sound::Sound(const std::string& filepath, SoundFormat format, float sample_rate)
{
    // Create buffer
    alGenBuffers(1, &buffer_);
    if (alGetError() != AL_NO_ERROR)
    {
        error_ = Soundlib::Error::BUFFER_CREATE_FAIL;
        buffer_ = AL_NONE;
        return;
    }

    LoadSoundRaw(filepath, format, sample_rate);
}

void Soundlib::Sound::LoadSound(const std::string& filepath)
{
    ma_decoder decoder;
    ma_decoder_config config = ma_decoder_config_init(ma_format_s16, 0, 0);
    ma_result result = ma_decoder_init_file(filepath.c_str(), &config, &decoder);
    if (result != MA_SUCCESS)
    {
        error_ = Soundlib::Error::FILE_OPEN_FAIL;
        return;
    }

    ma_uint64 frame_count;
    ma_decoder_get_length_in_pcm_frames(&decoder, &frame_count);

    size_t data_size = frame_count * decoder.outputChannels * sizeof(short);
    short *pcm_data = (short *)malloc(data_size);

    ma_uint64 frames_read;
    ma_decoder_read_pcm_frames(&decoder, (void *)pcm_data, frame_count, &frames_read);
    if (frames_read != frame_count)
    {
        error_ = Soundlib::Error::FILE_READ_FAIL;
        ma_decoder_uninit(&decoder);
        free(pcm_data);
        return;
    }

    // Load audio data into buffer
    ALenum al_format = (decoder.outputChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16; // Data was converted to signed 16-bit PCM (by sf_readf_short())
    alBufferData(buffer_, al_format, (void *)pcm_data, data_size, decoder.outputSampleRate);

    if (alGetError() != AL_NO_ERROR)
        error_ = Soundlib::Error::BUFFER_UPLOAD_FAIL;

    ma_decoder_uninit(&decoder);
    free(pcm_data);
}

void Soundlib::Sound::LoadSoundRaw(const std::string& filepath, SoundFormat format, float sample_rate)
{
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);
    if (!file)
    {
        error_ = Soundlib::Error::FILE_OPEN_FAIL;
        return;
    }

    file.seekg(0, file.end);
    size_t length = file.tellg();
    file.seekg(0, file.beg);

    char *pcm_data = (char *)malloc(length);
    file.read(pcm_data, length);
    file.close();

    ALenum al_format;
    switch (format)
    {
        case SoundFormat::MONO8:   al_format = AL_FORMAT_MONO8; break;
        case SoundFormat::MONO16:  al_format = AL_FORMAT_MONO16; break;
        case SoundFormat::STEREO8: al_format = AL_FORMAT_STEREO8; break;
        default:                   al_format = AL_FORMAT_STEREO16; break;
    }
    alBufferData(buffer_, al_format, (void *)pcm_data, length, sample_rate);

    if (alGetError() != AL_NO_ERROR)
        error_ = Soundlib::Error::BUFFER_UPLOAD_FAIL;

    free(pcm_data);
}

Soundlib::Error Soundlib::Sound::GetError() noexcept
{
    return error_;
}
