---

# Wall-Climber Glass Cleaner Robot (Arduino) 🤖🧼

Welcome to the **Wall-Climber Glass Cleaner Robot** project! This Arduino-powered marvel is designed to autonomously clean vertical glass surfaces while safely navigating along edges. Combining motor control, ultrasonic sensing, and cleaning mechanisms, this project offers a robust platform for experimentation and further enhancements. 🚀

> **Inspiration:** This project draws its inspiration from the "Wall Climber Glass Cleaner Robot" featured on [nevonprojects.com](https://nevonprojects.com/wall-climber-glass-cleaner-robot/). Their innovative concept inspired the design and functionality of this implementation.

---

## Project Overview

The goal of this project is to build a robot that can:
- **Climb Vertical Surfaces:** Using suction techniques, the robot adheres to glass surfaces.
- **Clean Efficiently:** Equipped with a suction motor and a brush motor, it actively cleans dirt and grime from glass.
- **Navigate Safely:** With four ultrasonic sensors strategically placed (front, back, left, and right), the robot detects obstacles as well as potential edges. This ensures it avoids collisions and maintains a safe distance from the glass edges—preventing falls and damage.

The robot continuously monitors its surroundings and makes quick decisions:
- **Edge Detection:** If the sensor reads a distance beyond a safe threshold (indicating a potential drop), the robot stops, reverses, and then turns to adjust its path.
- **Obstacle Avoidance:** When an obstacle is too close, the robot backs up and then maneuvers to find an alternate route.
- **Continuous Cleaning:** As it navigates, the suction and brush mechanisms remain active, ensuring that cleaning never stops.

---

## Features

- **Multi-Directional Movement:**  
  The robot employs two motors on each side (left and right) for independent control. This allows for forward movement, reversing, and simple left/right turns.
  
- **Advanced Sensor Integration:**  
  Four ultrasonic sensors provide real-time distance measurements. Two critical thresholds are employed:
  - **`minDistance`**: Detects when obstacles are too near.
  - **`maxSafeDistance`**: Flags when an edge is near, preventing the robot from unintentionally exiting the glass surface.
  
- **Dedicated Cleaning Mechanism:**  
  A suction motor coupled with a brush motor actively cleans as the robot moves, ensuring efficient removal of contaminants from vertical glass surfaces.

- **Modular and Extensible Code:**  
  The code is structured with modular functions for movement and sensing, making it easy to extend or modify according to your project needs.

---

## Hardware Components

Below is a summary of the hardware components and their respective Arduino pin assignments:

### Motors
| Component               | Arduino Pin | Description                       |
| ----------------------- | ----------- | --------------------------------- |
| **Left Motor Forward**  | 3           | Drives left side forward          |
| **Left Motor Backward** | 4           | Drives left side backward         |
| **Right Motor Forward** | 5           | Drives right side forward         |
| **Right Motor Backward**| 6           | Drives right side backward        |

### Cleaning Mechanisms
| Component       | Arduino Pin | Description                        |
| --------------- | ----------- | ---------------------------------- |
| **Suction Motor** | 7         | Activates suction for wall adhesion and cleaning |
| **Brush Motor**   | 8         | Powers the cleaning brush          |

### Ultrasonic Sensors
| Sensor          | Trig Pin | Echo Pin | Location      |
| --------------- | -------- | -------- | ------------- |
| **Front Sensor**| 9        | 10       | Front         |
| **Back Sensor** | 11       | 12       | Rear          |
| **Left Sensor** | 13       | A0       | Left Side     |
| **Right Sensor**| A1       | A2       | Right Side    |

The sensors are integrated using a simple structure to keep code clean and readable. The measured distance is computed using the standard formula based on sound speed (approximately 0.034 cm per µs) and divided by 2 to account for the round trip.

---

## Software Architecture

### Initialization
- **Pin Setup:**  
  The `setup()` function initializes all motor and sensor pins. The suction and brush motors are activated at startup, ensuring that cleaning begins immediately.
  
- **Serial Communication:**  
  Serial debugging (`Serial.begin(9600)`) is included for live monitoring of sensor readings.

### Core Loop
Within the `loop()` function:
1. **Sensor Readings:**  
   All four ultrasonic sensors are read, calculating distances in centimeters.
2. **Decision Logic:**  
   - **If the front sensor detects an edge (distance ≥ `maxSafeDistance`):** The robot stops, reverses, and then turns (choosing the side with a safer distance).
   - **If an obstacle is too close (distance ≤ `minDistance`):** The robot performs a reverse and a left turn.
   - **If left or right sensors detect an edge:** The robot adjusts by turning in the opposite direction.
   - **Otherwise:** The robot moves forward continuously.
3. **Movement Functions:**  
   Modular functions (`moveForward()`, `reverse()`, `stopMovement()`, `turnRightSimple()`, `turnLeftSimple()`) simplify motor commands and enhance code reusability and clarity.

### Sensor Functions
- **`setupUltrasonic()`**: Configures the trigger and echo pins.
- **`getDistance()`**: Sends a pulse from the ultrasonic sensor and calculates the distance based on the pulse duration. A timeout mechanism returns a high value (1000 cm) if no echo is detected, ensuring robust behavior even when the sensor fails to sense an obstacle.

---

## Getting Started

1. **Hardware Setup:**  
   Assemble the robot by connecting the motors, suction system, brush mechanism, and ultrasonic sensors to the specified Arduino pins.
   
2. **Code Upload:**  
   Open the project in the Arduino IDE, connect your Arduino board, and upload the code.

3. **Run and Observe:**  
   Place the robot on a glass surface. Open the Serial Monitor to observe the sensor readings and debugging information as the robot navigates and cleans.

4. **Tinker and Enhance:**  
   Modify thresholds, add additional sensors, or integrate new behaviors. The modular design welcomes experimentation. 🔧

---

## Future Enhancements

- **PID-Based Control:**  
  Fine-tune the movement algorithms with PID controllers for smoother and more accurate navigation.
  
- **Enhanced Cleaning Modes:**  
  Develop additional cleaning routines or integrate water spraying for stubborn stains.
  
- **User Interface:**  
  Incorporate remote control or a mobile app to allow manual intervention or status monitoring during operation.

---

## Conclusion

The **Wall-Climber Glass Cleaner Robot** is a unique fusion of robotics, sensor technology, and automation. Its design promises a blend of functionality and safety, making it an ideal starting point for researchers, hobbyists, and DIY enthusiasts aiming to explore robotics on vertical surfaces. Enjoy building, experimenting, and innovating with this project! 😃

---

Happy Coding and Robotic Adventures! 🚀
