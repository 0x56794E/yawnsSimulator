#! /bin/bash
# Script input: <node count> <min deg> <proc count>  <delim for graph file; opt>
# Does the following things
# (1) Generate graph file
# (2) Generate files listing links for each proc
# (3) Generate files listing neighbors for ea link (java)
# (4) Generate traffic (java)


# Missing args
if [ $# -lt 3 ]
then
    echo "Usage: ./gen_input.sh <node count> <min deg> <proc count> <opt, delim>"

    # Have required args
else

    ######
    # (1) Generate graph file
    #####
    python python/GenGraph.py $1 $2 > "g$1_$2"

    ######
    # (2) Generate files listing links for ea proc
    #####
    link_count=$(cat "g$1_$2" | wc -l)
    python python/GenLinkPerProc.py "g$1_$2" " " $3 $link_count

    #####
    # (3) Generate neighbor files
    # - Compile the prog (just to be sure)
    # - Run the prog
    #####

    # No longer needed; keep for ref
    # compile and spec output at root
    #for f in "g$1_$2"*; do
    #    echo "Copying $f"
    #    cp "$f" java/util/$f
    #done

    javac -d . java/util/NeighborFinder.java
    java NeighborFinder "g$1_$2" " "

    ####
    # (4) Generate Traffic
    ####

    javac -d . java/util/TrafficGenerator.java
    java TrafficGenerator "g$1_$2" $3 " "
fi
