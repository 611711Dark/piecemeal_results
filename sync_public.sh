#!/bin/bash
git checkout public
git checkout main -- public/
git commit -m "自动同步公有内容"
git push -f origin public
git checkout main
