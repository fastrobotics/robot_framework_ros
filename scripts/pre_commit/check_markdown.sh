#!/bin/bash
echo "Checking Markdown Links..."
find . -name "*.md" | xargs lychee --exclude-path "templates/*"
if [ $? -ne 0 ]; then
    echo "❌ Error: Markdown Link Checking failed. Aborting commit."
    exit 1
fi
echo "Ok!"