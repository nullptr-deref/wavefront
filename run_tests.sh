tests_dir=$1
counter=1
for test_case in "$tests_dir"/*
do
    echo -e "\n=== $counter. Running test $test_case... ===\n"
    $test_case
    echo -e "\n=== Done running test $test_case. ==="
    counter=$(($counter+1))
done
