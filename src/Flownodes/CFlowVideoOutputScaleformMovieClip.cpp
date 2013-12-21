/* Videoplayer_Plugin - for licensing and copyright see license.txt */

#include <StdAfx.h>
#include <IPluginVideoplayer.h>
#include <Nodes/G2FlowBaseNode.h>
#include <CPluginVideoplayer.h>
#include <CVideoplayerSystem.h>
#include <IFlashUI.h>

namespace VideoplayerPlugin
{
    class CFlowVideoOutputScaleformMovieClipNode : public CFlowBaseNode<eNCT_Instanced>
    {
        private:
            IVideoplayer* m_pVideo;
            int m_nID;
            string m_sName;
            ITexture* m_pTex;
            IUIElement* m_pElement;
            IUIElement* m_pInstance;
            int m_nInstanceId;

            enum EInputPorts
            {
                EIP_OVERRIDE = 0,
                EIP_VIDEOID,
                EIP_MOVIECLIP,
                EIP_INSTANCEID,
            };

        public:
            CFlowVideoOutputScaleformMovieClipNode( SActivationInfo* pActInfo )
            {
                m_pVideo = NULL;
                m_nID = -1;
                m_sName = "";
                m_pElement = NULL;
                m_pInstance = NULL;
                m_nInstanceId = -1;
                m_pTex = NULL;
            }

            virtual ~CFlowVideoOutputScaleformMovieClipNode()
            {
                m_pVideo = NULL;
            }

            virtual IFlowNodePtr Clone( SActivationInfo* pActInfo )
            {
                return new CFlowVideoOutputScaleformMovieClipNode( pActInfo );
            }

            virtual void GetMemoryUsage( ICrySizer* s ) const
            {
                s->Add( *this );
            }

            void Serialize( SActivationInfo* pActInfo, TSerialize ser )
            {

            }

            virtual void GetConfiguration( SFlowNodeConfig& config )
            {
                static const SInputPortConfig inputs[] =
                {
                    InputPortConfig_Void( "Override", _HELP( "override material now" ) ),
                    InputPortConfig<int>( "VideoID", -1, _HELP( "id" ), "nVideoID" ),
                    InputPortConfig<string>( "uiMovieclips_MovieClip", "", _HELP( "movieclip to be modified" ), "sMovieClip", _UICONFIG( "" ) ),
                    InputPortConfig<int>( "InstanceID", -1, _HELP( "Instance ID for the element, -1 = all instances (lazy init), -2 = all initialized instances" ), "nInstanceID" ),
                    InputPortConfig_Null(),
                };

                config.pInputPorts = inputs;
                config.pOutputPorts = NULL;
                config.sDescription = _HELP( PLUGIN_CONSOLE_PREFIX "Videodestination/Texture for use in Scaleform MovieClips" );

                config.SetCategory( EFLN_APPROVED );
            }

            void TexIntoMc( bool bLoad )
            {
                if ( m_pElement )
                {
                    size_t nPos = m_sName.rfind( ':' );

                    if ( string::npos != nPos )
                    {
                        string sMC = m_sName.substr( nPos + 1 );

                        if ( m_nInstanceId < 0 )
                        {
                            IUIElementIteratorPtr iter = m_pElement->GetInstances();

                            while ( m_pInstance = iter->Next() )
                            {
                                if ( m_nInstanceId == -2 && !m_pInstance->IsInit() )
                                {
                                    continue;
                                }

                                if ( bLoad )
                                {
                                    m_pInstance->LoadTexIntoMc( sMC, m_pTex );
                                }

                                else
                                {
                                    m_pElement->UnloadTexFromMc( sMC, m_pTex );
                                }
                            }
                        }

                        else if ( m_pInstance = m_pElement->GetInstance( m_nInstanceId ) )
                        {
                            if ( bLoad )
                            {
                                m_pInstance->LoadTexIntoMc( sMC, m_pTex );
                            }

                            else
                            {
                                m_pElement->UnloadTexFromMc( sMC, m_pTex );
                            }
                        }
                    }
                }
            }

            virtual void ProcessEvent( EFlowEvent evt, SActivationInfo* pActInfo )
            {
                switch ( evt )
                {
                    case eFE_Suspend:
                        pActInfo->pGraph->SetRegularlyUpdated( pActInfo->myID, false );
                        break;

                    case eFE_Resume:
                        pActInfo->pGraph->SetRegularlyUpdated( pActInfo->myID, true );
                        break;

                    case eFE_Initialize:
                        break;

                    case eFE_Activate:

                        if ( IsPortActive( pActInfo, EIP_OVERRIDE ) && !m_pVideo )
                        {
                            m_sName = GetPortString( pActInfo, EIP_MOVIECLIP );
                            m_pVideo = gVideoplayerSystem->GetVideoplayerById( GetPortInt( pActInfo, EIP_VIDEOID ) );
                            pActInfo->pGraph->SetRegularlyUpdated( pActInfo->myID, true );
                        }

                        else if ( m_pVideo )
                        {
                            // Set changed properties
                            if ( IsPortActive( pActInfo, EIP_VIDEOID ) )
                            {
                                m_sName = GetPortString( pActInfo, EIP_MOVIECLIP );
                                m_pVideo = gVideoplayerSystem->GetVideoplayerById( GetPortInt( pActInfo, EIP_VIDEOID ) );
                            }
                        }

                        break;

                    case eFE_Update:
                        if ( m_pVideo )
                        {
                            m_pTex = m_pVideo->GetTexture();

                            if ( m_pTex  && m_nID != m_pTex->GetTextureID() )
                            {
                                m_nID = m_pTex->GetTextureID();
                                size_t nPos = m_sName.rfind( ':' );

                                if ( string::npos != nPos )
                                {
                                    m_pElement = gEnv->pFlashUI->GetUIElement( m_sName.substr( 0, nPos ).c_str() );
                                    m_nInstanceId = GetPortInt( pActInfo, EIP_INSTANCEID );
                                    TexIntoMc( true );
                                }
                            }
                        }

                        if ( !m_pVideo && m_nID != -1 )
                        {
                            TexIntoMc( false );

                            m_pTex = NULL;
                            m_nID = -1;
                            m_pElement = NULL;

                            pActInfo->pGraph->SetRegularlyUpdated( pActInfo->myID, false );
                        }

                        break;
                }
            }
    };
}

REGISTER_FLOW_NODE_EX( "Videoplayer_Plugin:OutputScaleformMovieClip", VideoplayerPlugin::CFlowVideoOutputScaleformMovieClipNode, CFlowVideoOutputScaleformMovieClipNode );