# Demographics parameters

The parameters described in this reference section can be added to the **JSON (JavaScript Object Notation)** formatted demographics file to determine the demographics of the population within each geographic **node** in a simulation. For example, the number of individuals and the
distribution for age, gender, immunity, risk, and mortality. These parameters work closely with the
[parameter-configuration-population](parameter-configuration-population.md) parameters in the **configuration file**, which are simulation-wide and
generally control whether certain events, such as births or deaths, are enabled in a simulation.

Additionally, you can use more than one demographics file, with one serving as the base layer and
the one or more others acting as overlays that override the values in the base layer. This can be
helpful if you want to experiment with different values in the overlay without modifying your base
file. For more information, see [software-demographics](../md_software/software-demographics.md).

At least one demographics file is required for every simulation unless you set the parameter
**Enable_Demographics_Builtin** to 1 (one) in the **configuration file**. This setting does not
represent a real location and is generally only used for testing and validating code pathways rather
than actual modeling of disease.

Demographics files are organized into four main sections: **Metadata**, **NodeProperties**,
**Defaults**, and **Nodes**. The following example shows the skeletal format of a demographics file.

[link](../json/demographics-skeletal.json)

All parameters except those in the **Metadata** and **NodeProperties** sections below can appear in
either the **Defaults** section or the **Nodes** section of the demographics file. Parameters under
**Defaults** will be applied to all nodes in the simulation. Parameters under **Nodes** will be
applied to specific nodes, overriding the values in **Defaults** if they appear in both. Each node in
the **Nodes** section is identified using a unique **NodeID**.

The tables below contain only parameters available when using the generic **simulation type**.

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

## Metadata

Metadata provides information about data provenance. **IdReference** is the
only parameter used by EMOD, but you are encouraged to include information for your
own reference. For example, author, date created, tool used, NodeCount and more are commonly included
in the **Metadata** section. You can include any information you like here provided it is
in valid JSON format.

If you generate **input files** using COMPS, the following **IdReference** values are
possible and indicate how the **NodeID** values are generated:

Gridded world grump30arcsec: Nodes are approximately square regions defined by a 30-arc second grid and the **NodeID** values
are generated from the latitude and longitude of the northwest corner.

Gridded world grump2.5arcmin: Nodes are approximately square regions defined by a 2.5-arc minute grid and the **NodeID** values
are generated from the latitude and longitude of the northwest corner.

Gridded world grump1degree: Nodes are approximately square regions defined by a 1-degree grid and the **NodeID** values are
generated from the latitude and longitude of the northwest corner.

The algorithm for encoding latitude and longitude into a **NodeID** is as follows:

```
   unsigned int xpix = math.floor((lon + 180.0) / resolution)
   unsigned int ypix = math.floor((lat + 90.0) / resolution)
   unsigned int NodeID = (xpix << 16) + ypix + 1
```

This generates a **NodeID** that is a 4-byte unsigned integer; the first two bytes represent the
longitude of the node and the second two bytes represent the latitude. To reserve 0 to be used as a
null value, 1 is added to the **NodeID** as part of the final calculation.

{{ read_csv("csv/demo-metadata-generic.csv") }}

## NodeProperties and IndividualProperties

Node properties and individual properties are set similarly and share many of the same parameters.
Properties can be thought of as tags that are assigned to nodes or individuals and can then be used
to either target interventions to nodes or individuals with certain properties (or prevent them from
being targeted). For example, you could define individual properties for disease risk and then
target an intervention to only those at high risk. Similarly, you could define properties for node
accessibility and set lower intervention coverage for nodes that are difficult to access.

Individual properties are also used to simulate health care cascades. For example, you can
disqualify an individual who would otherwise receive an intervention; such as treating a segment of
the population with a second-line treatment but disqualifying those who haven't already received the
first-line treatment. Then you can change the property value after the treatment has been received.

The **NodeProperties** section is a top-level section at the same level as **Defaults** and **Nodes**
that contains parameters that assign properties to nodes in a simulation. The
**IndividualProperties** section is under either **Defaults** or **Nodes** and contains parameters
that assign properties to individuals in a simulation. [model-properties](../md_model/model-properties.md) provides more guidance.

{{ read_csv("csv/demo-properties-generic.csv") }}

## NodeAttributes

The **NodeAttributes** section contains parameters that add or modify information
regarding the location, migration, habitat, and population of node. Some **NodeAttributes**
depend on values set in the configuration parameters.

{{ read_csv("csv/demo-nodeattributes-generic.csv") }}

## IndividualAttributes

The **IndividualAttributes** section contains parameters that initialize the distribution of
attributes across individuals, such as the age or immunity. An initial value for an
individual is a randomly selected value from a given distribution. These distributions can be
configured using a simple flag system of three parameters or a complex system of
many more parameters. The following table contains the parameters that can be used with either
distribution system.

{{ read_csv("csv/demo-individualattributes-generic.csv") }}

### Complex distributions

Complex distributions are more effort to configure, but are useful for representing real-world data
where the distribution does not fit a standard. Individual attribute values are drawn from a piecewise
linear distribution. The distribution is configured using arrays of axes (such as gender or age) and
values at points along each of these axes. This allows you to have different distributions for
different groups in the population.

{{ read_csv("csv/demo-complexdistro-generic.csv") }}
