
#pragma once

// Product version numbers.
// Definition for version number in the forma of major.minor.revision, 1.2.3
//
// Major version: Major feature release, e.g., Version 1.0 release with major features of Malaria, Polio modeling,
// Minor version: Minor feature release, e.g., Version 1.2 release with particular vector species for Malaria modeling,
// Revision number: Bug fixing release, e.g.,  Version 1.2.3 release with model serialization bug fixing
//
// Note: 1. All numbers could be jumped to indicate significant changes
//       2. The build script will first set these three numbers before start the building process

#define VER_LEN 256

#ifdef __GNUC__
#include <stdint.h>
#endif

class ProgDllVersion
{
public:
    ProgDllVersion();

    virtual ~ProgDllVersion () { };

    uint64_t getMajorVersion() const { return m_nMajor; }
    uint64_t getMinorVersion() const { return m_nMinor; }
    uint64_t getRevisionNumber() const { return m_nRevision; }

    const char* getSccsBranch() const { return m_sSccsBranch; }
    const char* getSccsDate() const { return m_sSccsDate; }
    const char* getBuilderName() const { return m_builderName; }
    const char* getBuildDate() const { return m_sBuildDate; }

    const char* getVersion() const { return m_sVersion; }

    // Check the input version (0.0.0 with build number excluded) against this program's version
    // return 0: exact the same version
    //        1: input version is higher than this program's version
    //       -1: input version is lower than this program's version
    //
    int checkProgVersion(uint8_t nMajor, uint8_t nMinor, uint16_t nRevision) const;
    int checkProgVersion(const char* sVersion) const;

protected:
    bool parseProgVersion(const char* sVersion, uint8_t& maj, uint8_t& min, uint16_t& rev) const;

    void updateVersionString();

private:
    uint64_t m_nMajor;
    uint64_t m_nMinor;
    uint64_t m_nRevision;

    char m_sBuildDate[VER_LEN];
    char m_builderName[VER_LEN];
    char m_sSccsBranch[VER_LEN];
    char m_sSccsDate[VER_LEN];

    char m_sVersion[VER_LEN];

    static uint32_t combineVersion( uint8_t nMajor, uint8_t nMinor, uint16_t nRevision );
};
