#!/bin/bash
# This script checks results.
set -evx

function check_docs {
    if [ ! -d ../docs/html ]
    then
       return "-1"
    elif [ ! -d ../docs/latex ]
    then
        return "-1"
    elif [ ! -d ../docs/man ]
    then
        return "-1"
    fi

    return "0"
}

function check_lib {
    if [ ! -d ../lib ]
    then
       return "-1"
    elif [ ! -f ../lib/lib$1.so ]
    then
        return "-1"
    fi

    return "0"
}

function check_test {
    if [ ! -d ../test ]
    then
       return "-1"
    elif [ ! -f ../test/$1 ]
    then
        return "-1"
    fi

    return "0"
}

function check_includes {
    for i in $1
    do
        if [ ! -L ${HOME_INC}/$1 ] # Check if the "file" exists and is a symbolic link 
        then
            return "-1"
        fi
    done
    
    return "0"
}

function check_gtest_verdicts {
    if [ ! -f ../objs/gtestresults.xml ]
    then
        return "-1"
    fi
    nb_failures=cat ../objs/gtestresults.xml | grep "<testsuites tests=" | cut -d' ' -f3 | cut -d'=' -f2

    return $nb_failures
}

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/projects/embedded/logger/objs
check_lib "logger"
if [ "$?" != "0" ]
then
    echo "Checking lib failed"
    exit -2
fi
check_test "testlib"
if [ "$?" != "0" ]
then
    echo "Checking test failed"
    exit -3
fi
declare -a include_files=("logger_factory.h" "logger.h" "logger_levels.h" "logger_time_formats.h")
check_includes "${include_files[@]}"
if [ "$?" != "0" ]
then
    echo "Checking includes failed"
    exit -4
fi
check_docs
if [ "$?" != "0" ]
then
    echo "Checking docs failed"
    exit -1
fi
check_gtest_verdicts
if [ "$?" != "0" ]
then
    echo "GoogleTest found FAIL verdicts"
    exit -1
fi

cd ${OLD_PWD}

exit 0
