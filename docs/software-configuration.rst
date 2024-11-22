==================
Configuration file
==================

The primary means of configuring the disease simulation is the :term:`configuration file`. This
required file is a :term:`JSON (JavaScript Object Notation)` formatted file that is typically named
config.json. The configuration file controls many different aspects of the simulation. For example,

* The names of the :term:`campaign file` and other :term:`input files` to use
* How to use additional demographics, climate, and migration data (such as enabling features or scaling values)
* General disease attributes such as infectivity, immunity, mortality, and so on
* Attributes specific to the disease type being modeled, such as infectivity and mortality
* The reports to output from the simulation


Although you can create configuration files entirely from scratch, it is often easier to start from
an existing configuration file and modify it to meet your needs. You can download sets of configuration
and campaign files that illustrate how to model different disease scenarios at `EMOD scenarios`_. For more
information, see :doc:`tutorials`. 


The simplest method of working with the configuration files is to use a text editor to directly edit
the parameters or parameter values in the JSON file. However, you may want to use Python or another
scripting language to make large modifications. See :doc:`software-configuration-script` for more
information.

For a complete list of configuration parameters that are available to use with this simulation type,
see :doc:`parameter-configuration`. For more information about JSON, see :doc:`parameter-overview`.


Flattened configuration files
=============================

A flattened configuration file is generally a single-depth JSON file with configuration parameters
listed alphabetically. This is the configuration file format that |exe_s| and |linux_binary| both
require for running simulations.


Below is an example of a flattened configuration file:

.. literalinclude:: json/howto-generic-config-flat-full.json
   :language: json

Hierarchical configuration files
================================

A hierarchical file allows you to organize parameters into logical groups by nesting them within
JSON objects, making them easier to manage. If you use hierarchical configuration files, you must
flatten them prior to running a simulation. The names you use to label these logical categories are
unimportant; the scripts used to flatten the files will search through the hierarchies and retain
only the "leaf" values in the resulting flattened file. See :doc:`software-configuration-overlay`
for more information on flattening files.

Below is an example of a hierarchical configuration file:

.. literalinclude:: json/howto-generic-default-config.json
   :language: json


.. toctree::

   software-configuration-overlay
   software-configuration-script
   software-parameter-sweep

.. _EMOD scenarios: https://github.com/InstituteforDiseaseModeling/docs-emod-scenarios/releases
