# Configuration parameters

The parameters described in this reference section can be added to the **JSON (JavaScript Object Notation)** formatted configuration file to determine the core behavior of a simulation
including the computing environment, functionality to enable, additional files to use, and
characteristics of the disease being modeled. This file contains mostly a flat list of JSON
key:value pairs.

For more information on the structure of these files, see [software-configuration](software-configuration.md).

The topics in this section contain only parameters available when using the generic **simulation type**.
Some parameters may appear in multiple categories.

!!! note

        Parameters are case-sensitive. For Boolean parameters, set to 1 for true or 0 for false.
        Minimum, maximum, or default values of "NA" indicate that those values are not applicable for
        that parameter.

        EMOD does not use true defaults; that is, if the dependency relationships indicate that a parameter is required, you must supply a value for it. However, many of the tools used to work with EMOD will use the default values provided below.

        JSON format does not permit comments, but you can add "dummy" parameters to add contextual
        information to your files. Any keys that are not EMOD parameter names will be ignored by the
        model.

- [parameter-configuration-enable](parameter-configuration-enable.md)
- [parameter-configuration-env](parameter-configuration-env.md)
- [parameter-configuration-general](parameter-configuration-general.md)
- [parameter-configuration-immunity](parameter-configuration-immunity.md)
- [parameter-configuration-incubation](parameter-configuration-incubation.md)
- [parameter-configuration-infectivity](parameter-configuration-infectivity.md)
- [parameter-configuration-input](parameter-configuration-input.md)
- [parameter-configuration-migration](parameter-configuration-migration.md)
- [parameter-configuration-mortality](parameter-configuration-mortality.md)
- [parameter-configuration-output](parameter-configuration-output.md)
- [parameter-configuration-population](parameter-configuration-population.md)
- [parameter-configuration-sampling](parameter-configuration-sampling.md)
- [parameter-configuration-scalars](parameter-configuration-scalars.md)
- [parameter-configuration-setup](parameter-configuration-setup.md)
