#**************************************************************
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
#**************************************************************



$(eval $(call gb_Library_Library,editeng))

$(eval $(call gb_Library_add_package_headers,editeng,editeng_inc))

$(eval $(call gb_Library_add_precompiled_header,editeng,$(SRCDIR)/editeng/inc/pch/precompiled_editeng))

$(eval $(call gb_Library_set_include,editeng,\
	$$(INCLUDE) \
	-I$(SRCDIR)/editeng/inc/pch \
    -I$(SRCDIR)/editeng/inc \
))

$(eval $(call gb_Library_set_defs,editeng,\
	$$(DEFS) \
	-DEDITENG_DLLIMPLEMENTATION \
))

ifneq ($(strip $(EDITDEBUG)),)
$(eval $(call gb_Library_set_defs,editeng,\
	$$(DEFS) \
	-DEDITDEBUG \
))
endif

$(eval $(call gb_Library_add_api,editeng,\
	udkapi \
	offapi \
))

$(eval $(call gb_Library_add_exception_objects,editeng,\
    editeng/inc/pch/precompiled_editeng \
    editeng/source/accessibility/AccessibleComponentBase \
    editeng/source/accessibility/AccessibleContextBase \
    editeng/source/accessibility/AccessibleEditableTextPara \
    editeng/source/accessibility/AccessibleHyperlink \
    editeng/source/accessibility/AccessibleImageBullet \
    editeng/source/accessibility/AccessibleParaManager \
    editeng/source/accessibility/AccessibleSelectionBase \
    editeng/source/accessibility/AccessibleStaticTextBase \
    editeng/source/accessibility/AccessibleStringWrap \
    editeng/source/editeng/editattr \
    editeng/source/editeng/editdbg \
    editeng/source/editeng/editdoc \
    editeng/source/editeng/editdoc2 \
    editeng/source/editeng/editeng \
    editeng/source/editeng/editobj \
    editeng/source/editeng/editsel \
    editeng/source/editeng/editundo \
    editeng/source/editeng/editview \
    editeng/source/editeng/edtspell \
    editeng/source/editeng/eehtml \
    editeng/source/editeng/eeng_pch \
    editeng/source/editeng/eeobj \
    editeng/source/editeng/eerdll \
    editeng/source/editeng/eertfpar \
    editeng/source/editeng/impedit \
    editeng/source/editeng/impedit2 \
    editeng/source/editeng/impedit3 \
    editeng/source/editeng/impedit4 \
    editeng/source/editeng/impedit5 \
    editeng/source/editeng/textconv \
    editeng/source/items/bulitem \
    editeng/source/items/charhiddenitem \
    editeng/source/items/flditem \
    editeng/source/items/frmitems \
    editeng/source/items/itemtype \
    editeng/source/items/numitem \
    editeng/source/items/optitems \
    editeng/source/items/paperinf \
    editeng/source/items/paraitem \
    editeng/source/items/svdfield \
    editeng/source/items/svxfont \
    editeng/source/items/textitem \
    editeng/source/items/writingmodeitem \
    editeng/source/items/xmlcnitm \
    editeng/source/misc/acorrcfg \
    editeng/source/misc/edtdlg \
    editeng/source/misc/forbiddencharacterstable \
    editeng/source/misc/hangulhanja \
    editeng/source/misc/splwrap \
    editeng/source/misc/svxacorr \
    editeng/source/misc/SvXMLAutoCorrectExport \
    editeng/source/misc/SvXMLAutoCorrectImport \
    editeng/source/misc/swafopt \
    editeng/source/misc/txtrange \
    editeng/source/misc/unolingu \
    editeng/source/outliner/outleeng \
    editeng/source/outliner/outlin2 \
    editeng/source/outliner/outliner \
    editeng/source/outliner/outlobj \
    editeng/source/outliner/outlundo \
    editeng/source/outliner/outlvw \
    editeng/source/outliner/outl_pch \
    editeng/source/outliner/paralist \
    editeng/source/rtf/rtfgrf \
    editeng/source/rtf/rtfitem \
    editeng/source/rtf/svxrtf \
    editeng/source/uno/unoedhlp \
    editeng/source/uno/unoedprx \
    editeng/source/uno/unoedsrc \
    editeng/source/uno/unofdesc \
    editeng/source/uno/unofield \
    editeng/source/uno/UnoForbiddenCharsTable \
    editeng/source/uno/unofored \
    editeng/source/uno/unoforou \
    editeng/source/uno/unoipset \
    editeng/source/uno/unonrule \
    editeng/source/uno/unopracc \
    editeng/source/uno/unotext \
    editeng/source/uno/unotext2 \
    editeng/source/uno/unoviwed \
    editeng/source/uno/unoviwou \
    editeng/source/xml/xmltxtexp \
    editeng/source/xml/xmltxtimp \
))

# add libraries to be linked to editeng; again these names need to be given as
# specified in Repository.mk
$(eval $(call gb_Library_add_linked_libs,editeng,\
    xo \
    basegfx \
    lng \
    svt \
    tk \
    vcl \
    svl \
    stl \
    sot \
    utl \
    tl \
    comphelper \
    ucbhelper \
    cppuhelper \
    cppu \
    vos3 \
    sal \
    icuuc \
    i18nisolang1 \
    i18npaper \
	$(gb_STDLIBS) \
))

# vim: set noet sw=4 ts=4:

