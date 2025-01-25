# **Smart Boat Navigation System**

## Overview

This project presents a smart boat navigation system designed to operate autonomously in small or medium water bodies. The system avoids collisions with obstacles using ultrasonic sensors, provides real-time information through an LCD, and includes both automatic and manual modes for flexibility.

## Features

- **Obstacle Detection and Avoidance:** Uses ultrasonic sensors to measure distances in multiple directions.
- **Automatic and Manual Modes:** Autonomous navigation and manual control via remote commands.
- **Real-Time Feedback:** Displays distance to obstacles and speed percentage on an LCD.
- **Safety Indicators:** LED lights indicate route clearance (green) or danger (red).
- **Emergency Stop Mechanism:** Stops the boat when no safe path is detected.

## Components

- Arduino UNO R3.
- 3 Ultrasonic Distance Sensors.
- LCD 16x2 I2C.
- Servo Motor (MG90S).
- DC Motor with Propeller.
- Green and Red LEDs.
- 1k Resistors.
- 100k Potentiometer.
- 400-point Breadboard.
- Male-to-Male, Male-to-Female and Female-to-Female Jump Wires.

## Architecture

The code uses Arduino IDE and the following libraries:
- Wire.h for I2C communication.
- LiquidCrystal_I2C.h for LCD control.
- Servo.h for servo motor operation.

## How to Use

1. **Setup the Hardware:** Connect the components as per the defined pins in the code.
2. **Upload the Code:** Use Arduino IDE to upload the provided sketch to the Arduino board.
3. **Operating Modes:**
   - **Automatic:** Press 'A' to activate autonomous navigation.
   - **Manual:** Press 'M' and use directional commands ('F', 'B', 'L', 'R') for manual control.
   - **Test:** Press 'T' to test sensors, motor, and LEDs.

## License

This project is licensed under the [MIT License](LICENSE) - see the [LICENSE](LICENSE) file for details.
