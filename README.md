# ESP32BeepCodes

A lightweight, non-blocking Arduino library for generating **BIOS-style beep codes** on ESP32 devices using a passive speaker.

The library is designed for **boot diagnostics and fault reporting**, similar to early PCs, and uses the ESP32's **LEDC hardware PWM** for accurate and stable tone generation.

---

## Features

- ✅ Works on **all ESP32 variants** using the Arduino core
- ✅ **Non-blocking** (no `delay()`)
- ✅ Uses **hardware PWM (LEDC)** — no CPU-intensive toggling
- ✅ Supports **short and long beeps** with fixed but overridable timing
- ✅ Arbitrary beep patterns using a **simple string syntax**
- ✅ **One-shot or repeating** playback
- ✅ Fully **instance-based** (multiple speakers supported)
- ✅ Ignores new patterns while one is already playing

---

## Beep Pattern Syntax

Patterns are defined using a string:

| Symbol | Meaning |
|------|--------|
| `.`  | Short beep |
| `-`  | Long beep |

Example:
```cpp
"..-.-"   // short, short, long, short, long
```

---

## Installation

### Manual Installation

1. Download or clone this repository
2. Copy the folder into your Arduino `libraries` directory
3. Restart the Arduino IDE

---

## Basic Usage

```cpp
#include <ESP32BeepCodes.h>

ESP32BeepCodes beeper(25);   // GPIO 25

void setup() {
  beeper.begin();
  beeper.play("..-.-", false);  // One-shot pattern
}

void loop() {
  beeper.update();  // Must be called regularly
}
```

---

## Constructor

```cpp
ESP32BeepCodes(
    uint8_t gpioPin,
    uint8_t ledcChannel = 0,
    uint16_t frequencyHz = 2000,
    uint32_t shortMs = 100,
    uint32_t longMs = 400,
    uint32_t gapMs = 100,
    uint32_t sequenceGapMs = 500
);
```

### Parameters

- **gpioPin** – GPIO connected to the passive speaker
- **ledcChannel** – LEDC channel (0–15)
- **frequencyHz** – Beep frequency in Hz
- **shortMs** – Duration of a short beep
- **longMs** – Duration of a long beep
- **gapMs** – Gap between beeps
- **sequenceGapMs** – Gap between repeated sequences

---

## Public Methods

### `begin()`
Initialises LEDC and prepares the GPIO pin.

---

### `play(pattern, repeat)`
Starts playing a beep pattern.

```cpp
bool play(const String &pattern, bool repeat = false);
```

- Returns `false` if a pattern is already playing
- `repeat = true` loops the pattern until `stop()` is called

---

### `stop()`
Stops playback immediately.

---

### `update()`
Advances the internal state machine.

> **Must be called regularly** from `loop()` or a task.

---

### `setFrequency(hz)`
Changes the beep frequency (takes effect immediately or on next play).

---

### `setTimings(shortMs, longMs, gapMs, sequenceGapMs)`
Overrides the default timing values.

---

### `isPlaying()`
Returns `true` while a pattern is currently playing.

---

## Example: Repeating Error Code

```cpp
// Continuous fatal error pattern
beeper.play("---", true);  // long-long-long
```

---

## Hardware Notes

- Designed for **passive piezo or PCB mount speakers**
- Speaker connects directly to a GPIO pin
- For louder output, a transistor or amplifier may be used

---

## License

MIT License — free to use in personal and commercial projects.

---

## Author

**Phil Brien**

Contributions and pull requests are welcome.

