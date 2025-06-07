
#pragma once

#include "OutbreakIndividual.h"

namespace Kernel
{
    class OutbreakIndividualMalaria : public OutbreakIndividual
    {
        IMPLEMENT_DEFAULT_REFERENCE_COUNTING()
        DECLARE_FACTORY_REGISTERED( IndividualIVFactory, OutbreakIndividualMalaria, IDistributableIntervention )

    public:
        OutbreakIndividualMalaria();
        virtual ~OutbreakIndividualMalaria();

        virtual bool Configure( const Configuration * inputJson ) override;

    protected:
        std::vector<std::string> m_GenomeMarkerNames;
    };
}
