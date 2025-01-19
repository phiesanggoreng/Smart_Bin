import cv2
import torch
import pathlib
import serial
import time
import numpy as np

# fixing path
temp = pathlib.PosixPath 
pathlib.PosixPath = pathlib.WindowsPath
from pathlib import Path

ser = serial.Serial('COM6',baudrate = 115200)
path=R'D:\smartBIN\best.pt'
model = torch.hub.load(R'D:\yolo\yolov5','custom',path, source='local',force_reload=True)
while 1:
    arduino = ser.readline().decode('utf-8').strip()
    if arduino == '1':
        print(arduino)
        # camera initialization
        # 0 = laptop camera
        # 1 = external webcam
        #http://192.168.112.18:81/stream = livestreaming url 
        cap = cv2.VideoCapture("http://192.168.207.18:81/stream")

        if not cap.isOpened():
            print("failed to open the camera")
            exit()

        ret, frame = cap.read()

        if not ret:
            print("failed to read the frame from the camera")
            exit()
        imgs=frame
        cap.release()  
        results = model(imgs)
        detected = False
        for pred in results.xyxy[0]:
            # Get class label 
                conf, cls = pred[-2:]
                label = f"{model.names[int(cls)]}"
                detected = True
                if label == "carboard":
                    ser.write(b'2')
                    print("sampah dengan jenis carboard")
                    break
                elif label == "glass" :
                    ser.write(b'3')
                    print("sampah dengan jenis glass")
                    break
                elif label == "metal" :
                    ser.write(b'4')
                    print("sampah dengan jenis metal")
                    break
                elif label == "paper" :
                    ser.write(b'5')
                    print("sampah dengan jenis paper")
                    break
                elif label == "plastik" :
                    ser.write(b'6')
                    print("sampah dengan jenis plastik")
                    break
                break
        if not detected:
            ser.write(b'7')
            print("jenis sampah plastik tidak terdeteksi")
        results.show()   



