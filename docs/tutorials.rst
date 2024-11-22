=================================
Tutorials and simulation examples
=================================

While the workings of the model are explained in detail in **Understanding the model**, it is often more useful to learn through hands-on implementation. To illustrate many of the concepts and capabilities of |EMOD_s|, |IDM_s| provides tested files to
run example simulations that model a variety of disease scenarios. The directory for each scenario
contains the files needed to run a simulation and generate the output graphs. Click `EMOD scenarios`_ to
download a zipped folder containing all required files. These scenarios are referenced throughout the documentation where relevant.


.. include:: reuse/note-stochastic.txt

Included files
==============

The compiled |exe_l| is included. Unlike other executables, you *do not* double-click this file to
run simulations. Instead, |EMOD_s| simulations are run by calling |exe_s| from the command line. For
these scenarios, you can simply double-click the runEMOD.cmd file in each scenario directory, which
contains the commands that will run the simulation.

Of course, you can also run simulations directly from the command line or using |COMPS_l| if you
choose. Review :doc:`software-run-simulation` for complete information on running simulations from
the command line, or just open the runEMOD.cmd file in a text editor to see what commands it contains.

The Scenarios directory contains subdirectories that contain scenarios for different simulation
types. The README files in each of the scenario subdirectories describe what aspect of the model the
scenario illustrates. Each scenario subdirectory contains the configuration and campaign files
needed to run an |EMOD_s| simulation. Also included are plotting batch files that contain commands to
run Python scripts. You can simply double-click the batch files to run these scripts and produce graphs.

The Demographics_Files directory contains the demographics files for all scenarios. The demographics
files used with each scenario are listed in **Demographics_Filenames** in the configuration file.
This directory also contains a few climate files used with the malaria scenarios.

The Scripts directory contains the Python scripts used to plot the output of the |EMOD_s| simulations.
These are the scripts pointed to by the plotting batch files.

Most input files use :term:`JSON (JavaScript Object Notation)` format. If you are unfamiliar with
this format, see :doc:`parameter-overview`.


Prerequisites
=============

We recommend running these scenarios on a Windows computer. You must install the HPC, MPI, and
Python packages described in :doc:`install-windows`. You do not need to download another copy of
|exe_s|, although it won't hurt if you do.

Although |EMOD_s| also supports |Centos|, the scripts to run simulations will not work and the
installation instructions differ. However, you can still run simulations from the command line. See
the |Centos| installation instructions at :doc:`install-linux`.


General |EMOD_s| information
============================

The features of the model are described in greater detail elsewhere in the documentation, but this
provides a brief overview of the features most relevant to using these tutorials.

Manipulating population size
----------------------------

It is often useful to re-use the same demographics files for running different simulations.
Depending on the dynamics of the simulation, or the output reports that are desired, it may be
important to change the size of the population. An alternative to modifying the demographics file
is to use the configuration parameter, **x_Base_Population**. This parameter lets you
adjust the population set by the demographics parameter, **InitialPopulation**.


Overlay files
-------------

An alternative to changing parameter values or adding parameters to base files is to create what is
called an "overlay file." These files contain parameter settings or additional parameters that will
overwrite the settings configured in the base configuration and demographic files. When using
an overlay for configuration parameters, include the files in the working directory. For demographic
overlay files, all files to be used must be listed in the array for **Demographics_Filenames**.
The order of these files is important: the base file must be listed first, and the overlay listed
second. When there is more than one overlay file, the order of the overlays determines what information
will be overridden; the last file in order will overlay all files preceding it. Overlay files can
make it simple to swap out different parameter values without having to modify the base files, so
you can experiment with different settings. Demographic overlays make it simple to increase the
complexity of the population structure without having to create complex files.



Creating campaigns
------------------

Much of the power and flexibility of |EMOD_s| comes from the customizable campaign interventions.
For more information on creating campaigns, see :doc:`model-campaign`. Briefly, campaigns are created
through the hierarchical organization of JSON objects (parameter groups).

campaign event
    .. include:: reuse/campaign-event-overview.txt
event coordinator
    .. include:: reuse/campaign-ec-overview.txt
individual-level intervention
    .. include:: reuse/campaign-individual-intervention-overview.txt
node-level intervention
    .. include:: reuse/campaign-node-intervention-overview.txt


All campaign events and event coordinators can be found in :doc:`parameter-campaign-event-coordinators`;
node-level interventions can be found in :doc:`parameter-campaign-node-interventions`; and
individual-level interventions can be found in :doc:`parameter-campaign-individual-interventions`.



Output reports
--------------

When you run a simulation using the runEMOD.cmd file, an output directory will be created that
contains the output reports from the model. Some of these will be in JSON format and others in CSV.
The standard report is InsetChart.json, which includes many different channels including
births, deaths, cumulative infections, new infections, and more. You can enable other built-in
reports by setting the appropriate parameter to 1 in the configuration file.


For a complete list of all available output reports, see :doc:`parameter-configuration-output`.




.. _EMOD scenarios: https://github.com/InstituteforDiseaseModeling/docs-emod-scenarios/releases

.. toctree::

   generic-scenarios