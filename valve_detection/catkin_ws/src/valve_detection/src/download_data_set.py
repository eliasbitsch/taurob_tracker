#!/usr/bin/env python3
from roboflow import Roboflow
rf = Roboflow(api_key="4l1tSyZhso5GJY2usZTp")
project = rf.workspace("ws-m0cj0").project("valve_detection-31myq")
version = project.version(2)
dataset = version.download("yolov8")
                