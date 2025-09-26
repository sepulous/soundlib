#include <iostream>

#include "../include/soundlib.h"

/*
    This is a very simple example for illustrative purposes. Each device string contains extra information added
    by OpenAL, in addition to the "real" device name shown by the OS (ex: "OpenAL on Speakers (<real-device-name>)").
    For UI applications you'll want to extract this "real" device name to show the user.
*/

int main()
{
    // Show available output devices
    std::vector<std::string> deviceList = Soundlib::GetDeviceList();
    for (int i = 0; i < deviceList.size(); i++)
        std::cout << "(" << i << ") " << deviceList[i] << std::endl;

    // Select device
    int deviceIndex;
    std::cout << "Choose a device: ";
    std::cin >> deviceIndex;
    std::string selectedDevice = deviceList[deviceIndex];

    // Initialize with device
    Soundlib::Init(selectedDevice);

    // ...
}
