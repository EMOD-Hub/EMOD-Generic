=====================================================
Node demographics report (ReportNodeDemographics.csv)
=====================================================

The node demographics report is a CSV-formatted report that provides population information stratified
by node. For each time step, the report will collect data on each node and age bin.

The report name is ReportNodeDemographics.csv, and is produced via the libReportNodeDemographics.dll
file. See :doc:`software-custom-reporter` for information on using :term:`dynamic link library (DLL)`
files.



Configuration
=============

The following parameters need to be configured to generate the report:

.. csv-table::
    :header: Parameter name, Data type, Min, Max, Default, Description
    :widths: 8, 5, 5, 5, 5, 20

    **IP_Key_To_Collect**, string, NA, NA, (empty string), "The name of the IndividualProperty key to stratify by; an empty string implies 'do not stratify by IP.'"
    **Age_Bins**, Array of integers, 0, MAX_HUMAN_AGE, "[40, 80, 125]", "The Age Bins (in years) to aggregate within and report; an empty array implies 'do not stratify by age.'"
    **Stratify_By_Gender**, boolean, 0, 1, 1, "Set to true (1) to stratify by gender; a value of 0 will not stratify by gender."



Output file data
================

The report will contain the following output data, divided between stratification columns and data
columns.

Stratification columns
----------------------

.. csv-table::
    :header: Parameter, Data type, Description
    :widths: 8, 5, 20

    Time, integer, The day of the simulation that the data was collected.
    NodeID, string, The External ID of the node for the data in the row in the report.
    Gender, enum, "Possible values are M or F; the gender of the individuals in the row in the report."
    AgeYears, integer, The max age in years of the bin for the individuals in the row in the report.
    IndividualProp, string, The value of the IP for the individuals in the row in the report.


Data columns
------------

.. csv-table::
    :header: Parameter, Data type, Description
    :widths: 8, 5, 20

    NumIndividuals, integer, The number of individuals that meet the stratification values.
    NumInfected, integer, The number of infected individuals that meet the stratification values and are infected.



Example
=======


The following is an example of ReportNodeDemographics.csv.

.. csv-table::
    :header-rows: 1
    :file: report-node-demographics.csv


