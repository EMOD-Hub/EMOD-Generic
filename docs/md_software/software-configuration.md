# Configuration file

The primary means of configuring the disease simulation is the **configuration file**. This required file is a **JSON (JavaScript Object Notation)** formatted file that is typically named
config.json. The configuration file controls many different aspects of the simulation. For example,

* The names of the **campaign file** and other **input files** to use
* How to use additional demographics, climate, and migration data (such as enabling features or scaling values)
* General disease attributes such as infectivity, immunity, mortality, and so on
* Attributes specific to the disease type being modeled, such as infectivity and mortality
* The reports to output from the simulation

The simplest method of working with the configuration files is to use a text editor to directly edit
the parameters or parameter values in the JSON file. However, you may want to use Python or another
scripting language to make large modifications. See [software-configuration-script](software-configuration-script.md) for more
information.

For a complete list of configuration parameters that are available to use with this simulation type,
see [parameter-configuration](../parameter-configuration.md). For more information about JSON, see [parameter-overview](../parameter-overview.md).

## Flattened configuration files

A flattened configuration file is generally a single-depth JSON file with configuration parameters
listed alphabetically. This is the configuration file format that Eradication.exe and Eradication binary for Linux both
require for running simulations.

Below is an example of a flattened configuration file:

```json
{% include "json/howto-generic-config-flat-full.json" %}
```

## Hierarchical configuration files

A hierarchical file allows you to organize parameters into logical groups by nesting them within
JSON objects, making them easier to manage. If you use hierarchical configuration files, you must
flatten them prior to running a simulation. The names you use to label these logical categories are
unimportant; the scripts used to flatten the files will search through the hierarchies and retain
only the "leaf" values in the resulting flattened file. See [software-configuration-overlay](software-configuration-overlay.md)
for more information on flattening files.

Below is an example of a hierarchical configuration file:

```json
{% include "json/howto-generic-default-config.json" %}
```

