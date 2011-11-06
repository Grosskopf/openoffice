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


#include "unotools/unotoolsdllapi.h"

#ifndef _UNOTOOLS_TRANSLITERATIONWRAPPER_HXX
#define _UNOTOOLS_TRANSLITERATIONWRAPPER_HXX
#include <tools/string.hxx>
#include <tools/solar.h>
#include <com/sun/star/i18n/XExtendedTransliteration.hpp>

namespace com { namespace sun { namespace star {
	namespace lang {
		class XMultiServiceFactory;
	}
}}}

namespace utl
{

class UNOTOOLS_DLLPUBLIC TransliterationWrapper
{
	::com::sun::star::uno::Reference<
					::com::sun::star::lang::XMultiServiceFactory > xSMgr;
	::com::sun::star::uno::Reference<
        ::com::sun::star::i18n::XExtendedTransliteration > xTrans;
	::com::sun::star::lang::Locale aLocale;
	sal_uInt32 nType;
	sal_uInt16 nLanguage;
    mutable sal_Bool bFirstCall;

								// not implemented, prevent usage
	TransliterationWrapper( const TransliterationWrapper& );
	TransliterationWrapper&	operator=( const TransliterationWrapper& );

    void loadModuleImpl() const;
    void setLanguageLocaleImpl( sal_uInt16 nLang );

public:
	TransliterationWrapper( const ::com::sun::star::uno::Reference<
					::com::sun::star::lang::XMultiServiceFactory > & xSF,
					sal_uInt32 nType );

	~TransliterationWrapper();

	// get current Locale / Language
	const ::com::sun::star::lang::Locale& getLocale() const	{ return aLocale;}
	sal_uInt16 getLanguage() const { return nLanguage; }

	sal_uInt32 getType() const { return nType; }

	sal_Bool needLanguageForTheMode() const;

    /** set a new language and load the corresponding transliteration module if
        needed for the mode set with nType in the ctor */
    void loadModuleIfNeeded( sal_uInt16 nLang );

    /** Load the transliteration module specified by rModuleName, which has to
        be the UNO service implementation name that is expanded to the full UNO
        service implementation name, for example, "NumToCharKanjiShort_ja_JP"
        expands to
        "com.sun.star.i18n.Transliteration.NumToCharKanjiShort_ja_JP".
        @ATTENTION!
        This method ignores the mode type set with the constructor and
        interferes with the loadModuleIfNeeded() method and the transliterate()
        method that gets a LanguageType passed as parameter.  Using one of
        those may load a different module and overwrite this setting. Only the
        transliterate() method that takes no LanguageType parameter may be used
        for a specific module loaded with this method.  */
    void loadModuleByImplName( const String& rModuleName, sal_uInt16 nLang );

    /** This transliteration method corresponds with the loadModuleByImplName()
        method. It relies on a module being loaded and does not try load one.
        If for any reason the string can't be transliterated the original
        string is returned.  */
	String transliterate( const String& rStr,
						xub_StrLen nStart, xub_StrLen nLen,
						::com::sun::star::uno::Sequence <sal_Int32>* pOffset ) const;

	// Wrapper implementations of class Transliteration
	String transliterate( const String& rStr, sal_uInt16 nLanguage,
						xub_StrLen nStart, xub_StrLen nLen,
						::com::sun::star::uno::Sequence <sal_Int32>* pOffset );

    /** If two strings are equal per this transliteration.
        Returns the number of matched code points in any case, even if strings
        are not equal, for example:
        equals( "a", 0, 1, nMatch1, "aaa", 0, 3, nMatch2 )
        returns false and nMatch:=1 and nMatch2:=1
        equals( "aab", 0, 3, nMatch1, "aaa", 0, 3, nMatch2 )
        returns false and nMatch:=2 and nMatch2:=2
     */
    sal_Bool equals(
        const String& rStr1, sal_Int32 nPos1, sal_Int32 nCount1, sal_Int32& nMatch1,
        const String& rStr2, sal_Int32 nPos2, sal_Int32 nCount2, sal_Int32& nMatch2 ) const;

    sal_Int32 compareSubstring(
        const String& rStr1, sal_Int32 nOff1, sal_Int32 nLen1,
        const String& rStr2, sal_Int32 nOff2, sal_Int32 nLen2 ) const;

    sal_Int32 compareString( const String& rStr1, const String& rStr2 ) const;


    // helpers

    /** If two strings are really equal as per this translation, and not just
        one string is matching the start of the other. Use this method instead
        of compareString()==0 because it is much faster.
     */
    sal_Bool isEqual( const String& rStr1, const String& rStr2 ) const;

    /** If string rStr1 matches the start of string rStr2, i.e. "a" in "aaa"
     */
    sal_Bool isMatch( const String& rStr1, const String& rStr2 ) const;

};

// ............................................................................
}       // namespace utl
// ............................................................................

#endif
