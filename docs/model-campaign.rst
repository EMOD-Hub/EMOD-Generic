==================
Creating campaigns
==================

You define the initial disease outbreak and interventions used to combat it for a simulation
through a JSON-formatted campaign file, typically called campaign.json. It is hierarchically
organized into logical groups of parameters that can have arbitrary levels of nesting. It contains
an **Events** array of campaign events, each of which contains a JSON object describing the event
coordinator, which in turn contains a nested JSON object describing the intervention. Campaign
events determine *when* and *where* an intervention is distributed, event coordinators determine
*who* receives the intervention, and interventions determine *what* is actually distributed. For
example, a vaccination or diagnostic test.

Interventions can be targeted to particular nodes or individuals, based on age or other
characteristics. Additionally, you can structure campaigns to guide individuals through complex
health care systems. For example, administering a second-line treatment only after the preferred
treatment has proven ineffective for an individual.

For some interventions, there can be a very complex hierarchical structure, including recursion.
This framework enables rigorous testing of possible control strategies to determine which events or
combination of events will best aid in the elimination of disease for specific geographic locations.

Multiple interventions
======================

When creating multiple interventions, either of the same type or different types, they will
generally be distributed independently without regard to whether a person has already received another
intervention.

For example, say you create two **SimpleBednet** interventions and both interventions
have **Demographic_Coverage** set to 0.5 (50% demographic coverage). This value is the probability
that each individual in the target population will receive the intervention. It does not guarantee
that the exact fraction of the target population set by **Demographic_Coverage** receives the
intervention.

By default, each individual in the simulation will have a 50% chance of receiving a bednet in both
of the distributions and the two distributions will be independent. Therefore, each individual has a
75% chance of receiving at least one bednet.

.. image:: images/general/howto-multiple.png

Campaign file overview
======================

For the interventions to take place, the campaign file must be in the same directory as the
:term:`configuration file` and you must set the configuration parameters **Enable_Interventions** to
1 and **Campaign_Filename** to the name of the campaign file. When you run a simulation, you must
have a single campaign file. However, you can use a campaign overlay file that includes certain
parameters of interest that will override the settings in a base file; these files must be flattened
into a single file before running a simulation. See :doc:`software-campaign-overlay` for more information
flattening two campaign files.

Although you can create campaign files entirely from scratch, it is often easier to start from an
existing campaign file and modify it to meet your needs. The simplest method is to edit the
parameters or parameter values in the JSON file in any text editor. You may also create or modify
the files using a scripting language, as with configuration files. See :doc:`software-configuration-script`
for a couple examples.

The following is an example of campaign file that has two events (SimpleVaccine and Outbreak) that
occur in all nodes at day 1 and day 30, respectively. Each event contains an event coordinator that
describes who receives the intervention (everyone, with the vaccine repeated three times) and the
configuration for the intervention itself. Note that the nested JSON elements have been organized to
best illustrate their hierarchy, but that many files in the Regression directory list the parameters
within the objects differently. See :doc:`parameter-campaign` for more information on the structure
of these files and available parameters for this simulation type.

.. literalinclude:: json/howto-generic-campaign-flat-full.json
   :language: json
   :lines: 1, 2, 4-7, 9-36, 38-59

.. lines is included here to remove the VACCINATION labels that are used for the overlay file


For a complete list of campaign parameters that are available to use with this simulation type and
more detail about the campaign file structure, see :doc:`parameter-campaign`. For more information about
JSON, see :doc:`parameter-overview`.



.. toctree::
   :maxdepth: 3
   :titlesonly:

   model-targeted-interventions
   model-vaccination
   model-care-cascade
