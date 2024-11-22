======================================
Health events and interventions report
======================================

The health events and interventions report is a CSV-formatted output file which provides information
on each individual's events and interventions, such as Birth, Pregnant, DiseaseDeath,
HIVSymptomatic, etc. The filename is ReportEventRecorder.csv.

To generate the report, set the configuration parameter **Report_Event_Recorder** to 1. Note that
this file is modifiable; to see the list of parameters that will add new data channels to the report,
see :doc:`parameter-configuration-output`.

With this report, it is possible to customize which events are reported. You may list the specific
events to record, or you may record all events EXCEPT those listed. Use the configuration parameter
**Report_Event_Recorder_Events** to list events. If the configuration parameter
**Report_Event_Recorder_Ignore_Events_In_List** is set to 0, then only the events listed in
**Report_Event_Recorder_Events** will be reported. If it is set to 1, then all events EXCEPT those
listed will be reported.


Report structure and data channel descriptions
==============================================

The file contains the following data channels:

.. csv-table::
    :header: Data channel, Data type, Description
    :widths: 10, 5, 20

    Year, float, "The time of the event in units of calendar year, including fractions of years up to two decimal places."
    Node_ID, integer, "The identification number of the node."
    Event_Name, string, "The event being logged. If **Report_Event_Recorder_Ignore_Events_In_List** is set to 0, then the event name will be one of the ones listed under **Report_Event_Recorder_Events**. Otherwise, it will be the name of any other event that occurs and is not listed under **Report_Event_Recorder_Events**."
    Individual_ID, integer, The individual's unique identifying number
    Age, integer, "The age of the individual in units of days. Divide by 365 to obtain age in years."
    Gender, boolean, "Identifies the individual's gender; 0 is assigned to males, 1 is assigned to females."
    Infected, boolean, "Describes whether the individual is infected or not; 0 when not infected, 1 for infected."
    Infectiousness, float, "The per-act probability of transmission, including intrahost factors like disease stage and ART, but excluding condoms."
    "<custom channels>", string, "If any **IndividualProperties** are listed under the config.json array **Report_Event_Recorder_Individual_Properties**, they will appear as additional columns between the Infectiousness and HasHIV columns of the CSV file."
    HasHIV, string, "This column is included for HIV simulations only. N if the individual is not infected, Y if the individual is infected with HIV."
    OnART, string, "This column is included for HIV simulations only. N if the individual is not on ART, Y if the individual is on ART."
    CD4, float, "The current CD4 count, regardless of when CD4 testing was performed."
    WHO_Stage, float, "The individual's WHO stage, linearly interpolated between integer values. Round down to obtain the integer value for the WHO clinical stage. Uninfected individuals will be assigned a value of -1."
    Intervention_Status, string, "The individual's intervention status."


In addition to the default channels listed above, it is possible to list whether an individual has
been assigned **IndividualProperties**. To do so, ad the name of the property to the array for the
configuration parameter **Report_Event_Recorder_Individual_Properties**.


Example
=======

The following is an example of a ReportEventRecorder.csv report:

.. csv-table::
    :file: reporteventrecorder.csv