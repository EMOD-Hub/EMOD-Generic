# Configuration overlay files

You can use two configuration files when setting up a simulation. One file contains default
parameter settings and an **overlay file** contains additional parameters or different parameter
values that override the values in the default file.

Overlay files allow you to easily separate a subset of parameters that are of particular interest
from the rest of the parameters needed to run a simulation. You can easily modify the parameters in
the overlay file without needing to maintain a complete configuration file. This can be
especially helpful when you want to experiment with the values set in certain parameters of interest
without modifying the rest of the settings. You can have one default file and many different overlay
files for different configurations. It also allows you to easily update the default values across
multiple simulations.

These files must be flattened into a single file and the values in the overlay file will override
those in the default file. When using overlay files, the parameters are often organized into
logical groups using the hierarchical file format. See [software-configuration](software-configuration.md) for more
information. When using any kind of hierarchical file, whether or not you are using an overlay file,
it must also be flattened using the Python script below.

To flatten two configuration files:

1. Create the default configuration file in JSON. You may, though it is not required, organize the
    parameters into logical categories of nested JSON objects to make managing the parameters
    easier. See [parameter-configuration](parameter-configuration.md) for a complete list of all parameters that are
    available. See the example default configuration file below.

```json
{% include "json/howto-generic-default-config.json" %}
```

1. Create the overlay configuration file in JSON. This file must include the parameter
    **Default_Config_Path**, set to the path to the default configuration file, relative to the
    location of the flatten_config.py script in the EMOD Regression_ folder. Again, you may
    organize the parameters into logical categories if you desire. See the example overlay
    configuration file below.

```json
{% include "json/howto-param-overlay.json" %}
```

1. In a Command Prompt window, navigate to the Regression folder.

1. Run the flatten_config.py script, providing the relative path to the overlay file::

        python flatten_config.py experiment/parameter_overrides.json

1. Open the resulting config.json file in the same folder as parameter_overrides.json and see that it
    has been flattened into a single layer with all parameters listed alphabetically and any logical
    categories *removed*. Eradication.exe will not accept a configuration file with nested JSON objects.

```json
{% include "json/howto-generic-config-flat-full.json" %}
```
