==============================================
Example scripts to modify a configuration file
==============================================

Direct editing of the configuration file in a text editor may be sufficient for small and infrequent
changes, but you will likely find that scripting tools are more powerful and reliable for both
creating and modifying files.

The following example shows how to read a configuration file to a Python dictionary, modify a
parameter, and write it back out to the file:

.. code-block:: python

    import json

    # load the current config.json
    config_file = open( "config.json" )
    config_json = json.load( config_file )
    config_file.close()

    # modify one of the parameter values, e.g. "base_infectivity"
    config_json["parameters"]["base_infectivity"] = 0.5

    # write the modified config file
    modified_file = open( "modified_config.json", "w" )
    json.dump( config_json, modified_file, sort_keys=True, indent=4 )
    modified_file.close()

The following example shows how to modify a configuration file in MATLAB:

.. code-block:: matlab

    addpath Matlab
    addpath Matlab\test

    % load the simulation configuration file into MATLAB structure
    configjson = loadJson( "config.json" );

    % modify one of the values
    configjson.parameters.Base_Infectivity = 08.5;

    % save the new configuration to file
    saveJson( "modified_config.json", configjson );