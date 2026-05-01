# Run a simulation using the command line

Using command-line options is the simplest way to run an EMOD simulation. This topic describes
the commands available for running simulations.

The EMOD executable (Eradication.exe) and Eradication binary for Linux also provide commands that provide information about the version of
EMOD that is installed, such as available parameters and simulation types. For more information,
see [parameter-schema](parameter-schema.md). The examples below show the Windows Eradication.exe, but the options are the same
for the Eradication binary for Linux on CentOS.

## Command options

The following command-line options are available for running EMOD simulations. Paths can be
absolute or relative to the directory you are in when running the commands, unless otherwise
specified.

| "Long form" | "Short form" | "Description" |
| --- | --- | --- |
| `--version` | `-v` | Get version information.    `--schema-path` |
| campaign file | which must be in the current directory). If not specified | EMOD will look for |
| files in the current directory. The specific demographics | climate | etc. files to use must be |
| listed in the configuration file." |  |  |
| `--output-path` | `-O` | Path to the directory where output files will be written. If not specified, EMOD will create an "output" directory and overwrite any previous output in that directory.    `--dll-path` |

The following options are for monitoring the progress of simulations running on an
**high-performance computing (HPC)** cluster. They are optional for any simulation, but they must
be used together. To monitor progress, listen for User Datagram Protocol (UDP) messages on the specified
host and port.

| "Long form" | "Description" |
| --- | --- |
| `--monitor_host` | The IP address of the commissioning/monitoring host. Set to "none" for no IP address.    `--monitor_port` |

## Usage

1. Open a Command Prompt window and navigate to the **working directory**, which contains the
    configuration and campaign files.
1. Enter a command like the one illustrated below, substituting the appropriate paths and file
    names for your simulation:

```
        ../Eradication.exe -C config.json -I C:\EMOD\InputFiles -O Sim1Output
```

    If you do not specify anything when invoking Eradication.exe, it will not execute with all defaults, but
    will instead tell you how to invoke the `--help` command.

1. EMOD will display logging information, including an errors that occur, while running
    the simulation. See [software-error-logging](software-error-logging.md) for more information.

1. Output files will be placed in the directory specified. For more information on how to evaluate
    and analyze the output, see [software-outputs](software-outputs.md).
