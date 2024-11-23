==============================================
Event counter report (ReportEventCounter.json)
==============================================

The event counter report is a JSON-formatted file that keeps track of how many of each event types
occurs during a time step. The report produced is similar to the InsetChart.json channel report,
where there is one channel for each event defined in the configuration file (config.json).

The output file is named ReportEventCounter.json, and is produced via the libreporteventcounter.dll
file. See :doc:`software-custom-reporter` for information on using  using :term:`dynamic link library (DLL)`
files.



Configuration
=============

The following parameters need to be configured to generate the report:

.. csv-table::
    :header: Parameter name, Data type, Min, Max, Default, Description
    :widths: 8, 5, 5, 5, 5, 20

    **Start_Day**, float, 0, 3.40E+38, 0, The day to start collecting data for the report.
    **Duration_Days**, float, 0, 3.40E+38, 3.40E+38, The duration of simulation days over which to report events.
    **Report_Description**, string, NA, NA, NA, "Name of the report (it augments the filename of the report). If multiple CSV reports are being generated, this allows the user to distinguish one report from another."
    **Nodeset_Config**, JSON object, NA, NA, NA, An object that specifies in which nodes the interventions will be distributed.
    **Event_Trigger_List**, list of strings, NA, NA, NA, The list of event triggers for the events included in the report.

.. none of the above parameters had actual descriptions, so I guessed. Will need to have them checked!



Header
======

The header section contains the following parameters:

.. csv-table::
    :header: Parameter, Data type, Description
    :widths: 8, 5, 20

    Channels, integer, "The number of entries in the 'Channels' map (e.g. the number of events that the report is counting)."
    DTK_Version, string, The version of |EMOD_s| that was used.
    DateTime, string, The date and time the report was created.
    Report_Type, string, "The type of report created (it should always be InsetChart/Channel Report)."
    Report_Version, string, The version of the report format.
    Simulation_Timestep, integer, The number of days in one time step of the simulation.
    Timesteps, integer, "The number of time steps recorded in the file, Each channel should have this number of entries."



Channels
========

The channels section contains the following parameters:

.. csv-table::
    :header: Parameter, Data type, Description
    :widths: 8, 5, 20

    <Event Names>, string, The name of the event.
    Data, array, An array of event counts where each entry is the number of events that occurred during the timestep.
    Units, string, "Empty string, but it is the 'event count'."



Example
=======

The following is an example of ReportEventCounter.json.

.. literalinclude:: json/report-event-counter.json
    :language: json
