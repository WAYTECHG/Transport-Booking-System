# 🚗 Universal Transports Company System

### C++ Object-Oriented Programming Project

---

## 🧭 Overview

The **Universal Transports Company System** is a **menu-driven C++ application** designed to manage transport fleets, users, and booking operations.
It demonstrates **Object-Oriented Programming (OOP)** principles such as inheritance, polymorphism, abstraction, encapsulation, and file I/O handling.

This project simulates a real-world transport service system where:

- **Admins** manage the fleet database (add, view, delete transports)
- **Clients** can register, log in, and book available transports

---

## 🧩 Features

### 👨‍💼 Admin Account

- Add new transport entries (via `TransportFactory`)
- View all registered transports
- Delete existing transport records

### 👥 Client Account

- Register new account with contact and ID
- View available transports
- Book or cancel transport rentals
- Display current bookings

### 💾 Data Persistence

- Save and load all **accounts** and **fleet** data through text-based databases:
  - `accounts.txt`
  - `fleet.txt`

---

## 🧱 Project Structure

📁 UniversalTransportSystem/

│

├── Account.h

├── AdminAccount.h

├── ClientAccount.h

├── Client.h

├── Transport.h

├── TransportFactory.h

├── main.cpp

│

├── accounts.txt     # Stores admin and client login info

├── fleet.txt        # Stores transport details

│

└── README.md

---



## 🧑‍💻 Developer Notes

* The **hardcoded fleet initialization** in `main.cpp` (commented section) is intended for first-time setup only.
* Once initial fleet entries are created and saved in `fleet.txt`, comment out or remove that section to prevent duplication.
* Exception handling is implemented for invalid account parsing and missing files.

---

## 🧠 Object-Oriented Concepts Used

* **Abstraction:** `Account` and `Transport` serve as abstract base classes.
* **Inheritance:** `AdminAccount` and `ClientAccount` inherit from `Account`.
* **Polymorphism:** Dynamic binding via `performActions()` overrides.
* **Encapsulation:** Private/protected member variables with getter/setter methods.
* **File Handling:** Persistent data stored in `.txt` databases using file streams.

---

## 🏆 Authors

**Wilbert Yonathan,  Siah Jie Sheng, Edward Ooi Wee Xue**

*CST209 – Project: Object-Oriented Programming (C++)*

**Universal Transports Management System**

---
