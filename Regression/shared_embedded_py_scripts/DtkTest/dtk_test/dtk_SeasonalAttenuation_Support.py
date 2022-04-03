#!/usr/bin/python
# This SFT test the following statement:
# Seasonality. The amount of environmental contagion (dose) to which an individual is exposed is attenuated/amplified
# by a seasonality factor
#              that varies throughout the year according to a trapezoidal pattern.
#              The ramp durantions and cutoff days need to follow a rule where: ramp_up_duration + ramp_down_duration
#               + cutoff_days < 365
#              check environmental amplification and exposure for each individual, in each time step

import math
import dtk_test.dtk_sft as sft
import json
np = sft.np
with open("config.json") as infile:
    run_number = json.load(infile)['parameters']['Run_Number']
np.random.seed(run_number)

    
def application(report_file, debug=False):
    sft.wait_for_done()
    """
    Parse this line from test.txt:
    00:00:00 [0] [V] [IndividualTyphoid] amplification calculated as 0.997059: day of year=1, start=360.000000,
    end=365.000000, ramp_up=30.000000, ramp_down=170.000000, cutoff=160.000000.
    00:00:00 [0] [V] [IndividualTyphoid] Exposing individual 2 age 8582.488281 on route 'environment': prob=0.000000,
    infects=0.000008, immunity=1.000000, num_exposures=0, exposure=0.997059, environment=1.000000, iv_mult=1.000000.
    """
    # print( "Post-processing: " + report_file )
    # get params from config.json
    cdj = json.loads(open("config.json").read())["parameters"]
    start_time = cdj["Start_Time"]

    lines = []
    timestep = start_time
    count = 0
    with open("test.txt") as logfile:
        for line in logfile:
            if "Update(): Time:" in line:
                # calculate time step
                timestep += 1
                line = "TimeStep: " + str(timestep) + " " + line
                lines.append(line)
            elif "amplification calculated as" in line:
                if timestep > start_time:
                    count += 1
                    line = "TimeStep: " + str(timestep) + " " + line
                    lines.append(line)
            elif "Exposing" in line and "environment" in line:
                if timestep > start_time:
                    line = "TimeStep: " + str(timestep) + " " + line
                    lines.append(line)
            elif "(environmental) Normalized contagion for" in line:
                if timestep > start_time:
                    line = "TimeStep: " + str(timestep) + " " + line
                    lines.append(line)

    # more params from config file
    rud = cdj["Environmental_Ramp_Up_Duration"]
    rdd = cdj["Environmental_Ramp_Down_Duration"]
    ecd = cdj["Environmental_Cutoff_Days"]
    eps = cdj["Environmental_Peak_Start"]
    config_name = cdj["Config_Name"]

    if debug:
        with open("filtered_lines_debug.txt", "w") as debug_file:
            for line in lines:
                debug_file.write(line)

    peak_duration = 365 - rud - rdd - ecd
    # for eps > 365
    peak_starttime = eps % 365
    peak_endtime = peak_starttime + peak_duration
    cutoff_starttime = peak_starttime + peak_duration + rdd
    success = True
    amp = []
    environmental_amp = []
    expected_exposure_list = []
    actual_exposure_list = []
    with open(sft.sft_output_filename, "w") as report_file:
        report_file.write(f"Running {config_name} Test: \n")
        if count == 0:
            success = False
            report_file.write("Found no data matching test case.\n")
        elif peak_duration < 0:
            success = False
            report_file.write("BAD: Environmental peak duration should be larger or equal to 0, the actual value is {}"
                              ".\n The ramp durantions and cutoff days need to follow a rule where: ramp_up_duration + "
                              "ramp_down_duration + cutoff_days < 365.\n".format(peak_duration))
        else:
            # adjust the times so that the ramp up starts at time 0, which means the cut off ends at time 0 too.
            adjust_time = peak_starttime - rud
            peak_starttime -= adjust_time
            peak_endtime -= adjust_time
            cutoff_starttime -= adjust_time
            day_in_year = 0
            environment_contagion = 0
            for line in lines:
                if "Update(): Time: " in line:
                    timestep = int(sft.get_val("TimeStep: ", line))
                    timestep -= adjust_time
                    day_in_year = timestep % 365
                elif "amplification calculated as" in line:
                    amplification = float(sft.get_val("amplification calculated as ", line))
                    # Environment Ramp Up
                    if day_in_year < peak_starttime:
                        environmental_amplification = day_in_year/float(rud)
                    # Environment peak
                    elif peak_starttime <= day_in_year <= peak_endtime:
                        environmental_amplification = 1
                    # Environment Ramp Down
                    elif peak_endtime < day_in_year < cutoff_starttime:
                        environmental_amplification = (cutoff_starttime - day_in_year) / float(rdd)
                    # Environment cutoff
                    elif day_in_year >= cutoff_starttime:
                        environmental_amplification = 0
                    if math.fabs(amplification - environmental_amplification) > 5e-2:
                        success = False
                        timestep = int(sft.get_val("TimeStep: ", line))
                        report_file.write("BAD: at time {0}, day of year = {1}, the environmental amplification is {2},"
                                          " expected {3}.\n".format(timestep, day_in_year + adjust_time, amplification,
                                                                    environmental_amplification))

                    amp.append(amplification)
                    environmental_amp.append(environmental_amplification)
                elif "(environmental) Normalized contagion for" in line:
                    environment_contagion = float(sft.get_val("is ", line))
                elif "Exposing" in line:
                    ind_id = sft.get_val("individual ", line)
                    exposure = float(sft.get_val("environment=", line))
                    expected_exposure = environment_contagion * amplification
                    actual_exposure_list.append(exposure)
                    expected_exposure_list.append(expected_exposure)
                    if math.fabs(expected_exposure - exposure) > 1e-2:
                        success = False
                        TimeStep = int(sft.get_val("TimeStep: ", line))
                        report_file.write("BAD: at time {0}, day of year = {1}, the amount of environmental contagion "
                                          "that individual {2} is exposed is {3}, expected {4}."
                                          "\n".format(TimeStep, day_in_year + adjust_time, ind_id,
                                                      exposure, expected_exposure))

        sft.plot_data_unsorted(amp, environmental_amp,
                      label1="Actual Seasonal Attenuation",
                      label2="Expected Seasonal Attenuation",
                      title=f"Seasonal Attenuation for {config_name}", xlabel="Time",
                      ylabel="Attenuation",
                      category="attenuation_{}".format(config_name.replace(":", "_")))
        sft.plot_data_unsorted(actual_exposure_list, expected_exposure_list,
                      label1="Actual Exposure Level",
                      label2="Expected Exposure Level",
                      title=f"Exposure for {config_name}", xlabel="Time",
                      ylabel="Exposure",
                      category="exposure_{}".format(config_name.replace(":", "_")))

        report_file.write(sft.format_success_msg(success))


if __name__ == "__main__":
    # execute only if run as a script
    application("")
