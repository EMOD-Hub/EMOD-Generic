# Cascade of care

Some diseases, such as HIV, have a complex sequential cascade of care that individuals must
navigate. For example, going from testing to diagnosis, receiving medical counseling, taking
antiretroviral therapy, and achieving viral suppression. Other life events, such as pregnancy,
migration, relationship changes, or diagnostic criteria may trigger different medical interventions.

Health care in EMOD can be applied to individuals, such as through a vaccination campaign, or be
sought out by various triggering events including birth, pregnancy, or symptoms. A potential problem
created by this structure is that an individual could end up in care multiple times. For example, an
individual might have an antenatal care (ANC) visit and, in the same time step, seek health care for
AIDS symptoms, both leading to HIV testing and staging.

To avoid this situation, you can configure interventions using the InterventionStatus individual
property in the demographics file (see [demo-properties](#demo-properties) for more information). In the
demographics file, create as many property values as necessary to describe the care cascade. For
example, undiagnosed, positive diagnosis, on therapy, lost to care, etc.

In the campaign file, set up your **event coordinator** as you typically would, using
**Target_Demographic**, **Property_Restrictions_Within_Node**, and other available parameters to
target the desired individuals. See [model-targeted-interventions](model-targeted-interventions.md) for more information on targeting interventions and [parameter-campaign-event-coordinators](parameter-campaign-event-coordinators.md) for all available
event coordinators.

Then, in the intervention itself, you can add any properties that should prevent someone who would
otherwise qualify for the intervention from receiving it. For example, someone who has already
received a positive diagnosis would be prevented from receiving a diagnostic test if they sought out
medical care for symptoms. You can also add the new property that should be assigned to the
individual if they receive the intervention.

The following example shows a simplified example with two interventions, a diagnostic event and
distribution of medication. The demographics file defines intervention status values for having
tested positive and for being on medication.

```json
{% include "json/howto-carecascade.json" %}
```
