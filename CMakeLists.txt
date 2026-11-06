cmake_minimum_required(VERSION 3.22)
project(BLUECLION2026PitchShifter VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.6
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_gui_app(BLUECLION2026PitchShifter
    PRODUCT_NAME "BLUECLION2026 Pitch Shifter"
    COMPANY_NAME "BLUECLION2026"
    BUNDLE_ID "com.blueclion2026.pitchshifter"
)

target_sources(BLUECLION2026PitchShifter PRIVATE
    Source/Main.cpp
    Source/PitchEngine.h
    Source/ElastiqueProEngine.h
    Source/ElastiqueProEngine.cpp
)

target_compile_features(BLUECLION2026PitchShifter PRIVATE cxx_std_17)
target_compile_definitions(BLUECLION2026PitchShifter PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:BLUECLION2026PitchShifter,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="${PROJECT_VERSION}"
)

target_link_libraries(BLUECLION2026PitchShifter PRIVATE
    juce::juce_gui_extra
    juce::juce_audio_utils
    juce::juce_audio_formats
    juce::juce_audio_devices
    juce::juce_dsp
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
