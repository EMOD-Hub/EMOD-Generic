import numpy as np
import json
import os.path as path
import dtk_test.dtk_sft as dtk_sft
import pandas as pd
import os
import math
import random

matches = ["Update(): Time: ",
           "Infected: ",
           "total contagion = ",
           "StatPop: "]

KEY_INFECTED = "infected"
KEY_INFECTIOUSNESS = "infectiousness"
KEY_NEW_INFECTIONS = "New Infections"
KEY_CUMULATIVE_INFECTIONS = "Cumulative Infections"
KEY_STAT_POP = "Stat Pop"
KEY_BOXCAR = "ANNUAL_BOXCAR_FUNCTION"


class ConfigKeys:
    TOTAL_TIMESTEPS = "Simulation_Duration"
    SIMULATION_TIMESTEP = "Simulation_Timestep"
    BASE_INFECTIVITY = "Base_Infectivity"
    INFECTIVITY_SCALE_TYPE = "Infectivity_Scale_Type"
    CONFIG_NAME = "Config_Name"
    RUN_NUMBER = "Run_Number"

    class InfectivityScaling:
        AMPLITUDE = "Infectivity_Boxcar_Forcing_Amplitude"
        END_TIME = "Infectivity_Boxcar_Forcing_End_Time"
        START_TIME = "Infectivity_Boxcar_Forcing_Start_Time"


class Constants:
    MAX_BASE_INFECTIVITY = 2
    MAX_AMPLITUDE = 5


class Modes:
    EQUAL, LESS, LARGER = range(3)


"""
The purpose of this ENUM is to modulate the infectivity of the disease, at a period of
1 year with a single value for the high season and a single value for the low season.
The name could probably be better.  The epidemiological justification is the same as above,
the implementation is just different.  An issue with the sine wave is that if there is
a large increase in infectivity during the high season, there must be a corresponding
huge dip in infectivity during the low season.  The boxcar simplifies to one infectivity
value during the high season, and a second one for the rest of the year.
The implementation is intended to reproduce the following behavior, where tstart/end are
the configurable parameters Infectivity_Boxcar_(Start/End)_Time, and are specifically meant
to indicate the start and end times of the high season within the year (that is, the
parameter A in the equations below is a non-negative float).  One key note is that tstart
is not required to be less than tend, because the user may want the high season to begin at
the end of the calendar year and loop into the next year.
If t_start < t_end, then the boxcar starts and ends within the same calendar year,
so if "t" is between t_start and t_end, then we are in the high season.
beta(t)=beta_0;     when mod(t,365)<t_start  OR>t_end
beta(t)=beta_0 [1+A];  when mod(t,365)>t_start  AND <t_end
If t_start > t_end, then the boxcar starts at the end of one "calendar year" and ends
in the next calendar year, so we are in the high season if t is either higher than tstart
or lower than tend.
beta(t)=beta_0;     when mod(t,365)<t_start  AND>t_end
beta(t)=beta_0 [1+A];  when mod(t,365)>t_start  OR <t_end
Where beta_0= Base_Infectivity, A = Infectivity_Boxcar_Forcing_Amplitude, t_start/end are
the configurable parameters Infectivity_Boxcar_(Start/End)_Time, and t is time in days,
within the simulation.
Similar to the sinusoid, it should be verified that the boxcar works when the simulation
timestep is not equal to 1 day
"""


# region pre_process support
def draw_random_mode():
    random_num = np.random.randint(0, 3)
    if random_num == 0:
        return Modes.EQUAL
    elif random_num == 1:
        return Modes.LESS
    else:
        return Modes.LARGER


def generate_start_and_end_time(mode=Modes.LESS):
    """
    generate random start time and end time
    :param mode: 0: start_time = end_time, 1: start_time < end_time, 2: start_time > end_time
    :return: start_time, end_time
    """
    random_days = sorted(random.sample(range(1, 366), 2))  # two non-duplicate integers from [1, 366)
    random_day_1 = random_days[0]
    random_day_2 = random_days[1]

    if mode == Modes.LESS:
        start_time = random_day_1
        end_time = random_day_2
    elif mode == Modes.LARGER:
        start_time = random_day_2
        end_time = random_day_1
    elif mode == Modes.EQUAL:
        start_time = random_day_1
        end_time = random_day_1
    else:
        msg = "mode is {}, it should be 0(Modes.EQUAL), 1(Modes.LESS) or 2(Modes.LARGER).".format(mode)
        raise ValueError(msg)
    return start_time, end_time


def set_random_config_file(config_filename="config.json", mode=Modes.LESS, debug=False):
    """
    set Base_Infectivity, Infectivity_Boxcar_Forcing_Amplitude, Infectivity_Boxcar_Forcing_End_Time and
    Infectivity_Boxcar_Forcing_Start_Time in config.json file
    :param config_filename: name of config file(config.json)
    :param debug:
    :return: None
    """
    with open("config.json", "r") as infile:
        config_json = json.load(infile)
    with open("config_template.json", "w") as outfile:
        json.dump(config_json, outfile, indent=4, sort_keys=True)

    start_time, end_time = generate_start_and_end_time(mode)
    base_infectivity = np.random.random() * Constants.MAX_BASE_INFECTIVITY
    amplitude = np.random.random() * Constants.MAX_AMPLITUDE

    config_json["parameters"][ConfigKeys.BASE_INFECTIVITY] = base_infectivity
    config_json["parameters"][ConfigKeys.InfectivityScaling.AMPLITUDE] = amplitude
    config_json["parameters"][ConfigKeys.InfectivityScaling.START_TIME] = start_time
    config_json["parameters"][ConfigKeys.InfectivityScaling.END_TIME] = end_time
    config_json["parameters"][ConfigKeys.INFECTIVITY_SCALE_TYPE] = KEY_BOXCAR

    with open(config_filename, "w") as outfile:
        json.dump(config_json, outfile, indent=4, sort_keys=True)

    if debug:
        print("start time and end time are {0} and {1}.\n".format(start_time, end_time))
        print("base infectivity and amplitude are {0} and {1}.\n".format(base_infectivity, amplitude))
    return


# endregion


# region post_process support
def load_emod_parameters(config_filename="config.json"):
    """
    reads config file and populates params_obj
    :param config_filename: name of config file (config.json)
    :returns param_obj:     dictionary with ConfigKeys.TOTAL_TIMESTEPS, etc., keys (e.g.)
    """
    with open(config_filename) as infile:
        cdj = json.load(infile)["parameters"]
    param_obj = {}
    param_obj[ConfigKeys.TOTAL_TIMESTEPS] = cdj[ConfigKeys.TOTAL_TIMESTEPS]
    param_obj[ConfigKeys.BASE_INFECTIVITY] = cdj[ConfigKeys.BASE_INFECTIVITY]
    param_obj[ConfigKeys.InfectivityScaling.AMPLITUDE] = cdj[ConfigKeys.InfectivityScaling.AMPLITUDE]
    param_obj[ConfigKeys.InfectivityScaling.START_TIME] = cdj[ConfigKeys.InfectivityScaling.START_TIME]
    param_obj[ConfigKeys.InfectivityScaling.END_TIME] = cdj[ConfigKeys.InfectivityScaling.END_TIME]
    param_obj[ConfigKeys.SIMULATION_TIMESTEP] = cdj[ConfigKeys.SIMULATION_TIMESTEP]
    param_obj[ConfigKeys.CONFIG_NAME] = cdj[ConfigKeys.CONFIG_NAME]
    param_obj[ConfigKeys.RUN_NUMBER] = cdj[ConfigKeys.RUN_NUMBER]
    return param_obj


def parse_output_file(output_filename="test.txt", simulation_timestep=1, debug=False):
    """
    creates a dataframe of time step and infected,  infectiousness and stat populations
    :param output_filename: file to parse (test.txt)
    :return:                output_df:  # of infected population and infectiousness per person at each time step
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
    if debug:
        res_path = r'./DEBUG_infected_vs_infectiousness.csv'
        if not os.path.exists(os.path.dirname(res_path)):
            os.makedirs(os.path.dirname(res_path))
        output_df.to_csv(res_path)
    return output_df


def parse_json_report(output_folder="output", insetchart_name="InsetChart.json", debug=False):
    """
    creates report_df dataframe with "new_infections" and "cumulative_infections" columns
    :param insetchart_name: InsetChart.json file with location (output/InsetChart.json)
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


def calculate_infectiousness(infected_pop, index, simulation_timestep, start_time, end_time, base_infectivity,
                             amplitude, debug=False):
    """
    calculate infectiousness at each time step
    :param infected_pop:
    :param index:
    :param simulation_timestep:
    :param start_time:
    :param end_time:
    :param base_infectivity:
    :param amplitude:
    :param debug:
    :return:
    """
    timestep = index * simulation_timestep
    day_in_year = float(timestep % 365)
    infectiousness = base_infectivity
    infectiousness_amplitude = base_infectivity * (1.0 + amplitude)
    if start_time < end_time:
        if start_time < day_in_year < end_time:
            infectiousness = infectiousness_amplitude
    elif start_time > end_time:
        if day_in_year > start_time or day_in_year < end_time:
            infectiousness = infectiousness_amplitude
    infectiousness *= infected_pop
    if debug:
        print(infectiousness)
    return infectiousness


def create_report_file(param_obj, output_df, report_df, report_name, debug=False):
    simulation_timestep = float(param_obj[ConfigKeys.SIMULATION_TIMESTEP])
    base_infectivity = float(param_obj[ConfigKeys.BASE_INFECTIVITY])
    amplitude = float(param_obj[ConfigKeys.InfectivityScaling.AMPLITUDE])
    start_time = float(param_obj[ConfigKeys.InfectivityScaling.START_TIME])
    end_time = float(param_obj[ConfigKeys.InfectivityScaling.END_TIME])
    infected = output_df[KEY_INFECTED]
    infectiousness = output_df[KEY_INFECTIOUSNESS]
    statpop = output_df[KEY_STAT_POP]
    new_infections = report_df[KEY_NEW_INFECTIONS]
    dtk_sft.plot_data(new_infections, label1="new infections", label2="NA",
                      title="Start_time: {0} day, End_time: {1} day".format(start_time, end_time),
                      xlabel="Time_Step_{0}_Days".format(simulation_timestep), ylabel=None,
                      category='New_infections',
                      show=True, line=True)
    with open(report_name, "w") as outfile:
        outfile.write(
            "# Test name: " + str(param_obj["Config_Name"]) + ", Run number: " + str(param_obj["Run_Number"]) +
            "\n# Tests if the start and the end are correct relative to each other as well as if expected and "
            "actual infectiousness match.\n")
        success = True
        expected_infectiousness = []
        actual_infectiousness_all = []
        calc_infectiousness_all = []
        for index in range(len(infected)):
            infected_pop = int(infected[index])
            expected_infectiousness.append(calculate_infectiousness(infected_pop, index, simulation_timestep,
                                                                    start_time, end_time,
                                                                    base_infectivity, amplitude, debug))
            timestep = index * simulation_timestep
            actual_infectiousness = float(infectiousness[index])
            calc_infectiousness = expected_infectiousness[index] / float(statpop[index])
            actual_infectiousness_all.append(actual_infectiousness)
            calc_infectiousness_all.append(calc_infectiousness)
            tolerance = 0 if calc_infectiousness == 0 else 5e-2 * calc_infectiousness
            upper_acceptance_bound = calc_infectiousness + tolerance
            lower_acceptance_bound = calc_infectiousness - tolerance
            if math.fabs(actual_infectiousness - calc_infectiousness) > tolerance:
                success = False
                outfile.write("BAD: actual infectiousness at time step {0} is {1}, which is not within"
                              " accepted range ({2},{3}).  Expected {4}.\n"
                              .format(timestep, actual_infectiousness, lower_acceptance_bound,
                                      upper_acceptance_bound, calc_infectiousness))

        if success:
            outfile.write("GOOD: Actual infectiousness for all time steps was within 5% of the calculated "
                          "infectiousness\n")
        outfile.write("Start infectiousness was {0} and end infectiousness was {1}\n"
                      .format(actual_infectiousness_all[int(start_time)], actual_infectiousness_all[int(end_time)]))
        outfile.write(dtk_sft.format_success_msg(success))
    dtk_sft.plot_data(actual_infectiousness_all, calc_infectiousness_all,
                      label1="actual infectiousness", label2="calc infectiousness",
                      title="Start_time: {0} day, End_time: {1} day".format(start_time, end_time),
                      xlabel="Time_Step_{0}_Days".format(simulation_timestep), ylabel="Infectiousness",
                      category='Infectiousness',
                      show=True, line=True)
    return success
# endregion
