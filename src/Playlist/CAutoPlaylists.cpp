/* Videoplayer_Plugin - for licensing and copyright see license.txt */

#include <StdAfx.h>
#include <CPluginVideoplayer.h>
#include <CVideoplayerSystem.h>
#include <Playlist/CAutoPlaylists.h>

namespace VideoplayerPlugin
{
    CAutoPlaylists::CAutoPlaylists()
    {
        m_pUIFunctions = NULL;
        InitEventSystem();

        m_pSplashScreen = gVideoplayerSystem->CreatePlaylist( true ); // Only for Playlist the default is to show the menu
        m_pMenu = gVideoplayerSystem->CreatePlaylist();
        m_pMenuIngame = gVideoplayerSystem->CreatePlaylist();
        m_pLevelLoaded = gVideoplayerSystem->CreatePlaylist();
    }

    CAutoPlaylists::~CAutoPlaylists()
    {
        if ( gVideoplayerSystem && gEnv && gEnv->pSystem && !gEnv->pSystem->IsQuitting() )
        {
            if ( m_pSplashScreen )
            {
                gVideoplayerSystem->DeletePlaylist( m_pSplashScreen );
            }

            if ( m_pMenu )
            {
                gVideoplayerSystem->DeletePlaylist( m_pMenu );
            }

            if ( m_pMenuIngame )
            {
                gVideoplayerSystem->DeletePlaylist( m_pMenuIngame );
            }

            if ( m_pLevelLoaded )
            {
                gVideoplayerSystem->DeletePlaylist( m_pLevelLoaded );
            }
        }

        UnloadEventSystem();
    }

    void CAutoPlaylists::OnSkip( bool bForce )
    {
        if ( m_pSplashScreen )
        {
            m_pSplashScreen->Skip( bForce );
        }

        if ( m_pMenu )
        {
            m_pMenu->Skip( bForce );
        }

        if ( m_pMenuIngame )
        {
            m_pMenuIngame->Skip( bForce );
        }

        if ( m_pLevelLoaded )
        {
            m_pLevelLoaded->Skip( bForce );
        }
    }

    void CAutoPlaylists::OnScreenChange()
    {
        if ( m_pSplashScreen )
        {
            //  m_pSplashScreen->UnregisterListener(this);
            m_pSplashScreen->Close();
        }

        if ( m_pMenu )
        {
            m_pMenu->Close();
        }

        if ( m_pMenuIngame )
        {
            m_pMenuIngame->Close();
        }

        if ( m_pLevelLoaded )
        {
            m_pLevelLoaded->Close();
        }
    }

    void CAutoPlaylists::OnStart()
    {
        OnScreenChange();

        if ( m_pSplashScreen )
        {
            m_pSplashScreen->RegisterListener( this );

            if ( m_pSplashScreen->Open( AUTOPLAY_SPLASHSCREEN ) )
            {
                gVideoplayerSystem->ShowMenu( false );
                m_pSplashScreen->Resume();
            }

            else
            {
                OnEndPlaylist( m_pSplashScreen );
            }
        }

        if ( m_pLevelLoaded )
        {
            m_pLevelLoaded->RegisterListener( this );
        }
    }

    void CAutoPlaylists::OnStartPlaylist( IVideoplayerPlaylist* pPlaylist )
    {
        if ( pPlaylist == m_pSplashScreen )
        {
            m_eventSender.SendEvent<eUIE_OnAutoplaylist_SplashscreenStart>();
        }

        else if ( pPlaylist == m_pLevelLoaded )
        {
            m_eventSender.SendEvent<eUIE_OnAutoplaylist_LevelLoadedStart>();
        }
    }

    void CAutoPlaylists::OnEndPlaylist( IVideoplayerPlaylist* pPlaylist )
    {
        if ( pPlaylist == m_pSplashScreen )
        {
            m_eventSender.SendEvent<eUIE_OnAutoplaylist_SplashscreenEnd>();

            if ( gVideoplayerSystem->GetScreenState() == eSS_StartScreen )
            {
                OnMenu( false );
            }
        }

        else if ( pPlaylist == m_pLevelLoaded )
        {
            m_eventSender.SendEvent<eUIE_OnAutoplaylist_LevelLoadedEnd>();
        }
    }

    void CAutoPlaylists::OnMenu( bool bInGame )
    {
        OnScreenChange();

        if ( m_pMenu && !bInGame )
        {
            m_pMenu->Open( AUTOPLAY_MENU );
            m_pMenu->Resume();
        }

        else if ( m_pMenuIngame && bInGame )
        {
            m_pMenuIngame->Open( AUTOPLAY_MENU_INGAME );
            m_pMenuIngame->Resume();
        }
    }

    void CAutoPlaylists::OnLevelLoaded( const char* path )
    {
        OnScreenChange();

        if ( m_pLevelLoaded )
        {
            string sPath = path;
            sPath += "/";
            sPath += AUTOPLAY_LEVEL;

            if ( m_pLevelLoaded->Open( sPath.c_str() ) )
            {
                m_pLevelLoaded->Resume();
            }

            else
            {
                OnEndPlaylist( m_pLevelLoaded );
            }
        }
    }

    void CAutoPlaylists::InitEventSystem()
    {
        if ( !gEnv->pFlashUI )
        {
            return;
        }

        m_pUIFunctions = gEnv->pFlashUI->CreateEventSystem( PLUGIN_UIEVENTSYSTEM, IUIEventSystem::eEST_SYSTEM_TO_UI );
        m_eventSender.Init( m_pUIFunctions );

        {
            SUIEventDesc evtDesc( "Autoplaylist:Splashscreen:OnStart", "Triggered when the Splashscreen video play list starts" );
            m_eventSender.RegisterEvent<eUIE_OnAutoplaylist_SplashscreenStart>( evtDesc );
        }

        {
            SUIEventDesc evtDesc( "Autoplaylist:Splashscreen:OnEnd", "Triggered when the Splashscreen video play list ended or was skipped" );
            m_eventSender.RegisterEvent<eUIE_OnAutoplaylist_SplashscreenEnd>( evtDesc );
        }

        {
            SUIEventDesc evtDesc( "Autoplaylist:LevelLoaded:OnStart", "Triggered when the level video play list starts" );
            m_eventSender.RegisterEvent<eUIE_OnAutoplaylist_LevelLoadedStart>( evtDesc );
        }

        {
            SUIEventDesc evtDesc( "Autoplaylist:LevelLoaded:OnEnd", "Triggered when the level video play list ended or was skipped" );
            m_eventSender.RegisterEvent<eUIE_OnAutoplaylist_LevelLoadedEnd>( evtDesc );
        }
    }

    void CAutoPlaylists::UnloadEventSystem()
    {

    }
}