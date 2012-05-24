MediaPlayerX
============

####Description:

Win32 API build for a basic audio/video player

Created as a project to learn Win32 API programming and audio/video file playback and as a project for college

####License:

Uses mutiple licenses.Consult with individual softwares.
Code is in public domain.


####Uses the following  softwares:

1. SQLite
2. SDL
3. CppDB
4. TagLib


####Issues / TODO list:

* Transparency makes text hard to read, the choice of the background chrome affects the text.
* Prepared SQL does not work for some characters yet, prob need to circument CppDB and use SQLite directly.
* Issues while playing some types of audio/video file specifically ogg/theora and flash video > 8.0.
* UI is still basic, need to polish the UI and add more features.
* Contains duplicated code and redundant files so as not to break new/old features.
* Direct2D was used to render effects however discarded due to complexity and still not sure if using Direct2d will help in any way.

####Tools:

* Compiled with VC++ 2010 express edition.
* TestApp is the project for program until it is ready to be moved to the MediaPlayerX project folder.
* TagLib has been compiled from source only for debug release.

####Files

1. TestApp
 * CppSqLite3.cpp -> CppDB code.
 * funcs.h -> Functions for audio/video synchronization/playback and SDL.  
 * glb_funcs.cpp -> UI functionality and other system calls.
 * main.cpp -> Main Win32 code for the UI.

####References / Thanks:

1. Dranger turorial
2. Codeplex