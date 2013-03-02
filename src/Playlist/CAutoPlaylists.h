/* Videoplayer_Plugin - for licensing and copyright see license.txt */

#include <Playlist/IVideoplayerPlaylist.h>

#include <IFlashUI.h>

#pragma once

namespace VideoplayerPlugin
{
    /**
    * @brief Contains Default Playlists for easier access to often used functionality like splash screens
    */
    class CAutoPlaylists :
        public IVideoplayerPlaylistEventListener
    {
        private:
            IVideoplayerPlaylist* m_pSplashScreen; //!< Splashscreen Playlist
            IVideoplayerPlaylist* m_pMenu; //!< Mainmenu Playlist
            IVideoplayerPlaylist* m_pMenuIngame; //!< Ingame Menu Playlist
            IVideoplayerPlaylist* m_pLevelLoaded; //!< Level Introduction Playlist

            /**
            * @brief resize modes for 2D output
            */
            enum EUIEvent
            {
                eUIE_OnAutoplaylist_SplashscreenStart,
                eUIE_OnAutoplaylist_SplashscreenEnd,
                eUIE_OnAutoplaylist_LevelLoadedStart,
                eUIE_OnAutoplaylist_LevelLoadedEnd,
            };

            // UI Events
            SUIEventSenderDispatcher<EUIEvent> m_eventSender; //!< Dispatches our messages to the UI Event Flownodes
            IUIEventSystem* m_pUIFunctions; //!< Pointer to UI Event System containing Video Events

            /**
            * @brief Initializes/Registers the UI Event System (can't be unregistered once registered)
            */
            void InitEventSystem();

        public:
            CAutoPlaylists();
            ~CAutoPlaylists();

            /**
            * @brief Notify auto playlist that a state change occured and all playlists need to be closed
            */
            void OnScreenChange();

            /**
            * @brief Notify auto playlist that the ingame or main menu has been opened
            * @param bInGame A level is loaded or not
            */
            void OnMenu( bool bInGame = false );

            /**
            * @brief Notify auto playlists that the user wants to skip
            */
            void OnSkip( bool bForce = false );

            /**
            * @brief Notify auto playlists that the system started up and splashscreen can now be played
            */
            void OnStart();

            /**
            * @brief Notify auto playlist to check for level introduction playlist and start it if existing.
            * @param sPath Path of the Level
            */
            void OnLevelLoaded( const char* sPath );

            // IVideoplayerPlaylistEventListener
            virtual void OnStartPlaylist( IVideoplayerPlaylist* pPlaylist );
            virtual void OnBeginScene( IVideoplayerPlaylist* pPlaylist, int nIndex ) {};
            virtual void OnVideoStart( IVideoplayerPlaylist* pPlaylist, IVideoplayer* pVideo ) {};
            virtual void OnVideoEnd( IVideoplayerPlaylist* pPlaylist, IVideoplayer* pVideo ) {};
            virtual void OnEndScene( IVideoplayerPlaylist* pPlaylist, int nIndex ) {};
            virtual void OnEndPlaylist( IVideoplayerPlaylist* pPlaylist );
    };
}