BUILD_DIR="build"
TEST_DIR="test"
SMOKE_DIR="example"

unit_test() {
    local path=$1
    local test_file="$1/test.twge"
    local test_result="$1/output.events"
    local test_error="$1/error.log"
    local test_print="$1/print.twge"
    option=""
    if [[ -s "$test_file" ]]; then
        # Fail case
        if [[ -s $test_error ]] && [[ ! -s $test_result ]] && [[ ! -s $test_print ]]; then
            first_line=$(head -n 1 $test_file)
            if [[ $first_line == //\ OPTION:\ * ]]; then
                option="${first_line:11}"
            fi
            eval "$BUILD_DIR/twgec $test_file $option" 2> error.log
            if diff error.log $test_error ; then
                echo "twge test success:    $test_file"
            else
                echo -e "\033[0;31mtwge test fail:       $test_file\033[0m"
            fi
            rm error.log
        # Pass case
        elif [[ -s $test_result ]] && [[ ! -s $test_error ]] && [[ ! -s $test_print ]]; then
            eval $BUILD_DIR/twgec $test_file 2> error.log
            if [[ ! -s error.log ]]; then
                if diff game.events "$test_result"; then
                    echo "twge test success:    $test_file"
                else
                    echo -e "\033[0;31mtwge test fail:       $test_file\033[0m"
                fi
            else
                cat error.log
                echo -e "\033[0;31mtwge test fail:       $test_file\033[0m"
                rm error.log
            fi
            rm game.events
        # Pass case with print
        elif [[ -s $test_print ]] && [[ ! -s $test_result ]] && [[ ! -s $test_error ]]; then
            eval $BUILD_DIR/twgec $test_file 2> error.log
            first_line=$(head -n 1 $test_file)
            if [[ $first_line == //\ OPTION:\ * ]]; then
                command="${first_line:11}"
                eval "$BUILD_DIR/twgec $test_file $command" 2> error.log 1> print.twge 
                if diff print.twge "$test_print"; then
                    echo "twge test success:    $test_file"
                else
                    echo -e "\033[0;31mtwge test fail:       $test_file\033[0m"
                fi
            else
                echo "The first line does not start with '// OPTION: '"
                exit 1
            fi
            rm error.log
            rm print.twge
            rm game.events
        else
            echo "Please provide exactly ONE \"output.events\", \"error.log\", or \"print.twge\" under \"$path\""
        fi
    fi
}

smoke_test() {
    local input_file="$1"
    local ast_after_file="$(dirname "$input_file")/output/ast-after.twge"
    local output_file="$(dirname "$input_file")/output/output.events"
    $BUILD_DIR/twgec "$input_file" -o=$output_file 2> error.log
    if [[ ! -s error.log ]]; then
        $BUILD_DIR/twgec "$input_file" --print-ast-after > $ast_after_file 
        echo "twge test success:    $input_file"
    else
        cat error.log
        echo -e "\033[0;31mtwge test fail:       $input_file\033[0m"
    fi
    rm game.events
    rm error.log
}

for dir in $(find $TEST_DIR -type d); do
    unit_test $dir
done

for file in $(find $SMOKE_DIR -type f -name "main.twge"); do
    smoke_test $file
done
