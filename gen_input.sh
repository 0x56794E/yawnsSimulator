#! /bin/bash

# Script input: <node count> <min deg> <proc count> <max pkg ct, opt> <max stop, opt>
# Does the following things
# (1) Generate graph file
# (2) Generate files listing LPs for each proc
# (3) Generate files listing neighbors for ea LP (java)
# (4) Generate traffic (java)


# Missing args
if [ $# -lt 3 ]
then
    echo "Len=$#"
    echo "Usage: ./gen_input.sh <node count> <min deg> <proc count> <opt, delim>"

    # Have required args
else

    ######
    # (1) Generate graph file
    #####
    python python/GenGraph.py $1 $2 > "g$1_$2"

    ######
    # (2) Generate files listing links and nodes for ea proc
    #####

    # Gen for links
    link_count=$(cat "g$1_$2" | wc -l)
    python python/GenLinkPerProc.py "g$1_$2" " " $3 $link_count


    # Gen for nodes
    python python/GenNodePerProc.py "g$1_$2" $3 $1
    
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

    # link stuff
    javac -d . java/util/NeighborFinder.java
    java NeighborFinder "g$1_$2" " "


    # node stuff
    javac -d . java/util/NodeNeighborFinder.java
    java NodeNeighborFinder "g$1_$2" " "
    
    ####
    # (4) Generate Traffic
    ####

    javac -d . java/util/TrafficGenerator.java

    # If max pkg ct and max stop is spec
    if [ $# -eq 5 ] 
    then
        java TrafficGenerator "g$1_$2" $3 " " $4 $5
    else
        java TrafficGenerator "g$1_$2" $3 " "
    fi

    #clean up
    rm *.class
    
fi
