#!/usr/bin/env bash
set -x

check_root() {
    if [ "$(id -u)" -ne 0 ]; then
        echo "ERROR: Hak akses root dibutuhkan untuk menginstal paket."
        echo "Silakan jalankan ulang skrip ini sebagai root."
        echo "Contoh: su -c \"./compile.sh\""
        exit 1
    fi
}

check_sampctl() {
    if ! dpkg -s sampctl > /dev/null 2>&1; then
        check_root ""
        apt-get update && apt-get install -y wget curl
        curl https://raw.githubusercontent.com/Southclaws/sampctl/master/install-deb.sh | sed "s/sudo //g" | sh
    fi
}

check_lib_compiler() {
    if ! dpkg -s libc6:i386 lib32stdc++6 > /dev/null 2>&1; then
        check_root ""
        dpkg --add-architecture i386
        apt-get update && apt-get install -y libc6:i386 lib32stdc++6
    fi
}

compile_gamemode() {
    check_sampctl ""
    sampctl ensure && sampctl build
}

compile_voice() {
    check_lib_compiler ""
    if [ ! -d "filterscripts" ] && mkdir -p "filterscripts"
    local LIBRARY_PATH=".vscode/pawnc/lib" COMPILER_PATH=".vscode/pawnc/bin/pawncc" GAMEMODE_PATH="src/Voice.pwn" OUTPUT_PATH="filterscripts/Voice.amx" INCLUDE_PATHS="-i=src"
    if [ -d "dependencies" ]; then
        for DIR in dependencies/*; do
            if [ -d "${DIR}" ]; then
                if [[ "$(basename "${DIR}")" != ".resources" ]]; then
                    INCLUDE_PATHS="${INCLUDE_PATHS} -i=${DIR}"
                fi
            fi
        done
        
        if [ -d "dependencies/.resources" ]; then
            for DIR2 in dependencies/.resources/*; do
                if [ -d "${DIR2}" ]; then
                    INCLUDE_PATHS="${INCLUDE_PATHS} -i=${DIR2}"
                fi
            done
        fi
    fi

    LD_LIBRARY_PATH=${LIBRARY_PATH} ${COMPILER_PATH} ${GAMEMODE_PATH} ${INCLUDE_PATHS} -o=${OUTPUT_PATH} "-;+" "-(+" "-\+" "-d3" "-Z+"
}

case $1 in
    "")
        compile_gamemode
        compile_voice
        ;;
    "gm")
        compile_gamemode
        ;;
    "voice")
        compile_voice
        ;;
    *)
        echo -e "Error: Opsi tidak dikenal '$1'"
        exit 1
        ;;
esac
