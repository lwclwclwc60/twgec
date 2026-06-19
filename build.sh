WORKSPACE="$(cd "$(dirname "$0")" && pwd)"
RUN_TESTS=1
UPDATE_HELP=1

# Options:
# -t: skip test execution
# -u: skip help file update
for arg in "$@"; do
    if [ "${arg#-}" != "$arg" ]; then
        flags="${arg#-}"
        i=0
        while [ $i -lt "${#flags}" ]; do
            flag="${flags:$i:1}"
            if [ "$flag" = "t" ]; then
                RUN_TESTS=0
            elif [ "$flag" = "u" ]; then
                UPDATE_HELP=0
            fi
            i=$((i + 1))
        done
    fi
done

mkdir -p $WORKSPACE/build
cd $WORKSPACE/build
cmake $WORKSPACE/src -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
if cmake --build .; then
    cd $WORKSPACE
    if [ "$RUN_TESTS" -eq 1 ]; then
        bash $WORKSPACE/test/run-test.sh
    fi
    if [ "$UPDATE_HELP" -eq 1 ]; then
        $WORKSPACE/build/twgec --help 2> $WORKSPACE/example/help.txt
        echo "Help Document is updated"
    fi
fi