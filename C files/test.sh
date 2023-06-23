#!/bin/bash

# This script performs a backup of a specified directory

# Set the source and destination directories
source_dir="/path/to/source"
destination_dir="/path/to/destination"

# Create a timestamp for the backup filename
timestamp=$(date +%Y-%m-%d-%H-%M-%S)
backup_filename="backup_${timestamp}.tar.gz"

# Archive the source directory
tar -czf "${backup_filename}" -C "${source_dir}" .

# Move the backup file to the destination directory
mv "${backup_filename}" "${destination_dir}"

# Clean up older backups (retain last 7 days)
find "${destination_dir}" -type f -name "backup_*" -mtime +7 -exec rm {} \;

# Print the backup completion message
echo "Backup completed successfully."
