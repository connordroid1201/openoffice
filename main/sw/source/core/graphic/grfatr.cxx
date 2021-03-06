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
#include "precompiled_sw.hxx"


#include <com/sun/star/text/RelOrientation.hpp>
#include <com/sun/star/text/VertOrientation.hpp>
#include <com/sun/star/text/HorizontalAdjust.hpp>
#include <com/sun/star/text/DocumentStatistic.hpp>
#include <com/sun/star/text/HoriOrientation.hpp>
#include <com/sun/star/text/HoriOrientationFormat.hpp>
#include <com/sun/star/text/NotePrintMode.hpp>
#include <com/sun/star/text/SizeType.hpp>
#include <com/sun/star/text/VertOrientationFormat.hpp>
#include <com/sun/star/text/WrapTextMode.hpp>
#include <com/sun/star/text/GraphicCrop.hpp>
#include <com/sun/star/text/XTextGraphicObjectsSupplier.hpp>
#include <com/sun/star/drawing/ColorMode.hpp>
#include <svtools/grfmgr.hxx>
#include <swtypes.hxx>
#include <grfatr.hxx>
#include <swunohelper.hxx>

#ifndef _CMDID_H
#include <cmdid.h>
#endif
#ifndef _UNOMID_H
#include <unomid.h>
#endif

using namespace ::com::sun::star;

TYPEINIT1_AUTOFACTORY(SwCropGrf, SfxPoolItem)
TYPEINIT1_AUTOFACTORY(SwGammaGrf, SfxPoolItem)

/******************************************************************************
 *	Implementierung		class SwMirrorGrf
 ******************************************************************************/

SfxPoolItem* SwMirrorGrf::Clone( SfxItemPool* ) const
{
	return new SwMirrorGrf( *this );
}

sal_uInt16 SwMirrorGrf::GetValueCount() const
{
    return RES_MIRROR_GRAPH_END - RES_MIRROR_GRAPH_BEGIN;
}

int SwMirrorGrf::operator==( const SfxPoolItem& rItem) const
{
	return SfxEnumItem::operator==(rItem) &&
			((SwMirrorGrf&)rItem).IsGrfToggle() == IsGrfToggle();
}

sal_Bool lcl_IsHoriOnEvenPages(int nEnum, sal_Bool bToggle)
{
    sal_Bool bEnum = nEnum == RES_MIRROR_GRAPH_VERT ||
                   nEnum == RES_MIRROR_GRAPH_BOTH;
			return bEnum != bToggle;
}
sal_Bool lcl_IsHoriOnOddPages(int nEnum)
{
    sal_Bool bEnum = nEnum == RES_MIRROR_GRAPH_VERT ||
                   nEnum == RES_MIRROR_GRAPH_BOTH;
			return bEnum;
}
sal_Bool SwMirrorGrf::QueryValue( uno::Any& rVal, sal_uInt8 nMemberId ) const
{
	sal_Bool bRet = sal_True,
		 bVal;
	// Vertikal und Horizontal sind mal getauscht worden!
    nMemberId &= ~CONVERT_TWIPS;
	switch ( nMemberId )
	{
		case MID_MIRROR_HORZ_EVEN_PAGES:
			bVal = lcl_IsHoriOnEvenPages(GetValue(), IsGrfToggle());
		break;
		case MID_MIRROR_HORZ_ODD_PAGES:
			bVal = lcl_IsHoriOnOddPages(GetValue());
		break;
		case MID_MIRROR_VERT:
            bVal = GetValue() == RES_MIRROR_GRAPH_HOR ||
                   GetValue() == RES_MIRROR_GRAPH_BOTH;
			break;
		default:
			ASSERT( !this, "unknown MemberId" );
			bRet = sal_False;
	}
	rVal.setValue( &bVal, ::getBooleanCppuType() );
	return bRet;
}

sal_Bool SwMirrorGrf::PutValue( const uno::Any& rVal, sal_uInt8 nMemberId )
{
	sal_Bool bRet = sal_True;
	sal_Bool bVal = *(sal_Bool*)rVal.getValue();
	// Vertikal und Horizontal sind mal getauscht worden!
    nMemberId &= ~CONVERT_TWIPS;
	switch ( nMemberId )
	{
		case MID_MIRROR_HORZ_EVEN_PAGES:
		case MID_MIRROR_HORZ_ODD_PAGES:
		{
            sal_Bool bIsVert = GetValue() == RES_MIRROR_GRAPH_HOR ||
                                GetValue() == RES_MIRROR_GRAPH_BOTH;
			sal_Bool bOnOddPages = nMemberId == MID_MIRROR_HORZ_EVEN_PAGES ?
									lcl_IsHoriOnOddPages(GetValue()) : bVal;
			sal_Bool bOnEvenPages = nMemberId == MID_MIRROR_HORZ_ODD_PAGES ?
									   lcl_IsHoriOnEvenPages(GetValue(), IsGrfToggle()) : bVal;
            MirrorGraph nEnum = bOnOddPages ?
                    bIsVert ? RES_MIRROR_GRAPH_BOTH : RES_MIRROR_GRAPH_VERT :
                        bIsVert ? RES_MIRROR_GRAPH_HOR : RES_MIRROR_GRAPH_DONT;
			sal_Bool bToggle = bOnOddPages != bOnEvenPages;
			SetValue(static_cast<sal_uInt16>(nEnum));
			SetGrfToggle( bToggle );
		}
		break;
		case MID_MIRROR_VERT:
			if ( bVal )
			{
                if ( GetValue() == RES_MIRROR_GRAPH_VERT )
                    SetValue( RES_MIRROR_GRAPH_BOTH );
                else if ( GetValue() != RES_MIRROR_GRAPH_BOTH )
                    SetValue( RES_MIRROR_GRAPH_HOR );
			}
			else
			{
                if ( GetValue() == RES_MIRROR_GRAPH_BOTH )
                    SetValue( RES_MIRROR_GRAPH_VERT );
                else if ( GetValue() == RES_MIRROR_GRAPH_HOR )
                    SetValue( RES_MIRROR_GRAPH_DONT );
			}
			break;
		default:
			ASSERT( !this, "unknown MemberId" );
			bRet = sal_False;
	}
	return bRet;
}


/******************************************************************************
 *	Implementierung		class SwCropGrf
 ******************************************************************************/

SwCropGrf::SwCropGrf()
	: SvxGrfCrop( RES_GRFATR_CROPGRF )
{}

SwCropGrf::SwCropGrf(sal_Int32 nL, sal_Int32 nR, sal_Int32 nT, sal_Int32 nB )
	: SvxGrfCrop( nL, nR, nT, nB, RES_GRFATR_CROPGRF )
{}

SfxPoolItem* SwCropGrf::Clone( SfxItemPool* ) const
{
	return new SwCropGrf( *this );
}

// ------------------------------------------------------------------

SfxPoolItem* SwRotationGrf::Clone( SfxItemPool * ) const
{
	return new SwRotationGrf( GetValue(), aUnrotatedSize );
}


int	SwRotationGrf::operator==( const SfxPoolItem& rCmp ) const
{
	return SfxUInt16Item::operator==( rCmp ) &&
		GetUnrotatedSize() == ((SwRotationGrf&)rCmp).GetUnrotatedSize();
}


sal_Bool SwRotationGrf::QueryValue( uno::Any& rVal, sal_uInt8 ) const
{
    // SfxUInt16Item::QueryValue returns sal_Int32 in Any now... (srx642w)
    // where we still want this to be a sal_Int16
    rVal <<= (sal_Int16)GetValue();
    return sal_True;
}

sal_Bool SwRotationGrf::PutValue( const uno::Any& rVal, sal_uInt8 )
{
    // SfxUInt16Item::QueryValue returns sal_Int32 in Any now... (srx642w)
    // where we still want this to be a sal_Int16
    sal_Int16 nValue = 0;
	if (rVal >>= nValue)
	{
        // sal_uInt16 argument needed
        SetValue( (sal_uInt16) nValue );
		return sal_True;
	}

    DBG_ERROR( "SwRotationGrf::PutValue - Wrong type!" );
	return sal_False;
}

// ------------------------------------------------------------------

SfxPoolItem* SwLuminanceGrf::Clone( SfxItemPool * ) const
{
	return new SwLuminanceGrf( *this );
}

// ------------------------------------------------------------------

SfxPoolItem* SwContrastGrf::Clone( SfxItemPool * ) const
{
	return new SwContrastGrf( *this );
}

// ------------------------------------------------------------------

SfxPoolItem* SwChannelRGrf::Clone( SfxItemPool * ) const
{
	return new SwChannelRGrf( *this );
}

// ------------------------------------------------------------------

SfxPoolItem* SwChannelGGrf::Clone( SfxItemPool * ) const
{
	return new SwChannelGGrf( *this );
}

// ------------------------------------------------------------------

SfxPoolItem* SwChannelBGrf::Clone( SfxItemPool * ) const
{
	return new SwChannelBGrf( *this );
}

// ------------------------------------------------------------------

SfxPoolItem* SwGammaGrf::Clone( SfxItemPool * ) const
{
	return new SwGammaGrf( *this );
}

int	SwGammaGrf::operator==( const SfxPoolItem& rCmp ) const
{
	return SfxPoolItem::operator==( rCmp ) &&
		nValue == ((SwGammaGrf&)rCmp).GetValue();
}

sal_Bool SwGammaGrf::QueryValue( uno::Any& rVal, sal_uInt8 ) const
{
	rVal <<= nValue;
	return sal_True;
}

sal_Bool SwGammaGrf::PutValue( const uno::Any& rVal, sal_uInt8 )
{
	return rVal >>= nValue;
}

// ------------------------------------------------------------------

SfxPoolItem* SwInvertGrf::Clone( SfxItemPool * ) const
{
	return new SwInvertGrf( *this );
}

// ------------------------------------------------------------------

SfxPoolItem* SwTransparencyGrf::Clone( SfxItemPool * ) const
{
	return new SwTransparencyGrf( *this );
}
// ------------------------------------------------------------------
sal_Bool SwTransparencyGrf::QueryValue( uno::Any& rVal,
										sal_uInt8 ) const
{
	DBG_ASSERT(ISA(SfxByteItem),"Put/QueryValue should be removed!");
	sal_Int16 nRet = GetValue();
    DBG_ASSERT( 0 <= nRet && nRet <= 100, "value out of range" );
    rVal <<= nRet;
	return sal_True;
}
// ------------------------------------------------------------------
sal_Bool SwTransparencyGrf::PutValue( const uno::Any& rVal,
										sal_uInt8 )
{
	//temporary conversion until this is a SfxInt16Item!
	DBG_ASSERT(ISA(SfxByteItem),"Put/QueryValue should be removed!");
	sal_Int16 nVal = 0;
	if(!(rVal >>= nVal) || nVal < -100 || nVal > 100)
		return sal_False;
    if(nVal < 0)
    {
        // for compatibility with old documents
        // OD 05.11.2002 #104308# - introduce rounding as for SO 6.0 PP2
        // introduced by fix of #104293#.
        nVal = ( ( nVal * 128 ) - (99/2) ) / 100;
        nVal += 128;
    }
    DBG_ASSERT( 0 <= nVal && nVal <= 100, "value out of range" );
	SetValue(static_cast<sal_uInt8>(nVal));
	return sal_True;
}

// ------------------------------------------------------------------

SfxPoolItem* SwDrawModeGrf::Clone( SfxItemPool * ) const
{
	return new SwDrawModeGrf( *this );
}

sal_uInt16 SwDrawModeGrf::GetValueCount() const
{
	// GRAPHICDRAWMODE_STANDARD = 0,
	// GRAPHICDRAWMODE_GREYS = 1,
	// GRAPHICDRAWMODE_MONO = 2,
	// GRAPHICDRAWMODE_WATERMARK = 3
	return GRAPHICDRAWMODE_WATERMARK + 1;
}

sal_Bool SwDrawModeGrf::QueryValue( uno::Any& rVal,
								sal_uInt8 ) const
{
	drawing::ColorMode eRet = (drawing::ColorMode)GetEnumValue();
	rVal <<= eRet;
	return sal_True;
}

sal_Bool SwDrawModeGrf::PutValue( const uno::Any& rVal,
								sal_uInt8 )
{
	sal_Int32 eVal = SWUnoHelper::GetEnumAsInt32( rVal );
	if(eVal >= 0 && eVal <= GRAPHICDRAWMODE_WATERMARK)
	{
		SetEnumValue((sal_uInt16)eVal);
		return sal_True;
	}
	return sal_False;
}



