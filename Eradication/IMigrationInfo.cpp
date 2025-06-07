
#include "stdafx.h"
#include "IMigrationInfo.h"
#include "Migration.h"

#ifndef DISABLE_VECTOR
#include "MigrationInfoVector.h"
#endif

SETUP_LOGGING( "IMigrationInfo" )

namespace Kernel
{
    IMigrationInfoFactory* ConstructMigrationInfoFactory( const std::string& idreference,
                                                          SimType::Enum sim_type,
                                                          bool useDefaultMigration,
                                                          int defaultTorusSize )
    {
        IMigrationInfoFactory* p_mif = nullptr;

        if( (sim_type == SimType::VECTOR_SIM) || (sim_type == SimType::MALARIA_SIM) || (sim_type == SimType::DENGUE_SIM) )
        {
#ifndef DISABLE_VECTOR
            if( useDefaultMigration )
            {
                p_mif = new MigrationInfoFactoryVectorDefault( defaultTorusSize );
            }
            else
            {
                p_mif = new MigrationInfoFactoryVector();
            }
#endif
        }
        else
        {
            if( useDefaultMigration )
            {
                p_mif = new MigrationInfoFactoryDefault( defaultTorusSize );
            }
            else
            {
                p_mif = new MigrationInfoFactoryFile();
            }
        }

        if(p_mif)
        {
            p_mif->Initialize( idreference );
        }

        return p_mif;
    }
}
