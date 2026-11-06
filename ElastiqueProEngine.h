#pragma once

#include "PitchEngine.h"

// Integration boundary for the official zplane ELASTIQUE PRO SDK.
//
// Once zplane supplies the licensed SDK, implement this class using the exact
// API and headers from that SDK version. Do not distribute SDK binaries or
// headers unless your agreement explicitly permits redistribution.
class ElastiqueProEngine final : public PitchEngine
{
public:
    juce::String getEngineName() const override;
    bool isAvailable() const override;
    void setPitchSemitones(double semitones) override;
    void setFormantPreservation(bool enabled) override;
    bool process(juce::AudioBuffer<float>& audio) noexcept override;

private:
    double pitchSemitones = 0.0;
    bool preserveFormants = true;
};
