
#include "stdafx.h"
#include "PairFormationParamsFactory.h"
#include "PairFormationParametersImpl.h"

namespace Kernel {

    const IPairFormationParameters* PairFormationParamsFactory::Create( RelationshipType::Enum relType,
                                                                        const Configuration* pConfig )
    {
        return PairFormationParametersImpl::CreateParameters( relType, pConfig );
    }
}
