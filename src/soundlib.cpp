#include <iostream>
#include <sstream>
#include <cstdlib>

#include <AL/al.h>
#include <AL/alc.h>

#include "soundlib.h"

using namespace Soundlib;

bool Soundlib::Initialize()
{
    ALCdevice* device = alcOpenDevice(nullptr);
    if (device)
    {
        ALCcontext* context = alcCreateContext(device, nullptr);
        alcMakeContextCurrent(context);
        return true;
    }
    return false;
}

bool Soundlib::Initialize(std::string deviceName)
{
    ALCdevice* device = alcOpenDevice(deviceName.c_str());
    if (device)
    {
        ALCcontext* context = alcCreateContext(device, nullptr);
        alcMakeContextCurrent(context);
        return true;
    }
    return false;
}

std::vector<std::string> Soundlib::GetDeviceList()
{
    if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") == AL_TRUE)
    {
        const ALCchar* alcDevicesStr = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER); // Device names are separated by '\0', and this string ends with "\0\0"
        std::vector<std::string> deviceList;
        std::stringstream device;
        while (true)
        {
            if (*alcDevicesStr != '\0')
            {
                device << *alcDevicesStr;
            }
            else
            {
                deviceList.push_back(device.str());
                device.str("");
                device.clear();
                if (*(alcDevicesStr + 1) == '\0')
                    break;
            }
            alcDevicesStr++;
        }
        return deviceList;
    }
    else
    {
        std::vector<std::string> empty;
        return empty;
    }
}

DistanceModel Soundlib::GetDistanceModel()
{
    ALint distanceModel = alGetInteger(AL_DISTANCE_MODEL);
    switch (distanceModel)
    {
        case AL_INVERSE_DISTANCE:          return Soundlib::DistanceModel::INVERSE_DISTANCE;
        case AL_INVERSE_DISTANCE_CLAMPED:  return Soundlib::DistanceModel::INVERSE_DISTANCE_CLAMPED;
        case AL_LINEAR_DISTANCE:           return Soundlib::DistanceModel::LINEAR_DISTANCE;
        case AL_LINEAR_DISTANCE_CLAMPED:   return Soundlib::DistanceModel::LINEAR_DISTANCE_CLAMPED;
        case AL_EXPONENT_DISTANCE:         return Soundlib::DistanceModel::EXPONENT_DISTANCE;
        case AL_EXPONENT_DISTANCE_CLAMPED: return Soundlib::DistanceModel::EXPONENT_DISTANCE_CLAMPED;
        default:                           return Soundlib::DistanceModel::INVERSE_DISTANCE_CLAMPED;
    }
}

void Soundlib::SetDistanceModel(DistanceModel distanceModel)
{
    switch (distanceModel)
    {
        case DistanceModel::INVERSE_DISTANCE:          alDistanceModel(AL_INVERSE_DISTANCE);
        case DistanceModel::INVERSE_DISTANCE_CLAMPED:  alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
        case DistanceModel::LINEAR_DISTANCE:           alDistanceModel(AL_LINEAR_DISTANCE);
        case DistanceModel::LINEAR_DISTANCE_CLAMPED:   alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
        case DistanceModel::EXPONENT_DISTANCE:         alDistanceModel(AL_EXPONENT_DISTANCE);
        case DistanceModel::EXPONENT_DISTANCE_CLAMPED: alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED);
        default:                                       alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    }
}

float Soundlib::GetDopplerFactor()
{
    return alGetFloat(AL_DOPPLER_FACTOR);
}

void Soundlib::SetDopplerFactor(float factor)
{
    alDopplerFactor(factor);
}

float Soundlib::GetSpeedOfSound()
{
    return alGetFloat(AL_SPEED_OF_SOUND);
}

void Soundlib::SetSpeedOfSound(float speed)
{
    alSpeedOfSound(speed);
}

void Soundlib::SetListenerVolume(float volume)
{
    alListenerf(AL_GAIN, volume);
}

float Soundlib::GetListenerVolume()
{
    ALfloat volume;
    alGetListenerf(AL_GAIN, &volume);
    return volume;
}

void Soundlib::SetListenerPosition(float x, float y, float z)
{
    alListener3f(AL_POSITION, x, y, z);
}

void Soundlib::SetListenerPosition(Vector3 position)
{
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

Vector3 Soundlib::GetListenerPosition()
{
    Vector3 position;
    alGetListener3f(AL_POSITION, &position.x, &position.y, &position.z);
    return position;
}

void Soundlib::SetListenerVelocity(float velX, float velY, float velZ)
{
    alListener3f(AL_VELOCITY, velX, velY, velZ);
}

void Soundlib::SetListenerVelocity(Vector3 velocity)
{
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

Vector3 Soundlib::GetListenerVelocity()
{
    Vector3 velocity;
    alGetListener3f(AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z);
    return velocity;
}

void Soundlib::SetListenerOrientation(float forwardX, float forwardY, float forwardZ, float upX, float upY, float upZ)
{
    ALfloat values[6] = {
        forwardX, forwardY, forwardZ,
        upX, upY, upZ
    };
    alListenerfv(AL_ORIENTATION, values);
}

void Soundlib::SetListenerOrientation(Vector3 forward, Vector3 up)
{
    ALfloat values[6] = {
        forward.x, forward.y, forward.z,
        up.x, up.y, up.z
    };
    alListenerfv(AL_ORIENTATION, values);
}

void Soundlib::SetListenerOrientation(Orientation o)
{
    alListenerfv(AL_ORIENTATION, (ALfloat*)&o);
}

Orientation Soundlib::GetListenerOrientation()
{
    Orientation o;
    alGetListenerfv(AL_ORIENTATION, (ALfloat*)&o);
    return o;
}
