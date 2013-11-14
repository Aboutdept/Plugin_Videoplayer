/* Videoplayer_Plugin - for licensing and copyright see license.txt */

#include <StdAfx.h>
#include <CPluginVideoplayer.h>
#include <CVideoplayerSystem.h>

namespace VideoplayerPlugin
{
    CPluginVideoplayer* gPlugin = NULL;
    D3DPlugin::IPluginD3D* gD3DSystem = NULL;

    CPluginVideoplayer::CPluginVideoplayer()
    {
        gPlugin = this;
        gVideoplayerSystem = NULL;
        gD3DSystem = NULL;
    }

    CPluginVideoplayer::~CPluginVideoplayer()
    {
        Release( true );

        gPlugin = NULL;
        gVideoplayerSystem = NULL;
        gD3DSystem = NULL;
    }

    bool CPluginVideoplayer::Release( bool bForce )
    {
        bool bRet = true;

        if ( !m_bCanUnload )
        {
            // Should be called while Game is still active otherwise there might be leaks/problems
            bRet = CPluginBase::Release( bForce );

            if ( bRet )
            {
                // Depending on your plugin you might not want to unregister anything
                // if the System is quitting.
                if ( gVideoplayerSystem )
                {
                    delete gVideoplayerSystem;
                    gVideoplayerSystem = NULL;
                }

                if ( gD3DSystem )
                {
                    PluginManager::safeReleasePlugin( "D3D", gD3DSystem );
                }

                // Cleanup like this always (since the class is static its cleaned up when the dll is unloaded)
                gPluginManager->UnloadPlugin( GetName() );

                // Allow Plugin Manager garbage collector to unload this plugin
                AllowDllUnload();
            }
        }

        return bRet;
    };

    bool CPluginVideoplayer::Init( SSystemGlobalEnvironment& env, SSystemInitParams& startupParams, IPluginBase* pPluginManager, const char* sPluginDirectory )
    {
        gPluginManager = ( PluginManager::IPluginManager* )pPluginManager->GetConcreteInterface( NULL );
        CPluginBase::Init( env, startupParams, pPluginManager, sPluginDirectory );

        if ( gEnv && gEnv->pSystem && !gEnv->pSystem->IsQuitting() )
        {
            gVideoplayerSystem = new CVideoplayerSystem();
        }

        // Note: Autoregister Flownodes will be automatically registered

        return true;
    }

    bool CPluginVideoplayer::InitDependencies()
    {
        if ( gEnv && gEnv->pSystem && !gEnv->pSystem->IsQuitting() && gVideoplayerSystem )
        {
            if ( gPluginManager->GetPluginByName( "D3D" ) )
            {
                gD3DSystem = PluginManager::safeUsePluginConcreteInterface<D3DPlugin::IPluginD3D*>( "D3D" );
            }

            gVideoplayerSystem->Initialize();
        }

        return CPluginBase::InitDependencies();
    }

    const char* CPluginVideoplayer::ListCVars() const
    {
        return "vp_playbackmode, vp_seekthreshold, vp_dropthreshold, vp_dropmaxduration,vp_defaultcolor";
    }

    const char* CPluginVideoplayer::GetStatus() const
    {
        return "OK";
    }

    void* CPluginVideoplayer::GetConcreteInterface( const char* sInterfaceVersion )
    {
        return static_cast <IPluginVideoplayer*>( gVideoplayerSystem );
    };

    bool CPluginVideoplayer::RegisterTypes( int nFactoryType, bool bUnregister )
    {
        // Note: Autoregister Flownodes will be automatically registered by the Base class
        bool bRet = CPluginBase::RegisterTypes( nFactoryType, bUnregister );

        using namespace PluginManager;
        eFactoryType enFactoryType = eFactoryType( nFactoryType );

        if ( bRet )
        {
            if ( gEnv && gEnv->pSystem && !gEnv->pSystem->IsQuitting() )
            {
                // UI Events
                if ( gEnv->pConsole && ( enFactoryType == FT_All || enFactoryType == FT_UIEvent ) )
                {
                    if ( !bUnregister )
                    {
                        if ( gVideoplayerSystem && gVideoplayerSystem->m_pAutoPlaylists )
                        {
                            gVideoplayerSystem->m_pAutoPlaylists->InitEventSystem();
                        }
                    }
                }

                // CVars
                if ( gEnv->pConsole && ( enFactoryType == FT_All || enFactoryType == FT_CVar ) )
                {
                    if ( !bUnregister )
                    {
                        // TODO: Register CVars here if you have some
                        // ...
                    }

                    else
                    {
                        // TODO: Unregister CVars here if you have some
                        // ...
                    }
                }

                // CVars Commands
                if ( gEnv->pConsole && ( enFactoryType == FT_All || enFactoryType == FT_CVarCommand ) )
                {
                    if ( !bUnregister )
                    {
                        // TODO: Register CVar Commands here if you have some
                        // ...
                    }

                    else
                    {
                        // TODO: Unregister CVar Commands here if you have some
                        // ...
                    }
                }

                // Game Objects
                if ( gEnv->pGame && gEnv->pGame->GetIGameFramework() && ( enFactoryType == FT_All || enFactoryType == FT_GameObjectExtension ) )
                {
                    if ( !bUnregister )
                    {
                        // TODO: Register Game Object Extensions here if you have some
                        // ...
                    }
                }
            }
        }

        return bRet;
    }

}