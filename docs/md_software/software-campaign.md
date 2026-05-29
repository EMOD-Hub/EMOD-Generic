# Campaign file

The **campaign file** is an optional **JSON (JavaScript Object Notation)** file that
distributes outbreaks and contains all parameters that define the collection of interventions that
make up a disease control or eradication **campaign**. For example, campaign parameters can
control the following:

* Size and location of outbreaks
* Target demographic (age, location, gender, etc.) for interventions
* Diagnostic tests to use
* The cost and timing of interventions

Much of the power and flexibility of EMOD comes from the customizable
campaign interventions. Briefly, campaigns are created through the hierarchical organization of
parameter groups. It is hierarchically organized into logical groups of parameters that can have
arbitrary levels of nesting. Typically, the file is named campaign.json. The relative path to this
file is specified by **Campaign_Filename** in the configuration file.

To distribute an intervention, you must configure the following nested JSON objects:

## campaign event

Campaign events determine *when* and *where* an intervention is distributed during a campaign. "When"
can be the number of days after the beginning of the simulation or at a point during a particular
calendar year. "Where" is the geographic **node** or nodes in which the event takes place.

## event coordinator

Event coordinators are nested within the campaign event JSON object and determine *who* receives the
intervention. "Who" is determined by filtering on age, gender, or on the individual properties
configured in the demographics files, such as risk group or sociodemographic category. See [model-properties](../md_model/model-properties.md).

## individual-level intervention

Individual-level interventions determine *what* will be distributed to individuals to reduce the
spread of a disease. For example, distributing vaccines or drugs are individual-level interventions.
In the schema, these are labeled as **IndividualTargeted**.

It is also possible (but not required) to configure *why* a particular intervention is distributed
by adding trigger conditions to the intervention. For example, interventions can be triggered by
notifications broadcast after some an event, such as Births (the individual’s own
birth), GaveBirth, NewInfectionEvent, and more. It's also possible to have one intervention trigger
another intervention by asking the first intervention to broadcast a unique string, and having the
second intervention be triggered upon receipt of that string. See [parameter-campaign-event-list](../md_parameter/parameter-campaign-event-list.md).

Individual-level interventions can be used as part of configuring a cascade of care along with the individual
properties set in the demographics file. Use **Disqualifying_Properties** to disqualify individuals
who would otherwise receive the intervention and **New_Property_Value** to assign a new value when
the intervention is received. For example, you can assign a property value after receiving the
first-line treatment for a disease and prevent anyone from receiving the second-line treatment
unless they have that property value and are still symptomatic.

## node-level intervention

Node-level interventions determine *what* will be distributed to each **node** to reduce the spread of a
disease. For example, spraying larvicide in a village to kill mosquito larvae is a node-level malaria
intervention. Sometimes this can be an intermediate intervention that schedules another
intervention. Node-level disease outbreaks are also configured as "interventions". In the schema,
these are labeled as **NodeTargeted**.

It is also possible (but not required) to configure *why* a particular intervention is distributed
by adding trigger conditions to the intervention. For example, interventions can be triggered by
notifications broadcast after some an event, such as Births, NewInfectionEvent, and more. It's also
possible to have one intervention trigger another intervention by asking the first intervention to
broadcast a unique string, and having the second intervention be triggered upon receipt of that
string. See [parameter-campaign-event-list](../md_parameter/parameter-campaign-event-list.md).

[model-campaign](../md_model/model-campaign.md) describes some ways to configure a campaign file to target individuals with particular characteristics, repeat interventions, and more. See [parameter-campaign](../md_parameter/parameter-campaign.md) for a
comprehensive list and description of all parameters available to use in the campaign file for this
simulation type.
