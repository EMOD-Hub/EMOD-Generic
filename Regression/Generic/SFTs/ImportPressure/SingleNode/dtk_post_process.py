#!/usr/bin/python

import dtk_test.dtk_ImportPressure_Support as ips
import dtk_test.dtk_sft as sft
import json
import numpy as np
with open("config.json") as infile:
    run_number=json.load(infile)['parameters']['Run_Number']
np.random.seed(run_number)


def application(output_folder="output",
                config_filename="config.json", campaign_filename="campaign.json",
                inset_chart_name="InsetChart.json",
                report_name=sft.sft_output_filename,
                debug=False):

    if debug:
        print("output_folder: " + output_folder)
        print("config_filename: " + config_filename + "\n")
        print("campaign_filename: " + campaign_filename + "\n")
        print("inset_chart_name: " + inset_chart_name + "\n")
        print("debug: " + str(debug) + "\n")

    sft.wait_for_done()

    param_obj = ips.load_emod_parameters(config_filename, debug)
    campaign_obj = ips.load_campaign_file(param_obj[ips.ConfigKeys.CAMPAIGN_NAME], debug)
    demographics_obj = ips.load_demographics_file(param_obj[ips.ConfigKeys.DEMOGRAPHICS_NAME][-1], debug)
    report_data_obj = ips.parse_json_report(output_folder, inset_chart_name, debug)

    sft.plot_data(report_data_obj[ips.KEY_NEW_INFECTIONS],
                  title="new infections",
                  label1="New Infections",
                  label2="NA",
                  xlabel="time steps", ylabel="new infection",
                  category='New_infections',
                  show=True)
    sft.plot_data(report_data_obj[ips.KEY_STATISTICAL_POPULATION],
                  title="Statistical Population",
                  label1="Statistical Population",
                  label2="NA",
                  xlabel="time steps", ylabel="Statistical Population",
                  category='Statistical_population',
                  show=True, line=True)

    ips.create_report_file(param_obj, campaign_obj, demographics_obj, report_data_obj, report_name, debug)


if __name__ == "__main__":

    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument('-o', '--output', default="output", help="Folder to load outputs from (output)")
    parser.add_argument('-i', '--insetchartname', default="InsetChart.json", help="insetchart to test(InsetChart.json)")
    parser.add_argument('-c', '--config', default="config.json", help="Config name to load (config.json)")
    parser.add_argument('-d', '--debug', action='store_true', help="Turns on debugging")
    args = parser.parse_args()

    application(output_folder=args.output,
                inset_chart_name=args.insetchartname,
                config_filename=args.config,
                debug=args.debug)
