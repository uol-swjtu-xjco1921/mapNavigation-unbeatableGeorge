#! /bin/bash

# Initialize pass and fail variables
pass=0
fail=0

# Execute test case 1
echo "Running Test Case 1..."
echo -e "leeds.map\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output1.txt
if diff -w ./expected_output/expected_output1.txt ./output/output1.txt; then
    echo "Test Case 1 Passed!"
    ((pass++))
else
    echo "Test Case 1 Failed! test read, read file nodes"
    ((fail++))
fi

echo "Running Test Case 2..."
echo -e "leeds.map\n2\n454231775\n-2544\n1\n1\n" | ./main > ./output/output2.txt
if diff -w ./expected_output/expected_output2.txt ./output/output2.txt; then
    echo "Test Case 2 Passed!"
    ((pass++))
else
    echo "Test Case 2 Failed! test read, read file nodes"
    ((fail++))
fi

echo "Running Test Case 3..."
echo -e "leeds.map\n2\n454231775\n-2548\n1\n1\n" | ./main > ./output/output3.txt
if diff -w ./expected_output/expected_output3.txt ./output/output3.txt; then
    echo "Test Case 3 Passed!"
    ((pass++))
else
    echo "Test Case 3 Failed! test read, read file links"
    ((fail++))
fi

echo "Running Test Case 4..."
echo -e "leeds.map\n2\n-8849\n-2544\n1\n1\n" | ./main > ./output/output4.txt
if diff -w ./expected_output/expected_output4.txt ./output/output4.txt; then
    echo "Test Case 4 Passed!"
    ((pass++))
else
    echo "Test Case 4 Failed! test read, read file links"
    ((fail++))
fi

echo "Running Test Case 5..."
echo -e "leeds.map\n2\n-2546\n-2526\n1\n1\n" | ./main > ./output/output5.txt
if diff -w ./expected_output/expected_output5.txt ./output/output5.txt; then
    echo "Test Case 5 Passed!"
    ((pass++))
else
    echo "Test Case 5 Failed! test read, read file geoms"
    ((fail++))
fi

echo "Running Test Case 6..."
echo -e "leeds.map\n2\n-2440\n-2442\n1\n1\n" | ./main > ./output/output6.txt
if diff -w ./expected_output/expected_output6.txt ./output/output6.txt; then
    echo "Test Case 6 Passed!"
    ((pass++))
else
    echo "Test Case 6 Failed! test read, read file geoms"
    ((fail++))
fi

echo "Running Test Case 7..."
echo -e "leeds.map\n1\n1\n-1\n2\n3\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output7.txt
if diff -w ./expected_output/expected_output7.txt ./output/output7.txt; then
    echo "Test Case 7 Passed!"
    ((pass++))
else
    echo "Test Case 7 Failed! test modity, add point"
    ((fail++))
fi

echo "Running Test Case 8..."
echo -e "leeds.map\n1\n1\n-1\n2\n1\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output8.txt
if diff -w ./expected_output/expected_output8.txt ./output/output8.txt; then
    echo "Test Case 8 Passed!"
    ((pass++))
else
    echo "Test Case 8 Failed! test modity, add point"
    ((fail++))
fi

echo "Running Test Case 9..."
echo -e "leeds.map\n1\n1\n-1\n99\n2\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output9.txt
if diff -w ./expected_output/expected_output9.txt ./output/output9.txt; then
    echo "Test Case 9 Passed!"
    ((pass++))
else
    echo "Test Case 9 Failed! test modity, add point"
    ((fail++))
fi

echo "Running Test Case 10..."
echo -e "leeds.map\n1\n2\n4\n2\n2\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output10.txt
if diff -w ./expected_output/expected_output10.txt ./output/output10.txt; then
    echo "Test Case 10 Passed!"
    ((pass++))
else
    echo "Test Case 10 Failed! test modity, add edge"
    ((fail++))
fi

echo "Running Test Case 11..."
echo -e "leeds.map\n1\n2\n4\n-33\n89\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output11.txt
if diff -w ./expected_output/expected_output11.txt ./output/output11.txt; then
    echo "Test Case 11 Passed!"
    ((pass++))
else
    echo "Test Case 11 Failed! test modity, add edge"
    ((fail++))
fi

echo "Running Test Case 12..."
echo -e "leeds.map\n1\n2\n4\n23\n22\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output12.txt
if diff -w ./expected_output/expected_output12.txt ./output/output12.txt; then
    echo "Test Case 12 Passed!"
    ((pass++))
else
    echo "Test Case 12 Failed! test modity, add edge"
    ((fail++))
fi

echo "Running Test Case 13..."
echo -e "leeds.map\n1\n3\n589\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output13.txt
if diff -w ./expected_output/expected_output13.txt ./output/output13.txt; then
    echo "Test Case 13 Passed!"
    ((pass++))
else
    echo "Test Case 13 Failed! test modity, edit edge"
    ((fail++))
fi

echo "Running Test Case 14..."
echo -e "leeds.map\n1\n3\n948\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output14.txt
if diff -w ./expected_output/expected_output14.txt ./output/output14.txt; then
    echo "Test Case 14 Passed!"
    ((pass++))
else
    echo "Test Case 14 Failed! test modity, edit edge"
    ((fail++))
fi

echo "Running Test Case 15..."
echo -e "leeds.map\n1\n3\n12\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output15.txt
if diff -w ./expected_output/expected_output15.txt ./output/output15.txt; then
    echo "Test Case 15 Passed!"
    ((pass++))
else
    echo "Test Case 15 Failed! test modity, edit edge"
    ((fail++))
fi

echo "Running Test Case 16..."
echo -e "leeds.map\n2\n1659456324\n-2560\n1\n1\n" | ./main > ./output/output16.txt
if diff -w ./expected_output/expected_output16.txt ./output/output16.txt; then
    echo "Test Case 16 Passed!"
    ((pass++))
else
    echo "Test Case 16 Failed! test find path, fastest path"
    ((fail++))
fi

echo "Running Test Case 17..."
echo -e "leeds.map\n2\n-8849\n1659456324\n1\n1\n" | ./main > ./output/output17.txt
if diff -w ./expected_output/expected_output17.txt ./output/output17.txt; then
    echo "Test Case 17 Passed!"
    ((pass++))
else
    echo "Test Case 17 Failed! test find path, fastest path"
    ((fail++))
fi

echo "Running Test Case 18..."
echo -e "leeds.map\n2\n249612090\n-2560\n1\n1\n" | ./main > ./output/output18.txt
if diff -w ./expected_output/expected_output18.txt ./output/output18.txt; then
    echo "Test Case 18 Passed!"
    ((pass++))
else
    echo "Test Case 18 Failed! test find path, fastest path"
    ((fail++))
fi

echo "Running Test Case 19..."
echo -e "leeds.map\n2\n-8849\n1659456324\n1\n2\n" | ./main > ./output/output19.txt
if diff -w ./expected_output/expected_output19.txt ./output/output19.txt; then
    echo "Test Case 19 Passed!"
    ((pass++))
else
    echo "Test Case 19 Failed! test find path, shortest path"
    ((fail++))
fi

echo "Running Test Case 20..."
echo -e "leeds.map\n2\n249612090\n-2560\n1\n2\n" | ./main > ./output/output20.txt
if diff -w ./expected_output/expected_output20.txt ./output/output20.txt; then
    echo "Test Case 20 Passed!"
    ((pass++))
else
    echo "Test Case 20 Failed! test find path, shortest path"
    ((fail++))
fi

echo "Running Test Case 21..."
echo -e "leeds.map\n2\n1659456324\n-2560\n1\n2\n" | ./main > ./output/output21.txt
if diff -w ./expected_output/expected_output21.txt ./output/output21.txt; then
    echo "Test Case 21 Passed!"
    ((pass++))
else
    echo "Test Case 21 Failed! test find path, shortest path"
    ((fail++))
fi

echo "Running Test Case 22..."
echo -e "leeds.map\n2\n-8849\n-2560\n2\n2\n1725582979\n" | ./main > ./output/output22.txt
if diff -w ./expected_output/expected_output22.txt ./output/output22.txt; then
    echo "Test Case 22 Passed!"
    ((pass++))
else
    echo "Test Case 22 Failed! test find path, pass a location"
    ((fail++))
fi

echo "Running Test Case 23..."
echo -e "leeds.map\n2\n249612090\n1659456324\n2\n2\n1967770418\n" | ./main > ./output/output23.txt
if diff -w ./expected_output/expected_output23.txt ./output/output23.txt; then
    echo "Test Case 23 Passed!"
    ((pass++))
else
    echo "Test Case 23 Failed! test find path, pass a location"
    ((fail++))
fi

echo "Running Test Case 24..."
echo -e "leeds.map\n2\n1659456324\n-2560\n2\n2\n1967770418\n" | ./main > ./output/output24.txt
if diff -w ./expected_output/expected_output24.txt ./output/output24.txt; then
    echo "Test Case 24 Passed!"
    ((pass++))
else
    echo "Test Case 24 Failed! test find path, pass a location"
    ((fail++))
fi

echo "Running Test Case 25..."
echo -e "leeds.map\n2\n-8849\n-2560\n3\n3\n" | ./main > ./output/output25.txt
if diff -w ./expected_output/expected_output25.txt ./output/output25.txt; then
    echo "Test Case 25 Passed!"
    ((pass++))
else
    echo "Test Case 25 Failed! test find path, pass a POI"
    ((fail++))
fi

echo "Running Test Case 26..."
echo -e "leeds.map\n2\n-8849\n-2560\n3\n12\n" | ./main > ./output/output26.txt
if diff -w ./expected_output/expected_output26.txt ./output/output26.txt; then
    echo "Test Case 26 Passed!"
    ((pass++))
else
    echo "Test Case 26 Failed! test find path, pass a POI"
    ((fail++))
fi

echo "Running Test Case 27..."
echo -e "leeds.map\n2\n-8849\n-2560\n3\n8\n" | ./main > ./output/output27.txt
if diff -w ./expected_output/expected_output27.txt ./output/output27.txt; then
    echo "Test Case 27 Passed!"
    ((pass++))
else
    echo "Test Case 27 Failed! test find path, pass a POI"
    ((fail++))
fi

echo "Running Test Case 28..."
echo -e "leedddsss.map\nleeds.map\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output28.txt
if diff -w ./expected_output/expected_output28.txt ./output/output28.txt; then
    echo "Test Case 28 Passed!"
    ((pass++))
else
    echo "Test Case 28 Failed! test invalid input"
    ((fail++))
fi

echo "Running Test Case 29..."
echo -e "leeds.map\n23\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output29.txt
if diff -w ./expected_output/expected_output29.txt ./output/output29.txt; then
    echo "Test Case 29 Passed!"
    ((pass++))
else
    echo "Test Case 29 Failed! test invalid input"
    ((fail++))
fi

echo "Running Test Case 30..."
echo -e "leeds.map\n2\na\n-8849\n-2560\n1\n1\n" | ./main > ./output/output30.txt
if diff -w ./expected_output/expected_output30.txt ./output/output30.txt; then
    echo "Test Case 30 Passed!"
    ((pass++))
else
    echo "Test Case 30 Failed! test write file"
    ((fail++))
fi

echo "Running Test Case 31..."
echo -e "leeds.map\n1\n34\n1\n2\n2\n2\n2\n-8849\n-2560\n1\n1\n" | ./main > ./output/output31.txt
if diff -w ./expected_output/expected_output31.txt ./output/output31.txt; then
    echo "Test Case 31 Passed!"
    ((pass++))
else
    echo "Test Case 31 Failed! test invalid input"
    ((fail++))
fi

# Output test results
echo "Pass: $pass"
echo "Fail: $fail"
