#!/bin/bash
ffmpeg -y -i /dev/video0 -c:v libvpx -r 30 -g 90 -s 1280x720 -quality realtime -speed 5 -threads 2 -crf 10 -b:v 1M -map 0 -segment_time 00:00:25 -f segment -reset_timestamps 1 -c:a libvorbis ./output%01d.webm

