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
#include "precompiled_xmloff.hxx"

#include "xmloff/MarkerStyle.hxx"
#include "xexptran.hxx"
#include <xmloff/attrlist.hxx>
#include <xmloff/nmspmap.hxx>
#include <xmloff/xmluconv.hxx>
#include "xmloff/xmlnmspe.hxx"
#include <xmloff/xmltoken.hxx>
#include <xmloff/xmlexp.hxx>
#include <xmloff/xmlimp.hxx>
#include <rtl/ustrbuf.hxx>
#include <rtl/ustring.hxx>
#include <com/sun/star/drawing/PolyPolygonBezierCoords.hpp>
#include <basegfx/polygon/b2dpolypolygon.hxx>
#include <basegfx/polygon/b2dpolypolygontools.hxx>
#include <basegfx/matrix/b2dhommatrixtools.hxx>

using namespace ::com::sun::star;
using ::rtl::OUString;
using ::rtl::OUStringBuffer;

using namespace ::xmloff::token;


//-------------------------------------------------------------
// Import
//-------------------------------------------------------------

XMLMarkerStyleImport::XMLMarkerStyleImport( SvXMLImport& rImp )
    : rImport( rImp )
{
}

XMLMarkerStyleImport::~XMLMarkerStyleImport()
{
}

sal_Bool XMLMarkerStyleImport::importXML( 
    const uno::Reference< xml::sax::XAttributeList >& xAttrList, 
    uno::Any& rValue, 
    OUString& rStrName )
{
	sal_Bool bHasViewBox    = sal_False;
	sal_Bool bHasPathData   = sal_False;
	OUString aDisplayName;

	SdXMLImExViewBox* pViewBox = NULL;

    SvXMLNamespaceMap& rNamespaceMap = rImport.GetNamespaceMap();
    SvXMLUnitConverter& rUnitConverter = rImport.GetMM100UnitConverter();

	OUString strPathData;

	sal_Int16 nAttrCount = xAttrList.is() ? xAttrList->getLength() : 0;
	for( sal_Int16 i = 0; i < nAttrCount; i++ )
	{
		OUString aStrFullAttrName = xAttrList->getNameByIndex( i );
		OUString aStrAttrName;
		rNamespaceMap.GetKeyByAttrName( aStrFullAttrName, &aStrAttrName );
		OUString aStrValue = xAttrList->getValueByIndex( i );

		if( IsXMLToken( aStrAttrName, XML_NAME ) )
		{
			rStrName = aStrValue;
		} 
		else if( IsXMLToken( aStrAttrName, XML_DISPLAY_NAME ) )
		{
			aDisplayName = aStrValue;
		} 
		else if( IsXMLToken( aStrAttrName, XML_VIEWBOX ) )
		{
			pViewBox = new SdXMLImExViewBox( aStrValue, rUnitConverter );
			bHasViewBox = sal_True;

		} 
		else if( IsXMLToken( aStrAttrName, XML_D ) )
		{
			strPathData = aStrValue;
			bHasPathData = sal_True;
		}
	}

    if( bHasViewBox && bHasPathData )
    {
        basegfx::B2DPolyPolygon aPolyPolygon;

        if(basegfx::tools::importFromSvgD(aPolyPolygon, strPathData, true, 0))
        {
            if(aPolyPolygon.count())
            {
                // ViewBox probably not used, but stay with former processing inside of
                // SdXMLImExSvgDElement
                const basegfx::B2DRange aSourceRange(
                    pViewBox->GetX(), pViewBox->GetY(), 
                    pViewBox->GetX() + pViewBox->GetWidth(), pViewBox->GetY() + pViewBox->GetHeight());
                const basegfx::B2DRange aTargetRange(
                    0.0, 0.0, 
                    pViewBox->GetWidth(), pViewBox->GetHeight());

                if(!aSourceRange.equal(aTargetRange))
                {
                    aPolyPolygon.transform(
                        basegfx::tools::createSourceRangeTargetRangeTransform(
                            aSourceRange,
                            aTargetRange));
                }

                // always use PolyPolygonBezierCoords here
                drawing::PolyPolygonBezierCoords aSourcePolyPolygon;

                basegfx::tools::B2DPolyPolygonToUnoPolyPolygonBezierCoords(
                    aPolyPolygon,
                    aSourcePolyPolygon);
                rValue <<= aSourcePolyPolygon;
            }
        }

        if( aDisplayName.getLength() )
        {
            rImport.AddStyleDisplayName( XML_STYLE_FAMILY_SD_MARKER_ID, rStrName, 
                                        aDisplayName );
            rStrName = aDisplayName;
        }
    }

	if( pViewBox )
		delete pViewBox;

	return bHasViewBox && bHasPathData;
}


//-------------------------------------------------------------
// Export
//-------------------------------------------------------------

#ifndef SVX_LIGHT

XMLMarkerStyleExport::XMLMarkerStyleExport( SvXMLExport& rExp )
    : rExport( rExp )
{
}

XMLMarkerStyleExport::~XMLMarkerStyleExport()
{
}

sal_Bool XMLMarkerStyleExport::exportXML( 
    const OUString& rStrName, 
    const uno::Any& rValue )
{
    sal_Bool bRet(sal_False);
    
    if(rStrName.getLength())
    {
        drawing::PolyPolygonBezierCoords aBezier;

        if(rValue >>= aBezier)
        {
            /////////////////
            // Name
            sal_Bool bEncoded(sal_False);
            OUString aStrName( rStrName );

            rExport.AddAttribute(XML_NAMESPACE_DRAW, XML_NAME, rExport.EncodeStyleName( aStrName, &bEncoded ) );

            if( bEncoded )
            {
                rExport.AddAttribute( XML_NAMESPACE_DRAW, XML_DISPLAY_NAME, aStrName );
            }

            const basegfx::B2DPolyPolygon aPolyPolygon(
                basegfx::tools::UnoPolyPolygonBezierCoordsToB2DPolyPolygon(
                    aBezier));
            const basegfx::B2DRange aPolyPolygonRange(aPolyPolygon.getB2DRange());

            /////////////////
            // Viewbox (viewBox="0 0 1500 1000")

            SdXMLImExViewBox aViewBox(
                aPolyPolygonRange.getMinX(), 
                aPolyPolygonRange.getMinY(),
                aPolyPolygonRange.getWidth(),
                aPolyPolygonRange.getHeight());  
            rExport.AddAttribute( XML_NAMESPACE_SVG, XML_VIEWBOX, aViewBox.GetExportString() );
            
            /////////////////
            // Pathdata
            const ::rtl::OUString aPolygonString(
                basegfx::tools::exportToSvgD(
                    aPolyPolygon,
                    true,           // bUseRelativeCoordinates
                    false,          // bDetectQuadraticBeziers: not used in old, but maybe activated now
                    true));         // bHandleRelativeNextPointCompatible

            // write point array
            rExport.AddAttribute(XML_NAMESPACE_SVG, XML_D, aPolygonString);

            /////////////////
            // Do Write
            SvXMLElementExport rElem( rExport, XML_NAMESPACE_DRAW, XML_MARKER, sal_True, sal_False );
        }
    }
    
    return bRet;
}

#endif // #ifndef SVX_LIGHT
