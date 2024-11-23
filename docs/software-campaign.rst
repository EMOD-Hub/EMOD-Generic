=============
Campaign file
=============


The :term:`campaign file` is an optional :term:`JSON (JavaScript Object Notation)` file that
distributes outbreaks and contains all parameters that define the collection of interventions that
make up a disease control or eradication :term:`campaign`. For example, campaign parameters can
control the following:

* Size and location of outbreaks
* Target demographic (age, location, gender, etc.) for interventions
* Diagnostic tests to use
* The cost and timing of interventions

Much of the power and flexibility of |EMOD_s| comes from the customizable
campaign interventions. Briefly, campaigns are created through the hierarchical organization of
parameter groups. It is hierarchically organized into logical groups of parameters that can have
arbitrary levels of nesting. Typically, the file is named campaign.json. The relative path to this
file is specified by **Campaign_Filename** in the configuration file.

To distribute an intervention, you must configure the following nested JSON objects:

campaign event
    .. include:: reuse/campaign-event-overview.txt 
event coordinator
    .. include:: reuse/campaign-ec-overview.txt 
individual-level intervention
    .. include:: reuse/campaign-individual-intervention-overview.txt
node-level intervention
    .. include:: reuse/campaign-node-intervention-overview.txt


:doc:`model-campaign` describes some ways to configure a campaign file to target individuals with
particular characteristics, repeat interventions, and more. See :doc:`parameter-campaign` for a
comprehensive list and description of all parameters available to use in the campaign file for this
simulation type.


.. _EMOD scenarios: https://github.com/InstituteforDiseaseModeling/docs-emod-scenarios/releases


.. toctree::

   software-campaign-overlay