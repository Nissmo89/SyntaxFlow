#!/bin/bash
set -euo pipefail

PROJECT="${1:-${APPVEYOR_PROJECT:-}}"

if [[ -z "$PROJECT" ]]; then
    echo "Usage: $0 <account/project>"
    exit 2
fi

MAX_ATTEMPTS="${APPVEYOR_MAX_ATTEMPTS:-10}"
ATTEMPT=1

echo "Starting AppVeyor CI loop for $PROJECT. Max attempts: $MAX_ATTEMPTS"

while [[ $ATTEMPT -le $MAX_ATTEMPTS ]]; do
    echo "=========================================="
    echo " Attempt $ATTEMPT / $MAX_ATTEMPTS"
    echo "=========================================="

    # Push current commit (assume everything is committed for this attempt)
    echo "Pushing to remote..."
    export EXPECTED_COMMIT="$(git rev-parse HEAD)"
    git push origin main

    echo "Waiting for AppVeyor build for commit $EXPECTED_COMMIT..."
    if ./scripts/appveyor-ci.sh "$PROJECT"; then
        echo "=========================================="
        echo " AppVeyor CI SUCCESS"
        echo "=========================================="
        echo "Project : $PROJECT"
        echo "Commit  : $EXPECTED_COMMIT"
        echo "Status  : success"
        echo ""
        echo "Artifact:"
        echo "SyntaxFlow-Windows-x64.zip"
        echo ""
        echo "Attempts: $ATTEMPT"
        echo "=========================================="
        exit 0
    else
        echo "AppVeyor build failed!"
        echo "Please examine the log, diagnose, fix, and commit your changes."
        echo "Then press enter to trigger the next attempt, or Ctrl+C to abort."
        read -r
    fi

    ATTEMPT=$((ATTEMPT + 1))
done

echo "AppVeyor did not become successful after $MAX_ATTEMPTS attempts."
exit 1
