minDet=5
detStep=100
maxDet=1000

minMetric=2
metricStep=2
maxMetric=10

testGenPath="./build/genTest"
testsFolder="tests"
testIdx=1

detIter=$minDet
metricIter=$minMetric

rm $testsFolder/genTest* $testsFolder/genAns*

while [ "$detIter" -le "$maxDet" ]; do
    while [ "$metricIter" -le "$maxMetric" ]; do
        ./build/genTest "$detIter" "$metricIter" "$testsFolder/genTest$testIdx.dat"
        touch "$testsFolder/genAns$testIdx.dat"
        echo "$detIter" > "$testsFolder/genAns$testIdx.dat"
        echo "Тест сгенерирован: Det = $detIter, Metric = $metricIter"

        metricIter=$((metricIter+metricStep))
        testIdx=$((testIdx+1))
    done
    detIter=$((detIter+detStep))
    metricIter=$minMetric
done