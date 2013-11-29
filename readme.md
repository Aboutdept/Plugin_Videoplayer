Videoplayer-Plugin
==================

Videoplayer for CryEngine featuring WebM, DirectX 9, DirectX 11 Support

If you want to support my endeavours please feel free to pre-order a copy of [Miscreated](http://miscreatedgame.com/buy) an multiplayer, post-apocalyptic, survival-based sandbox game created using CRYENGINE.

Features
--------
* Create Video Splashscreens / Menubackgrounds
* Replace any ingame 3D Entity/Material/Texture/Flash Movieclip with a video (from the filesystem or in pak file)
* Looping / Custom resolution / Seeking / Pause / Speed
* Play multiple video streams and reuse them on multiple targets
* Fullscreen/2D Videosupport (multiple videos also possible)
  supports scaling, aspect ratio, placement, rotation,
  black border (or colored), transparency...
* Synchronize to multiple timesources (e.g. sound, supporting bullet time or playback during pause)
* 2D Sound / 3D Sounds on Entity
* Playlists (for videos and console commands e.g. loading a map as mainmenu background)
* Flowgraphnodes
* C++ Interface

Tutorial videos
===============
* [Part 3: 1.6 Demonstration](http://www.youtube.com/watch?v=I0x343yvtsM)
* [Part 2: 1.5 Features and Playlists](http://www.youtube.com/watch?v=AGEEjqRHfTU)
* [Part 1: Introduction and Howto play video on ingame objects](http://www.youtube.com/watch?v=g0feGWMsSCE) (Version 1.0.1 so expect some differences)

Preparing a video with audio
============================
* See [Wiki: Preparing a video with audio](https://github.com/hendrikp/Plugin_Videoplayer/wiki/Preparing-Video-and-Audio)
* FMOD Sound Tutorial see [Wiki: Creating FMOD Soundevents](https://github.com/hendrikp/Plugin_Videoplayer/wiki/Creating-FMOD-Soundevents)

Installation / Integration
==========================
This plugin requires the Plugin SDK to be installed.
The plugin manager will automatically load up the plugin when the game/editor is restarted or if you directly load it.

Users
-----
* Use the installer or extract the files to your CryEngine SDK Folder so that the Code and BinXX/Plugins directory match up.
* Make sure that you also install the Direct3D Plugin if you want playback not just in the menu but ingame too. (Else the playback will be unstable/crash)

Designers
---------
* If you want the sample logos/backgrounds and level please download the additional sample package.

Developers
----------
* If you want to extend video functions using C++ or recompile the plugin please clone the latest stable branch
  and add ```;$(SolutionDir)..\Plugin_Videoplayer\inc``` path to your include directories
* For the Debug Configuration please download the [DirectX SDK](http://www.microsoft.com/en-us/download/details.aspx?id=6812)

CVars
=====
* See [Wiki: CVar Commands](https://github.com/hendrikp/Plugin_Videoplayer/wiki/CVar-Commands)

Flownodes
=========
* See [Wiki: Flownodes](https://github.com/hendrikp/Plugin_Videoplayer/wiki/Flownodes)

Playlists
=========
* See [Wiki: Autoevent Playlists](https://github.com/hendrikp/Plugin_Videoplayer/wiki/Autoevent-Playlists)
* See [Wiki: Playlist XML Format](https://github.com/hendrikp/Plugin_Videoplayer/wiki/Playlist-XML-Format)

Contributing
============
* See [Plugin SDK Wiki: Contribution Guideline](https://github.com/hendrikp/Plugin_SDK/wiki/Contribution-Guideline)
* [Wishes / Issues](https://github.com/hendrikp/Plugin_Videoplayer/issues)
