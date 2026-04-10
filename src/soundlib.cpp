
#include <string>
#include <sstream>
#include <cstdlib>

#include <AL/al.h>
#include <AL/alc.h>

#include "soundlib.h"

using namespace Soundlib;

bool Soundlib::Init()
{
    ALCdevice *device = alcOpenDevice(NULL);
    if (device)
    {
        ALCcontext *context = alcCreateContext(device, NULL);
        alcMakeContextCurrent(context);
        return true;
    }
    return false;
}

bool Soundlib::Init(const std::string& device_name)
{
    ALCdevice *device = alcOpenDevice(device_name.c_str());
    if (device)
    {
        ALCcontext *context = alcCreateContext(device, NULL);
        alcMakeContextCurrent(context);
        return true;
    }
    return false;
}

// This doesn't strictly need to be called at the end of the program, but some OpenAL implementations will complain if you don't
void Soundlib::Exit()
{
    ALCcontext *context = alcGetCurrentContext();
    ALCdevice *device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

std::vector<std::string> Soundlib::GetDeviceList()
{
    std::vector<std::string> device_list;

    if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") == AL_TRUE)
    {
        const ALCchar *alc_devices = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER); // Device names are separated by '\0', and this string ends with "\0\0"
        std::stringstream device;
        while (true)
        {
            if (*alc_devices != '\0')
            {
                device << *alc_devices;
            }
            else
            {
                device_list.push_back(device.str());
                device.str("");
                device.clear();
                if (*(alc_devices + 1) == '\0')
                    break;
            }
            alc_devices++;
        }
    }

    return device_list;
}

AttenuationModel Soundlib::GetAttenuationModel() noexcept
{
    ALint attenuation_model = alGetInteger(AL_DISTANCE_MODEL);
    switch (attenuation_model)
    {
        case AL_INVERSE_DISTANCE:          return AttenuationModel::INVERSE_DISTANCE;
        case AL_LINEAR_DISTANCE:           return AttenuationModel::LINEAR_DISTANCE;
        case AL_LINEAR_DISTANCE_CLAMPED:   return AttenuationModel::LINEAR_DISTANCE_CLAMPED;
        case AL_EXPONENT_DISTANCE:         return AttenuationModel::EXPONENT_DISTANCE;
        case AL_EXPONENT_DISTANCE_CLAMPED: return AttenuationModel::EXPONENT_DISTANCE_CLAMPED;
        default:                           return AttenuationModel::INVERSE_DISTANCE_CLAMPED;
    }
}

void Soundlib::SetAttenuationModel(AttenuationModel attenuation_model) noexcept
{
    switch (attenuation_model)
    {
        case AttenuationModel::INVERSE_DISTANCE:          alDistanceModel(AL_INVERSE_DISTANCE); break;
        case AttenuationModel::LINEAR_DISTANCE:           alDistanceModel(AL_LINEAR_DISTANCE); break;
        case AttenuationModel::LINEAR_DISTANCE_CLAMPED:   alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED); break;
        case AttenuationModel::EXPONENT_DISTANCE:         alDistanceModel(AL_EXPONENT_DISTANCE); break;
        case AttenuationModel::EXPONENT_DISTANCE_CLAMPED: alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED); break;
        default:                                          alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED); break;
    }
}

float Soundlib::GetDopplerFactor() noexcept
{
    return alGetFloat(AL_DOPPLER_FACTOR);
}

void Soundlib::SetDopplerFactor(float factor) noexcept
{
    alDopplerFactor(factor);
}

float Soundlib::GetSpeedOfSound() noexcept
{
    return alGetFloat(AL_SPEED_OF_SOUND);
}

void Soundlib::SetSpeedOfSound(float speed) noexcept
{
    alSpeedOfSound(speed);
}

void Soundlib::SetListenerVolume(float volume) noexcept
{
    alListenerf(AL_GAIN, volume);
}

float Soundlib::GetListenerVolume() noexcept
{
    ALfloat volume;
    alGetListenerf(AL_GAIN, &volume);
    return volume;
}

void Soundlib::SetListenerPosition(float x, float y, float z) noexcept
{
    alListener3f(AL_POSITION, x, y, z);
}

void Soundlib::SetListenerPosition(Vector3 position) noexcept
{
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

Vector3 Soundlib::GetListenerPosition() noexcept
{
    Vector3 position;
    alGetListener3f(AL_POSITION, &position.x, &position.y, &position.z);
    return position;
}

void Soundlib::SetListenerVelocity(float x, float y, float z) noexcept
{
    alListener3f(AL_VELOCITY, x, y, z);
}

void Soundlib::SetListenerVelocity(Vector3 velocity) noexcept
{
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

Vector3 Soundlib::GetListenerVelocity() noexcept
{
    Vector3 velocity;
    alGetListener3f(AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z);
    return velocity;
}

void Soundlib::SetListenerOrientation(float forward_x, float forward_y, float forward_z, float up_x, float up_y, float up_z) noexcept
{
    ALfloat values[6] = {
        forward_x, forward_y, forward_z,
        up_x, up_y, up_z
    };
    alListenerfv(AL_ORIENTATION, values);
}

void Soundlib::SetListenerOrientation(Vector3 forward, Vector3 up) noexcept
{
    ALfloat values[6] = {
        forward.x, forward.y, forward.z,
        up.x, up.y, up.z
    };
    alListenerfv(AL_ORIENTATION, values);
}

void Soundlib::SetListenerOrientation(Orientation o) noexcept
{
    alListenerfv(AL_ORIENTATION, (ALfloat*)&o);
}

Orientation Soundlib::GetListenerOrientation() noexcept
{
    Orientation o;
    alGetListenerfv(AL_ORIENTATION, (ALfloat*)&o);
    return o;
}
