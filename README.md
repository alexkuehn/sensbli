# SENSBLI
eye blink detection software

## Description
the software detects the eyes of a person who is visible in a portrait view in a videostream. After the eyes are detected, the eyes are tracked and the state of the lid (open, closed) is evaluated and sent over a TCP socket.

## Requirements
* a Webcam with 640x480 pixel resolution
* Qt 4.5
* OpenCV 2.0

## Documentation
for the algorithmic and software architecture, refer to the [master thesis of Alexander Kühn](http://alexkuehn.github.io/blob/thesis_akuehn.pdf) (german only).

There is also a reference implementation [PLATAN](http://github.com/alexkuehn/platan), which reacts on eyelid events.
