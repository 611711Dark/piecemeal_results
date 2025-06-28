### Embedded Systems Lab Exercises - README

This repository contains solutions for a series of 27 embedded systems exercises targeting the **MSP430F5529 Launchpad** and **Booster Pack MKII**. The exercises cover fundamental to advanced concepts including GPIO, LCD control, timers, ADC, PWM, interrupts, and sensor integration.

---

#### 📁 **Lab Structure**
Exercises are divided into 3 parts:
1. **Part 1 (Exercises 1-9)**  
   - LCD drawing, button polling/interrupts, state machines, RGB LED control.
2. **Part 2 (Exercises 10-16)**  
   - Timer-based delays, PWM for LED intensity, interrupt synchronization.
3. **Part 3 (Exercises 17-27)**  
   - ADC for joystick/accelerometer, real-time LCD graphics, time-triggered systems.

---

#### ⚙️ **Hardware Setup**
- **Microcontroller**: MSP430F5529
- **Peripherals**: 
  - 128x128 TFT LCD
  - Joystick (analog channels)
  - Kionix KXTC9-2050 3-axis accelerometer
  - RGB LED (PWM-controlled)
  - Tactile buttons

---

#### 🛠️ **Key Libraries Used**
1. **Display Library** (`display.h`):  
   - Functions: `Init_display()`, `Filled_rectangle()`, `Filled_circle()`, `write_string()`.
2. **Digital I/O Module** (`digital.h`):  
   - Button/GPIO initialization and state checks.
3. **Timer Module** (`time.h`):  
   - Hardware timers for delays, PWM, periodic interrupts.
4. **Analog Module** (`analog.h`):  
   - ADC configuration for joystick/accelerometer.

---

#### ✅ **Critical Requirements**
- **Modularity**: Each exercise builds on prior work via new functions/modules.
- **Teacher Validation**:  
  - Solutions **must be demonstrated** during lab sessions.
  - Outputs (LCD images/videos) uploaded to `ent.esigelec.fr`.
- **Code Practices**:
  - Avoid global variables; use atomic sections for shared data.
  - Handle PWM duty-cycle updates synchronously (Exercise 16).
  - Use time-triggered systems for real-time polling (Exercise 24).

---

#### 🚀 **Exercise Highlights**
- **ADC & Sensors** (Exercises 17-22):  
  - Convert analog joystick/accelerometer data to digital values and voltages.
  - Sequence-of-channels ADC for multi-sensor sampling.
- **Real-Time Graphics** (Exercises 24-25):  
  - Move shapes on LCD using joystick/accelerometer at 40ms intervals.
- **Advanced PWM** (Exercises 15, 26):  
  - RGB LED control with duty-cycle rollover and hardware synchronization.
- **Button State Machines** (Exercises 5, 27):  
  - Toggle persistent graphics using interrupts and debouncing.

---

#### 📝 **Submission**
1. **Per Exercise**:
   - Source code (`.c`/`.h` files).
   - Output screenshots/videos.
   - Text responses to questions (uploaded to Moodle).
2. **Teacher Checkpoints**:  
   - Solutions validated in-lab after each exercise.

---

> 💡 **Tip**: Use the debugger (yellow pause/green resume) to inspect real-time counter values (Exercise 17)!  
> 🔧 **Hardware Notes**: Verify pin mappings in *F5529 Launchpad User’s Guide* and *Booster Pack Schematics*.  
