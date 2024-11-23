==================================================================
Human migration tracking report (ReportHumanMigrationTracking.csv)
==================================================================

The human migration tracking report is a CSV-formatted report that provides details about human
travel during simulations. The finished report will provide one line for each surviving individual
that migrates during the simulation.

The report name is ReportHumanMigrationTracking.csv, and is produced via the libhumanmigrationtracking.dll
file. See :doc:`software-custom-reporter` for information on using :term:`dynamic link library (DLL)`
files.



Configuration
=============

There are no special parameters that need to be configured to generate the report. However, the
simulation must have migration enabled (see the migration parameters in :doc:`parameter-configuration`).


Report structure and channel descriptions
=========================================

The file contains the following data channels:

.. csv-table::
    :header: Data channel, Data type, Description
    :widths: 8, 5, 20

    Time, integer, The day that an individual leaves their current node.
    IndividualID, integer, The ID of the migrating individual.
    AgeYears, integer, The age (in years) of the migrating individual.
    Gender, string, The gender of the migrating individual (M or F).
    IsAdult, enum, "Possible values are T or F; T (true) if individual's age is greater than the configuration parameter **Minimum_Adult_Age_Years.**"
    Home_NodeID, string, The external ID of the node the individual considers to be their 'home' node.
    From_NodeID, string, The external ID of the node the individual is moving from.
    To_NodeID, string, the external ID of the node the individual is moving to.
    MigrationType, enum, "Possible values are air, local, sea, regional, local, family, intervention; the type of migration that is occurring."
    Event, enum, "Possible values are Emigrating or SimulationEnd. Emigrating means the individual is moving, SimulationEnd means the simulation has finished."


Example
=======

The following is an example of ReportHumanMigrationTracking.csv.

.. csv-table::
    :header-rows: 1
    :file: reporthumanmigration.csv


