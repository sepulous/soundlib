
#include <AL/al.h>
#include <AL/alc.h>

#include "soundlib.h"

using namespace Soundlib;

Soundlib::SoundSource::~SoundSource()
{
    alDeleteSources(1, &source_);
}

Soundlib::SoundSource::SoundSource()
{
    // Create source
    alGenSources(1, &source_);
    if (alGetError() != AL_NO_ERROR)
        error_ = Soundlib::Error::SOURCE_CREATE_FAIL;
}

Soundlib::SoundSource::SoundSource(const Sound& sound)
{
    // Create source
    alGenSources(1, &source_);
    if (alGetError() != AL_NO_ERROR)
    {
        error_ = Soundlib::Error::SOURCE_CREATE_FAIL;
        return;
    }

    // Bind buffer to source
    alSourcei(source_, AL_BUFFER, sound.buffer_);
    if (alGetError() != AL_NO_ERROR)
        error_ = Soundlib::Error::SOURCE_BIND_FAIL;
}

void Soundlib::SoundSource::SetSound(const Sound& sound) noexcept
{
    // Bind buffer to source
    alSourcei(source_, AL_BUFFER, sound.buffer_);
    if (alGetError() != AL_NO_ERROR)
        error_ = Soundlib::Error::SOURCE_BIND_FAIL;
}

void Soundlib::SoundSource::Play() noexcept
{
    alSourcePlay(source_);
}

void Soundlib::SoundSource::Pause() noexcept
{
    alSourcePause(source_);
}

void Soundlib::SoundSource::Stop() noexcept
{
    alSourceStop(source_);
}

Soundlib::Error Soundlib::SoundSource::GetError() noexcept
{
    return error_;
}

SourceState Soundlib::SoundSource::GetState() noexcept
{
    ALint state;
    alGetSourcei(source_, AL_SOURCE_STATE, &state);
    switch (state)
    {
        case AL_INITIAL: return SourceState::INITIAL;
        case AL_PLAYING: return SourceState::PLAYING;
        case AL_PAUSED:  return SourceState::PAUSED;
        case AL_STOPPED: return SourceState::STOPPED;
        default:         return SourceState::INITIAL;
    }
}

float Soundlib::SoundSource::GetGain() noexcept
{
    ALfloat gain;
    alGetSourcef(source_, AL_GAIN, &gain);
    return gain;
}

void Soundlib::SoundSource::SetGain(float gain) noexcept
{
    alSourcef(source_, AL_GAIN, gain);
}

float Soundlib::SoundSource::GetMinGain() noexcept
{
    ALfloat min_gain;
    alGetSourcef(source_, AL_MIN_GAIN, &min_gain);
    return min_gain;
}

void Soundlib::SoundSource::SetMinGain(float min_gain) noexcept
{
    alSourcef(source_, AL_MIN_GAIN, min_gain);
}

float Soundlib::SoundSource::GetMaxGain() noexcept
{
    ALfloat max_gain;
    alGetSourcef(source_, AL_MAX_GAIN, &max_gain);
    return max_gain;
}

void Soundlib::SoundSource::SetMaxGain(float max_gain) noexcept
{
    alSourcef(source_, AL_MAX_GAIN, max_gain);
}

float Soundlib::SoundSource::GetMaxDistance() noexcept
{
    ALfloat max_distance;
    alGetSourcef(source_, AL_MAX_DISTANCE, &max_distance);
    return max_distance;
}

void Soundlib::SoundSource::SetMaxDistance(float max_distance) noexcept
{
    alSourcef(source_, AL_MAX_DISTANCE, max_distance);
}

float Soundlib::SoundSource::GetReferenceDistance() noexcept
{
    ALfloat ref_distance;
    alGetSourcef(source_, AL_REFERENCE_DISTANCE, &ref_distance);
    return ref_distance;
}

void Soundlib::SoundSource::SetReferenceDistance(float ref_distance) noexcept
{
    alSourcef(source_, AL_REFERENCE_DISTANCE, ref_distance);
}

float Soundlib::SoundSource::GetRolloffFactor() noexcept
{
    ALfloat rolloff_factor;
    alGetSourcef(source_, AL_ROLLOFF_FACTOR, &rolloff_factor);
    return rolloff_factor;
}

void Soundlib::SoundSource::SetRolloffFactor(float rolloff_factor) noexcept
{
    alSourcef(source_, AL_ROLLOFF_FACTOR, rolloff_factor);
}

float Soundlib::SoundSource::GetConeInnerAngle() noexcept
{
    ALfloat inner_angle;
    alGetSourcef(source_, AL_CONE_INNER_ANGLE, &inner_angle);
    return inner_angle;
}

void Soundlib::SoundSource::SetConeInnerAngle(float inner_angle) noexcept
{
    alSourcef(source_, AL_CONE_INNER_ANGLE, inner_angle);
}

float Soundlib::SoundSource::GetConeOuterAngle() noexcept
{
    ALfloat outer_angle;
    alGetSourcef(source_, AL_CONE_OUTER_ANGLE, &outer_angle);
    return outer_angle;
}

void Soundlib::SoundSource::SetConeOuterAngle(float outer_angle) noexcept
{
    alSourcef(source_, AL_CONE_OUTER_ANGLE, outer_angle);
}

float Soundlib::SoundSource::GetConeOuterGain() noexcept
{
    ALfloat outer_gain;
    alGetSourcef(source_, AL_CONE_OUTER_GAIN, &outer_gain);
    return outer_gain;
}

void Soundlib::SoundSource::SetConeOuterGain(float outer_gain) noexcept
{
    alSourcef(source_, AL_CONE_OUTER_GAIN, outer_gain);
}

float Soundlib::SoundSource::GetPitch() noexcept
{
    ALfloat pitch;
    alGetSourcef(source_, AL_PITCH, &pitch);
    return pitch;
}

void Soundlib::SoundSource::SetPitch(float pitch) noexcept
{
    alSourcef(source_, AL_PITCH, pitch);
}

Vector3 Soundlib::SoundSource::GetPosition() noexcept
{
    Vector3 position;
    alGetSourcefv(source_, AL_POSITION, (ALfloat *)&position);
    return position;
}

void Soundlib::SoundSource::SetPosition(float x, float y, float z) noexcept
{
    alSource3f(source_, AL_POSITION, x, y, z);
}

void Soundlib::SoundSource::SetPosition(Vector3 position) noexcept
{
    alSourcefv(source_, AL_POSITION, (ALfloat *)&position);
}

Vector3 Soundlib::SoundSource::GetVelocity() noexcept
{
    Vector3 velocity;
    alGetSourcefv(source_, AL_VELOCITY, (ALfloat *)&velocity);
    return velocity;
}

void Soundlib::SoundSource::SetVelocity(float x, float y, float z) noexcept
{
    alSource3f(source_, AL_VELOCITY, x, y, z);
}

void Soundlib::SoundSource::SetVelocity(Vector3 velocity) noexcept
{
    alSourcefv(source_, AL_VELOCITY, (ALfloat *)&velocity);
}

Vector3 Soundlib::SoundSource::GetDirection() noexcept
{
    Vector3 direction;
    alGetSourcefv(source_, AL_DIRECTION, (ALfloat *)&direction);
    return direction;
}

void Soundlib::SoundSource::SetDirection(float x, float y, float z) noexcept
{
    alSource3f(source_, AL_DIRECTION, x, y, z);
}

void Soundlib::SoundSource::SetDirection(Vector3 direction) noexcept
{
    alSourcefv(source_, AL_DIRECTION, (ALfloat *)&direction);
}

bool Soundlib::SoundSource::IsLooping() noexcept
{
    ALint looping;
    alGetSourcei(source_, AL_LOOPING, &looping);
    return looping == 1;
}

void Soundlib::SoundSource::SetLooping(bool loop) noexcept
{
    alSourcei(source_, AL_LOOPING, loop ? 1 : 0);
}
