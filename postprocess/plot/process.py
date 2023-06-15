#!/usr/bin/env python3
import csv
import sys

# Script to convert the serial vomit produced by the ground station tty
# into parseable CSV data


def get_val(in_text):
    split = in_text.split(":")
    if split[1] == "ovf" or split[1] == "0.00":
        return None
    return float(split[1])


if __name__ == "__main__":
    with open(sys.argv[1], "rt") as data_file:
        ll_raw = (fl.split(" ")[:2] for fl in data_file.readlines()[0::5])
        pairs = [[get_val(line[0]), get_val(line[1])] for line in ll_raw]

    with open(sys.argv[2], "wt") as csv_file:
        writer = csv.writer(csv_file)
        writer.writerow(["latitude", "longitude"])
        for row in pairs:
            writer.writerow(row)
