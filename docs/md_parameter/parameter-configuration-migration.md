# Migration

The following parameters determine aspects of population migration into and outside of a **node**,
including daily commutes, seasonal migration, and one-way moves. Modes of transport includes travel
by foot, automobile, sea, or air. Migration can also be configured to move all individuals in a
family at the same time.

!!! note

        Parameters are case-sensitive. For Boolean parameters, set to 1 for true or 0 for false.
        Minimum, maximum, or default values of "NA" indicate that those values are not applicable for
        that parameter.

        EMOD does not use true defaults; that is, if the dependency relationships indicate that a 
        parameter is required, you must supply a value for it. However, many of the tools used to work 
        with EMOD will use the default values provided below.

        JSON format does not permit comments, but you can add "dummy" parameters to add contextual
        information to your files. Any keys that are not EMOD parameter names will be ignored by the
        model.

{{ read_csv("csv/config-migration-generic.csv") }}
