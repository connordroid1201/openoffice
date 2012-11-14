/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_framework.hxx"
#include <uielement/logotextstatusbarcontroller.hxx>
#include <classes/fwlresid.hxx>
#include <services.h>
#include <classes/resource.hrc>
#include <vos/mutex.hxx>
#include <vcl/svapp.hxx>
#include <vcl/window.hxx>
#include <vcl/status.hxx>
#ifndef _TOOLKIT_HELPER_VCLUNOHELPER_HXX_
#include <toolkit/helper/vclunohelper.hxx>
#endif
#include <toolkit/helper/convert.hxx>

using namespace ::rtl;
using namespace ::cppu;
using namespace ::com::sun::star;
using namespace ::com::sun::star::awt;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::frame;

namespace framework
{

DEFINE_XSERVICEINFO_MULTISERVICE        (   LogoTextStatusbarController     	    ,
                                            OWeakObject                             ,
                                            SERVICENAME_STATUSBARCONTROLLER		    ,
											IMPLEMENTATIONNAME_LOGOTEXTSTATUSBARCONTROLLER
										)

DEFINE_INIT_SERVICE                     (   LogoTextStatusbarController, {} )

LogoTextStatusbarController::LogoTextStatusbarController( const uno::Reference< lang::XMultiServiceFactory >& xServiceManager ) :
    svt::StatusbarController( xServiceManager, uno::Reference< frame::XFrame >(), rtl::OUString(), 0 )
{
    m_aLogoText = String( FwlResId( STR_STATUSBAR_LOGOTEXT ));
}

LogoTextStatusbarController::~LogoTextStatusbarController()
{
}

// XInterface
Any SAL_CALL LogoTextStatusbarController::queryInterface( const Type& rType )
throw ( RuntimeException )
{
    return svt::StatusbarController::queryInterface( rType );
}

void SAL_CALL LogoTextStatusbarController::acquire() throw ()
{
    svt::StatusbarController::acquire();
}

void SAL_CALL LogoTextStatusbarController::release() throw ()
{
    svt::StatusbarController::release();
}
 
void SAL_CALL LogoTextStatusbarController::initialize( const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any >& aArguments ) 
throw (::com::sun::star::uno::Exception, ::com::sun::star::uno::RuntimeException)
{
    vos::OGuard aSolarMutexGuard( Application::GetSolarMutex() ); 
    
    svt::StatusbarController::initialize( aArguments );

    if ( m_xParentWindow.is() && m_nID > 0 )
    {
        Window* pWindow = VCLUnoHelper::GetWindow( m_xParentWindow );
        if ( pWindow && ( pWindow->GetType() == WINDOW_STATUSBAR ))
        {
            StatusBar* pStatusBar = (StatusBar *)pWindow;
            pStatusBar->SetItemText( m_nID, m_aLogoText );
        }
    }
}

// XComponent
void SAL_CALL LogoTextStatusbarController::dispose()
throw (::com::sun::star::uno::RuntimeException)
{
    svt::StatusbarController::dispose();
}
 
// XEventListener
void SAL_CALL LogoTextStatusbarController::disposing( const EventObject& Source )
throw ( RuntimeException )
{
    svt::StatusbarController::disposing( Source );
}
 
// XStatusListener
void SAL_CALL LogoTextStatusbarController::statusChanged( const FeatureStateEvent& )
throw ( RuntimeException )
{
}

// XStatusbarController
::sal_Bool SAL_CALL LogoTextStatusbarController::mouseButtonDown(
    const ::com::sun::star::awt::MouseEvent& )
throw (::com::sun::star::uno::RuntimeException)
{
    return sal_False;
}

::sal_Bool SAL_CALL LogoTextStatusbarController::mouseMove(
    const ::com::sun::star::awt::MouseEvent& )
throw (::com::sun::star::uno::RuntimeException)
{
    return sal_False;
}

::sal_Bool SAL_CALL LogoTextStatusbarController::mouseButtonUp(
    const ::com::sun::star::awt::MouseEvent& )
throw (::com::sun::star::uno::RuntimeException)
{
    return sal_False;
}

void SAL_CALL LogoTextStatusbarController::command(
    const ::com::sun::star::awt::Point& aPos,
    ::sal_Int32 nCommand,
    ::sal_Bool bMouseEvent,
    const ::com::sun::star::uno::Any& aData )
throw (::com::sun::star::uno::RuntimeException)
{
    svt::StatusbarController::command( aPos, nCommand, bMouseEvent, aData );
}

void SAL_CALL LogoTextStatusbarController::paint(
    const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XGraphics >& xGraphics,
    const ::com::sun::star::awt::Rectangle& rOutputRectangle,
    ::sal_Int32 nItemId,
    ::sal_Int32 nStyle )
throw (::com::sun::star::uno::RuntimeException)
{
    svt::StatusbarController::paint( xGraphics, rOutputRectangle, nItemId, nStyle );
}

void SAL_CALL LogoTextStatusbarController::click()
throw (::com::sun::star::uno::RuntimeException)
{
    svt::StatusbarController::click();
}

void SAL_CALL LogoTextStatusbarController::doubleClick() throw (::com::sun::star::uno::RuntimeException)
{
    svt::StatusbarController::doubleClick();
}

}
