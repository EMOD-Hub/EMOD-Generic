/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#include "stdafx.h"

#include "TransmissionGroupsBase.h"
#include "Exceptions.h"
#include "StrainIdentity.h"
#include "Log.h"

SETUP_LOGGING( "TransmissionGroupsBase" )

namespace Kernel
{

    TransmissionGroupsBase::TransmissionGroupsBase()
        : propertyNameToMatrixMap()
        , propertyValueToIndexMap()
    {

    }

    void TransmissionGroupsBase::checkForDuplicatePropertyName( const string& property ) const
    {
        if (propertyNameToMatrixMap.find(property) != propertyNameToMatrixMap.end())
        {
            throw GeneralConfigurationException(__FILE__, __LINE__, __FUNCTION__, "Duplicated property name.");
        }
    }

    void TransmissionGroupsBase::checkForValidValueListSize( const PropertyValueList_t& values ) const
    {
        if (values.size() == 0)
        {
            throw GeneralConfigurationException(__FILE__, __LINE__, __FUNCTION__, "Empty list of property values.");
        }
    }

    void TransmissionGroupsBase::checkForValidScalingMatrixSize( const ScalingMatrix_t& scalingMatrix, const PropertyValueList_t& values ) const
    {
        int matrixSize = scalingMatrix.size();
        int valueCount = values.size();

        // scalingMatrix must be values.size() x values.size()

        if (matrixSize != valueCount)
        {
            throw GeneralConfigurationException(__FILE__, __LINE__, __FUNCTION__, "Scaling matrix size (rows) doesn't match property value count.");
        }

        for (int iRow = 0; iRow < scalingMatrix.size(); iRow++)
        {
            if (scalingMatrix[iRow].size() != valueCount)
            {
                throw GeneralConfigurationException(__FILE__, __LINE__, __FUNCTION__, "Scaling matrix size (columns) doesn't match property value count.");
            }
        }
    }

    void TransmissionGroupsBase::addScalingMatrixToPropertyToMatrixMap( const string& property, const ScalingMatrix_t& scalingMatrix )
    {
        propertyNameToMatrixMap[property] = scalingMatrix;
    }

    void TransmissionGroupsBase::CheckForValidStrainListSize( const StrainIdentitySet_t& strains ) const
    {
        // We need at least one strain, even if it's a dummy.
        if (strains.size() == 0)
        {
            throw GeneralConfigurationException(__FILE__, __LINE__, __FUNCTION__, "Strain list is not allowed to be empty.");
        }
    }

    void TransmissionGroupsBase::InitializeCumulativeMatrix(ScalingMatrix_t& cumulativeMatrix)
    {
        cumulativeMatrix.clear();

        // [[1.0]]
        MatrixRow_t row(1, 1.0f);           // one element == 1.0f
        cumulativeMatrix.push_back(row);    // one row
    }

    void TransmissionGroupsBase::addPropertyValuesToValueToIndexMap( const string& propertyName, const PropertyValueList_t& valueSet, int currentMatrixSize )
    {
        ValueToIndexMap_t valueToIndexMap;
        int valueIndex = 0;
        for (auto& value : valueSet)
        {
            valueToIndexMap[value] = valueIndex * currentMatrixSize;
            valueIndex++;
        }

        propertyValueToIndexMap[propertyName] = valueToIndexMap;
    }

    void TransmissionGroupsBase::getGroupIndicesForProperty( const IPKeyValue& property_value, const PropertyToValuesMap_t& propertyNameToValuesMap, std::vector<size_t>& indices )
    {
        const string& key = property_value.GetKeyAsString();

        indices.clear();
        indices.push_back( 0 );
        // propertyNameToValuesMap is the list of properties to be considered.
        // Iterate over these properties for the following:
        // If the property is the one passed in, use the propertyValueToIndexMap to get the index offset for the value passed in.
        // Else, it is a different property, add indices for each possible value of this property with its offset.
        // E.g. single property (RISK:HIGH|MEDIUM|LOW), property_value is "RISK:LOW":
        //  indices ends up holding a single index, 2, which will fetch the contagion deposited to the RISK:LOW group.
        // E.g. multiple properties  (RISK:HIGH|MEDIUM|LOW) & (GEOGRAPHY:BOTHELL|BELLEVUE), property_value is "GEOGRAPHY:BOTHELL"
        //  indices ends up holding three index values, 0, 1, and 2, which correspond to HIGH-BOTHELL, MEDIUM-BOTHELL, and LOW-BOTHELL.
        for (auto& entry : propertyNameToValuesMap)
        {
            // entry.first == property name
            // entry.second == values list
            if (entry.first == key)
            {
                // For every value in indices, _add_ the offset to the property value
                size_t offset = propertyValueToIndexMap.at(key).at(property_value.GetValueAsString());
                for (auto& index : indices)
                {
                    index += offset;
                }
            }
            else
            {
                std::vector<size_t> temp(indices);
                indices.clear();
                for (auto& value : entry.second)
                {
                    size_t offset = propertyValueToIndexMap.at(entry.first).at(value);
                    for (size_t index : temp)
                    {
                        indices.push_back(index + offset);
                    }
                }
            }
        }

        return;
    }

    void TransmissionGroupsBase::AggregatePropertyMatrixWithCumulativeMatrix( const ScalingMatrix_t& propertyMatrix, ScalingMatrix_t& cumulativeMatrix )
    {
        int propertyMatrixSize = propertyMatrix.size();
        int currentMatrixSize  = cumulativeMatrix.size();
        MatrixRow_t emptyRow(propertyMatrixSize * currentMatrixSize, 0.0f);
        ScalingMatrix_t aggregateMatrix(propertyMatrixSize * currentMatrixSize, emptyRow);

        for (int propSource = 0; propSource < propertyMatrix.size(); propSource++)
        {
            for (int yB = 0; yB < cumulativeMatrix.size(); yB++)
            {
                for (int propSink = 0; propSink < propertyMatrix[propSource].size(); propSink++)
                {
                    for (int xB = 0; xB < cumulativeMatrix[yB].size(); xB++)
                    {
                        int xResult = (propSource * cumulativeMatrix.size()) + xB;
                        int yResult = (propSink   * cumulativeMatrix.size()) + yB;
                        aggregateMatrix[yResult][xResult] = propertyMatrix[propSource][propSink] * cumulativeMatrix[yB][xB];
                    }
                }
            }
        }

        cumulativeMatrix = aggregateMatrix;
    }

    BEGIN_QUERY_INTERFACE_BODY(TransmissionGroupsBase::ContagionPopulationImpl)
    END_QUERY_INTERFACE_BODY(TransmissionGroupsBase::ContagionPopulationImpl)

    TransmissionGroupsBase::ContagionPopulationImpl::ContagionPopulationImpl( IStrainIdentity * strain, float quantity )
    : contagionQuantity(quantity)
    , cladeId( strain->GetCladeID() )
    {
        LOG_DEBUG_F( "Creating contagion population with clade id %d and quantity %f\n", cladeId, contagionQuantity );
    }

    int TransmissionGroupsBase::ContagionPopulationImpl::GetCladeID() const
    {
        return cladeId;
    }

    int TransmissionGroupsBase::ContagionPopulationImpl::GetGeneticID() const
    {
        // Never valid code path, have to implement this method due to interface.
        throw IllegalOperationException( __FILE__, __LINE__, __FUNCTION__, "Not valid for ContagionPopulationImpl" );
    }

    float TransmissionGroupsBase::ContagionPopulationImpl::GetTotalContagion( void ) const
    {
        return contagionQuantity;
    }

    void TransmissionGroupsBase::ContagionPopulationImpl::ResolveInfectingStrain( IStrainIdentity* strainId ) const
    {
        strainId->SetCladeID(cladeId);
        strainId->SetGeneticID(0);
    }

    act_prob_vec_t TransmissionGroupsBase::DiscreteGetTotalContagion( void )
    {
       throw NotYetImplementedException( __FILE__, __LINE__, __FUNCTION__, 
            "The use of DiscreteGetTotalContagion is not supported in \"GENERIC_SIM\".  \
             To use discrete transmission, please use a simulation type derived from either \
             \"STI_SIM\" or \"HIV_SIM\"." ); 
    }
}
