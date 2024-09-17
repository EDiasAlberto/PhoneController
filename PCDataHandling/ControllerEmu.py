from pyvjoy import VJoyDevice, HID_USAGE_X, HID_USAGE_Y
from time import sleep

class Controller:

    def __init__(self):
        self.device = VJoyDevice(1)
        self.buttonMapping = {"A": 1, "B": 2}
        self.reboundDelay = 0.1

    def pressButton(self, button):
        buttonInt = self.buttonMapping[button]
        self.device.set_button(buttonInt, 1)
        sleep(self.reboundDelay)
        self.device.set_button(buttonInt, 0)
    
    def decimalToHex(self, decimal_str):
        # Convert the input string to a float
        decimal_value = float(decimal_str)
        
        # Ensure the decimal value is in the range [-1, 1]
        if decimal_value < -1 or decimal_value > 1:
            decimal_value = 0
        
        # Map the decimal value [-1, 1] to [0, 32768] (0x0000 to 0x8000)
        # Scaling: -1 -> 0x0000 (0), 0 -> 0x4000 (16384), 1 -> 0x8000 (32768)
        scaled_value = int((decimal_value + 1) * (32768 / 2))
        
        # Convert to hexadecimal and return the result
        return scaled_value

    def moveJoyStick(self, x, y):
        self.device.update()
        hexX = self.decimalToHex(x)
        hexY = self.decimalToHex(-y)
        if(x == 0 and y==0):
            self.device.set_axis(HID_USAGE_X, hexX)
            self.device.set_axis(HID_USAGE_Y, hexY)
            return
        self.device.data.wAxisX = hexX
        self.device.data.wAxisY = hexY
        self.device.update()
        print(hexX, hexY)