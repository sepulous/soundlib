#include <iostream>
#include <chrono>
#include <thread>

#include "../include/soundlib.h"

using namespace std::chrono_literals;

// IMPORTANT: Attenuation and the Doppler effect are only applied to mono sounds, not stereo sounds

int main()
{
    Soundlib::Init();

    // OpenAL should default to these settings, but just to be sure
    Soundlib::SetDopplerFactor(1.0F);
    Soundlib::SetSpeedOfSound(343.3F);
    Soundlib::SetAttenuationModel(Soundlib::AttenuationModel::INVERSE_DISTANCE_CLAMPED);

    // Position/orient listener
    Soundlib::SetListenerPosition({0, 0, 0});
    Soundlib::SetListenerVelocity({0, 0, 0});
    Soundlib::SetListenerOrientation({
        {0, 0, -1}, // Listener faces -z direction
        {0, 1, 0}   // Listener's head points in +y direction
    });

    // Create SoundSource and set its position and velocity
    Soundlib::SoundSource source;
    source.SetPosition({-50, 0, 0});
    source.SetVelocity({20, 0, 0});

    // Load sound, attach to source, and play
    Soundlib::Sound sound("/path/to/sound.wav");
    source.SetSound(sound);
    source.Play();

    // Source position must be manually updated; OpenAL doesn't do that
    while (source.GetState() == Soundlib::SourceState::PLAYING)
    {
        Soundlib::Vector3 position = source.GetPosition();
        position.x += source.GetVelocity().x * 0.05F;
        source.SetPosition(position);

        std::this_thread::sleep_for(50ms);
    }
}
