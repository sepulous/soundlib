#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

#include "soundlib.h"

using namespace Soundlib;

Soundlib::SoundSource::~SoundSource()
{
    alDeleteSources(1, &m_source);
}

Soundlib::SoundSource::SoundSource()
{
    ALenum error;

    // Create source
    alGenSources(1, &m_source);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        return;
    }
}

Soundlib::SoundSource::SoundSource(const Sound& sound)
{
    ALenum error;

    // Create source
    alGenSources(1, &m_source);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        return;
    }

    // Bind buffer to source
    alSourcei(m_source, AL_BUFFER, sound.m_buffer);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        return;
    }
}

void Soundlib::SoundSource::SetSound(const Sound& sound) noexcept
{
    ALenum error;

    // Bind buffer to source
    alSourcei(m_source, AL_BUFFER, sound.m_buffer);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        std::cerr << "OpenAL ERROR: " << error << std::endl;
        return;
    }
}

void Soundlib::SoundSource::Play() noexcept
{
    alSourcePlay(m_source);
}

void Soundlib::SoundSource::Pause() noexcept
{
    alSourcePause(m_source);
}

void Soundlib::SoundSource::Stop() noexcept
{
    alSourceStop(m_source);
}

SourceState Soundlib::SoundSource::GetState() noexcept
{
    ALint state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
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
    alGetSourcef(m_source, AL_GAIN, &gain);
    return gain;
}

void Soundlib::SoundSource::SetGain(float gain) noexcept
{
    alSourcef(m_source, AL_GAIN, gain);
}

float Soundlib::SoundSource::GetMinGain() noexcept
{
    ALfloat minGain;
    alGetSourcef(m_source, AL_MIN_GAIN, &minGain);
    return minGain;
}

void Soundlib::SoundSource::SetMinGain(float minGain) noexcept
{
    alSourcef(m_source, AL_MIN_GAIN, minGain);
}

float Soundlib::SoundSource::GetMaxGain() noexcept
{
    ALfloat maxGain;
    alGetSourcef(m_source, AL_MAX_GAIN, &maxGain);
    return maxGain;
}

void Soundlib::SoundSource::SetMaxGain(float maxGain) noexcept
{
    alSourcef(m_source, AL_MAX_GAIN, maxGain);
}

float Soundlib::SoundSource::GetMaxDistance() noexcept
{
    ALfloat maxDistance;
    alGetSourcef(m_source, AL_MAX_DISTANCE, &maxDistance);
    return maxDistance;
}

void Soundlib::SoundSource::SetMaxDistance(float maxDistance) noexcept
{
    alSourcef(m_source, AL_MAX_DISTANCE, maxDistance);
}

float Soundlib::SoundSource::GetReferenceDistance() noexcept
{
    ALfloat refDistance;
    alGetSourcef(m_source, AL_REFERENCE_DISTANCE, &refDistance);
    return refDistance;
}

void Soundlib::SoundSource::SetReferenceDistance(float refDistance) noexcept
{
    alSourcef(m_source, AL_REFERENCE_DISTANCE, refDistance);
}

float Soundlib::SoundSource::GetRolloffFactor() noexcept
{
    ALfloat rolloffFactor;
    alGetSourcef(m_source, AL_ROLLOFF_FACTOR, &rolloffFactor);
    return rolloffFactor;
}

void Soundlib::SoundSource::SetRolloffFactor(float rolloffFactor) noexcept
{
    alSourcef(m_source, AL_ROLLOFF_FACTOR, rolloffFactor);
}

float Soundlib::SoundSource::GetConeInnerAngle() noexcept
{
    ALfloat innerAngle;
    alGetSourcef(m_source, AL_CONE_INNER_ANGLE, &innerAngle);
    return innerAngle;
}

void Soundlib::SoundSource::SetConeInnerAngle(float innerAngle) noexcept
{
    alSourcef(m_source, AL_CONE_INNER_ANGLE, innerAngle);
}

float Soundlib::SoundSource::GetConeOuterAngle() noexcept
{
    ALfloat outerAngle;
    alGetSourcef(m_source, AL_CONE_OUTER_ANGLE, &outerAngle);
    return outerAngle;
}

void Soundlib::SoundSource::SetConeOuterAngle(float outerAngle) noexcept
{
    alSourcef(m_source, AL_CONE_OUTER_ANGLE, outerAngle);
}

float Soundlib::SoundSource::GetConeOuterGain() noexcept
{
    ALfloat outerGain;
    alGetSourcef(m_source, AL_CONE_OUTER_GAIN, &outerGain);
    return outerGain;
}

void Soundlib::SoundSource::SetConeOuterGain(float outerGain) noexcept
{
    alSourcef(m_source, AL_CONE_OUTER_GAIN, outerGain);
}

float Soundlib::SoundSource::GetPitch() noexcept
{
    ALfloat pitch;
    alGetSourcef(m_source, AL_PITCH, &pitch);
    return pitch;
}

void Soundlib::SoundSource::SetPitch(float pitch) noexcept
{
    alSourcef(m_source, AL_PITCH, pitch);
}

Vector3 Soundlib::SoundSource::GetPosition() noexcept
{
    Vector3 position;
    alGetSourcefv(m_source, AL_POSITION, (ALfloat*)&position);
    return position;
}

void Soundlib::SoundSource::SetPosition(float x, float y, float z) noexcept
{
    alSource3f(m_source, AL_POSITION, x, y, z);
}

void Soundlib::SoundSource::SetPosition(Vector3 position) noexcept
{
    alSourcefv(m_source, AL_POSITION, (ALfloat*)&position);
}

Vector3 Soundlib::SoundSource::GetVelocity() noexcept
{
    Vector3 velocity;
    alGetSourcefv(m_source, AL_VELOCITY, (ALfloat*)&velocity);
    return velocity;
}

void Soundlib::SoundSource::SetVelocity(float x, float y, float z) noexcept
{
    alSource3f(m_source, AL_VELOCITY, x, y, z);
}

void Soundlib::SoundSource::SetVelocity(Vector3 velocity) noexcept
{
    alSourcefv(m_source, AL_VELOCITY, (ALfloat*)&velocity);
}

Vector3 Soundlib::SoundSource::GetDirection() noexcept
{
    Vector3 direction;
    alGetSourcefv(m_source, AL_DIRECTION, (ALfloat*)&direction);
    return direction;
}

void Soundlib::SoundSource::SetDirection(float x, float y, float z) noexcept
{
    alSource3f(m_source, AL_DIRECTION, x, y, z);
}

void Soundlib::SoundSource::SetDirection(Vector3 direction) noexcept
{
    alSourcefv(m_source, AL_DIRECTION, (ALfloat*)&direction);
}

bool Soundlib::SoundSource::IsLooping() noexcept
{
    ALint looping;
    alGetSourcei(m_source, AL_LOOPING, &looping);
    return looping == 1;
}

void Soundlib::SoundSource::SetLooping(bool loop) noexcept
{
    alSourcei(m_source, AL_LOOPING, loop ? 1 : 0);
}
