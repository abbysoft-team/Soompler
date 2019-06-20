# Soompler
Soompler is JUCE based sampler VST and Standalone plugin for Linux, MacOS, Windows.

Notice. At this point license is proprietary, thus any commercial usage of this code or its parts prohibited. All code is property of AbbySoft team and all its members.

# Building

Сurrent build instruction based on ElementaryOS 5.0 bui I think it will be compatible with Ubuntu 18.08 and many more Linux distributions.

1. Obtain free JUCE Framework from official web-site. https://shop.juce.com/get-juce
2. Open Projucer from downloaded JUCE framework.
3. Edit global paths in Projucer (location of framework directories, VST2, VST3, and other sdk's location if you want to build specific plugin types)
4. Select desired exporter or add for your system. 
5. Click on LinuxMake exported, right click and choose "Save this exporter"
6. Do the same as 5 with CLion exporter, if you want to open it in CLion or QtCreator
7. Open project in desired IDE
