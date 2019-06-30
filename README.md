<img src="http://img.shields.io/liberapay/receives/abbysoft.svg?logo=liberapay">

# Soompler
Soompler is JUCE based sampler VST and Standalone plugin for Linux, MacOS, Windows.

![alt Screenshot](https://i.imgur.com/xgWU7Ad.png)

Notice. At this point license is proprietary, thus any commercial usage of this code or its parts prohibited. All code is property of AbbySoft team and all its members.

**Please, if you want us to continue development of this plugin consider donate us a dime. We are thankful to all support, it will motivate us to bring better software to community.** <noscript><a href="https://liberapay.com/abbysoft/donate"><img alt="Donate using Liberapay" src="https://liberapay.com/assets/widgets/donate.svg"></a></noscript>

# Building

Сurrent build instruction based on ElementaryOS 5.0 bui I think it will be compatible with Ubuntu 18.08 and many more Linux distributions.

1. Obtain free JUCE Framework from official web-site. https://shop.juce.com/get-juce
2. Open Projucer from downloaded JUCE framework.
3. Open soompler.jucer project file in Projucer
4. Edit global paths in Projucer (location of framework directories, VST2, VST3, and other sdk's location if you want to build specific plugin types)
5. Select desired exporter or add for your system. 
6. Click on LinuxMake exported, right click and choose "Save this exporter"
7. Do the same as 5 with CLion exporter, if you want to open it in CLion or QtCreator
8. Open project in desired IDE
9. Build it like plain project for your OS
