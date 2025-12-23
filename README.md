# Smart Street Light System using ESP32

## Description
This project implements an ESP32-based smart street lighting system that adapts lighting based on:
- Ambient light (LDR)
- Rain conditions
- Emergency siren detection (ambulance)
- Vehicle detection and speed monitoring

## Features
- Automatic day/night detection
- Rain-based brightness control
- Emergency siren detection
- Vehicle speed calculation
- Servo-based directional control
- Buzzer alert for overspeed

## Hardware Used
- ESP32
- LDR
- Rain Sensor
- LM393 Sound Sensor
- Ultrasonic Sensor (HC-SR04)
- Servo Motor
- LED
- Buzzer

## Hardware Connections (ESP32)

| Component            | ESP32 Pin | Description |
|---------------------|-----------|-------------|
| LDR Sensor          | GPIO 34   | Ambient light detection (Analog) |
| Rain Sensor (AO)    | GPIO 36   | Rain detection (Analog) |
| Sound Sensor (LM393)| GPIO 39   | Ambulance siren detection |
| Ultrasonic TRIG     | GPIO 18   | Trigger pin |
| Ultrasonic ECHO     | GPIO 19   | Echo pin |
| Servo Motor         | GPIO 14   | Directional rotation |
| Street Light LED    | GPIO 25   | PWM brightness control |
| Buzzer              | GPIO 26   | Overspeed / emergency alert |
| VCC (All Sensors)   | 5V / 3.3V | Power supply |
| GND (All Sensors)   | GND       | Common ground |

## Wiring Notes

- All GND pins must be connected to a **common ground**
- Servo motor should be powered using **external 5V supply**
- ESP32 GND and external power GND must be connected
- Sound sensor sensitivity should be adjusted using onboard potentiometer
- Rain sensor gives **lower analog value when wet**
- Ultrasonic sensor works best between **5 cm – 100 cm**
## System Overview

The ESP32 continuously monitors environmental conditions such as ambient light, rain, and sound levels.  
It scans the road using an ultrasonic sensor mounted on a servo motor.  
If a vehicle is detected, speed is calculated using real-time distance change.  
In case of overspeeding or emergency siren detection, alerts are triggered and street light intensity is increased.


## Applications
- Smart cities
- Highway lighting
- Emergency vehicle priority systems

## Author
Rithika.s
