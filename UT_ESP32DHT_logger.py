import serial
import datetime


print("UT_ESP32DHT_logger (v1 by Harmen Hoek, 2021-05-11) started. \n")
print("Specify the COM port of the ESPDHT_logger.")
print("If none is available, make sure to install the driver: CP210x from Silabs.")
print("COM port (e.g. 'COM3'):")
device = input()
print("Selected port: " + device)


# device = '/dev/tty.usbserial-02EO5VOQ'  # serial port
# device = 'COM3'
baud = 115200  # baud rate
# filename = 'bald-log.txt'  # log file to save data in

timeStamp = datetime.datetime.now()
timeStampStr = timeStamp.strftime("%Y%m%d_%H%M%S")
# filename = os.environ['USERPROFILE'] + "\Desktop\\" + timeStampStr + "_UTDHTESP.log"
filename =  timeStampStr + "_UTDHTESP.log"
print(f"Data will be logged to {filename}.")

with serial.Serial(device, baud) as serialPort, open(filename, 'wb') as outFile:
    while (1):
        line = serialPort.readline()  # must send \n! get a line of log
        print(line)  # show line on screen
        outFile.write(line)  # write line of text to file
        outFile.flush()  # make sure it actually gets written out