# ESP32-Based RFID Access Control System

## Overview

A hardware-based RFID access control system developed using an **ESP32** and **RC522 RFID reader**. The RC522 communicates with the ESP32 through **SPI**, and the system identifies RFID cards using their UID and provides access status through an LED indicator.

## Features

* RFID card detection using RC522
* SPI communication between ESP32 and RC522
* RFID UID reading
* UID-based authorization
* Access granted/denied indication using LED
* Serial Monitor debugging
* Physically assembled and tested hardware

## Components

* ESP32
* RC522 RFID Reader
* RFID Card/Tag
* LED
* 220Ω Resistor
* Breadboard
* Jumper Wires
* USB Cable

## Communication Protocol

**SPI (Serial Peripheral Interface)**

The RC522 is interfaced with the ESP32 using SPI communication.

Main SPI signals:

* **SCK** — Serial Clock
* **MOSI** — Master Out Slave In
* **MISO** — Master In Slave Out
* **SS/CS** — Slave Select / Chip Select

## Working Principle

1. The ESP32 initializes the SPI interface and RC522 RFID reader.
2. The RC522 waits for an RFID card or tag.
3. When a card is detected, the RC522 reads its UID.
4. The UID is transferred to the ESP32 through SPI.
5. The ESP32 compares the detected UID with the authorized UID stored in the firmware.
6. If the UID matches, access is granted and the LED provides the corresponding indication.
7. If the UID does not match, access is denied.
8. The system then waits for the next RFID card.

## Hardware Connections

| RC522 Pin | ESP32   |
| --------- | ------- |
| 3.3V      | 3.3V    |
| GND       | GND     |
| SDA/SS    | GPIO 5  |
| SCK       | GPIO 18 |
| MOSI      | GPIO 23 |
| MISO      | GPIO 19 |
| RST       | GPIO 22 |

> Verify these GPIO numbers against your actual hardware wiring and Arduino code before finalizing the repository.

## Software

* Arduino IDE
* ESP32 Board Package
* MFRC522 RFID Library

## Testing

The system was physically assembled and tested using an ESP32, RC522 RFID reader, RFID card and LED indicator.

RFID card detection and UID-based access decisions were verified using the Serial Monitor and LED status indication.

## Project Structure

```text
ESP32-RFID-Access-Control-System/
│
├── ESP32-RFID-Access-Control-System.ino
├── rfid-hardware.jpg
└── README.md
```

## Limitations

This project implements basic UID-based authorization for demonstration and learning p


