====================
Binned output report
====================

The binned output report is a JSON-formatted file where the channel data has been sorted into bins.
It is very similar to an inset chart. However, with the binned report, channels are broken down into
sub-channels (bins) based on various criteria. For example, instead of having a single prevalence
channel, you might have prevalence in the "0-3 years old bin" and the "4-6 years old bin, and so
forth. Currently the age bins are set and not editable by the user. The file name is BinnedReport.json.

To generate the binned report, set the **Enable_Demographics_Reporting** configuration parameter
to 1. The demographics summary output report will also be generated.

The file contains a header and a channels section.

Header
======

The header section contains the following parameters.

.. csv-table::
   :header: Parameter, Data type, Description
   :widths: 7, 7, 20

   DateTime, string, The time stamp indicating when the report was generated.
   DTK_Version, string, The version of |EMOD_s| used.
   Report_Type, string, The type of output report.
   Report_Version, string, The format version of the report.
   Timesteps, integer, The number of time steps in this simulation.
   Channels, integer, The number of channels in the simulation.
   Subchannel_Metadata, nested JSON object, "Metadata that describes the bins and axis information. The metadata includes the following parameters:

   .. list-table::
      :header-rows: 1

      * - Parameter
        - Data type
        - Description
      * - AxisLabels
        - array of strings
        - The name of each axis.
      * - NumBinsPerAxis
        - array of integers
        - The number of bins per axis.
      * - ValuesPerAxis
        - array of integers
        - The maximum value for each bin in the axis. For example, if the axis is ""Age"", the values are age maximums in days.
      * - MeaningPerAxis
        - array of strings
        - The ""meaning"" of each element in **ValuesPerAxis**. For example, if the axis is ""Age"", this shows those values binned by age range, such as younger than 5 years, 5 to 9, and so on.

    "

Channels
========

The channels section contains the following parameters.

.. csv-table::
   :header: Parameter, Data type, Description
   :widths: 7, 5, 7

   <Channel_Title>, string, The title of the particular channel.
   Units, string, The units used for this channel.
   Data, array, A list of the channel data at each time step.

Example
=======

The following is a sample of an BinnedReport.json file.

.. literalinclude:: json/report-binned.json
   :language: json
