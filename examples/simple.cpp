#include "../include/soundlib.h"

int main()
{
    // Acquire default audio device
    Soundlib::Init();

    // Load sound
    Soundlib::Sound sound("/path/to/sound.wav");

    // Attach sound to source and play it
    Soundlib::SoundSource source(sound);
    source.Play();

    /*
        Alternatively, you can load a sound after constructing a SoundSource as follows:
            Soundlib::SoundSource source;
            source.SetSound(sound);
    */

    // Play until completion
    while (source.GetState() == Soundlib::SourceState::PLAYING) {}

    // This doesn't really need to be called at the end of the program, but some OpenAL implementations
    // will print a useless error message if you don't
    Soundlib::Exit();
}
