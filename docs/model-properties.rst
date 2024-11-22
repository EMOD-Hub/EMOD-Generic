==============================
Individual and node properties
==============================

One of the strengths of an agent-based model, as opposed to a compartmental model governed by ODEs,
is that you can introduce heterogeneity in individuals and regions. For example, you can define
property values for accessibility, age, geography, risk, and other properties and assign these
values to individuals or nodes in the simulation.

These properties are most commonly used to target (or avoid targeting) particular nodes or
individuals with interventions. For example, you might want to put individuals into different age
bins and then target interventions to individuals in a particular age bin. Another common use is to
configure treatment coverage to be higher for nodes that are easy to access and lower for nodes that
are difficult to access. For more information on creating campaign interventions, see
:doc:`model-campaign`.

For some simulation types, you can also configure heterogeneous transmission between
individuals with different property values. For more information, see :doc:`model-hint`.

For other
simulation types, transmission is configured using mechanistic parameter settings, such as parasite
density, viral load, biting frequency, and other measures relevant to the disease being modeled. See
:doc:`parameter-configuration-infectivity` for more information.

The following sections describe how to define individual properties and assign different values to
individuals in a simulation. However, with the exception of setting up age bins, you can use the
same process to assign properties to a :term:`node`. To see all individual and node property
parameters, see :ref:`demo-properties`.


Create individual properties other than age
===========================================

Assigning property values to individuals uses the **IndividualProperties** parameter in the
demographics file. See :doc:`parameter-demographics` for a list of supported properties. The values
you assign to properties are user-defined and can be applied to individuals in all nodes or only in
particular nodes in a simulation.

Note that although |EMOD_s| provides several different properties, such as risk and accessibility,
these properties do not add logic, in and of themselves, to the simulation. For example, if you
define individuals as high and low risk, that does not add different risk factors to the
individuals. Higher prevalence or any other differences must be configured separately. Therefore,
the different properties are merely suggestions and can be used to track any property you like.

#.  In the demographics file, add the **IndividualProperties** parameter and set it to an empty array.
    If you want the values to apply to all nodes, add it in the **Defaults** section; if you want the
    values to be applied to specific nodes, add it to the **Nodes** section.

#.  In the array, add an empty JSON object. Within it, do the following:

    #.  Add the **Property** parameter and set it to one of the supported values.

    #.  Add the **Values** parameter and set it to an array of possible values that can
        be assigned to individuals. You can define any string value here.

    #.  Add the **Initial_Distribution** parameter and set it to an array of numbers that add
        up to 1. This configures the initial distribution of the values assigned to individuals
        in one or all nodes.

#.  If you want to add another property and associated values, add a new JSON object in the
    **IndividualProperties** array as above.

.. note::

    Multiple properties must be defined in one file. They can be defined in either the base
    layer demographics file or an overlay file, but they cannot be split between the files.
    The maximum number of property types that can be added is two.

.. TBD is this still accurate or can you add more groups?

Create properties for age ranges
================================

Creating properties based on age ranges works a little differently than other properties.
**Age_Bin** is tied to the simulated age of an individual rather than being an independent property.
Some of its characteristics, such as initial distribution and transitions, are dependent on
information from the demographics file and |EMOD_s| that manages individual aging during the
simulation.

#.  In the demographics file, add the **IndividualProperties** parameter and set it to an empty array.
    If you want the values to apply to all nodes, add it in the **Defaults** section; if you want the
    values to be applied to specific nodes, add it to the **Nodes** section.

#.  In the array, add an empty JSON object. Within it, do the following:

    #.  Add the **Property** parameter and set it to "Age_Bin".

    #.  Add the **Age_Bin_Edges_In_Years** parameter and set it to an array that contains a comma-
        delimited list of integers in ascending order that define the boundaries used for each of
        the age bins, in years. The first number must always be 0 (zero) to indicate the age at
        birth and the last number must be -1 to indicate the maximum age in the simulation.


The example below shows how to set up several property values based on disease risk and physical
place. It also defines three age bins: 0 to 5 years, older than 5 to 13, and older than 13 to the
maximum age.

.. literalinclude:: json/howto-demographics-groups.json

For an example of a complete demographics file with individual properties set, see the
`demographics file`_ used in the 11_HINT_AgeAndAccess scenario. This scenario is described in more
detail in :doc:`model-hint`.

.. _demographics file: https://github.com/InstituteforDiseaseModeling/EMOD-InputData/blob/master/SamplesInput/hint_ageandaccess_overlay.json

