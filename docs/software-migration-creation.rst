=============================
How to create migration files
=============================

You can create the JSON metadata and binary migration files needed by |EMOD_s| to run simulations
from either CSV or JSON data using Python scripts provided by |IDM_s|. You can assign the same
probability of migration to each individual in a node (vector or human), or you can assign different
migration rates based on age and/or gender (human only).

.. note:: 

    The **IdReference** must match the value in the demographics file. Each node can be connected a
    maximum of 100 destination nodes.

    For both scripts, use one of the following migration types:

    * LOCAL_MIGRATION
    * AIR_MIGRATION
    * REGIONAL_MIGRATION
    * SEA_MIGRATION

For regional migration, you may want to set up migration such that if a node is not part of the
network, the migration of individuals to and from that node considers the closest road hub city. You
can do this by constructing a Voronoi_ tiling based on road hubs, with each non-hub connected to
the hub of its tile.

.. _Voronoi: https://en.wikipedia.org/wiki/Voronoi_diagram

Create from CSV input
=====================

To use the same average migration rate for every individual in a node, create the migration files from a CSV input file. 


#.  Create a CSV file with the following three columns (do not include column headers):

    From_Node_ID
        The origin node, which must match a node ID in the demographics file. You do not need to have the same number of entries for each **From_Node_ID**. 
    To_Node_ID
        The destination node, which must match a node ID in the demographics file.
    Rate
        The average number of trips per day.

#.  Run the `convert_txt_to_bin.py <https://github.com/InstituteforDiseaseModeling/EMOD/blob/master/Scripts/MigrationTools/convert_txt_to_bin.py>`_ script using the command format below::

        python convert_txt_to_bin.py [input-migration-csv] [output-bin] [migration-type] [idreference]


This will create both the metadata and binary file needed by |EMOD_s|. 


Create from JSON input
======================

To vary the average migration rate based on age and/or gender, create the migration files from a JSON input file.

#.  Create a JSON file with the structure described in the sections below.

#.  Run the `convert_json_to_bin.py <https://github.com/InstituteforDiseaseModeling/EMOD/blob/master/Scripts/MigrationTools/convert_json_to_bin.py>`_ script using the command format below::

        python convert_json_to_bin.py [input-json] [output-bin] [migration-type]

This will create both the metadata and binary file needed by |EMOD_s|. 


JSON parameters
---------------

The following parameters are used in the JSON file for migration file generation.  

.. csv-table::
    :header: Parameter, Data type, Description
    :widths: 10, 5, 20

    IdReference, string, The metadata identifier used to generate the NodeID associated with each node in a simulation. The values for IdReference and NodeID must be the same across all input files used in a simulation.
    Interpolation_Type, enum, The method by which to interpolate the age dependent rate data. Accepted values are LINEAR_INTERPOLATION and PIECEWISE_CONSTANT.
    Gender_Data_Type, enum, Whether age data is provided for each gender separately or is the same for both. Accepted values are ONE_FOR_BOTH_GENDERS and ONE_FOR_EACH_GENDER. 
    Ages_Years, array, An array that defines the age bins by which to separate the population and define migration rates. The first value defines the upper bound of a bin starting at zero. 
    Node_Data, JSON object, The structure that contains the migration rate data for each node. 
    From_Node_ID, integer, The origin node for which to define migration rate. 
    Rate_Data, array, The structure that contains migration rate data for a single destination node.  
    To_Node_ID, integer, The destination node for which to define migration rate. 
    Avg_Num_Trips_Per_Day_Both, array, The array that lists the average number of trips per day for each age bin  defined in **AgesYears** (male and female).
    Avg_Num_Trips_Per_Day_Female, array, The array that lists the average number of trips per day for each female age bin defined in **AgesYears**.
    Avg_Num_Trips_Per_Day_Male, array, The array that lists the average number of trips per day for each male age bin defined in **AgesYears**.



Example file
------------

.. literalinclude:: json/migration-input-file.json
   :language: json