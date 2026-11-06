#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "ElastiqueProEngine.h"

class MainComponent final : public juce::AudioAppComponent,
                            private juce::Button::Listener,
                            private juce::Slider::Listener
{
public:
    MainComponent()
    {
        formatManager.registerBasicFormats();

        title.setText("BLUECLION2026 Pitch Shifter", juce::dontSendNotification);
        title.setFont(juce::FontOptions(25.0f, juce::Font::bold));
        title.setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(title);

        subtitle.setText("Standalone audio pitch-shifting workstation", juce::dontSendNotification);
        subtitle.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
        addAndMakeVisible(subtitle);

        importButton.setButtonText("Import audio");
        importButton.addListener(this);
        addAndMakeVisible(importButton);

        playButton.setButtonText("Play");
        playButton.addListener(this);
        playButton.setEnabled(false);
        addAndMakeVisible(playButton);

        pitchSlider.setRange(-24.0, 24.0, 0.01);
        pitchSlider.setValue(0.0);
        pitchSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 120, 28);
        pitchSlider.setTextValueSuffix(" semitones");
        pitchSlider.addListener(this);
        addAndMakeVisible(pitchSlider);

        pitchLabel.setText("Pitch", juce::dontSendNotification);
        pitchLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(pitchLabel);

        formantToggle.setButtonText("Preserve formants");
        formantToggle.setToggleState(true, juce::dontSendNotification);
        formantToggle.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
        formantToggle.onClick = [this]
        {
            engine.setFormantPreservation(formantToggle.getToggleState());
        };
        addAndMakeVisible(formantToggle);

        status.setColour(juce::Label::textColourId, juce::Colours::orange);
        status.setText("ELASTIQUE PRO SDK not connected. Audio preview is currently unshifted.",
                       juce::dontSendNotification);
        status.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(status);

        fileName.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
        fileName.setText("No audio file loaded", juce::dontSendNotification);
        addAndMakeVisible(fileName);

        setSize(760, 470);
        setAudioChannels(0, 2);
    }

    ~MainComponent() override
    {
        shutdownAudio();
        transportSource.setSource(nullptr);
        readerSource.reset();
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& info) override
    {
        transportSource.getNextAudioBlock(info);
        // SDK processing will be inserted in the real-time-safe audio path here.
        // Do not call UI methods or allocate memory in this callback.
    }

    void releaseResources() override
    {
        transportSource.releaseResources();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xff171a21));
        g.setColour(juce::Colour(0xff252b36));
        g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(18.0f), 14.0f);
        g.setColour(juce::Colour(0xff3c4657));
        g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(18.0f), 14.0f, 1.0f);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(42, 34);
        title.setBounds(area.removeFromTop(38));
        subtitle.setBounds(area.removeFromTop(28));
        area.removeFromTop(24);

        auto buttons = area.removeFromTop(42);
        importButton.setBounds(buttons.removeFromLeft(150));
        buttons.removeFromLeft(12);
        playButton.setBounds(buttons.removeFromLeft(100));

        area.removeFromTop(24);
        pitchLabel.setBounds(area.removeFromTop(24));
        pitchSlider.setBounds(area.removeFromTop(100).reduced(0, 0));
        formantToggle.setBounds(area.removeFromTop(34));
        area.removeFromTop(14);
        fileName.setBounds(area.removeFromTop(28));
        status.setBounds(area.removeFromTop(52));
    }

private:
    void buttonClicked(juce::Button* button) override
    {
        if (button == &importButton)
            chooseAudioFile();
        else if (button == &playButton)
        {
            if (transportSource.isPlaying())
            {
                transportSource.stop();
                playButton.setButtonText("Play");
            }
            else
            {
                transportSource.setPosition(0.0);
                transportSource.start();
                playButton.setButtonText("Stop");
            }
        }
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &pitchSlider)
        {
            engine.setPitchSemitones(pitchSlider.getValue());
            if (!engine.isAvailable())
                status.setText("Pitch set to " + juce::String(pitchSlider.getValue(), 2)
                    + " semitones. Connect the licensed ELASTIQUE PRO SDK to process audio.",
                    juce::dontSendNotification);
        }
    }

    void chooseAudioFile()
    {
        juce::FileChooser chooser("Select an audio file", juce::File{},
                                  formatManager.getWildcardForAllFormats());
        if (!chooser.browseForFileToOpen())
            return;

        auto file = chooser.getResult();
        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
        if (reader == nullptr)
        {
            status.setText("Could not open this audio file.", juce::dontSendNotification);
            return;
        }

        auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader.release(), true);
        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource = std::move(newSource);
        transportSource.setSource(readerSource.get(), 0, nullptr,
                                  readerSource->getAudioFormatReader()->sampleRate);
        fileName.setText(file.getFileName(), juce::dontSendNotification);
        playButton.setEnabled(true);
        playButton.setButtonText("Play");
        status.setText("Audio loaded. Preview works; pitch processing awaits the licensed SDK.",
                       juce::dontSendNotification);
    }

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    ElastiqueProEngine engine;

    juce::Label title, subtitle, pitchLabel, fileName, status;
    juce::TextButton importButton, playButton;
    juce::Slider pitchSlider;
    juce::ToggleButton formantToggle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

class PitchShifterApplication final : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "BLUECLION2026 Pitch Shifter"; }
    const juce::String getApplicationVersion() override { return "0.1.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String&) override
    {
        mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }

    void shutdown() override { mainWindow.reset(); }
    void systemRequestedQuit() override { quit(); }
    void anotherInstanceStarted(const juce::String&) override {}

private:
    class MainWindow final : public juce::DocumentWindow
    {
    public:
        explicit MainWindow(const juce::String& name)
            : DocumentWindow(name, juce::Colour(0xff171a21), allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);
            centreWithSize(getWidth(), getHeight());
            setResizable(true, true);
            setVisible(true);
        }

        void closeButtonPressed() override { juce::JUCEApplication::getInstance()->systemRequestedQuit(); }
    };

    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(PitchShifterApplication)
