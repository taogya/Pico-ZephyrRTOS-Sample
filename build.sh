#!/bin/bash

zephyr="$HOME/develop/zephyrproject"
vendor="raspberrypi"
board="rpi_pico_custom"
source="$(pwd)/src"
destination="$(pwd)/build"

print_help(){
    echo "Usage: build.sh [-z zephyr] [-v vendor] [-b board] [-s source] [-d destination] [-h]"
    echo "Options:"
    echo "  -z zephyr         Specify the path to the zephyrproject directory (default: $HOME/develop/zephyrproject)"
    echo "  -v vendor         Specify the vendor name (default: raspberrypi)"
    echo "  -b board          Specify the board name (default: rpi_pico_custom)"
    echo "  -s source         Specify the source directory (default: $(pwd)/src)"
    echo "  -d destination    Specify the destination directory (default: $(pwd)/build)"
    echo "  -h                Display this help message"
}

while getopts "z:v:b:s:d:h" opt; do
    case $opt in
        z)
            zephyr=$OPTARG
            ;;
        v)
            vendor=$OPTARG
            ;;
        b)
            board=$OPTARG
            ;;
        s)
            source=$OPTARG
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

# ビルド処理
rm -rf "$zephyr/zephyr/boards/$vendor/$board"
cp -r "boards/$vendor/$board" "$zephyr/zephyr/boards/$vendor"

. "$zephyr/.venv/bin/activate"
pushd "$zephyr/zephyr"
west build -b "$board" -s "$source" -d "$destination"
popd