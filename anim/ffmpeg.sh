ffmpeg -r 24 -f image2 -s 640x480 -i image%d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p test.mp4
