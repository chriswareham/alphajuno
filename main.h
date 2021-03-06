/*
 * Copyright (c) 2004 Chris Wareham <chris@chriswareham.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef MAIN_H
#define MAIN_H

typedef enum {
    PARAMETER_DCO_ENV_MODE,
    PARAMETER_LPF_ENV_MODE,
    PARAMETER_VCA_ENV_MODE,
    PARAMETER_DCO_PULSE_WAVEFORM,
    PARAMETER_DCO_SAWTOOTH_WAVEFORM,
    PARAMETER_DCO_SUB_OSCILLATOR_WAVEFORM,
    PARAMETER_DCO_RANGE,
    PARAMETER_DCO_SUB_OSCILLATOR_LEVEL,
    PARAMETER_DCO_NOISE_LEVEL,
    PARAMETER_HPF_CUTOFF_FREQUENCY,
    PARAMETER_CHORUS,
    PARAMETER_DCO_LFO_DEPTH,
    PARAMETER_DCO_ENV_DEPTH,
    PARAMETER_DCO_AFTERTOUCH_SENSITIVITY,
    PARAMETER_DCO_PWM_DEPTH,
    PARAMETER_DCO_PWM_RATE,
    PARAMETER_LPF_CUTOFF_FREQUENCY,
    PARAMETER_LPF_RESONANCE,
    PARAMETER_LPF_LFO_DEPTH,
    PARAMETER_LPF_ENV_DEPTH,
    PARAMETER_LPF_KEYBOARD_FOLLOWER,
    PARAMETER_LPF_AFTERTOUCH_SENSITIVITY,
    PARAMETER_VCA_LEVEL,
    PARAMETER_VCA_AFTERTOUCH_SENSITIVITY,
    PARAMETER_LFO_RATE,
    PARAMETER_LFO_DELAY,
    PARAMETER_ENV_T1,
    PARAMETER_ENV_L1,
    PARAMETER_ENV_T2,
    PARAMETER_ENV_L2,
    PARAMETER_ENV_T3,
    PARAMETER_ENV_L3,
    PARAMETER_ENV_T4,
    PARAMETER_ENV_KEYBOARD_FOLLOWER,
    PARAMETER_CHORUS_RATE,
    PARAMETER_DCO_BENDER_RANGE,
    PARAMETER_COUNT
} Parameters;

typedef struct {
    gchar *filename, *name, *type;
    guchar parameters[PARAMETER_COUNT];
} Patch;

extern Patch *current_patch;

typedef struct {
    GtkWidget *statusbar;
    guint statusbar_context_id;
} Statusbar;

void update_statusbar(Statusbar *, const gchar *);

#endif /* !MAIN_H */
