=================
Demographics file
=================

Demographics files are JSON-formatted files that contain information on the demographics of the
population in each :term:`node` in the simulation. For example, the longitude and latitude, the
number of individuals, and the distribution for age, gender, immunity, risk, and mortality. Once you
have it configured for a given population and location, you will likely reuse it across many
different simulation scenarios.

In addition, demographics files are useful for creating heterogeneity in a population. You can define
values for accessibility, age bins, geography, risk, and other properties and assign values to
individuals or nodes. For example, you might want to divide a population up into different bins
based on age so you can target interventions to individuals in a particular age range. Another
common use is to configure treatment coverage to be higher for individuals in easily accessible
regions and lower for individuals in areas that are difficult to access. For more information,
see :doc:`model-properties`.

You can also simulate a complex health care system using property values that represent the
intervention status for each individual. For example, consider a disease that has a second-line
treatment that is not used unless the first-line treatment has proven ineffective. You can assign a
property value after receiving the first-line treatment and prevent anyone from receiving the
second-line treatment unless they have that property value and are still symptomatic. For more
information, see :doc:`model-care-cascade`.

|EMOD_s| assumes homogeneous mixing and disease transmission for the generic simulation type. Using
the individual properties in the demographics file, you can also use the |HINT_s| feature to add
heterogeneous transmission to your generic model. You cannot manually configure heterogeneous
transmission using |HINT_s| with other simulation types because the heterogeneity in transmission
for specific diseases and disease classes is configured by transmission parameters specific to each
disease model. For more information, see :doc:`model-hint`.

You do have the option to run a simulation *without* a demographics
file if you set **Enable_Demographics_Builtin** to 1 in the configuration file. However, this option
is primarily used for software testing. It will not provide meaningful simulation data as it does
not represent the population of a real geographic location.

The demographics file structure and parameters are described in more detail in
:doc:`parameter-demographics`.


.. toctree::
   :maxdepth: 2
   :titlesonly:

   software-demographics-overlay
