#!/bin/bash
VERSION=$1
if [ -z "$VERSION" ]; then
    echo "Usage: ./release.sh <version>"
    echo "Example: ./release.sh 0.1.0"
    exit 1
fi

# Ensure we are up to date
git pull origin master

# Tag and push
git tag v$VERSION
git push origin v$VERSION

echo "Pushed tag v$VERSION. GitHub Action should start building now."
