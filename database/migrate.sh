#!/bin/sh
# Simple migration runner
# Usage: ./database/migrate.sh DBNAME USERNAME

DB="$1"
USER="$2"
DIR="$(dirname "$0")"

if [ -z "$DB" ] || [ -z "$USER" ]; then
    echo "Usage: $0 DBNAME USERNAME"
    exit 1
fi

for file in "$DIR"/migrations/*.sql; do
    [ -e "$file" ] || continue
    echo "Applying $file"
    psql -d "$DB" -U "$USER" -f "$file"
    if [ $? -ne 0 ]; then
        echo "Error applying $file"
        exit 1
    fi
done
