!/bin/bash
source="/home/mati77765/Desktop/ProgramowanieSkryptowe"
destination="/usr/local/bin/backup"
log="/var/log/backup.log"
date=$(date '+%Y-%m-%d')
backup="backup_$date.ter.gz"

{
        echo "$(date '+%Y-%m-%d %H:%M:%S') - Rozpoczęcie tworzenia kopii zapasowej"
        if [ ! -d "$destination" ]; then
                echo "Tworzenie katalogu docelowego: $destination"
                mkdir -p "$destination" || { echo "Błąd: Nie można utworzyć katalogu docelowego"; exit 1; }
        fi

        echo "Tworzenie kopii zapasowej w: $destination/$backup"
        tar -czf "$destination/$backup" $source 2>&1

        if [ $? -eq 0 ]; then
                echo "Kopia zapasowa została pomyślnie utworzona."
        else
                echo "Błąd podczas tworzenia kopii zapasowej."
        fi

        echo "Zakończenie operacji: $(date '+%Y-%m-%d %H:%M:%S')"
} >> "$log" 2>&1

