import serial
import time

# Initialize serial connection
arduino = serial.Serial(port='COM7', baudrate=115200, timeout=0.1)

def write_read(x):
    """
    Send a string to Arduino and read the response
    
    Args:
        x (str): String to send to Arduino
    
    Returns:
        str: Response from Arduino, decoded from bytes
    """
    arduino.write(bytes(x + '\n', 'utf-8'))  # Add newline as delimiter
    time.sleep(0.05)
    data = arduino.readline().decode('utf-8').strip()  # Decode and remove whitespace
    return data

# Main communication loop
while True:
    message = input("Enter a string: ")
    value = write_read(message)
    print("Received:", value)