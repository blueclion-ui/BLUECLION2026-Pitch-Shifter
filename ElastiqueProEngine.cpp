#include "ElastiqueProEngine.h"

juce::String ElastiqueProEngine::getEngineName() const
{
    return "zplane ELASTIQUE PRO (SDK integration pending)";
}

bool ElastiqueProEngine::isAvailable() const
{
    // Returns false until the licensed SDK adapter is implemented.
    return false;
}

void ElastiqueProEngine::setPitchSemitones(double semitones)
{
    pitchSemitones = juce::jlimit(-24.0, 24.0, semitones);
}

void ElastiqueProEngine::setFormantPreservation(bool enabled)
{
    preserveFormants = enabled;
}

bool ElastiqueProEngine::process(juce::AudioBuffer<float>& audio) noexcept
{
    juce::ignoreUnused(audio, pitchSemitones, preserveFormants);
    // Intentionally do not alter audio: this is NOT a substitute pitch shifter.
    // Replace this body with the official SDK's processing calls after licensing.
    return false;
}
