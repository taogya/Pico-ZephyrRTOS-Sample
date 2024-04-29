#!/bin/bash

source_file="build/zephyr/zephyr.uf2"
destination="/Volumes/RPI-RP2"

print_help(){
    echo "Usage: flash.sh [-s source_file] [-d destination] [-h]"
    echo "Options:"
    echo "  -s source_file    Specify the source file to be copied (default: build/zephyr/zephyr.uf2)"
    echo "  -d destination    Specify the destination directory (default: /Volumes/RPI-RP2)"
    echo "  -h                Display this help message"
}

while getopts "s:d:h" opt; do
    case $opt in
        s)
            source_file=$OPTARG
            ;;
        d)
            destination=$OPTARG
            ;;
        h)
            print_help
            exit 0
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            print_help
            exit 1
            ;;
    esac
done

# ファイルのコピー処理
cp "$source_file" "$destination" && echo "Success"
