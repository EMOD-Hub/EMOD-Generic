==========
What’s new
==========

This topic describes new functionality and breaking changes for recently released versions of
|EMOD_l|.


.. contents:: Contents
   :local:

|EMOD_s| v2.20
==============

The |EMOD_s| v2.20 release includes support for typhoid disease modeling, including new campaign classes: **EnvironmentalDiagnostic**, **TyphoidCarrierDiagnostic**, **TyphoidVaccine**, and **TyphoidWASH**. 

**ImmunityBloodTest** was added for identifying whether an individual’s immunity meets a specified threshold and then broadcasts an event based on the results. This new campaign class can be used with all supported disease modeling sim types. 

**OutbreakIndividualTBorHIV** extends **OutbreakIndividual** and allows for specifying HIV or a specific strain of infection for TB. 

In addition, configuration and campaign parameters that set the type of distribution (uniform, Gaussian, etc.) of infectiousness, incubation period, and delivery of interventions have been refactored. The number of distributions available and naming conventions used are now consistent across the configuration and campaign files. This change does not affect the distributions used in the demographics files.

A beta release of new campaign classes (not yet fully tested) are included to support surveillance of events, where events are listened to, detected, and broadcast when a threshold has been met. These classes include: **BroadcastCoordinatorEvent**, **BroadcastNodeEvent**, **DelayEventCoordinator**,  **SurveillanceEventCoordinator**, and **TriggeredEventCoordinator**.

.. raw:: html

    <button class="collapse-table-button btn btn-info">Collapse all parameter tables</button>

New configuration parameters
----------------------------

For the generic simulation type, the following new configuration parameters are available:

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/220-config-generic.csv


New configuration parameters (Distribution)
-------------------------------------------

Note: These configuration parameters are part of the refactoring of distribution parameters.

For the generic simulation type, the following new configuration parameters are available:

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/220dist-config-generic.csv


New configuration parameters (Beta)
-----------------------------------

Note: These configuration parameters are currently in beta release and have not yet been fully tested.

For the generic simulation type, the following new configuration parameters are available:

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/220beta-config-generic.csv

New demographics parameters
---------------------------

In all simulation types, you can now specify the quantity-per-timestep added to the total infectivity present in a node; it is equivalent to the expected number of additional infections in a node, per timestep. For example, if timestep is equal to a day, then setting **InfectivityReservoirSize** to a value of 0.1 would introduce an infection every 10 days from the exogenous reservoir.

For more information, see the table below.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/220-demo-generic.csv

New campaign parameters
-----------------------

The following campaign classes are new and can be used in the (specified) models:

ImmunityBloodTest (generic)
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **ImmunityBloodTest** intervention class identifies whether an individual’s immunity meets a specified threshold (as set with the **Positive_Threshold_AcquisitionImmunity** campaign parameter) and then broadcasts an event based on the results; positive has immunity while negative does not.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-immunitybloodtest.csv


New campaign parameters (Beta)
------------------------------

.. note:: 

    These campaign classes and associated parameters are currently in beta release and have not yet been fully tested.

BroadcastCoordinatorEvent (generic)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **BroadcastCoordinatorEvent** coordinator class broadcasts the event you specify. This can be used with the campaign class, **SurveillanceEventCoordinator**, that can monitor and listen for events received from **BroadcastCoordinatorEvent** and then perform an action based on the broadcasted event. You can also use this for the reporting of the broadcasted events by setting the configuraton parameters, **Report_Node_Event_Recorder** and **Report_Surveillance_Event_Recorder**, which listen to events to be recorded.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-broadcastcoordinatorevent.csv

BroadcastNodeEvent (generic)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **BroadcastNodeEvent** coordinator class broadcasts node events. This can be used with the campaign class, **SurveillanceEventCoordinator**, that can monitor and listen for events received from **BroadcastNodeEvent** and then perform an action based on the broadcasted event. You can also use this for the reporting of the broadcasted events by setting the configuraton parameters, **Report_Node_Event_Recorder** and **Report_Surveillance_Event_Recorder**, which listen to events to be recorded.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-broadcastnodeevent.csv

DelayEventCoordinator (generic)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **DelayEventCoordinator** coordinator class insert delays into coordinator event chains. This campaign event is typically used with **BroadcastCoordinatorEvent** to broadcast events after the delays.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-delayeventcoordinator.csv

SurveillanceEventCoordinator (generic)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **SurveillanceEventCoordinator** coordinator class listens for and detects events happening and then responds with broadcasted events when a threshold has been met. This campaign event is typically used with other classes, such as **BroadcastCoordinatorEvent**, **TriggeredEventCoordinator**, and **DelayEventCoordinator**.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-surveillanceeventcoordinator.csv

TriggeredEventCoordinator (generic)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **TriggeredEventCoordinator** coordinator class listens for trigger events, begins a series of repetitions of intervention distributions, and then broadcasts an event upon completion. This campaign event is typically used with other classes that broadcast and distribute events, such as **BroadcastCoordinatorEvent**, **DelayEventCoordinator**, and **SurveillanceEventCoordinator**.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-triggeredeventcoordinator.csv

Deprecated configuration parameters
-----------------------------------

**Base_Population_Scale_Factor** has been renamed to **x_Base_Population**, which is grouped together with the other scale factor parameters beginning with `x_`. The functional remains the same. **Enable_Demographics_Gender** has been deprecated. **Animal_Reservoir_Type** has been replaced with **Enable_Infectivity_Reservoir**.

The following configuration parameters have been deprecated as a result of the refactoring of distribution parameters for better consistency across the configuration and campaign files.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter
    :widths: 10
    :file: csv/220dist-config-deprecated.csv

Deprecated campaign parameters
------------------------------

The following campaign parameters have been deprecated as a result of the refactoring of distribution parameters for better consistency across the configuration and campaign files.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter
    :widths: 10
    :file: csv/220dist-campaign-deprecated.csv

|EMOD_s| v2.18
==============

The |EMOD_s| v2.18 release includes many new features for all supported simulation types. In particular, the TB_SIM simulation type has been deprecated and replaced with TBHIV_SIM, which does not include HIV transmission but adds the ability to model the effect of HIV coinfection on the spread of TB.

.. raw:: html

    <button class="collapse-table-button btn btn-info">Collapse all parameter tables</button>

New configuration parameters
----------------------------


For the generic simulation type, the following new configuration parameters are available:

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/218-config-generic.csv


To view all available configuration parameters, see :doc:`parameter-configuration`.


New campaign parameters
-----------------------

The following campaign classes are new for the Generic model and can be used in all other models:

IncidenceEventCoordinator (generic)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **IncidenceEventCoordinator** coordinator class distributes interventions based on the number of events counted over a period of time.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-incidenceeventcoordinator.csv


MultiNodeInterventionDistributor (generic)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **MultiNodeInterventionDistributor** intervention class distributes multiple node-level interventions when the distributor only allows specifying one intervention.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-multinodeinterventiondistributor.csv


WaningEffectCombo (generic)
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **WaningEffectCombo** class is used within individual-level interventions and allows for specifiying a list of effects when the intervention only has one **WaningEffect** defined. These effects can be added or multiplied.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table


.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-waningeffectcombo.csv



Deprecated demographics parameters
----------------------------------

**ImmunityDistributionFlag**, **ImmunityDistribution1**, and **ImmunityDistribution2**  were renamed to **SusceptibilityDistributionFlag**, **SusceptibilityDistribution1**, and **SusceptibilityDistribution2**. In previous versions of EMOD, the naming was counterintuitive to the functionality. For example, setting a value of 1 for the immunity indicated zero immunity/complete susceptibility. Now the parameters more accurately reflect that you are setting a susceptibility value. The functionality is the same.


Deprecated config parameters
----------------------------

**Immunity_Transmission_Factor**, **Immunity_Mortality_Factor**, and **Immunity_Acquisition_Factor** were renamed to **Post_Infection_Transmission_Multiplier**, **Post_Infection_Mortality_Multiplier**, and **Post_Infection_Acquistion_Multiplier**. The functionality is the same.


Deprecated campaign parameters
------------------------------

The TB_SIM simulation type has been deprecated and replaced with TBHIV_SIM, which does not include HIV transmission but adds the ability to model the effect of HIV coinfection on the spread of TB.

The following campaign classes, which have not yet been fully tested with the TBHIV simulation type, have been disabled:

* HealthSeekingBehaviorUpdate
* HealthSeekingBehaviorUpdateable
* AntiTBPropDepDrug
* SmearDiagnostic

In previous versions of EMOD, you could set the tendency of individuals to seek out health care using **HealthSeekingBehaviorUpdateable** and then update the value of the **Tendency** parameter using **HealthSeekingBehaviorUpdate**. Now, you use individual properties to update individuals when they receive the **SimpleHealthSeekingBehavior** intervention, as you would to control the flow of individuals through other intervention classes. For example, you could create an individual property with HSBold and HSBnew values in the demographics file and assign all individuals to HSBold. Then you could distribute the first **SimpleHealthSeekingBehavior** (with one Tendency value) to all HSBold individuals and use **New_Property_Value** to assign them to HSBnew after receiving the intervention. The next **SimpleHealthSeekingBehavior** intervention (with a different Tendency value) could be distributed, setting **Disqualifying_Properties** to HSBold and, if desired, using **New_Property_Value** to reassign HSBold to those individuals.

Similarly, **AntiTBPropDepDrug** was disabled and superseded with **TBHIVConfigurableTBDrug**, which allows for drug effects based on HIV status and where dependence on **IndividualProperties** is configured through **Property_Restrictions**. In addition, **AntiTBPropDepDrub** can be replaced with **AntiTBDrug**, also using **Property_Restrictions** and new property values to target particular individuals with drug interventions for tuberculosis without HIV coinfections.

**SmearDiagnostic** was disabled and can be replaced with **DiagnosticTreatNeg**. While SmearDiagnostic would only broadcast when an individual had a positive smear diagnostic, **DiagnosticTreatNeg** has the added benefit of broadcasting negative and default diagnostic test events.

**TB_Drug_Clearance_Rate_HIV** and **TB_Drug_Clearance_Rate_MDR** parameters have been renamed to **TB_Drug_Cure_Rate_HIV** and **TB_Drug_Cure_Rate_MDR**.


Error messages
--------------

When attempting to run an intervention using one of the disabled tuberculosis classes, such as **AntiTBPropDepDrug**, **HealthSeekingBehaviorUpdate**, **HealthSeekingBehaviorUpdateable**, and **SmearDiagnostic**, you will receive an error similar to the following: "00:00:01 [0] [I] [JsonConfigurable] Using the default value ( "Intervention_Name" : "HealthSeekingBehaviorUpdateable" ) for unspecified parameter.
00:00:02 [0] [E] [Eradication] 00:00:02 [0] [E] [Eradication]
GeneralConfigurationException:
Exception in SimulationEventContext.cpp at 242 in Kernel::SimulationEventContextHost::LoadCampaignFromFile.
Array out of bounds"

.. note:: 

    These campaign classes have been disabled because they have not yet been fully tested with the TBHIV simulation type.


|EMOD_s| schema and simulation types
------------------------------------

In the schema for the **Simulation_Type** parameter the enum values list additional simulation types
that are not supported by |EMOD_s|. 

.. include:: reuse/sim-types.txt

For information about building the schema, see :doc:`parameter-schema`.


|EMOD_s| v2.13
==============

The |EMOD_s| v2.13 release includes many new features for all supported simulation types.

.. raw:: html

    <button class="collapse-table-button btn btn-info">Collapse all parameter tables</button>

New configuration parameters
----------------------------


For the generic simulation type, the following new configuration parameters are available:

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/213-config-generic.csv


To view all available configuration parameters, see :doc:`parameter-configuration`.


New demographics parameters
---------------------------

In all simulation types, you can now assign properties like risk or quality of care to nodes using
**NodeProperties**, which are configured much like **IndividualProperties**. In addition, a new
property type is available for both nodes and individuals called **InterventionStatus**, which is
used by the campaign file to distribute interventions based on the other interventions an individual
or node has received. This property type was previously available only for individuals in the HIV
simulation type and was known as the **CascadeState**. The relevant campaign parameters are
described in the next section.

For more information, see the table below.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/213-demo-generic.csv

New campaign parameters
-----------------------

The addition of **NodeProperties** in the demographics file also necessitated the addition of
**Node_Property_Restrictions** to control how interventions are distributed based on the
property values assigned to each node.

The new campaign parameters **Disqualifying_Properties** and **New_Property_Value** were added to
every intervention to control how interventions are distributed based on properties assigned to
individuals or nodes. **Disqualifying_Properties** prevents an intervention from being distributed
to individuals or nodes with certain property values. **New_Property_Value** updates the property
value after they receive an intervention.

These are generally used with the the property type **InterventionStatus** to control how
interventions are distributed based on the interventions already received. For example, a household
may be ineligible for clinical treatment for a length of time if they already received treatment
during a drug campaign. This functionality was previously only available for individuals in the HIV
simulation type and used parameters previously called **Abort_States** and **Valid_Cascade_States**.

The following event coordinators and intervention classes are new for this simulation type.

CommunityHealthWorkerEventCoordinator
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **CommunityHealthWorkerEventCoordinator** coordinator class is used to model a health care worker's ability
to distribute interventions to the nodes and individual in their coverage area. This coordinator
distributes a limited number of interventions per day, and can create a backlog of individuals or
nodes requiring the intervention. For example, individual-level interventions could be distribution
of drugs  and node-level interventions could be spraying houses with insecticide.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-communityhealthworkereventcoordinator.csv

ImportPressure
~~~~~~~~~~~~~~

The **ImportPressure** intervention class extends the **ImportCases** outbreak event. Rather than importing a
deterministic number of cases on a scheduled day, **ImportPressure** applies a set of per-day rates
of importation of infected individuals, over a corresponding set of durations. **ImportPressure**
inherits from **Outbreak**; the **Antigen** and **Genome** parameters are defined as they are for all
**Outbreak** events.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table


.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-importpressure.csv


IndividualImmunityChanger
~~~~~~~~~~~~~~~~~~~~~~~~~

The **IndividualImmunityChanger** intervention class acts essentially as a
:doc:`parameter-campaign-individual-multieffectvaccine`,
with the exception of how the behavior is implemented. Rather than
attaching a persistent vaccine intervention object to an individual’s intervention list (as a
campaign-individual-multieffectboostervaccine does), the **IndividualImmunityChanger** directly
alters the immune modifiers of the individual’s susceptibility object and is then immediately disposed
of. Any immune waning is not governed by :doc:`parameter-campaign-waningeffects`, as
:doc:`parameter-campaign-individual-multieffectvaccine` is, but rather
by the immunity waning parameters in the configuration file.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-individualimmunitychanger.csv



MultiEffectBoosterVaccine
~~~~~~~~~~~~~~~~~~~~~~~~~

The **MultiEffectBoosterVaccine** intervention class is derived from **MultiEffectVaccine** and
preserves many of the same parameters. Upon distribution and successful take, the vaccine’s effect
in each immunity compartment (acquisition, transmission,  and mortality) is determined by the
recipient’s immune state. If the recipient’s immunity modifier in the corresponding compartment is
above a user-specified threshold, then the vaccine’s initial effect will be equal to the
corresponding priming parameter. If the recipient’s immune modifier is below this threshold, then
the vaccine’s initial effect will be equal to the corresponding boost parameter. After distribution,
the effect wanes, just like a :doc:`parameter-campaign-individual-multieffectvaccine`. The behavior
is intended to mimic biological priming and boosting.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-multieffectboostervaccine.csv


NodePropertyValueChanger
~~~~~~~~~~~~~~~~~~~~~~~~

The **NodePropertyValueChanger** intervention class sets a given node property to a new value. You can
also define a duration in days before the node property reverts back to its original value, the
probability that a node will change its node property to the target value, and the number of days
over which nodes will attempt to change their individual properties to the target value. This
node-level intervention functions in a similar manner as the individual-level intervention,
:doc:`parameter-campaign-individual-propertyvaluechanger`.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table


.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-nodepropertyvaluechanger.csv

SimpleBoosterVaccine
~~~~~~~~~~~~~~~~~~~~

The **SimpleBoosterVaccine** intervention class is derived from :doc:`parameter-campaign-individual-simplevaccine`
and preserves many of the same parameters. The behavior is much like **SimpleVaccine**, except that upon distribution
and successful take, the vaccine's effect is determined by the recipient's immune state. If the
recipient’s immunity modifier in the corresponding channel (acquisition, transmission, or mortality) is
above a user-specified threshold, then the vaccine’s initial effect will be equal to the
corresponding priming parameter. If the recipient’s immune modifier is below this threshold, then
the vaccine's initial effect will be equal to the corresponding boosting parameter. After
distribution, the effect wanes, just like **SimpleVaccine**. In essence, this intervention
provides a **SimpleVaccine** intervention with one effect to all naive (below- threshold)
individuals, and another effect to all primed (above-threshold) individuals; this behavior is
intended to mimic biological priming and boosting.

.. raw:: html

    <button class="toggle-button btn btn-info">Toggle parameter table</button>

.. rst-class:: toggle-table


.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-simpleboostervaccine.csv
