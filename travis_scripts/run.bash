#!/bin/bash
# Execute all test library applications. This script does not check results.
set -evx

function make_runs {
    if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
    then
        make test_d
        make run
    elif [ ${TRAVIS_CONTEXT} == "WithCoveralls" ]
    then
        make test_c
        make run_c
        if [ -f ./logger.coverage.info ]
        then
            coveralls-lcov --verbose ./logger.coverage.info
        fi
    else
        make test
        make run_v
    fi
}

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/projects/embedded/
make_runs
cd ${OLD_PWD}

exit 0
