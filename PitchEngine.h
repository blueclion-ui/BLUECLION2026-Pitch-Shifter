#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

// Common interface between the app and a licensed pitch-shifting engine.
// The interface deliberately does not imitate or reimplement zplane's API.
class PitchEngine
{
public:
    virtual ~PitchEngine() = default;

    virtual juce::String getEngineName() const = 0;
    virtual bool isAvailable() const = 0;

    // semitones is clamped by the UI to [-24, +24].
    virtual void setPitchSemitones(double semitones) = 0;
    virtual void setFormantPreservation(bool enabled) = 0;

    // Called from the audio callback. Implementations must be real-time safe:
    // no allocations, file I/O, locks, or UI calls here.
    virtual bool process(juce::AudioBuffer<float>& audio) noexcept = 0;
};
