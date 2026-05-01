# Input files

EMOD takes a number of files as inputs for running simulations. The input files indicate the
transmission mode, population size, age distribution, disease interventions, and many more qualities
that affect disease transmission. The EMOD executable (Eradication.exe) takes these input files and uses them to run a simulation
of the disease dynamics.

Many of the input files are formatted as **JSON (JavaScript Object Notation)** files. JSON is a
simple text format that uses key/value pairs to encode data. In EMOD, the key is a parameter
name and the value is the setting for that parameter. For example, **"Base_Incubation_Period": 5**
sets the incubation period for the disease being modeled to be five days. JSON files are easy to
read and edit. For more information about JSON, see  [parameter-overview](parameter-overview.md). Some of the inputs
are compiled binary files.

## Primary files

The primary files used for EMOD simulations are the following:

**demographics file**     Defines the geography and the population being modeled. Each geographic **node** has a location,
    population, and certain characteristics assigned to it. These files are often reused across multiple
    simulations and disease scenarios.

**configuration file**
    Defines many aspects of the simulation, including disease characteristics like infectivity and transmission mode and simulation characteristics like simulation length and additional input files
    to use.

**campaign file**
    Defines the events that occur during the simulation. Primarily, these are the various disease
    interventions that will take place, but they also include the disease outbreaks.

## Supplementary files

There are other optional files that EMOD can use as inputs to the simulation. These files are
not necessary for every simulation or every disease. For example, climate files are only used for
vector-borne diseases because weather affects mosquito populations. Migration files are only used
for multi-node simulations where human or vector movement is important. Because EMOD is
**stochastic**, it requires running many simulations which may require a lot of processing
power. Load-balancing and serialization files are for managing computing resources.

These files use both a JSON file for metadata and an associated binary file that contains the actual
data. You will typically use these input files in their default state.

## Input file IDs

The Institute for Disease Modeling (IDM) provides collections of climate and demographics files for many different locations in
the world for download on GitHub. For instructions, see [install-overview](install-overview.md).  Some input files
were prepared using CIESIN Gridded Population of the World (GPW) population distribution and a
corresponding spatial resolution grid (for example, 2.5 arc minutes) to define the initial
population and extent of the nodes for country-wide input files. Therefore, the naming convention
for this files usually leads with the geographic location, followed by the spatial resolution, and
input file type.

All input files except configuration and campaign files include the parameter **IdReference** in the
metadata, which is used to generate the **NodeID** associated with each node in a simulation. The
values for **IdReference** and **NodeID** must be the same across all input files used in a
simulation. See [parameter-demographics](parameter-demographics.md) for more information about **NodeID** generation.

- [software-demographics](software-demographics.md)
- [software-configuration](software-configuration.md)
- [software-campaign](software-campaign.md)
- [software-migration](software-migration.md)
- [software-climate](software-climate.md)
- [software-load-balancing](software-load-balancing.md)
- [software-serialized](software-serialized.md)
