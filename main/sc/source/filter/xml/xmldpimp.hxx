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


#ifndef SC_XMLDPIMP_HXX
#define SC_XMLDPIMP_HXX

#include <xmloff/xmlictxt.hxx>
#include <xmloff/xmlimp.hxx>
#include <com/sun/star/sheet/DataPilotFieldReference.hpp>
#include <com/sun/star/sheet/DataPilotFieldSortInfo.hpp>
#include <com/sun/star/sheet/DataPilotFieldAutoShowInfo.hpp>
#include <com/sun/star/sheet/DataPilotFieldLayoutInfo.hpp>

#include "global.hxx"
#include "dpobject.hxx"
#include "dpsave.hxx"
#include "queryparam.hxx"

#include <hash_set>

class ScXMLImport;
class ScDPSaveNumGroupDimension;
class ScDPSaveGroupDimension;

enum ScMySourceType
{
	SQL,
	TABLE,
	QUERY,
	SERVICE,
	CELLRANGE
};

class ScXMLDataPilotTablesContext : public SvXMLImportContext
{

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotTablesContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
										::com::sun::star::xml::sax::XAttributeList>& xAttrList);

	virtual ~ScXMLDataPilotTablesContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotTableContext : public SvXMLImportContext
{
    typedef ::std::hash_set< ::rtl::OUString, ::rtl::OUStringHash > StringSet;
    StringSet       maHiddenMemberFields;

    struct GrandTotalItem
    {
        ::rtl::OUString maDisplayName;
        bool            mbVisible;
        GrandTotalItem();
    };
	ScDocument*		pDoc;
	ScDPObject*		pDPObject;
	ScDPSaveData*	pDPSave;
    ScDPDimensionSaveData* pDPDimSaveData;
    GrandTotalItem  maRowGrandTotal;
    GrandTotalItem  maColGrandTotal;
	rtl::OUString	sDataPilotTableName;
	rtl::OUString	sApplicationData;
	rtl::OUString	sDatabaseName;
	rtl::OUString	sSourceObject;
	rtl::OUString	sServiceName;
	rtl::OUString	sServiceSourceName;
	rtl::OUString	sServiceSourceObject;
	rtl::OUString	sServiceUsername;
	rtl::OUString	sServicePassword;
	rtl::OUString	sButtons;
	ScRange			aSourceCellRangeAddress;
	ScRange			aTargetRangeAddress;
	ScRange			aFilterSourceRange;
	ScAddress		aFilterOutputPosition;
	ScQueryParam	aSourceQueryParam;
	ScMySourceType	nSourceType;
    sal_uInt32      mnRowFieldCount;
    sal_uInt32      mnColFieldCount;
    sal_uInt32      mnPageFieldCount;
    sal_uInt32      mnDataFieldCount;
	sal_Bool		bIsNative;
	sal_Bool		bIgnoreEmptyRows;
	sal_Bool		bIdentifyCategories;
	sal_Bool		bUseRegularExpression;
	sal_Bool		bIsCaseSensitive;
	sal_Bool		bSkipDuplicates;
	sal_Bool		bFilterCopyOutputData;
	sal_Bool		bTargetRangeAddress;
	sal_Bool		bSourceCellRange;
    sal_Bool        bShowFilter;
    sal_Bool        bDrillDown;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotTableContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
										::com::sun::star::xml::sax::XAttributeList>& xAttrList);

	virtual ~ScXMLDataPilotTableContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();

    void SetGrandTotal(::xmloff::token::XMLTokenEnum eOrientation, bool bVisible, const ::rtl::OUString& rDisplayName);
	void SetDatabaseName(const rtl::OUString& sValue) { sDatabaseName = sValue; }
	void SetSourceObject(const rtl::OUString& sValue) { sSourceObject = sValue; }
	void SetNative(const sal_Bool bValue) { bIsNative = bValue; }
	void SetServiceName(const rtl::OUString& sValue) { sServiceName = sValue; }
	void SetServiceSourceName(const rtl::OUString& sValue) { sServiceSourceName = sValue; }
	void SetServiceSourceObject(const rtl::OUString& sValue) { sServiceSourceObject = sValue; }
	void SetServiceUsername(const rtl::OUString& sValue) { sServiceUsername = sValue; }
	void SetServicePassword(const rtl::OUString& sValue) { sServicePassword = sValue; }
	void SetSourceCellRangeAddress(const ScRange& aValue) { aSourceCellRangeAddress = aValue; bSourceCellRange = sal_True; }
	void SetSourceQueryParam(const ScQueryParam& aValue) { aSourceQueryParam = aValue; }
//	void SetFilterUseRegularExpressions(const sal_Bool bValue) { aSourceQueryParam.bRegExp = bValue; }
	void SetFilterOutputPosition(const ScAddress& aValue) { aFilterOutputPosition = aValue; }
	void SetFilterCopyOutputData(const sal_Bool bValue) { bFilterCopyOutputData = bValue; }
	void SetFilterSourceRange(const ScRange& aValue) { aFilterSourceRange = aValue; }
//	void SetFilterIsCaseSensitive(const sal_Bool bValue) { aSourceQueryParam.bCaseSens = bValue; }
//	void SetFilterSkipDuplicates(const sal_Bool bValue) { aSourceQueryParam.bDuplicate = !bValue; }
	void AddDimension(ScDPSaveDimension* pDim, bool bHasHiddenMember);
    void AddGroupDim(const ScDPSaveNumGroupDimension& aNumGroupDim);
    void AddGroupDim(const ScDPSaveGroupDimension& aGroupDim);
	void SetButtons();
};

class ScXMLDPSourceSQLContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDPSourceSQLContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLDPSourceSQLContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDPSourceTableContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDPSourceTableContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLDPSourceTableContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDPSourceQueryContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDPSourceQueryContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLDPSourceQueryContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLSourceServiceContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLSourceServiceContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLSourceServiceContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotGrandTotalContext : public SvXMLImportContext
{
    enum Orientation { COLUMN, ROW, BOTH, NONE };

    ScXMLImport& GetScImport();

    ScXMLDataPilotTableContext* mpTableContext;
    ::rtl::OUString             maDisplayName;
    Orientation                 meOrientation;
    bool                        mbVisible;
    
public:
    ScXMLDataPilotGrandTotalContext( 
        ScXMLImport& rImport, sal_uInt16 nPrefix, const ::rtl::OUString& rLName,
        const ::com::sun::star::uno::Reference< 
            ::com::sun::star::xml::sax::XAttributeList>& xAttrList,
        ScXMLDataPilotTableContext* pTableContext );

    virtual ~ScXMLDataPilotGrandTotalContext();

    virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
                                     const ::rtl::OUString& rLocalName,
                                     const ::com::sun::star::uno::Reference<
                                        ::com::sun::star::xml::sax::XAttributeList>& xAttrList );

    virtual void EndElement();
};

class ScXMLSourceCellRangeContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLSourceCellRangeContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLSourceCellRangeContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

struct ScXMLDataPilotGroup
{
    ::std::vector<rtl::OUString> aMembers;
    rtl::OUString aName;
};

class ScXMLDataPilotFieldContext : public SvXMLImportContext
{
	ScXMLDataPilotTableContext*	pDataPilotTable;
	ScDPSaveDimension*			pDim;

    ::std::vector<ScXMLDataPilotGroup> aGroups;
    rtl::OUString               sGroupSource;
    rtl::OUString               sSelectedPage;
    rtl::OUString               sName;
    double                      fStart;
    double                      fEnd;
    double                      fStep;
	sal_Int32					nUsedHierarchy;
    sal_Int32                   nGroupPart;
	sal_Int16					nFunction;
	sal_Int16					nOrientation;
	sal_Bool					bShowEmpty:1;
    sal_Bool                    bSelectedPage:1;
    sal_Bool                    bIsGroupField:1;
    sal_Bool                    bDateValue:1;
    sal_Bool                    bAutoStart:1;
    sal_Bool                    bAutoEnd:1;
    bool                        mbHasHiddenMember:1;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotFieldContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotTableContext* pDataPilotTable);

	virtual ~ScXMLDataPilotFieldContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();

	void SetShowEmpty(const sal_Bool bValue) { if (pDim) pDim->SetShowEmpty(bValue); }
	void SetSubTotals(const sal_uInt16* pFunctions, const sal_Int16 nCount) { if(pDim) pDim->SetSubTotals(nCount, pFunctions); }
    void AddMember(ScDPSaveMember* pMember);
    void SetSubTotalName(const ::rtl::OUString& rName);
    void SetFieldReference(const com::sun::star::sheet::DataPilotFieldReference& aRef) { if (pDim) pDim->SetReferenceValue(&aRef); }
    void SetAutoShowInfo(const com::sun::star::sheet::DataPilotFieldAutoShowInfo& aInfo) { if (pDim) pDim->SetAutoShowInfo(&aInfo); }
    void SetSortInfo(const com::sun::star::sheet::DataPilotFieldSortInfo& aInfo) { if (pDim) pDim->SetSortInfo(&aInfo); }
    void SetLayoutInfo(const com::sun::star::sheet::DataPilotFieldLayoutInfo& aInfo) { if (pDim) pDim->SetLayoutInfo(&aInfo); }
    void SetGrouping(const rtl::OUString& rGroupSource, const double& rStart, const double& rEnd, const double& rStep, 
        sal_Int32 nPart, sal_Bool bDate, sal_Bool bAutoSt, sal_Bool bAutoE)
    {
        bIsGroupField = sal_True;
        sGroupSource = rGroupSource;
        fStart = rStart;
        fEnd = rEnd;
        fStep = rStep;
        nGroupPart = nPart;
        bDateValue = bDate;
        bAutoStart = bAutoSt;
        bAutoEnd = bAutoE;
    }
    void AddGroup(const ::std::vector<rtl::OUString>& rMembers, const rtl::OUString& rName);
};

class ScXMLDataPilotFieldReferenceContext : public SvXMLImportContext
{
//    com::sun::star::sheet::DataPilotFieldReference aReference;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotFieldReferenceContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotFieldReferenceContext();
};

class ScXMLDataPilotLevelContext : public SvXMLImportContext
{
	ScXMLDataPilotFieldContext*	pDataPilotField;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotLevelContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotLevelContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotDisplayInfoContext : public SvXMLImportContext
{
//    com::sun::star::sheet::DataPilotFieldAutoShowInfo aInfo;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotDisplayInfoContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotDisplayInfoContext();
};

class ScXMLDataPilotSortInfoContext : public SvXMLImportContext
{
//    com::sun::star::sheet::DataPilotFieldSortInfo aInfo;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotSortInfoContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotSortInfoContext();
};

class ScXMLDataPilotLayoutInfoContext : public SvXMLImportContext
{
//    com::sun::star::sheet::DataPilotFieldLayoutInfo aInfo;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotLayoutInfoContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotLayoutInfoContext();
};

class ScXMLDataPilotSubTotalsContext : public SvXMLImportContext
{
	ScXMLDataPilotFieldContext* pDataPilotField;

	sal_Int16	nFunctionCount;
	sal_uInt16*	pFunctions;
    ::rtl::OUString maDisplayName;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotFieldContext* GetDataPilotField() { return pDataPilotField; }

	ScXMLDataPilotSubTotalsContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotSubTotalsContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
	void AddFunction(sal_Int16 nFunction);
    void SetDisplayName(const ::rtl::OUString& rName);
};

class ScXMLDataPilotSubTotalContext : public SvXMLImportContext
{
	ScXMLDataPilotSubTotalsContext*	pDataPilotSubTotals;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotSubTotalContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotSubTotalsContext* pDataPilotSubTotals);

	virtual ~ScXMLDataPilotSubTotalContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotMembersContext : public SvXMLImportContext
{
	ScXMLDataPilotFieldContext* pDataPilotField;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotMembersContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotMembersContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotMemberContext : public SvXMLImportContext
{
	ScXMLDataPilotFieldContext*	pDataPilotField;

	rtl::OUString sName;
    rtl::OUString maDisplayName;
	sal_Bool	bDisplay;
	sal_Bool	bDisplayDetails;
    sal_Bool    bHasName;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotMemberContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotMemberContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotGroupsContext : public SvXMLImportContext
{
	ScXMLDataPilotFieldContext*	pDataPilotField;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotGroupsContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotGroupsContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

class ScXMLDataPilotGroupContext : public SvXMLImportContext
{
	ScXMLDataPilotFieldContext*	pDataPilotField;

	rtl::OUString sName;
    ::std::vector<rtl::OUString> aMembers;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotGroupContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotFieldContext* pDataPilotField);

	virtual ~ScXMLDataPilotGroupContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();

    void AddMember(const rtl::OUString& sMember) { aMembers.push_back(sMember); }
};

class ScXMLDataPilotGroupMemberContext : public SvXMLImportContext
{
	ScXMLDataPilotGroupContext*	pDataPilotGroup;

	rtl::OUString sName;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:

	ScXMLDataPilotGroupMemberContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
						const ::rtl::OUString& rLName,
						const ::com::sun::star::uno::Reference<
						::com::sun::star::xml::sax::XAttributeList>& xAttrList,
						ScXMLDataPilotGroupContext* pDataPilotGroup);

	virtual ~ScXMLDataPilotGroupMemberContext();

	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
									 const ::rtl::OUString& rLocalName,
									 const ::com::sun::star::uno::Reference<
									  	::com::sun::star::xml::sax::XAttributeList>& xAttrList );

	virtual void EndElement();
};

#endif

