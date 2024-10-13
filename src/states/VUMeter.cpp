// MIT License
//
// Copyright 2024 Eurofurence e.V.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the “Software”),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

/**
 * @author 32
 */

#include <EFLed.h>
#include <EFLogging.h>

#include "FSMState.h"

#define AUDIO_PIN 14

const uint8_t dragon_hue = 100;
const int samples = 300;

const char *VUMeter::getName()
{
    return "VUMeter";
}

bool VUMeter::shouldBeRemembered()
{
    return true;
}
void VUMeter::entry()
{
    this->tick = 0;
}

int sMin = 4096; // make shure to be outside the range from 0-4095
int sMax = -1;
int sMaxLevel = 1; // provide minimal level not to
int nl = 32;

CRGB leds[17];

void VUMeter::run()
{
    sMin = 4096; // make shure to be outside the range from 0-4095
    sMax = -1;

    // take "samples" Samples and determin min and max
    for (int s = 0; s < samples; s++)
    {
        int sample = analogRead(AUDIO_PIN);

        // Update max and min values
        if (sample < sMin)
        {
            sMin = sample;
        }
        if (sample > sMax)
        {
            sMax = sample;
        }
    }

    // Calculate peak-to-peak range (this is the signal strength)
    int peakToPeak = sMax - sMin;

    // Determin Maxlevel recorded
    if (sMaxLevel < peakToPeak)
        sMaxLevel = peakToPeak;

    // Map the peak-to-peak value to a range of 0 to NUM_LEDS * 256 - 1
    uint16_t n = map(peakToPeak, 0, sMaxLevel, 0, 256 * 11 - 1);

    LOGF_DEBUG("(VUMeter) Min: %d Max: %d peakToPeak: %d Maxlevel: %d Mapped: %d\r\n", sMin, sMax, peakToPeak, sMaxLevel, n);

    // VU Meter
    for (int i = 0; i < 11; ++i)
    {
        leds[17 + 1 - i] = CHSV((this->tick + i * 21) % 256, 255,
            (uint8_t)(n >= 256 * (i + 1) ? 255 : n >= 256 * (i) ? n % 256 : 0));
        LOGF_DEBUG("(VUMeter) LED: %d H: %d S: %d V: %d\r\n", i, (this->tick + i * 21) % 256, 255, (uint8_t)(n >= 256 * (11 - i + 1) ? 255 : n >= 256 * (11 - i) ? n % 256 : 0));

    }

    // Dragon Face
    for (int i = 0; 1 < 6; ++i) {
        leds[i] = CHSV(dragon_hue, 255, (uint8_t)( ((i * 3 + tick) * (256 / 15) ) % 256 ));
    }

    EFLed.setAll(leds);

    this->tick++;

}

std::unique_ptr<FSMState> VUMeter::touchEventFingerprintShortpress()
{
    if (this->isLocked())
    {
        return nullptr;
    }

    return std::make_unique<MenuMain>();
}

std::unique_ptr<FSMState> VUMeter::touchEventFingerprintLongpress()
{
    return this->touchEventFingerprintShortpress();
}

std::unique_ptr<FSMState> VUMeter::touchEventFingerprintRelease()
{
    if (this->isLocked())
    {
        return nullptr;
    }

    this->globals->animMatrixIdx = (this->globals->animMatrixIdx + 1) % 9;
    this->is_globals_dirty = true;
    this->tick = 0;

    return nullptr;
}

std::unique_ptr<FSMState> VUMeter::touchEventAllLongpress()
{
    this->toggleLock();
    return nullptr;
}

/* std::unique_ptr<FSMState> VUMeter::touchEventNoseShortpress()
{
    sMaxLevel = 1;
    return nullptr;
}
 */
