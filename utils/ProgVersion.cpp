#include "stdafx.h"

#include <iostream>
#include <string.h>

#include "ProgVersion.h"

// See the definition of version info
#include "version_info.h"
#include "Log.h"

SETUP_LOGGING( "ProgVersion" )

// for unsafe usage of sprintf
#pragma warning(disable: 4996)

ProgDllVersion::ProgDllVersion()
{
    m_nMajor = MAJOR_VERSION;
    m_nMinor = MINOR_VERSION;
    m_nRevision = REVISION_NUMBER;

    strncpy( m_sBuildDate, BUILD_DATE, VER_LEN );
    strncpy( m_builderName, BUILDER_NAME, VER_LEN );
    strncpy( m_sSccsBranch, SCCS_BRANCH, VER_LEN );
    strncpy( m_sSccsDate, SCCS_DATE, VER_LEN );
    for( int i=0; i<VER_LEN; i++ )
    {
        if( m_sSccsDate[i] == '_' )
            m_sSccsDate[i] = ' ';
    }

    updateVersionString();
}

void ProgDllVersion::updateVersionString()
{
    sprintf( m_sVersion, "%Iu.%Iu.%Iu", m_nMajor, m_nMinor, m_nRevision);
}

int ProgDllVersion::checkProgVersion(uint8_t nMajor, uint8_t nMinor, uint16_t nRevision) const
{
    int ret = 0;

    uint32_t num = ProgDllVersion::combineVersion( nMajor, nMinor, nRevision );
    uint32_t nVersion = ProgDllVersion::combineVersion( static_cast<uint8_t>( m_nMajor ), static_cast<uint8_t>( m_nMinor ), static_cast<uint16_t>( m_nRevision ) );

    if (num < nVersion)
        ret = -1;
    else if (num > nVersion)
        ret = 1;
     return ret;
}

int ProgDllVersion::checkProgVersion(const char* sVersion) const
{
    // -2 for error
    int ret = -2;
    uint8_t maj, min;
    uint16_t rev;
    if (parseProgVersion(sVersion, maj, min, rev))
    {
        ret = checkProgVersion(maj, min, rev);
    }
    return ret;
}

uint32_t ProgDllVersion::combineVersion( uint8_t nMajor, uint8_t nMinor, uint16_t nRevision )
{
    return (((nMajor) << 24) | ((nMinor) << 16) | (nRevision) << 8);
}

bool ProgDllVersion::parseProgVersion(const char* sVersion, uint8_t& maj, uint8_t& min, uint16_t& rev) const
{
    maj = 0;
    min = 0;
    rev = 0;

    char sTemp[VER_LEN];
    strncpy(sTemp, sVersion, VER_LEN);
    char* str = strchr(sTemp, '.');
    if (!str) return false;

    str[0] = '\0';
    maj = atoi(sTemp);

    char* str1 = strchr(str+1, '.');
    if (!str1) return false;

    str1[0] = '\0';
    min = atoi(str+1);

    char* str2 = strchr(str1+1, '.');
    if (!str2) return false;

    str2[0] = '\0';
    rev = atoi(str1+1);

    return true;
}

#pragma warning (default: 4996)
