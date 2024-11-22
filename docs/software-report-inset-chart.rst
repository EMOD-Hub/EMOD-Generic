=========================
Inset chart output report
=========================

The inset chart output report is output with every simulation. It is a JSON-formatted file with the
:term:`channel` output results of the simulation, consisting of simulation-wide averages
by time step. The channels in an inset chart are fully specified by the simulation type and cannot
be altered without making changes to the |EMOD_s| source code. The file name is InsetChart.json. You
can use the information contained in the file to create a chart, but |EMOD_s| does not automatically
output a chart.

The file contains a header and a channels section.

Header
======

The header section contains the following parameters.

.. csv-table::
   :header: Parameter, Data type, Description
   :widths: 8, 5, 10

   DateTime, string, The time stamp indicating when the report was generated.
   DTK_Version, string, The version of |EMOD_s| used.
   Report_Type, string, The type of output report.
   Report_Version, string, The format version of the report.
   Start_Time, integer, The time noted in days when the simulation begins.
   Simulation_Timestep, integer, The number of days in each time step.
   Timesteps, integer, The number of time steps in this simulation.
   Channels, integer, The number of channels in the simulation.

Example
=======

The following is a sample of an InsetChart.json file for a simulation with five time steps.

.. literalinclude:: json/report-insetchart.json
   :language: json

Channels
========

The channels section contains the following parameters.

.. csv-table::
   :header: Parameter, Data type, Description
   :widths: 8, 5, 10

   <Channel_Title>, string, The title of the particular channel.
   Units, string, The units used for this channel.
   Data, array, A list of the channel data at each time step.



When running generic simulations, the following channels are included in the InsetChart.json file.

.. csv-table::
   :header: Channel, Description
   :widths: 5, 8

   Births, The live births cumulative up to that day.
   Campaign Cost, The cost of campaigns cumulative up to that day (set by the **Cost_To_Consumer** parameter).
   Daily (Human) Infection Rate, The per-person daily rate of human infection (Human Infectious Reservoir divided by Statistical Population).
   Disease Deaths, "The number of individuals whose death is attributed to disease, cumulative up to that day."
   Exposed Population, The fraction of the population that is currently infected but not contagious.
   Human Infectious Reservoir, "The total node infectivity, after modifications by the infectious reservoir, seasonality, climate, etc. (if present). The expected number of infections distributed via the contact route, absent immunity or interventions."
   Infected, "The fraction of the population currently infected."
   Infectious Population, "The fraction of the population that is currently infected and contagious."
   New Infections, The number of individuals that got infected on that day.
   Newly Symptomatic, The number individuals that became symptomatic on that day.
   Recovered Population, The fraction of the population that is currently not infected and has zero susceptibility.
   Statistical Population, The total number of individuals in the simulation on that day.
   Susceptible Population, The fraction of the population that is currently not infected and is maximally susceptible.
   Symptomatic Population, The total number of individuals currently experiencing symptoms.
   Waning Population, "The fraction of the population that is currently not infected, has non-zero susceptibility, but is less than maximally susceptible."



