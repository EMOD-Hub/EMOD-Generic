=======================
Generic model scenarios
=======================

The |EMOD_s| generic model is explained in detail in :doc:`model-overview`. While the various
components that comprise the model are explained with examples, it may be more useful to learn the
model through hands-on implementation. The following sections will introduce sets of example files
that illustrate how the generic model works on particular topics. All files are available in the
downloadable `EMOD scenarios`_ > Scenarios > Generic folder and, in addition to the explanations below,
each scenario will have a more detailed README file to cover relevant information.

|EMOD_s| supports different simulation types for various diseases and disease classes. The features
present in the GENERIC_SIM :term:`simulation type`, which can be configured to model a variety of
different diseases, are inherited by the more specific simulation types, such as malaria or HIV.
Because all disease-specific |EMOD_s| simulation types are based on the generic model, the following
scenarios are helpful in learning the basics of modeling with |EMOD_s|, even if you intend to use
one of the disease-specific simulation types.

For more information on the software architecture and inheritance, see :doc:`software-overview`.

.. contents:: Contents
   :local:


Compartmental models
====================

Before working with an :term:`agent-based model` like |EMOD_s|, you will likely be familiar with
:term:`deterministic` compartmental models. In these models, an
:term:`ordinary differential equation (ODE)` controls the rate at which the population transitions from
each disease state (compartment) to another. These models assume that individuals within each
compartment are identical and the entire population is well-mixed. Given the same inputs, a
compartmental model will always produce the same outcomes.

While compartmental models are very useful, in many situations an agent-based model is a better tool
for describing complex phenomena. In these models, each agent (such as a human or a vector) is
simulated individually. Their behavior and interactions with one another are determined using
decision rules. For a more detailed comparison of |EMOD_s| and compartmental models, see
:doc:`model-compartments`.

The following scenarios under `EMOD scenarios`_ > Scenarios > Generic show how you can use |EMOD_s| to
simulate disease scenarios that might also be simulated using a compartmental model. For example, a
disease like measles could be modeled using  a compartmental SIR model or it could be modeled using
|EMOD_s| configured such that there is no incubation period or waning immunity after infection.

* SI
* SIS
* SIR
* SIRS
* SEIR
* SEIR_VitalDynamics
* SEIRS


Density scaling of infection
============================

By default, |EMOD_s| uses frequency-dependent transmission and the overall transmissibility does not
change with population size or density. An infected individual will infect the same number of people
either in a small village or in a metropolitan area. The `EMOD scenarios`_ > Scenarios > Generic >DensityScaling
scenario shows how you can configure |EMOD_s| so population density has an effect on the
transmissibility of disease. For a detailed description of the mathematics behind this
functionality, see :doc:`model-population-density`.

Heterogeneous transmission
==========================

By default, the probability of transmitting disease to another is the same within each :term:`node`
in an |EMOD_s| simulation. However, we know that some people may be at greater risk based on their
behavior, environment, or biology. The |HINT_l| feature enables you to specify values for
heterogeneity in transmission based on :term:`individual properties` in a :term:`WAIFW matrix`.
For detailed information, see :doc:`model-hint`.

`EMOD scenarios`_ > Scenarios > Generic > HINT_AgeAndAccess illustrates a common scenario in which the
population has properties applied based on age and accessibility. Transmission is higher among
individuals of similar ages and accessibility levels. Interventions are targeted based on these
properties.

`EMOD scenarios`_ > Scenarios > Generic > HINT_SeattleCommuting illustrates a more novel scenario that
uses the |HINT_s| feature. To simulate migration in |EMOD_s|, you can run multi-node simulations
where individuals have a certain probability of migrating to a different node at each timestep
(typically set to one day). This works well when individuals move to another location permanently or
seasonally. However, for daily movement like commuting for work, you can use |HINT_s| to make
transmission higher for people in the same area during the workday. This scenario assigns individual
properties that represent the area codes surrounding Seattle and configures higher transmission for
people in the same area code.


Disease reservoirs
==================

Generally with |EMOD_s|, you model a disease by introducing an outbreak at some point in the
simulation. However, endemic settings may involve a non-human disease reservoir that periodically
reintroduces the disease to the human population. The `EMOD scenarios`_ > Scenarios > Generic > Zoonosis
scenario illustrates how to configure |EMOD_s| to simulate such zoonotic diseases. For detailed
information, see :doc:`model-outbreak`.



Interventions
=============

The `EMOD scenarios`_ > Scenarios > Generic > Vaccinations scenario introduces the concept of adding
interventions to stop disease transmission. It compares the outcomes when a disease outbreak has no
interventions applied, when the entire population is vaccinated, and when you target the vaccination
to a particular segment of the population.

Because interventions are often very specific to the disease being modeled, the other simulation
types introduce diagnostic tests, drugs, and health system elements relevant to that disease.
Therefore, the scenarios for the disease-specific simulation types provide more examples of
intervention configuration. For detailed information, see :doc:`model-campaign`.

.. _EMOD scenarios: https://github.com/InstituteforDiseaseModeling/docs-emod-scenarios/releases