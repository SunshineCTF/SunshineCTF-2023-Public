compilerPath='$(which pdc)'
# simulatorPath="~/Developer/PlaydateSDK/bin/Playdate\ Simulator.app/Contents/MacOS/PlaydateSimulator"
PLAYDATE_SDK_PATH='~/Developer/PlaydateSDK'
simulatorPath="${PLAYDATE_SDK_PATH}/bin/Playdate\ Simulator.app/Contents/MacOS/Playdate\ Simulator"
sourcePath='./src'
outputPath='./attachments/first_date.pdx'

# pdc [sourcepath] [outputpath]

echo "Compiler path:  $compilerPath"
echo "Simulator path: $simulatorPath"
echo "Source path:    $sourcePath"
echo "Output path:    $outputPath"
echo ""

echo "Compile source..."
eval $compilerPath $sourcePath $outputPath

echo "Opening simulator..."
eval $simulatorPath $outputPath
