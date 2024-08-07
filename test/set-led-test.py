import serial
import time

# Configure serial connection
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)

# Write to the serial port. Command ends with \r
def write_to_serial(data):
    ser.write(data.encode('utf-8') + b'\r')

# Read from the serial port
def read_from_serial():
    response = ser.readline()
    if response:
        return response.decode('utf-8').strip()
    return None

# Print execution time
def print_exec_time(start_time):
    end_time = time.time() - start_time
    end_time *= 1000
    print(f"Execution time: {end_time:.1f} ms")

# Single test case
def run_test_case(command, expected_initial_response, expected_later_response=None, wait_time=5):
    write_to_serial(command)
    start_time = time.time()  # Start timing
    timeout = 2  # seconds for initial response

    # Check for the initial response
    while True:
        response = read_from_serial()
        if response:
            if response == expected_initial_response:
                print(f"Initial Response Passed: {command}")
            else:
                print(f"Initial Response Failed: {command}. Expected: '{expected_initial_response}', Got: '{response}'")
                print_exec_time(start_time)  # Print execution time
                return False
            break
        if time.time() - start_time > timeout:
            print(f"Initial Response Timeout: {command}. No response received.")
            print_exec_time(start_time)  # Print execution time
            return False

    # If there is an expected later response (like led-off:<id>)
    if expected_later_response:
        start_time = time.time()  # Restart timing for later response
        while True:
            response = read_from_serial()
            if response:
                if response == expected_later_response:
                    print(f"Later Response Passed: {command}")
                    print_exec_time(start_time)  # Print execution time
                    return True
                else:
                    print(f"Later Response Failed: {command}. Expected: '{expected_later_response}', Got: '{response}'")
                    print_exec_time(start_time)  # Print execution time
                    return False
            if time.time() - start_time > wait_time:
                print(f"Later Response Timeout: {command}. No later response received.")
                print_exec_time(start_time)  # Print execution time
                return False
            
    print_exec_time(start_time)  # Print execution time
    return True

# Test cases
test_cases = [
    # Valid cases
    ("set-led 1,200", "OK", "led-off: 1"),
    ("set-led 2,3000", "OK", "led-off: 2"),
    ("set-led 3,4999", "OK", "led-off: 3"),
    ("set-led 4,1", "OK", "led-off: 4"),
    # Invalid cases
    ("set-led 5,200", "ERROR"),  # Invalid LED id
    ("set-led 0,200", "ERROR"),  # Invalid LED id
    ("set-led x,200", "ERROR"),  # Invalid LED id format
    ("set-led 1,5000", "ERROR"),  # Invalid time
    ("set-led 2,-100", "ERROR"),  # Invalid time
    ("set-led 3,abcd", "ERROR"),  # Invalid time format
    ("set-led x,200", "ERROR"),  # Invalid LED command
    ("set-led x.200", "ERROR"),  # Invalid LED command
    ("set-ledx.200", "ERROR"),  # Invalid LED command
]

# Track overall test status
all_tests_passed = True

# Run all test cases
for command, expected_initial_response, *expected_later_response in test_cases:
    if expected_later_response:
        if not run_test_case(command, expected_initial_response, expected_later_response[0], wait_time=6):  # Adjust wait_time as needed
            all_tests_passed = False
    else:
        if not run_test_case(command, expected_initial_response):
            all_tests_passed = False

# Close the serial connection
ser.close()

# Print overall test result
if all_tests_passed:
    print("Tests passed")
else:
    print("Tests failed")
