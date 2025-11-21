#!/usr/bin/python


import os.path as path
import dtk_test.dtk_sft as dtk_sft
import json
import numpy as np
from statistics import mean

with open("config.json") as infile:
    run_number=json.load(infile)['parameters']['Run_Number']
np.random.seed(run_number)


TB_CD4_ACTIVATION_VECTOR = "TB_CD4_Activation_Vector"
TB_CD4_STRATA_ACTIVATION = "CD4_Strata_Activation"

"""
The activation time gets modified based on the TB_CD4_Activation_Vector, using CD4_Strata_Activation.
Also, if the cd4 count of the person exceeds the strata's max, currently there is an issue where the multiplier gets
really big (https://github.com/bradwgnr/DtkTrunk/issues/27) and my test catches that.

"""


def load_emod_parameters(config_filename="config.json"):
    """reads config file and populates params_obj

    :param config_filename: name of config file (config.json)
    :returns param_obj:     dictionary with keys
    """
    cdj = None
    with open(config_filename) as infile:
        cdj = json.load(infile)["parameters"]
    param_obj = {TB_CD4_ACTIVATION_VECTOR: cdj[TB_CD4_ACTIVATION_VECTOR],
                 TB_CD4_STRATA_ACTIVATION: cdj[TB_CD4_STRATA_ACTIVATION]}
    return param_obj


def parse_stdout_file(curr_timestep=0, stdout_filename="test.txt", debug=False):
    """
    :param curr_timestep:   first timestep from config
    :param stdout_filename: file to parse (test.txt)
    :param debug:           whether or not we write an additional file that's full of the matched lines
    :return:                array of lines of interest
    """
    filtered_lines = []
    update_time = "Update(): Time:"
    initial_incubation_timer = "Incubation_timer calculated as"
    incubation_timer_update = "LifeCourseLatencyTimerUpdate"
    time = 0
    with open(stdout_filename) as logfile:
        for line in logfile:
            if update_time in line:
                time += 1
            elif incubation_timer_update in line:
                new_line = dtk_sft.add_time_stamp(time, line)
                filtered_lines.append(new_line)
            elif initial_incubation_timer in line:
                new_line = dtk_sft.add_time_stamp(time, line)
                filtered_lines.append(new_line)
            elif "CD4_future" in line:
                filtered_lines.append(line)

    if debug:
        with open("filtered_lines.txt", "w") as outfile:
            outfile.writelines(filtered_lines)

    return filtered_lines


def create_report_file(data):
    report_name = data[0]
    lines = data[1]
    tb_cd4_activation_vector = data[2]  # this test assumes the vector is constant

    latency_update_data = []
    original_latency_data = []
    success = True
    with open(report_name, "w") as outfile:
        if not lines:
            outfile.write("BAD: No relevant test data found.\n")
            success = False
        for line in lines:
            if "Incubation_timer calculated as" in line:
                incubation_timer = float(dtk_sft.get_val("as ", line))
                original_latency_data.append(incubation_timer)
            if "LifeCourseLatencyTimerUpdate" in line:
                new_incubation_timer = float(dtk_sft.get_val("timer ", line))
                latency_update_data.append(new_incubation_timer)

        expected_update_data = np.random.exponential(1 / tb_cd4_activation_vector[2], len(latency_update_data))
        if not dtk_sft.test_exponential(latency_update_data, tb_cd4_activation_vector[2], outfile, integers=True,
                                        roundup=True, round_nearest=False):
            # as it should fail , success = bad.
            outfile.write("BAD: The updated latency data does not match the expected distribution.\n")
            success = False

        original_latency_data_mean = mean(original_latency_data)
        big_number = 1000000  # we expect the average orignally-calculated latency to be bigger than this
        latency_update_data_mean = mean(latency_update_data)
        small_number = 300  # we expect the re-calculated latency to be smaller than this
        if original_latency_data_mean < big_number:
            success = False
            outfile.write(f"BAD: Original latency calculation (tb with hiv) should have a large mean, at least "
                          f"bigger than {big_number}, but it was {original_latency_data_mean}. Please check the data.\n")
        else:
            outfile.write(f"GOOD: Original latency calculation (tb with hiv) should have a large mean, at least "
                          f"bigger than {big_number}, and it was {original_latency_data_mean}.\n")
        if latency_update_data_mean > small_number:
            success = False
            outfile.write(f"BAD: Update latency calculation (tb/hiv with art) should have a small mean, at most "
                          f"smaller than {small_number}, but it was {latency_update_data_mean}. Please check the data.\n")
        else:
            outfile.write(f"GOOD: Update latency calculation (tb/hiv with art) should have a small mean, at most "
                          f"smaller than {small_number}, and it was {latency_update_data_mean}.\n")

        outfile.write("Data points checked = {}.\n".format(len(latency_update_data)))
        outfile.write("SUMMARY: Success={0}\n".format(success))

        dtk_sft.plot_data(sorted(latency_update_data), sorted(expected_update_data), label1="Actual", label2="Expected",
                          title="Latency Duration recalculated for ART", xlabel="Data Points", ylabel="Days",
                          category="tb_activation_hiv_tb_art", line=True, overlap=True)
        dtk_sft.plot_data(sorted(original_latency_data), sorted(expected_update_data), label1="Actual",
                          label2="Expected",
                          title="Original Latency", xlabel="Data Points", ylabel="Days",
                          category="tb_activation_hiv_tb", line=True, overlap=True)


def application(output_folder="output", stdout_filename="test.txt",
                config_filename="config.json",
                insetchart_name="InsetChart.json",
                report_name=dtk_sft.sft_output_filename,
                debug=False):
    if debug:
        print("output_folder: " + output_folder)
        print("stdout_filename: " + stdout_filename + "\n")
        print("config_filename: " + config_filename + "\n")
        print("insetchart_name: " + insetchart_name + "\n")
        print("report_name: " + report_name + "\n")
        print("debug: " + str(debug) + "\n")
    dtk_sft.wait_for_done()
    param_obj = load_emod_parameters(config_filename)
    parsed_data = parse_stdout_file()
    create_report_file([report_name, parsed_data, param_obj.get(TB_CD4_ACTIVATION_VECTOR)])


if __name__ == "__main__":
    # execute only if run as a script
    application("output")
