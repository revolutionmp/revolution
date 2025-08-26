#!/bin/bash
set -e
echo "--- Running Entrypoint Setup ---"
echo "[INFO] Generating mysql.ini from .env variables..."
cat << EOF > mysql.ini
hostname = database
username = ${MYSQL_USER:-revolution}
password = ${MYSQL_PASSWORD:-87654321}
database = rmp
server_port = ${MYSQL_PORT:-3306}
pool_size = 5
EOF
echo "[SUCCESS] mysql.ini generated."
echo "--- Entrypoint Setup Finished. Starting main application... ---"
exec "$@"