#!/bin/bash

# Exit on any error
set -e

# Ensure tronics executable exists
if [ ! -f "./tronics" ]; then
    echo "Error: tronics executable not found. Please compile tronics.c first."
    exit 1
fi

# Clean up any existing testrepo
rm -rf testrepo
echo "=== Testing Tronics CAD Integration ==="

# Initialize repository
echo "Initializing repository..."
./tronics init testrepo
cd testrepo
if [ ! -d ".tronics" ]; then
    echo "Error: Repository initialization failed."
    exit 1
fi
echo "✓ Repository initialized"

# Create and commit initial CAD file
echo "Creating initial CAD file (design.stl)..."
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
echo "Checking add-cad output..."
if ../tronics add-cad design.stl > add-cad.log 2>&1 && grep -q "Staged CAD: design.stl" add-cad.log; then
    echo "✓ CAD file staged"
else
    echo "Error: Failed to stage CAD file."
    exit 1
fi
../tronics commit -m "Added initial CAD design"
echo "✓ Initial CAD design committed"

# Show commit log
echo "Checking commit log..."
../tronics log > log.txt
if grep -q "Added initial CAD design" log.txt; then
    echo "✓ Commit log correct"
else
    echo "Error: Commit log missing expected entry."
    exit 1
fi

# Show status
echo "Checking status..."
../tronics status > status.txt
if grep -q "design.stl" status.txt; then
    echo "✓ Status shows committed CAD file"
else
    echo "Error: Status missing committed CAD file."
    cat status.txt
    exit 1
fi

# Simulate IoT and CAD data
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

# Create and switch to cad-dev branch
echo "Creating and switching to cad-dev branch..."
../tronics branch cad-dev
../tronics checkout cad-dev
if grep -q "branch\": \"cad-dev" .tronics/config; then
    echo "✓ Switched to cad-dev branch"
else
    echo "Error: Failed to switch to cad-dev branch."
    exit 1
fi
../tronics list-branches > branches.txt
if grep -q "cad-dev" branches.txt && grep -q "main" branches.txt; then
    echo "✓ Branches listed correctly"
else
    echo "Error: Branch listing incorrect."
    exit 1
fi

# Edit CAD file and test diff
echo "Editing alt..."
../tronics add-cad design.stl
echo "Checking CAD diff..."
../tronics diff-cad design.stl > diff.txt
if grep -q "Metadata: vertices" diff.txt; then
    echo "✓ CAD diff shows metadata"
else
    echo "Error: CAD diff missing metadata."
    exit 1
fi
../tronics commit -m "Updated CAD geometry"

# Test stash functionality
echo "Testing stash with CAD file..."
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
../tronics list-stash > stash.txt
if grep -q "Stashed changes" stash.txt; then
    echo "✓ Changes stashed"
else
    echo "Error: Stash failed."
    exit 1
fi
../tronics pop
../tronics status > status.txt
if grep ^ "design.stl" status.txt; then
    echo "✓ Stash popped and CAD file staged"
else
    echo "Error: Stash pop failed to restore CAD file."
    exit 1
fi
../tronics commit -m "Restored and updated CAD design"

# Test collaboration
echo "Testing collaboration..."
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
if ! ../tronics merge cad-dev > merge.txt 2>&1; then
    echo "Error: Merge command caused a segmentation fault."
    exit 1
fi
if grep -q "CONFLICT: CAD file design.stl" merge.txt; then
    echo "✓ Merge detected CAD conflict"
else
    echo "Error: Merge failed to detect CAD conflict."
    cat merge.txt
    exit 1
fi
../tronics log > log.txt
if grep -q "Merged branch cad-dev" log.txt; then
    echo "✓ Merge logged correctly"
else
    echo "Error: Merge log incorrect."
    exit 1
fi

# Tag release
echo "Tagging release..."
../tronics tag design-v1.0
../tronics list-tags > tags.txt
if grep -q "design-v1.0" tags.txt; then
    echo "✓ Tag created"
else
    echo "Error: Tag creation failed."
    exit 1
fi

# Test history
echo "Checking CAD file history..."
../tronics history design.stl > history.txt
if grep -q "Added initial CAD design" history.txt && grep -q "Updated CAD geometry" history.txt; then
    echo "✓ CAD file history correct"
else
    echo "Error: CAD file history incorrect."
    exit 1
fi

# Final status
echo "Checking final status..."
../tronics status > status.txt
if grep -q "design.stl" status.txt && grep -q "main" status.txt; then
    echo "✓ Final status correct"
else
    echo "Error: Final status incorrect."
    cat status.txt
    exit 1
fi

echo "=== All tests passed! ==="