#pragma once
#include <Arduino.h>

class ESP32BeepCodes {
public:
    ESP32BeepCodes(uint8_t gpioPin,
                   uint8_t ledcChannel = 0,
                   uint16_t frequencyHz = 2000,
                   uint32_t shortMs = 100,
                   uint32_t longMs = 400,
                   uint32_t gapMs = 100,
                   uint32_t sequenceGapMs = 500);

    void begin();
    bool play(const String &pattern, bool repeat = false);
    void stop();
    void update();

    void setFrequency(uint16_t hz);
    void setTimings(uint32_t shortMs, uint32_t longMs, uint32_t gapMs, uint32_t sequenceGapMs);

    bool isPlaying() const { return _playing; }

private:
    enum State { IDLE, TONE_ON, GAP, SEQUENCE_GAP };

    void toneOn();
    void toneOff();

    uint8_t _pin;
    uint8_t _channel;
    uint16_t _frequency;

    uint32_t _shortMs;
    uint32_t _longMs;
    uint32_t _gapMs;
    uint32_t _sequenceGapMs;

    String _pattern;
    bool _repeat;
    bool _playing;

    State _state;
    size_t _index;
    uint32_t _stateStart;
};
