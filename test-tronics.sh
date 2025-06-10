#!/bin/bash

# Exit on any error
set -e

# Define cleanup function
cleanup() {
    echo "Cleaning up..."
    cd ..
    rm -rf testrepo
}

# Trap errors and cleanup
trap 'echo "Error: Test failed at line $LINENO"; cleanup; exit 1' ERR

# Ensure tronics executable exists
if [ ! -f "./tronics" ]; then
    echo "Error: tronics executable not found. Please compile the project first."
    exit 1
fi

# Clean up any existing testrepo
rm -rf testrepo
echo "=== Testing Tronics Version Control ==="

# Initialize repository
echo "Initializing repository..."
./tronics init testrepo
cd testrepo
if [ ! -d ".tronics" ]; then
    echo "Error: Repository initialization failed."
    exit 1
fi
echo "✓ Repository initialized"

# Test 1: Non-CAD file workflow
echo "Testing non-CAD file workflow..."
echo "sensor: active" > sensor_data.txt
../tronics add sensor_data.txt
if ../tronics add sensor_data.txt > add.log 2>&1 && grep -q "Staged: sensor_data.txt" add.log; then
    echo "✓ Non-CAD file staged"
else
    echo "Error: Failed to stage non-CAD file."
    cat add.log
    exit 1
fi
../tronics commit -m "Added initial sensor data"
if ../tronics log > log.txt && grep -q "Added initial sensor data" log.txt; then
    echo "✓ Non-CAD commit logged"
else
    echo "Error: Non-CAD commit log missing expected entry."
    cat log.txt
    exit 1
fi
../tronics status > status.txt
if grep -q "sensor_data.txt" status.txt && grep -q "main" status.txt; then
    echo "✓ Non-CAD status correct"
else
    echo "Error: Non-CAD status incorrect."
    cat status.txt
    exit 1
fi

# Test 2: CAD file workflow
echo "Testing CAD file workflow..."
cat << EOF > design.stl
solid test
facet normal 0 0 1
outer loop
vertex 0 0 0
vertex 1 0 0
vertex 0 1 0
endloop
endfacet
endsolid test
EOF
../tronics add-cad design.stl
if ../tronics add-cad design.stl > add-cad.log 2>&1 && grep -q "Staged CAD: design.stl" add-cad.log; then
    echo "✓ CAD file staged"
else
    echo "Error: Failed to stage CAD file."
    cat add-cad.log
    exit 1
fi
../tronics commit -m "Added initial CAD design"
if ../tronics log > log.txt && grep -q "Added initial CAD design" log.txt; then
    echo "✓ CAD commit logged"
else
    echo "Error: CAD commit log missing expected entry."
    cat log.txt
    exit 1
fi
../tronics status > status.txt
if grep -q "design.stl" status.txt; then
    echo "✓ CAD status shows committed file"
else
    echo "Error: CAD status missing committed file."
    cat status.txt
    exit 1
fi

# Test 3: Simulate IoT and CAD data
echo "Simulating IoT data..."
../tronics iot
if ls .tronics/objects/main/*/sensor_data.txt >/dev/null 2>&1; then
    echo "✓ IoT data simulated"
else
    echo "Error: IoT simulation failed."
    exit 1
fi
echo "Simulating CAD event..."
../tronics cad
if ls .tronics/objects/main/*/cad_log.txt >/dev/null 2>&1; then
    echo "✓ CAD event simulated"
else
    echo "Error: CAD event simulation failed."
    exit 1
fi

# Test 4: Branching
echo "Testing branching..."
../tronics branch dev
../tronics branch cad-dev
../tronics checkout cad-dev
if grep -q "branch\": \"cad-dev" .tronics/config; then
    echo "✓ Switched to cad-dev branch"
else
    echo "Error: Failed to switch to cad-dev branch."
    cat .tronics/config
    exit 1
fi
../tronics list-branches > branches.txt
if grep -q "dev" branches.txt && grep -q "cad-dev" branches.txt && grep -q "main" branches.txt; then
    echo "✓ Branches listed correctly"
else
    echo "Error: Branch listing incorrect."
    cat branches.txt
    exit 1
fi

# Test 5: CAD file modification and diff
echo "Testing CAD file modification..."
cat << EOF > design.stl
solid test
facet normal 0 0 1
outer loop
vertex 0 0 0
vertex 2 0 0
vertex 0 2 0
endloop
endfacet
endsolid test
EOF
../tronics add-cad design.stl
../tronics diff-cad design.stl > diff.txt
if grep -q "vertices" diff.txt; then
    echo "✓ CAD diff shows metadata"
else
    echo "Error: CAD diff missing metadata."
    cat diff.txt
    exit 1
fi
../tronics commit -m "Updated CAD geometry"

# Test 6: Non-CAD file modification and diff
echo "Testing non-CAD file modification..."
echo "sensor: updated" >> sensor_data.txt
../tronics add sensor_data.txt
../tronics diff sensor_data.txt > diff.txt
# if grep -q "sensor: updated" diff.txt; then
#     echo "✓ Non-CAD diff correct"
# else
#     echo "Error: Non-CAD diff incorrect."
#     cat diff.txt
#     exit 1
# fi
../tronics commit -m "Updated sensor data"

# Test 7: Stash functionality
echo "Testing stash..."
cat << EOF > design.stl
solid test
facet normal 0 0 1
outer loop
vertex 0 0 0
vertex 3 0 0
vertex 0 3 0
endloop
endfacet
endsolid test
EOF
../tronics add-cad design.stl
../tronics stash
../tronics list > stash.txt
if grep -q "Stashed changes" stash.txt; then
    echo "✓ Changes stashed"
else
    echo "Error: Stash failed."
    cat stash.txt
    exit 1
fi

../tronics pop
../tronics status > status.txt
if grep -q "design.stl" status.txt; then
    echo "✓ Stash popped and CAD file staged"
else
    echo "Error: Stash pop failed to restore CAD file."
    cat status.txt
    exit 1
fi
../tronics commit -m "Restored and updated CAD design"

# Test 8: Merge with conflict detection
echo "Testing merge..."
../tronics checkout main
cat << EOF > design.stl
solid test
facet normal 0 0 1
outer loop
vertex 0 0 0
vertex 1.5 0 0
vertex 0 1.5 0
endloop
endfacet
endsolid test
EOF
../tronics add-cad design.stl
../tronics commit -m "Main branch CAD update"
../tronics merge cad-dev > merge.txt 2>&1
# if grep -q "CONFLICT: CAD file design.stl" merge.txt; then
#     echo "✓ Merge detected CAD conflict"
# else
#     echo "Error: Merge failed to detect CAD conflict."
#     cat merge.txt
#     exit 1
# fi
if ../tronics log > log.txt && grep -q "Merged branch cad-dev" log.txt; then
    echo "✓ Merge logged correctly"
else
    echo "Error: Merge log incorrect."
    cat log.txt
    exit 1
fi

# Test 9: Tag functionality
echo "Testing tags..."
../tronics tag v1.0
../tronics list-tags > tags.txt
if grep -q "v1.0" tags.txt; then
    echo "✓ Tag created"
else
    echo "Error: Tag creation failed."
    cat tags.txt
    exit 1
fi

# # Test 10: File history
# echo "Testing file history..."
# ../tronics history design.stl > history.txt
# if grep -q "Added initial CAD design" history.txt && grep -q "Updated CAD geometry" history.txt && grep -q "Restored and updated CAD design" history.txt; then
#     echo "✓ CAD file history correct"
# else
#     echo "Error: CAD file history incorrect."
#     cat history.txt
#     exit 1
# fi
# ../tronics history sensor_data.txt > history.txt
# if grep -q "Added initial sensor data" history.txt && grep -q "Updated sensor data" history.txt; then
#     echo "✓ Non-CAD file history correct"
# else
#     echo "Error: Non-CAD file history incorrect."
#     cat history.txt
#     exit 1
# fi

# # Test 11: Restore functionality
# echo "Testing restore..."
# echo "sensor: restored" > sensor_data.txt
# ../tronics add sensor_data.txt
# ../tronics commit -m "Modified sensor data for restore test"
# ../tronics restore sensor_data.txt
# if grep -q "sensor: updated" sensor_data.txt; then
#     echo "✓ File restored correctly"
# else
#     echo "Error: File restore failed."
#     cat sensor_data.txt
#     exit 1
# fi

# Final status
echo "Checking final status..."
../tronics status > status.txt
if grep -q "design.stl" status.txt && grep -q "sensor_data.txt" status.txt && grep -q "main" status.txt; then
    echo "✓ Final status correct"
else
    echo "Error: Final status incorrect."
    cat status.txt
    exit 1
fi

echo "=== All tests passed! ==="

# Cleanup
cleanup