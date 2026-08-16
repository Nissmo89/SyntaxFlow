#!/bin/bash
set -euo pipefail

PROJECT="${1:-${APPVEYOR_PROJECT:-}}"

if [[ -z "$PROJECT" ]]; then
    echo "Usage: $0 <account/project>"
    exit 2
fi

AUTH_ARGS=()
if [[ -n "${APPVEYOR_TOKEN:-}" ]]; then
    AUTH_ARGS=(-H "Authorization: Bearer ${APPVEYOR_TOKEN}")
fi

EXPECTED_COMMIT="${EXPECTED_COMMIT:-}"

echo "Fetching latest AppVeyor build for project: $PROJECT..."

while true; do
    RESPONSE=$(curl -sS -f "${AUTH_ARGS[@]}" "https://ci.appveyor.com/api/projects/${PROJECT}")
    if [[ -z "$RESPONSE" ]]; then
        echo "Failed to fetch project info from AppVeyor API."
        exit 1
    fi

    BUILD_VERSION=$(echo "$RESPONSE" | jq -r '.build.version')
    STATUS=$(echo "$RESPONSE" | jq -r '.build.status')
    COMMIT=$(echo "$RESPONSE" | jq -r '.build.commitId')
    
    # Try to find a Windows x64 job, else take the first job
    JOB_ID=$(echo "$RESPONSE" | jq -r '.build.jobs[] | select(.name == "Environment: QT_VERSION=6.8.0, QT_ARCH=win64_msvc2022_64") | .jobId' | head -n1)
    if [[ -z "$JOB_ID" ]]; then
        JOB_ID=$(echo "$RESPONSE" | jq -r '.build.jobs[0].jobId')
    fi

    echo "=========================================="
    echo " AppVeyor Build"
    echo "=========================================="
    echo "Project : $PROJECT"
    echo "Build   : $BUILD_VERSION"
    echo "Commit  : $COMMIT"
    echo "Status  : $STATUS"
    echo "Job     : $JOB_ID"
    echo "=========================================="

    if [[ -n "$EXPECTED_COMMIT" && "$COMMIT" != "$EXPECTED_COMMIT"* ]]; then
        echo "Waiting for build corresponding to expected commit $EXPECTED_COMMIT..."
        sleep 15
        continue
    fi

    case "$STATUS" in
        success)
            echo "Build successful."
            exit 0
            ;;
        failed|error)
            echo "Build failed! Fetching log..."
            LOG_FILE="/tmp/appveyor-build.log"
            curl -sS -f "${AUTH_ARGS[@]}" "https://ci.appveyor.com/api/buildjobs/${JOB_ID}/log" > "$LOG_FILE"
            
            echo "Log saved to $LOG_FILE"
            echo "Searching for meaningful errors..."
            grep -i -C 3 -E 'error|failed|fatal|CMake Error|LINK : fatal|ninja: build stopped|Command exited with code|is not recognized' "$LOG_FILE" || echo "No explicit error keywords found. Check the full log."
            exit 1
            ;;
        queued|running|starting)
            echo "Build is $STATUS. Waiting 15 seconds..."
            sleep 15
            ;;
        *)
            echo "Unknown status: $STATUS. Waiting 15 seconds..."
            sleep 15
            ;;
    esac
done
