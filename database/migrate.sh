#!/bin/sh
# Simple migration runner
# Usage: ./database/migrate.sh DBNAME USERNAME
#
# Applies all SQL files in database/migrations and records each applied
# filename in the `schema_migrations` table. Existing entries are skipped
# so running the script multiple times is safe.

DB="$1"
USER="$2"
DIR="$(dirname "$0")"

if [ -z "$DB" ] || [ -z "$USER" ]; then
    echo "Usage: $0 DBNAME USERNAME"
    exit 1
fi

mysql -u "$USER" "$DB" -e "CREATE TABLE IF NOT EXISTS schema_migrations (filename VARCHAR(255) PRIMARY KEY)" || exit 1

for file in "$DIR"/migrations/*.sql; do
    [ -e "$file" ] || continue
    base="$(basename "$file")"
    already=$(mysql -N -s -u "$USER" "$DB" -e "SELECT 1 FROM schema_migrations WHERE filename='$base' LIMIT 1;")
    if [ "$already" = "1" ]; then
        echo "Skipping $base (already applied)"
        continue
    fi

    echo "Applying $file"
    mysql -u "$USER" "$DB" < "$file"
    if [ $? -ne 0 ]; then
        echo "Error applying $file"
        exit 1
    fi
    mysql -u "$USER" "$DB" -e "INSERT INTO schema_migrations (filename) VALUES ('$base')" || exit 1
done
