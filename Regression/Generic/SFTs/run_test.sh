#!/usr/bin/bash

rm *.txt
#touch ~/.rt_show.sft
bin_path="$(pwd|sed 's/Regression\/.*/\/build\/x64\/Release\/Eradication\/Eradication/g')"
p_path="$(pwd|sed 's/Regression\/.*/Regression\/shared_embedded_py_scripts/g')"
ip_path=".;$(pwd|sed 's/Regression\/.*/Regression\/Generic\/SFTs/g')"
ln -s $p_path dtk_test
touch dtk_test/__init__.py
"$bin_path" -C config.json -I "$ip_path" -P "$p_path" > test.txt
cat scientific_feature_report.txt
rm ~/.rt_show.sft
