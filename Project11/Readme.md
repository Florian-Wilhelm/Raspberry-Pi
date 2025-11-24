# Thermal imaging

## Description

Creating an IoT device providing thermal data and images (by using MLX90642 32x24 InfraRed (IR) array), and realizing a very basic IR camera thru an attached display. 
One obvious UseCase is to make this device a burglar detection / home intrusion system for e.g. a bicycle store room. Another UseCase could be monitoring terrariums and the like.

## Branches of development 

SW V0.2x -> non IoT / OLED display / acoustic burglar detection

SW V0.4x -> IoT (not all files will be uploaded here, e.g. the WiFi specific setup.h); OLED display functions will be removed by and by, for rendering the data will take place on a computer.

## Thermal image 

Moving object in 5m distance in an outdoors area, SW V0.492 (not yet uploaded). All 768 pixels are read and visualized.

![Moving-Object-marked](https://github.com/user-attachments/assets/5e45dc47-a529-4d92-a069-48e9517c1bd4)

