# Campaign overlay files

You can use two campaign files when setting up a simulation. One file contains default campaign
settings and an **overlay file** contains additional parameters or different parameter values
that override the values in the default file. The files must be flattened into a single file before
running a simulation.

Overlay files allow you to easily separate a subset of parameters that are of particular interest
from the rest of the parameters needed to run a simulation. You can easily modify the parameters in
the overlay file without needing to maintain a complete campaign file. This can be especially
helpful when you want to experiment with different interventions without modifying the rest of the
settings. You can have one default file and many different overlay files for different intervention
settings. It also allows you to easily update the default values across multiple simulations.

In addition to being used for model experimentation, overlay files are used when testing the
software functionality after making source code changes. If you run the EMOD regression tests
using regression_test.py, campaign files will be flattened as part of those tests. However, this may
take several hours if run locally. More guidance on modifying the EMOD source code is in the
"Advance the model" section.

The EMOD Regression_ directory contains many different subdirectories that contain
configuration, campaign, and other associated files to run simulations used in regression testing.
Some subdirectories include a campaign overlay file (campaign_overrides.json), which has been
created by combining campaign_overrides.json with one of the default files in
[Regression/defaults](https://github.com/InstituteforDiseaseModeling/EMOD/tree/master/Regression/defaults). The naming of these files is an arbitrary convention used at IDM; you
may name this files anything you choose. However, it may be useful to see some examples of these
files to understand how they are used.

To flatten campaign files:

1. Create the default campaign file in JSON. You may, though it is not required, organize the
    parameters into logical categories of nested JSON objects to make managing the parameters
    easier. See [parameter-campaign](parameter-campaign.md) for a complete list of all parameters that are
    available. See the example default campaign file below.

```json
{% include "json/howto-generic-default-campaign.json" %}
```

1. Create the overlay campaign file in JSON. This file must include the parameter
    **Default_Campaign_Path**, set to the path to the default campaign file, relative to the
    location of the flatten_campaign.py script in the EMOD Regression_ folder. Again, you may
    organize these into logical categories if you desire. See the example overlay campaign
    file below.

```json
{% include "json/howto-campaign-overlay.json" %}
```

1. In a Command Prompt window, navigate to the Regression folder.

1. Run the flatten_campaign.py script, providing the relative path to the overlay file and the
    path to and name of the new flattened campaign file that will be saved, using the arguments
    as shown below:

```
        python flatten_campaign.py --overlay experiment/campaign_overlay.json --saveto experiment/campaign.json
```

1. Open the resulting campaign.json file and see that it has been flattened into a single file with
    nested JSON objects and any logical categories *retained*.

```json
{% include "json/howto-generic-campaign-flat-full.json" %}
```

After the overlay files and default files are combined into a single campaign
file, you can run a simulation using the EMOD executable (Eradication.exe).
