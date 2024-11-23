===================
Campaign parameters
===================

The parameters described in this reference section can be added to the :term:`JSON (JavaScript Object Notation)` formatted :term:`campaign file` to determine the interventions that are used to
control the spread of disease and where, when, how, and to whom the interventions are distributed.
Additionally, the campaign file specifies the details of the outbreak of the disease itself. For
more conceptual background on how to create a campaign file, see :doc:`model-campaign`.

In the :term:`configuration file`, you must set the **Enable_Interventions** parameter to 1 and set
the **Campaign_Filename** parameter to the name of the campaign file for the interventions to take
place. The campaign file must be in the same directory as the configuration file.

The campaign file is hierarchically organized into logical groups of parameters that can have
arbitrary levels of nesting. It contains an **Events** array of campaign events, each of which
contains a JSON object describing the event coordinator, which in turn contains a nested JSON object
describing the intervention. At the same level as the **Events** array is the boolean
**Use_Defaults** to indicate whether or not to use the default values for required parameters that
are not included in the file. It is common to include JSON keys for campaign name, event name, or
intervention name; these are informational only and not used by |EMOD_s|.

The skeletal JSON example below illustrates the basic file structure (this does not include all
required parameters for each object). Note that the nested JSON elements have been organized to best
illustrate their hierarchy, but that many files in the |EMOD_s| GitHub
repository list the parameters and nested objects differently.

.. code-block:: json

    {
        "Campaign_Name": "Campaign example",
        "Use_Defaults": 1,
        "Events": [{
            "Event_Name": "The first event",
            "class": "CampaignEvent",
            "Event_Coordinator_Config": {
                "class": "StandardInterventionDistributionEventCoordinator",
                "Intervention_Config": {
                    "Intervention_Name": "The vaccine",
                    "class": "SimpleVaccine"
                }
            }
        }, {
            "Event_Name": "The second event",
            "class": "CampaignEvent",
            "Event_Coordinator_Config": {
                "class": "StandardInterventionDistributionEventCoordinator",
                "Intervention_Config": {
                    "Intervention_Name": "The disease outbreak",
                    "class": "OutbreakIndividual"
                }
            }
        }]
    }


.. _Regression: https://github.com/InstituteforDiseaseModeling/EMOD/tree/master/Regression

.. _Regression/defaults: https://github.com/InstituteforDiseaseModeling/EMOD/tree/master/Regression/defaults

The topics below contain only parameters available when using the generic :term:`simulation type`.


.. toctree::
   :maxdepth: 2

   parameter-campaign-event-coordinators
   parameter-campaign-node-interventions
   parameter-campaign-individual-interventions
   parameter-campaign-waningeffects
   parameter-campaign-event-list

