# life_sim

## Build and run

```sh
git clone --depth 1 https://github.com/libsdl-org/SDL.git vendored/SDL
rm -rf vendored/SDL/.git

make configure
make
./build/debug/zls
```

## Video capture

```sh
./build/debug/zls | ffmpeg \
    -f rawvideo \
    -pixel_format rgba \
    -video_size 1080x1920 \
    -framerate 60 \
    -i pipe:0 \
    -an \
    -c:v libx264 \
    -preset medium \
    -crf 18 \
    -pix_fmt yuv420p \
    -movflags +faststart \
    output.mp4
```
