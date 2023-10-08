#!/usr/bin/env sh
keymap -c config.yaml parse -c 10 -z ../config/3x5_2.keymap > keymap.yaml
keymap -c config.yaml draw -o '{split: true, rows: 3, columns: 5, thumbs: 2}' keymap.yaml > keymap.svg
