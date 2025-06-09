#!/bin/bash

# Initialize repository
./tronics init testrepo
cd testrepo

# Create and commit initial file
echo "sensor: active" > sensor_data.txt
../tronics add sensor_data.txt
../tronics commit -m "Added sensor data"

# Show commit log
../tronics log

# Show status
../tronics status

# Simulate IoT and CAD data
../tronics iot
../tronics cad

# Create and switch to dev branch
../tronics branch dev
../tronics list-branches
../tronics checkout dev

# Check diff and restore
../tronics diff sensor_data.txt
../tronics restore sensor_data.txt
../tronics status

# Test stash functionality
echo "sensor: updated" >> sensor_data.txt
../tronics add sensor_data.txt
../tronics stash
../tronics list-stash
../tronics status
../tronics pop
../tronics status
../tronics commit -m "Updated sensor data"

# Test file history
../tronics history sensor_data.txt

# Test merge
../tronics checkout main
echo "sensor: merged" >> sensor_data.txt
../tronics add sensor_data.txt
../tronics commit -m "Main branch update"
../tronics merge dev
../tronics log

# Test tags
../tronics tag v1.0
../tronics list-tags

# Final status
../tronics status