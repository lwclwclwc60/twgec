BUILD_DIR="build"
TEST_DIR="test"
SMOKE_DIR="example"

unit_test() {
    local test_file="$1"
    local path=$(dirname $test_file)
    local test_result="$path/output.events"
    local test_error="$path/error.log"
    local test_print="$path/print.twge"
    local output_dir="$path/output"
    mkdir -p "$output_dir"
    local output_print="$output_dir/print.twge"
    local output_event="$output_dir/output.events"
    local output_error="$output_dir/error.log"
    local option=""

    if [[ -s "$test_file" ]]; then
        # Fail case
        if [[ -s $test_error ]] && [[ ! -s $test_result ]] && [[ ! -s $test_print ]]; then
            first_line=$(head -n 1 $test_file)
            if [[ $first_line == //\ OPTION:\ * ]]; then
                option="${first_line:11}"
            fi
            $BUILD_DIR/twgec $test_file $option -o=$output_event 2> $output_error
            if diff $output_error $test_error ; then
                echo "twge test success:    $test_file"
            else
                echo -e "\033[0;31mtwge test fail:       $test_file\033[0m"
                exit 1
            fi
        # Pass case
        elif [[ -s $test_result ]] && [[ ! -s $test_error ]] && [[ ! -s $test_print ]]; then
            $BUILD_DIR/twgec $test_file -o=$output_event 2> $output_error
            if [[ ! -s $output_error ]]; then
                if diff $output_event "$test_result"; then
                    echo "twge test success:    $test_file"
                else
                    echo -e "\033[0;31mtwge test fail:       $test_file\033[0m"
                    exit 1
                fi
            else
                cat $output_error
                echo -e "\033[0;31mtwge test fail:       $test_file\033[0m"
                exit 1
            fi
        # Pass case with print
        elif [[ -s $test_print ]] && [[ ! -s $test_result ]] && [[ ! -s $test_error ]]; then
            $BUILD_DIR/twgec $test_file 2> $output_error
            first_line=$(head -n 1 $test_file)
            if [[ $first_line == //\ OPTION:\ * ]]; then
                command="${first_line:11}"
                $BUILD_DIR/twgec $test_file $command -o=$output_event 2> $output_error 1> $output_print
                if diff $output_print "$test_print"; then
                    echo "twge test success:    $test_file"
                else
                    echo -e "\033[0;31mtwge test fail:       $test_file\033[0m"
                    exit 1
                fi
            else
                echo "The first line does not start with '// OPTION: '"
                exit 1
            fi
        else
            echo "Please provide exactly ONE \"output.events\", \"error.log\", or \"print.twge\" under \"$path\""
        fi
    fi
    rm -rf $output_dir
}

smoke_test() {
    local input_file="$1"
    local path=$(dirname "$input_file")
    local output_ast="$path/output/ast-after.twge"
    local output_event="$path/output/output.events"
    local output_error="$path/output/error.log"

    $BUILD_DIR/twgec "$input_file" -o=$output_event 2> $output_error
    if [[ ! -s $output_error ]]; then
        $BUILD_DIR/twgec "$input_file" --print-ast-after > $output_ast 
        echo "twge test success:    $input_file"
    else
        cat $output_error
        echo -e "\033[0;31mtwge test fail:       $input_file\033[0m"
        exit 1
    fi
    rm $output_error
}

for file in $(find $SMOKE_DIR -type f -name "main.twge"); do
    smoke_test $file &
done

for file in $(find $TEST_DIR -type f -name "test.twge"); do
    unit_test $file
done

wait