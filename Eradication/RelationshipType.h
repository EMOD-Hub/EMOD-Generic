
#pragma once

#include "SimulationEnums.h"

namespace Kernel
{
    std::set<std::string> GetAllowableRelationshipTypes();
    std::vector<RelationshipType::Enum> GetRelationshipTypes();
    std::vector<RelationshipType::Enum> ConvertStringsToRelationshipTypes( const std::string& rParamName,
                                                                           const std::vector<std::string>& rStrings );
}
