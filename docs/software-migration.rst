===============
Migration files
===============

Migration files describe the rate of migration of individuals *out* of a geographic :term:`node`.
There are additional configuration parameters you can set to define the migration patterns and
return time. There are four types of migration files that can be used by |EMOD_s|, namely, local
migration, regional migration, air migration and sea migration files. The demographics file can be
configured to exclude some nodes from certain types of travel.

Local migration describes foot travel into adjacent nodes. Regional migration describes migration
that occurs on a road or rail network. Air migration describes migration that occurs by plane; you
must indicate that a node has an airport for air migration from that node to occur. Sea migration
describes migration that occurs by ship travel; you must indicate that a node has a seaport for sea
migration from that node to occur. For both air and sea migration, it's possible to originate in a
node with an airport or seaport and migrate to a node without one, but the reverse is not true.
Unlike the other migration files, the sea migration file only contains information for the nodes
that are seaports.

To use the migration files, in the configuration file you must set **Migration_Model** to a valid
migration type and indicate the path to the migration files you want to use. You must also set a
parameter to enable each type of migration you want to include in the simulation. There are
additional parameters in the configuration file you can use to scale or otherwise modify the data
included in the migration files. The migration rate can be differentially set by age and/or gender.
Additionally, a modifier can be applied for the migration rates to follow a distribution curve in
the population. For more information, see :doc:`parameter-configuration-migration` parameters.

Migration data is contained in a set of two files, a metadata file with header information and a
binary data file. Both files are required. To create these files see, :doc:`software-migration-creation`. 

JSON metadata file
==================

The metadata file is a JSON-formatted file that includes a metadata section and a node offsets
section. The **Metadata** section contains a JSON object with parameters, some of which are
strictly  informational and some of which are used by |exe_s|. However, the informational ones may
still be important to understand the provenance and meaning of the data.

Parameters
----------

The following parameters can be included in the migration metadata file:

.. csv-table::
    :header: Parameter, Data type, Description
    :widths: 10,5,20

    AgesYears, array, An array that defines the age bins by which to separate the population and define migration rates.
    Author, string, The author of the file.
    DatavalueCount, integer, "(Used by |EMOD_s|.) The number of outbound data values per node (max 100). The number must be the same across every node in the binary file. If you are using an older file that does not include this parameter, each migration type has the following maximum data values:

        * LOCAL_MIGRATION: 8
        * REGIONAL_MIGRATION: 30
        * AIR_MIGRATION: 60
        * SEA_MIGRATION: 5
    "    
    DateCreated, string, The day the file was created.
    GenderDataType, enum, Whether age data is provided for each gender separately or is the same for both. Accepted values are ONE_FOR_BOTH_GENDERS and ONE_FOR_EACH_GENDER.
    IdReference, string, "(Used by |EMOD_s|.) A unique, user-selected string that indicates the method used by |EMOD_s| for generating **NodeID** values in the input files. For more information, see :doc:`software-inputs`."
    InterpolationType, enum, The method by which to interpolate the age-dependent rate data. Accepted values are LINEAR_INTERPOLATION and PIECEWISE_CONSTANT.
    MigrationType, enum, "The type of migration the data describes. Accepted values are: 

        * LOCAL_MIGRATION
        * AIR_MIGRATION
        * REGIONAL_MIGRATION
        * SEA_MIGRATION
    "    
    NodeCount, integer, (Used by |EMOD_s|.) The number of nodes to expect in this file.
    NodeOffsets, string, "(Used by |EMOD_s|.) A string that is **NodeCount** :math:`\times` 16 characters long. For each node, the first 8 characters are the origin **NodeID** in hexadecimal. The second 8 characters are the byte offset in hex to the location in the binary file where the destination **NodeIDs** and migration rates appear."
    Tool, string, The script used to create the file.

Example
-------

.. literalinclude:: json/migration-metadata.json
   :language: json

Binary file
===========

The binary file contains the migration rate data. Migration rate determines the average time until an
individual takes a trip out of the node. This time is drawn from an exponential distribution with
the parameter :math:`\lambda` as the number of trips per day. Therefore, a migration rate of 0.1 can
be viewed as 10 days until migration, on average. You can adjust this base rate using the 
:doc:`parameter-configuration-scalars` parameters.

The data in the binary file is laid out in a sequential stream of 4-byte integers that identify the origin and destination nodes followed by a stream of 8-byte floats that contain the migration rate for those node pairs laid out in the same order. Therefore, the length of the stream is defined by **DatavalueCount**. For each source node, there must be **DatavalueCount** :math:`\times` (4 bytes + 8 bytes). 

The following image shows how a binary file with a **DatavalueCount** value of 8 would be laid out. 

.. image:: images/file-structure/localMigrationBFF.jpg

.. toctree::
 
    software-migration-creation
