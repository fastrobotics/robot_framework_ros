#!/bin/bash
topics=(
	"/imu"
	"/robot/arm_command"
	"/robot/nodeIMU/heartbeat"
)

function print_usage() {
	echo "Usage Instructions"
	echo "<Bag File> <Output Directory>"
	exit 1
}
function extract_bag() {
	bag_file=$1
	output_dir=$2
	filename="${bag_file##*/}"
	extract_folder=$output_dir${filename%.*}
	rm -rf $extract_folder
	mkdir -p $extract_folder
	for topic in "${topics[@]}"; do
    	echo "Extracting $topic"
		topic_filename="${topic////_}"
		output_csv=$extract_folder"/"$topic_filename.csv
		cmd="rostopic echo -b "$bag_file" -p "$topic" > "$output_csv
		eval $cmd
	done
}

if [ $# -eq 0 ]; then
	print_usage
else
	extract_bag $1 $2
fi
exit 0
