# Generate the schema

You can generate a **schema** from the EMOD executable (Eradication.exe) or Eradication binary for Linux that defines all configuration
parameters and campaign parameters available in the version of EMOD that is installed, for all
available simulation types. This includes parameter names, data types, defaults, ranges, and short
descriptions. The schema does not include demographics parameters.

Logging information is also produced as part of the schema. If you are using EMOD source and have
added or modified configuration parameters or intervention code, this logging information can help
you troubleshoot any errors that may occur.

!!! warning

        If you modify the source code to add or remove configuration or campaign parameters, you may
        need to update the code used to produce the schema. You must also verify that your simulations
        are still scientifically valid.

## Command options

The following command-line options are available for providing information about EMOD.

| "Long form" | "Short form" | "Description" |
| --- | --- | --- |
| `--help` | `-help` | Shows help options for Eradication.exe.    `--version` |

## Usage

1. Open a Command Prompt window and navigate to the directory where Eradication.exe is installed.
1. To output the schema to the Command Prompt window, enter:

    ```
                Eradication.exe --get-schema
    ```

1. To output the schema to a file, do one of the following (replacing &lt;filename> with the desired filename}:

    * To output a text file that includes logging information, enter:
  
        ```
                     Eradication.exe --get-schema > <filename>.txt
        ```

     * To display logging in the Command Prompt window and output a text file that does not include logging information, enter:

        ```
                     Eradication.exe --get-schema --schema-path <filename>.txt
        ``` 

     * To output the schema to a JSON file that includes logging information, enter:

        ```
                     Eradication.exe --get-schema > <filename>.json
        ```

     * To display logging in the Command Prompt window and output a JSON file that does not include logging information, enter:

        ```
                     Eradication.exe --get-schema --schema-path <filename>.json
        ```