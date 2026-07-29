import sys,getopt,os
from optparse import OptionParser
import shutil
from bagpy import bagreader

def extract_bags(bag_file):
    bag = bagreader(bag_file,delimiter=str(','))
    for topic in bag.topic_table['Topics']:
        csv_data = bag.message_by_topic(topic)
def main():
    parser = OptionParser()#"extract_bag_to_csv.py [options]")
    parser.add_option("-b","--bag_file",dest="bag_file",default="",help="Bag File")
    (opts,args) = parser.parse_args()
    extract_bags(opts.bag_file)


if __name__ == "__main__":
    main()
