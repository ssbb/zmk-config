#!/usr/bin/env sh
keymap -c config.yaml parse -c 10 -z ../config/3x5_2.keymap > 3x5_2.yaml
keymap -c config.yaml draw --ortho-layout '{split: true, rows: 3, columns: 5, thumbs: 2}' 3x5_2.yaml > 3x5_2.svg
