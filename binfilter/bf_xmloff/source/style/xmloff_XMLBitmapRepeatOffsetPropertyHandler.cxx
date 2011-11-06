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




#ifndef _XMLOFF_XMLUCONV_HXX 
#include "xmluconv.hxx"
#endif

#ifndef _RTL_USTRBUF_HXX_ 
#include <rtl/ustrbuf.hxx>
#endif


#ifndef _XMLOFF_XMLBITMAPREPEATOFFSETPROPERTYHANDLER_HXX
#include "XMLBitmapRepeatOffsetPropertyHandler.hxx"
#endif
namespace binfilter {

using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::rtl;

using ::binfilter::xmloff::token::GetXMLToken;
using ::binfilter::xmloff::token::XML_VERTICAL;
using ::binfilter::xmloff::token::XML_HORIZONTAL;


XMLBitmapRepeatOffsetPropertyHandler::XMLBitmapRepeatOffsetPropertyHandler( sal_Bool bX )
:	mbX( bX ),
	msVertical( GetXMLToken(XML_VERTICAL) ),
	msHorizontal( GetXMLToken(XML_HORIZONTAL) )
{
}

XMLBitmapRepeatOffsetPropertyHandler::~XMLBitmapRepeatOffsetPropertyHandler() 
{
}

sal_Bool XMLBitmapRepeatOffsetPropertyHandler::importXML(
	const OUString& rStrImpValue,
	Any& rValue,
	const SvXMLUnitConverter& rUnitConverter ) const
{ 
	SvXMLTokenEnumerator aTokenEnum( rStrImpValue );
	OUString aToken;
	if( aTokenEnum.getNextToken( aToken ) )
	{
		sal_Int32 nValue;
		if( SvXMLUnitConverter::convertPercent( nValue, aToken ) )
		{
			if( aTokenEnum.getNextToken( aToken ) )
			{
				if( ( mbX && ( aToken == msHorizontal ) ) || ( !mbX && ( aToken == msVertical ) ) )
				{
					rValue <<= nValue;
					return sal_True;
				}
			}
		}
	}

	return sal_False;
	
}

sal_Bool XMLBitmapRepeatOffsetPropertyHandler::exportXML( 
	OUString& rStrExpValue,
	const Any& rValue,
	const SvXMLUnitConverter& rUnitConverter ) const
{ 
	OUStringBuffer aOut;
	
	sal_Int32 nValue;
	if( rValue >>= nValue )
	{
		SvXMLUnitConverter::convertPercent( aOut, nValue );
		aOut.append( sal_Unicode( ' ' ) );
		aOut.append( mbX ? msHorizontal : msVertical );
		rStrExpValue = aOut.makeStringAndClear();

		return sal_True;
	}

	return sal_False;
}

}//end of namespace binfilter
