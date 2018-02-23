#!/bin/bash
# This script checks results.
#set -e # Exit with non 0 if any command fails
#set -vx

function check_docs {
    if [ -d ../docs/html ]
    then
        if [ ! -f ../docs/html/index.html ]
        then
            return "-1"
        fi
    else
        return "-1"
    fi
    if [ -d ../docs/latex ]
    then
        if [ ! -f ../docs/latex/Makefile ]
        then
            return "-1"
        fi
    else
        return "-1"
    fi
    if [ ! -d ../docs/man ]
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
        if [ ! -L "${HOME_INC}/$i" ] || [ ! -f "${HOME_INC}/$i" ] # Check if the "file" exists and is a symbolic link 
        then
            return "-1"
        fi
    done
    
    return "0"
}

function check_gtest_verdicts {
    if [ ! -f ../objs/gtestresults.xml ]
    then
        return "0" # It's not an error!
    fi
    nb_failure="-1"
    nb_failures=cat ../objs/gtestresults.xml | grep "<testsuites tests=" | cut -d' ' -f3 | cut -d'=' -f2

    return $nb_failures
}

if [ "${TRAVIS_CONTEXT}" != "BaremetalHW" ] && [ "${TRAVIS_CONTEXT}" != "LinuxHW" ]
then
    OLD_PWD=`pwd`
    project="logger"
    echo "Processing ${project} project"
    cd ${PATH_DEV}/g++/projects/embedded/${project}/objs
    check_lib ${project}
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
    declare -a include_files=("logger_factory.hh" "logger.hh" "logger_levels.hh" "logger_time_formats.hh")
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

    project="helper"
    echo "Processing ${project} project"
    cd ${PATH_DEV}/g++/projects/embedded/${project}/objs
    check_lib ${project}
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
    declare -a include_files=("date_time.hh" "helper.hh" "ibstream.hh" "keyboard.hh" "obstream.t.hh" "get_opt.hh" "helper.t.hh" "ibstream.t.hh" "obstream.hh" "runnable.hh")
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

    project="converter"
    echo "Processing ${project} project"
    cd ${PATH_DEV}/g++/projects/embedded/${project}/objs
    check_lib ${project}
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
    declare -a include_files=("converter.hh")
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

    project="ipc"
    echo "Processing ${project} project"
    cd ${PATH_DEV}/g++/projects/embedded/${project}/objs
    check_lib ${project}
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
    declare -a include_files=("abstract_ipc.hh" "ipc_rights.hh" "message_queue.hh" "ipc_manager.hh" "ipc_type.hh" "shared_memory.hh")
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
fi

exit 0
