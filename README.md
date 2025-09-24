# Soundlib
Soundlib is an convenient audio library that allows you to play sound in 3D space. It's essentially a wrapper over [OpenAL Soft](https://github.com/kcat/openal-soft) that utilizes [miniaudio](https://github.com/mackron/miniaudio) + [stb_vorbis](https://github.com/nothings/stb) to support a variety of common audio formats. Soundlib is only intended for _playing_ audio and does not support recording. Soundlib supports WAV, MP3, FLAC, and OGG; all right out of the box.

Soundlib's simple model mirrors that of OpenAL's, while abstracting away the cumbersome management of OpenAL objects. There is a single listener whose properties (position, velocity, orientation, etc.) are managed through global functions. Audio files are loaded into `Sound` objects, which are then attached to `SoundSource` objects to be played. These are the only two objects you have to deal with.

Playing a sound file is as simple as this:
```cpp
#include "soundlib.h"

using namespace Soundlib;

int  main()
{
	Soundlib::Initialize();

	Sound sound("/path/to/sound.wav");

	SoundSource source(sound);
	source.Play();

	while (source.GetState() == SourceState::PLAYING) {}
}
```
Before initializing, you can get a list of available playback devices and then choose the one you want:
```cpp
auto deviceList = Soundlib::GetDeviceList();
int selectedDeviceIndex = SelectDevice(); // Your code
Soundlib::Initialize(deviceList[selectedDeviceIndex]);
```

# Building
Soundlib uses CMake for its build system. Soundlib is built as a dynamic library by default, but it can be built as a static library through the CMake option `STATIC_SOUNDLIB`. You can also choose whether to bundle OpenAL Soft or use a system-installed version through the `BUNDLE_OPENAL` option. On Windows, OpenAL is bundled by default. For other systems it's assumed that OpenAL Soft is installed system-wide.

### Bundled Build
If you choose to bundle OpenAL Soft, building is as simple as running the following from the root directory:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

# TODO
I'm actively working on the following features:
* Changing playback device after initialization
* Buffer queuing
* Support for OpenAL EFX (reverb, filters, etc.)
* More systematic error handling
