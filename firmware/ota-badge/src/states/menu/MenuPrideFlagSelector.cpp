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
 * @author Honigeintopf
 */

#include "FSMState.h"

const char* MenuPrideFlagSelector::getName() {
    return "MenuPrideFlagSelector";
}

const unsigned int MenuPrideFlagSelector::getTickRateMs() {
    if (this->globals->prideFlagModeIdx == 0) {
        return 1000;
    } else {
        return 100;
    }
}

void MenuPrideFlagSelector::entry() {
    // Construct DisplayPrideFlag state locally to get a preview of the current mode
    this->prideFlagDisplayRunner = std::make_unique<DisplayPrideFlag>();
    this->prideFlagDisplayRunner->attachGlobals(this->globals);
}

void MenuPrideFlagSelector::run() {
    this->prideFlagDisplayRunner->run();
}

void MenuPrideFlagSelector::exit() {
    this->prideFlagDisplayRunner.reset();
}

std::unique_ptr<FSMState> MenuPrideFlagSelector::touchEventFingerprintRelease() {
    this->globals->prideFlagModeIdx = (this->globals->prideFlagModeIdx + 1) % 13;
    return nullptr;
}

std::unique_ptr<FSMState> MenuPrideFlagSelector::touchEventFingerprintShortpress() {
    return std::make_unique<MenuMain>();
}

std::unique_ptr<FSMState> MenuPrideFlagSelector::touchEventFingerprintLongpress() {
    return std::make_unique<MenuMain>();
}
