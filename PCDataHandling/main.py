import ControllerEmu
import SerialMonitor
from time import sleep

serial = SerialMonitor.SerialMonitor()
controller = ControllerEmu.Controller()

def mainLoop():
    while True:
        data = serial.getData()
        if data:
            if data.type == "JOYSTICK":
                coordData = data.attrs.split(", ")
                xData = float(coordData[0])
                yData = float(coordData[1])
                controller.moveJoyStick(xData, yData)
            elif data.type == "BUTTON":
                controller.pressButton(data.attrs)
            else:
                print("INVALID INSTR")

if __name__ == "__main__":
    mainLoop()