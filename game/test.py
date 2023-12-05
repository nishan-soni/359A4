import serial

# UART Constants

PORT = "/dev/ttyUSB1"
BAUD = 115200

def get_move():
    """
    Gets the move that should be executed via UART
    """

    s = serial.Serial(port = PORT, baudrate=BAUD)
    while True:
        b = s.read()
        print(b)



if __name__ == '__main__':
    # main()
    get_move()