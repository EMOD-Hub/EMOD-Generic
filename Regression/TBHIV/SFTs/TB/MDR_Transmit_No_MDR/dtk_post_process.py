#!/usr/bin/python


import dtk_test.dtk_sft as sft
import json
np=sft.np
with open("config.json") as infile:
    run_number=json.load(infile)['parameters']['Run_Number']
np.random.seed(run_number)
import os.path as path

"""
MDR_Transmit_No_MDR:
Infect 10% of individuals (probabilistically) with MDR TB at time = 1. We have set 'TB_MDR_Fitness_Multiplier' = 0.
Therefore there should be no TB transmission after the initial TB outbreak.
"""
KEY_NEW_INFECTIONS = "New Infections"
KEY_START_DAY = "Start_Day"
KEY_CONFIG_NAME = "Config_Name"
KEY_TB_MDR_FITNESS_MULTIPLIER = "TB_MDR_Fitness_Multiplier"

def load_emod_parameters(config_filename="config.json", debug=False):
    """reads config file and populates params_obj

    :param config_filename: name of config file (config.json)
    :returns param_obj:     dictionary with KEY_CONFIG_NAME, etc., keys (e.g.)
    """
    with open(config_filename) as infile:
        cdj = json.load(infile)["parameters"]
    param_obj = {}
    param_obj[KEY_CONFIG_NAME] = cdj[KEY_CONFIG_NAME]
    param_obj[KEY_TB_MDR_FITNESS_MULTIPLIER] = cdj[KEY_TB_MDR_FITNESS_MULTIPLIER]
    if debug:
        with open("DEBUG_param_object.json", 'w') as outfile:
            json.dump(param_obj, outfile, indent=4)
    return param_obj

def parse_json_report(insetchart_name="InsetChart.json", output_folder="output", debug=False):
    """
    creates report_data_obj structure with keys
    :param insetchart_name: file to parse (InsetChart.json)
    :param output_folder:
    :return: report_data_obj structure, dictionary with KEY_MDR_TB_Prevalence etc., keys (e.g.)
    """
    insetchart_path = path.join(output_folder, insetchart_name)
    with open(insetchart_path) as infile:
        icj = json.load(infile)["Channels"]
    report_data_obj = {}
    new_infections = icj[KEY_NEW_INFECTIONS]["Data"]
    report_data_obj[KEY_NEW_INFECTIONS] = new_infections

    if debug:
        with open("DEBUG_data_InsetChart.json", "w") as outfile:
            json.dump(report_data_obj, outfile, indent=4)

    return report_data_obj

def load_campaign_file(campaign_filename="campaign.json", debug=False):
    """reads campaign file

    :param campaign_filename: campaign.json file
    :returns: campaign_obj structure, dictionary with KEY_START_DAY, etc., keys (e.g.)
    """
    with open(campaign_filename) as infile:
        cf = json.load(infile)
    campaign_obj = {}
    start_day = cf["Events"][0][KEY_START_DAY]
    campaign_obj[KEY_START_DAY] = start_day

    if debug:
        with open("DEBUG_campaign_object.json", 'w') as outfile:
            json.dump(campaign_obj, outfile, indent=4)

    return campaign_obj

def create_report_file(param_obj, report_data_obj, campaign_obj, report_name, debug):
    with open(report_name, "w") as outfile:
        config_name = param_obj[KEY_CONFIG_NAME]
        outfile.write("Config_name = {}\n".format(config_name))
        success = True
        new_infections = report_data_obj[KEY_NEW_INFECTIONS]
        start_day = campaign_obj[KEY_START_DAY]
        tb_mdr_fitness_multiplier = param_obj[KEY_TB_MDR_FITNESS_MULTIPLIER]
        if tb_mdr_fitness_multiplier:
            success = False
            outfile.write("BAD: TB_MDR_Fitness_Multiplier must set to 0 in this test, it's {} now, please fix"
                          "the test.\n".format(tb_mdr_fitness_multiplier))
        if not len(new_infections):
            success = False
            outfile.write(sft.sft_no_test_data)
        if len(new_infections) < start_day + 1:
            success = False
            outfile.write("BAD: Outbreak start day is longer than simulation duration, please extend the duration.\n")
        for i in range(int(start_day) + 1, len(new_infections)):
            if new_infections[i] > 0:
                success = False
                outfile.write("BAD: Expected no new TB infection at timestep {0}, but found {1} new infections from"
                              "insetchart.json.\n".format(i, new_infections[i]))
        outfile.write(sft.format_success_msg(success))
    if debug:
        print( "SUMMARY: Success={0}\n".format(success) )
    return success

def application( output_folder="output", stdout_filename="test.txt", insetchart_name="InsetChart.json",
                 config_filename="config.json", campaign_filename="campaign.json",
                 report_name=sft.sft_output_filename,
                 debug=False):
    if debug:
        print( "output_folder: " + output_folder )
        print( "stdout_filename: " + stdout_filename+ "\n" )
        print( "insetchart_name: " + insetchart_name+ "\n" )
        print( "config_filename: " + config_filename + "\n" )
        print( "campaign_filename: " + campaign_filename + "\n" )
        print( "report_name: " + report_name + "\n" )
        print( "debug: " + str(debug) + "\n" )

    sft.wait_for_done()
    param_obj = load_emod_parameters(config_filename, debug)
    report_data_obj = parse_json_report(insetchart_name, output_folder, debug)
    campaign_obj = load_campaign_file(campaign_filename, debug)
    create_report_file(param_obj, report_data_obj, campaign_obj, report_name, debug)

if __name__ == "__main__":
    # execute only if run as a script
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument('-o', '--output', default="output", help="Folder to load outputs from (output)")
    parser.add_argument('-s', '--stdout', default="test.txt", help="Name of stdoutfile to parse (test.txt")
    parser.add_argument('-j', '--jsonreport', default="InsetChart.json", help="Json report to load (InsetChart.json)")
    parser.add_argument('-c', '--config', default="config.json", help="Config name to load (config.json)")
    parser.add_argument('-C', '--campaign', default="campaign.json", help="campaign name to load (campaign.json)")
    parser.add_argument('-r', '--reportname', default=sft.sft_output_filename, help="Report file to generate")
    args = parser.parse_args()

    application(output_folder=args.output, stdout_filename=args.stdout, insetchart_name=args.jsonreport,
                config_filename=args.config, campaign_filename=args.campaign,
                report_name=args.reportname, debug=False)
