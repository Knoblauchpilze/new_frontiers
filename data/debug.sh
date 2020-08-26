#!/bin/sh

CURR_DIR=$(dirname $0)

gdb --args ./bin/new_frontiers $(cat data/config/local)
