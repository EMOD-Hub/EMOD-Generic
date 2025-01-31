#!/usr/bin/python

import json
import os.path as path
import dtk_test.dtk_sft as dtk_sft
import pandas as pd
import os
import math
import numpy as np


class ConfigKeys:
    TOTAL_TIMESTEPS = "Simulation_Duration"
    SIMULATION_TIMESTEP = "Simulation_Timestep"
    BASE_INFECTIVITY_CONSTANT = "Base_Infectivity_Constant"
    CONFIG_NAME = "Config_Name"
    RUN_NUMBER = "Run_Number"

    class InfectivityScaling:
        EXPONENTIAL_BASELINE = "Infectivity_Exponential_Baseline"
        EXPONENTIAL_DELAY = "Infectivity_Exponential_Delay"
        EXPONENTIAL_RATE = "Infectivity_Exponential_Rate"


matches = ["Update(): Time: ",
           "Infected: ",
           "total contagion = ",
           "StatPop: "]

KEY_INFECTED = "infected"
KEY_INFECTIOUSNESS = "infectiousness"
KEY_NEW_INFECTIONS = "New Infections"
KEY_CUMULATIVE_INFECTIONS = "Cumulative Infections"
KEY_STAT_POP = "Stat Pop"





"""
This new infectivity scaling enum enables simulations in which the infectivity of 
the disease begins at one level, stays there for some period of time, and then 
exponentially approaches an asymptotic infectivity level. This mode is useful for 
modeling a disease that becomes more fit as it transmits (for example, Sabin 2 
polio vaccine may become more infectious as it reverts to a neurovirulent phenotype), 
and may also be useful for smoothing burn- ins. Specifically, the per-individual, 
per- timestep infectivity, normally determined by Base_Infectivity_Constant (beta), behaves as follows:
beta = <formula(g,t,d,lambda)>
where g, t_d, and lambda are new configuration parameters that are required when 
Infectivity_Scale_Type is set to EXPONENTIAL_FUNCTION_OF_TIME; these parameters, 
respectively, are called Infectivity_Exponential_Baseline, 
Infectivity_Exponential_Delay, and Infectivity_Exponential_Rate.
"""


def load_emod_parameters(config_filename="config.json"):
    """
    reads config file and populates params_obj
    :param config_filename: name of config file (config.json)
    :returns param_obj:     dictionary with KEY_TOTAL_TIMESTEPS, etc., keys (e.g.)
    """
    with open(config_filename) as infile:
        cdj = json.load(infile)["parameters"]
    param_obj = {}
    param_obj[ConfigKeys.TOTAL_TIMESTEPS] = cdj[ConfigKeys.TOTAL_TIMESTEPS]
    param_obj[ConfigKeys.BASE_INFECTIVITY_CONSTANT] = cdj[ConfigKeys.BASE_INFECTIVITY_CONSTANT]
    param_obj[ConfigKeys.InfectivityScaling.EXPONENTIAL_BASELINE] = \
        cdj[ConfigKeys.InfectivityScaling.EXPONENTIAL_BASELINE]
    param_obj[ConfigKeys.InfectivityScaling.EXPONENTIAL_DELAY] = cdj[ConfigKeys.InfectivityScaling.EXPONENTIAL_DELAY]
    param_obj[ConfigKeys.InfectivityScaling.EXPONENTIAL_RATE] = cdj[ConfigKeys.InfectivityScaling.EXPONENTIAL_RATE]
    param_obj[ConfigKeys.SIMULATION_TIMESTEP] = cdj[ConfigKeys.SIMULATION_TIMESTEP]
    param_obj[ConfigKeys.CONFIG_NAME] = cdj[ConfigKeys.CONFIG_NAME]
    param_obj[ConfigKeys.RUN_NUMBER] = cdj[ConfigKeys.RUN_NUMBER]
    return param_obj


def parse_output_file(output_filename="test.txt", simulation_timestep=1, debug=False):
    """
    creates a dataframe of time step, infected,  infectiouness and stat populations
    :param output_filename: file to parse (test.txt)
    :param simulation_timestep: simulation time step, * days
    :return: output_df:  data frame contains: 1, time step,
                                            2, # of infected population,
                                            3, infectiousness,
                                            4, statistical populations, at each time step
    """
    filtered_lines = []
    with open(output_filename) as logfile:
        for line in logfile:
            if dtk_sft.has_match(line, matches):
                filtered_lines.append(line)
    if debug:
        with open("DEBUG_filtered_lines.txt", "w") as outfile:
            outfile.writelines(filtered_lines)

    # initialize variables
    time_step = 0
    infectiousness = 0
    output_df = pd.DataFrame(columns=[ConfigKeys.SIMULATION_TIMESTEP, KEY_INFECTED, KEY_INFECTIOUSNESS, KEY_STAT_POP])
    output_df.index.name = "index"
    index = 0
    for line in filtered_lines:
        if matches[0] in line:
            infected = dtk_sft.get_val(matches[1], line)
            statpop = dtk_sft.get_val(matches[3], line)
            output_df.loc[index] = [time_step, infected, infectiousness, statpop]
            index += 1
            time_step += simulation_timestep
            infectiousness = 0
            continue
        if matches[2] in line:
            infectiousness = dtk_sft.get_val(matches[2], line)
            continue
    res_path = r'./DEBUG_infected_vs_infectiousness.csv'
    if not os.path.exists(os.path.dirname(res_path)):
        os.makedirs(os.path.dirname(res_path))
    output_df.to_csv(res_path)
    return output_df


def parse_json_report(output_folder="output", insetchart_name="InsetChart.json", debug=False):
    """
    creates report_df dataframe with "new_infections" and "cumulative_infections" columns
    :param output_folder: location (output/InsetChart.json)
    :param insetchart_name: InsetChart.json file
    :returns: report_df dataframe
    """
    insetchart_path = path.join(output_folder, insetchart_name)
    with open(insetchart_path) as infile:
        icj = json.load(infile)["Channels"]

    new_infections = icj[KEY_NEW_INFECTIONS]["Data"]
    cumulative_infections = icj[KEY_CUMULATIVE_INFECTIONS]["Data"]

    report_dict = {KEY_NEW_INFECTIONS: new_infections, KEY_CUMULATIVE_INFECTIONS: cumulative_infections}
    report_df = pd.DataFrame(report_dict)
    report_df.index.name = "index"

    if debug:
        res_path = r'./DEBUG_new_infections_vs_cumulative_infections.csv'
        if not os.path.exists(os.path.dirname(res_path)):
            os.makedirs(os.path.dirname(res_path))
        report_df.to_csv(res_path)
    return report_df


def calculate_infectiousness(new_infections, index, simulation_timestep, total_timesteps, base_infectivity,
                             baseline, delay, rate):
    infectiousness_ind = [0] * (int(total_timesteps / simulation_timestep) + 1)
    timestep = index * simulation_timestep
    for i in np.arange(timestep, total_timesteps, simulation_timestep):
        x = int(i / simulation_timestep)
        if i < delay:
            infectiousness_ind[x] = base_infectivity * baseline
        else:
            infectiousness_ind[x] = base_infectivity * (1.0 - ((1.0 - baseline) * math.exp((delay - i) / rate)))
    if index == 1 or timestep == 1:
        dtk_sft.plot_data(infectiousness_ind,
                          title="infectiousness for new infections at time step {0}".format(timestep),
                          xlabel="time step / simulation_timestep{0}".format(simulation_timestep),
                          ylabel="Exponential_Delay: {0} days, Exponential_Rate: {1} ".format(delay, rate),
                          label1="infectiousness",
                          label2="None",
                          category="infectiousness_for_new_infections_at_time_step_{0}".format(timestep),
                          show=True, line=True)
    infectiousness_ind = map(lambda x: x * new_infections, infectiousness_ind)
    return infectiousness_ind


def create_report_file(param_obj, output_df, report_df, report_name, debug):
    total_timesteps = int(param_obj[ConfigKeys.TOTAL_TIMESTEPS])
    simulation_timestep = float(param_obj[ConfigKeys.SIMULATION_TIMESTEP])
    base_infectivity = float(param_obj[ConfigKeys.BASE_INFECTIVITY_CONSTANT])
    baseline = float(param_obj[ConfigKeys.InfectivityScaling.EXPONENTIAL_BASELINE])
    delay = float(param_obj[ConfigKeys.InfectivityScaling.EXPONENTIAL_DELAY])
    rate = float(param_obj[ConfigKeys.InfectivityScaling.EXPONENTIAL_RATE])
    infected = output_df[KEY_INFECTED]
    infectiousness = output_df[KEY_INFECTIOUSNESS]
    statpop = output_df[KEY_STAT_POP]
    new_infections = report_df[KEY_NEW_INFECTIONS]
    cumulative_infections = report_df[KEY_CUMULATIVE_INFECTIONS]
    dtk_sft.plot_data(new_infections, cumulative_infections,
                      label1="new infections", label2="cumulative infections",
                      title="Exponential_Delay: {0} days, Exponential_Rate: {1} ".format(delay, rate),
                      xlabel="time step / simulation_timestep{0}".format(simulation_timestep), ylabel=None,
                      category='New_infections_vs_cumulative_infections',
                      show=True, line=True)

    with open(report_name, "w") as outfile:
        outfile.write(
            "# Test name: " + str(param_obj[ConfigKeys.CONFIG_NAME]) + ", Run number: " +
            str(param_obj[ConfigKeys.RUN_NUMBER]) +
            "\n# Tests if the expected and actual infectiousness match for exponential infectivity scaling.\n")
        expected_infectiousness = [0] * (int(total_timesteps / simulation_timestep) + 1)
        pre_infected = int(infected[0])
        success = True
        actual_infectiousness_all = []
        calc_infectiousness_all = []
        for index in range(len(infectiousness)):
            if index > 0:
                new_infected = int(infected[index]) - pre_infected
                pre_infected = int(infected[index])
                if new_infected:
                    new_expected_infectiousness = calculate_infectiousness(new_infected, index,
                                                                           simulation_timestep, total_timesteps,
                                                                           base_infectivity, baseline,
                                                                           delay, rate)
                    expected_infectiousness = list(map(sum, zip(expected_infectiousness, new_expected_infectiousness)))
            timestep = index * simulation_timestep
            actual_infectiousness = float(infectiousness[index])
            calc_infectiousness = expected_infectiousness[index] / float(statpop[index])
            actual_infectiousness_all.append(actual_infectiousness)
            calc_infectiousness_all.append(calc_infectiousness)
            tolerance = 0 if calc_infectiousness == 0 else 3e-2 * calc_infectiousness
            low_acceptance_bound = calc_infectiousness - tolerance
            high_acceptance_bound = calc_infectiousness + tolerance
            if math.fabs(actual_infectiousness - calc_infectiousness) > tolerance:
                success = False
                outfile.write("BAD: actual infectiousness at time step {0} is {1}, which is not within acceptance"
                              "range ({2}, {3}).  Expected {4}.\n"
                              .format(timestep, actual_infectiousness, low_acceptance_bound,
                                      high_acceptance_bound, calc_infectiousness))
        if success:
            outfile.write("GOOD: all actual infectiousness values were within 3% of the calculated infectiousness.\n")
        if debug:
            with open("DEBUG_actual_vs_calc_infectiousness.txt", "w") as file:
                for i in range(len(actual_infectiousness_all)):
                    file.write("Time Step: {0}, actual infectiousness: {1}, expected_infectiousness: {2}.\n"
                               .format(i * simulation_timestep, actual_infectiousness_all[i],
                                       calc_infectiousness_all[i]))
        dtk_sft.plot_data(actual_infectiousness_all, calc_infectiousness_all,
                          label1="actual infectiousness", label2="calc infectiousness",
                          title="Exponential_Delay: {0} days, Exponential_Rate: {1} ".format(delay, rate),
                          xlabel="time step / simulation_timestep{0}".format(simulation_timestep),
                          ylabel="Infectiousness",
                          category='Infectiousness',
                          show=True, line=True)
        outfile.write(dtk_sft.format_success_msg(success))
        return success
