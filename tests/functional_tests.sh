#!/bin/sh

# Check pytest command exists
if ! command -v pytest &> /dev/null; then
    echo "pytest not found"
    exit
fi

# Run pytest
pytest
