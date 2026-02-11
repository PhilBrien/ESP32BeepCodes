#include "ESP32BeepCodes.h"

ESP32BeepCodes::ESP32BeepCodes(uint8_t gpioPin,
                               uint8_t ledcChannel,
                               uint16_t frequencyHz,
                               uint32_t shortMs,
                               uint32_t longMs,
                               uint32_t gapMs,
                               uint32_t sequenceGapMs)
    : _pin(gpioPin),
      _channel(ledcChannel),
      _frequency(frequencyHz),
      _shortMs(shortMs),
      _longMs(longMs),
      _gapMs(gapMs),
      _sequenceGapMs(sequenceGapMs),
      _repeat(false),
      _playing(false),
      _state(IDLE),
      _index(0),
      _stateStart(0) {}

void ESP32BeepCodes::begin() {
    ledcSetup(_channel, _frequency, 10);
    ledcAttachPin(_pin, _channel);
    toneOff();
}

bool ESP32BeepCodes::play(const String &pattern, bool repeat) {
    if (_playing || pattern.length() == 0) return false;

    _pattern = pattern;
    _repeat = repeat;
    _index = 0;
    _playing = true;
    _state = TONE_ON;
    _stateStart = millis();
    toneOn();
    return true;
}

void ESP32BeepCodes::stop() {
    toneOff();
    _playing = false;
    _state = IDLE;
}

void ESP32BeepCodes::setFrequency(uint16_t hz) {
    _frequency = hz;
    ledcSetup(_channel, _frequency, 10);
}

void ESP32BeepCodes::setTimings(uint32_t shortMs, uint32_t longMs, uint32_t gapMs, uint32_t sequenceGapMs) {
    _shortMs = shortMs;
    _longMs = longMs;
    _gapMs = gapMs;
    _sequenceGapMs = sequenceGapMs;
}

void ESP32BeepCodes::update() {
    if (!_playing) return;
    uint32_t now = millis();

    switch (_state) {
        case TONE_ON: {
            char c = _pattern[_index];
            uint32_t duration = (c == '-') ? _longMs : _shortMs;
            if (now - _stateStart >= duration) {
                toneOff();
                _state = GAP;
                _stateStart = now;
            }
            break;
        }
        case GAP:
            if (now - _stateStart >= _gapMs) {
                _index++;
                if (_index >= _pattern.length()) {
                    if (_repeat) {
                        _state = SEQUENCE_GAP;
                        _stateStart = now;
                    } else {
                        stop();
                    }
                } else {
                    _state = TONE_ON;
                    _stateStart = now;
                    toneOn();
                }
            }
            break;
        case SEQUENCE_GAP:
            if (now - _stateStart >= _sequenceGapMs) {
                _index = 0;
                _state = TONE_ON;
                _stateStart = now;
                toneOn();
            }
            break;
        case IDLE:
        default:
            break;
    }
}

void ESP32BeepCodes::toneOn() {
    ledcWrite(_channel, 512);
}

void ESP32BeepCodes::toneOff() {
    ledcWrite(_channel, 0);
}
