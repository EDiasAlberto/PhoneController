from time import sleep
import serial

class PhoneData:
    def __init__(self, type, attrs):
        self.type = type #JOYSTICK -- BUTTON
        self.attrs = attrs

class SerialMonitor:
    def __init__(self, baudrate=9600):
        self.serial = serial.Serial()
        self.serial.baudrate = baudrate
        self.serial.port = "COM3"
        self.serial.open()

    def getData(self):
        newline = str(self.serial.readline()).strip("b'").strip("'").strip("\\n")
        if not ("BUTTON" in newline) and not ("JOYSTICK" in newline):
            print("INVALID INSTR TO BE PARSED")
            return None
        listStuff = newline.split(": ")
        type = listStuff[0]
        data = listStuff[1]
        data = PhoneData(type, data)
        return data


    def testReadLoop(self):
        while True:
            try:
                self.getData()
                sleep(0.01)
            except KeyboardInterrupt:
                exit()