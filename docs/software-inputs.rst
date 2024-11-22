===========
Input files
===========

|EMOD_s| takes a number of files as inputs for running simulations. The input files indicate the
transmission mode, population size, age distribution, disease interventions, and many more qualities
that affect disease transmission. The |exe_l| takes these input files and uses them to run a simulation
of the disease dynamics.

Many of the input files are formatted as :term:`JSON (JavaScript Object Notation)` files. JSON is a
simple text format that uses key/value pairs to encode data. In |EMOD_s|, the key is a parameter
name and the value is the setting for that parameter. For example, **"Base_Incubation_Period": 5**
sets the incubation period for the disease being modeled to be five days. JSON files are easy to
read and edit. For more information about JSON, see  :doc:`parameter-overview`. Some of the inputs
are compiled binary files.


Primary files
=============

The primary files used for |EMOD_s| simulations are the following: 

:term:`demographics file`
    Defines the geography and the population being modeled. Each geographic :term:`node` has a location,
    population, and certain characteristics assigned to it. These files are often reused across multiple 
    simulations and disease scenarios.

:term:`configuration file`
    Defines many aspects of the simulation, including disease characteristics like infectivity and transmission mode and simulation characteristics like simulation length and additional input files
    to use.

:term:`campaign file`
    Defines the events that occur during the simulation. Primarily, these are the various disease
    interventions that will take place, but they also include the disease outbreaks. 

Supplementary files
===================

There are other optional files that |EMOD_s| can use as inputs to the simulation. These files are
not necessary for every simulation or every disease. For example, climate files are only used for
vector-borne diseases because weather affects mosquito populations. Migration files are only used
for multi-node simulations where human or vector movement is important. Because |EMOD_s| is
:term:`stochastic`, it requires running many simulations which may require a lot of processing
power. Load-balancing and serialization files are for managing computing resources.

These files use both a JSON file for metadata and an associated binary file that contains the actual
data. You will typically use these input files in their default state. 

Input file IDs
==============

The |IDM_l| provides collections of climate and demographics files for many different locations in
the world for download on GitHub. For instructions, see :doc:`install-overview`.  Some input files
were prepared using CIESIN Gridded Population of the World (GPW) population distribution and a
corresponding spatial resolution grid (for example, 2.5 arc minutes) to define the initial
population and extent of the nodes for country-wide input files. Therefore, the naming convention
for this files usually leads with the geographic location, followed by the spatial resolution, and
input file type.

All input files except configuration and campaign files include the parameter **IdReference** in the
metadata, which is used to generate the **NodeID** associated with each node in a simulation. The
values for **IdReference** and **NodeID** must be the same across all input files used in a
simulation. See :doc:`parameter-demographics` for more information about **NodeID** generation.

.. toctree::
   :maxdepth: 2
   :titlesonly:

   software-demographics
   software-configuration
   software-campaign
   software-migration
   software-climate
   software-load-balancing
   software-serialized

