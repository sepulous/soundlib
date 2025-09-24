
#include "soundlib.h"

int main()
{
    Soundlib::Initialize();

    Soundlib::Sound sound("C:\\Users\\miner\\Desktop\\Soundlib Test\\sounds\\sound_ogg.ogg");

    Soundlib::SoundSource source(sound);
    source.Play();

    while (source.GetState() == Soundlib::SourceState::PLAYING) {}
}
