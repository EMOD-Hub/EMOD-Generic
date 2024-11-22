=================================
Demographic summary output report
=================================

The demographic summary output report is a JSON-formatted file with the demographic
:term:`channel` output results of the simulation, consisting of simulation-wide averages
by time step. The format is identical to the inset chart output report, except the channels
reflect demographic categories, such as gender ratio. The file name is DemographicsSummary.json.

To generate the demographics summary report, set the **Enable_Demographics_Reporting** configuration
parameter to 1. The binned output report will also be generated.

The file contains a header and a channels section.

Header
======

The header section contains the following parameters.

.. csv-table::
   :header: Parameter, Data type, Description
   :widths: 10,5,15

   DateTime, string, The time stamp indicating when the report was generated.
   DTK_Version, string, The version of |EMOD_s| used.
   Report_Type, string, The type of output report.
   Report_Version, string, The format version of the report.
   Start_Time, integer, The time noted in days when the simulation begins.
   Simulation_Timestep, integer, The number of days in each time step.
   Timesteps, integer, The number of time steps in this simulation.
   Channels, integer, The number of channels in the simulation.


Channels
========

The channels section contains the following parameters.

.. csv-table::
   :header: Parameter, Data type, Description
   :widths: 7,5,10

   <Channel_Title>, string, The title of the particular demographic channel.
   Units, string, The units used for this channel.
   Data, array, A list of the channel data at each time step.

Example
=======

The following is a sample of a DemographicsSummary.json file.

.. literalinclude:: json/report-demographic.json
   :language: json