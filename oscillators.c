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

#include <gtk/gtk.h>

#include "main.h"
#include "dialog.h"
#include "oscillators.h"

static gchar *dco_ranges[] = {
    "4'", "8'", "16'", "32'"
};

static ComboBoxEntry dco_env_modes[] = {
    { "Normal",            "images/env_mode_normal.png" },
    { "Invert",            "images/env_mode_invert.png" },
    { "Normal + Dynamics", "images/env_mode_normal_dynamics.png" },
    { "Invert + Dynamics", "images/env_mode_invert_dynamics.png" }
};

static ComboBoxEntry dco_pulse_waveforms[] = {
    { "Off",     "images/waveform_off.png" },
    { "Pulse 1", "images/waveform_square.png" },
    { "Pulse 2", "images/waveform_pulse.png" },
    { "Pulse 3", "images/waveform_pulse_modulated.png" }
};

static ComboBoxEntry dco_sawtooth_waveforms[] = {
    { "Off",        "images/waveform_off.png" },
    { "Sawtooth 1", "images/waveform_1.png" },
    { "Sawtooth 2", "images/waveform_2.png" },
    { "Sawtooth 3", "images/waveform_3.png" },
    { "Sawtooth 4", "images/waveform_4.png" },
    { "Sawtooth 5", "images/waveform_5.png" }
};

static ComboBoxEntry dco_sub_oscillator_waveforms[] = {
    { "Waveform 1", "images/waveform_1.png" },
    { "Waveform 2", "images/waveform_2.png" },
    { "Waveform 3", "images/waveform_3.png" },
    { "Waveform 4", "images/waveform_4.png" },
    { "Waveform 5", "images/waveform_5.png" },
    { "Waveform 6", "images/waveform_6.png" }
};

OscillatorsDialog *
new_oscillators_dialog(GtkWindow *parent)
{
    OscillatorsDialog *widgets;
    GtkGrid *grid;
    GtkWidget *label, *button_box, *button;

    widgets = g_new(OscillatorsDialog, 1);

    widgets->dialog = create_window(parent, "Oscillators", FALSE);

    grid = create_grid(GTK_CONTAINER(widgets->dialog));

    label = gtk_label_new("DCO Range:");
    widgets->dco_range = create_combo_box(dco_ranges, G_N_ELEMENTS(dco_ranges), PARAMETER_DCO_RANGE);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(widgets->dco_range));

    label = gtk_label_new("DCO LFO Depth:");
    widgets->dco_lfo_depth = create_hscale(0, 127, PARAMETER_DCO_LFO_DEPTH);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(widgets->dco_lfo_depth));

    label = gtk_label_new("DCO Env Depth:");
    widgets->dco_env_depth = create_hscale(0, 127, PARAMETER_DCO_ENV_DEPTH);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(widgets->dco_env_depth));

    label = gtk_label_new("DCO Env Mode:");
    widgets->dco_env_mode = create_combo_box_with_entries(dco_env_modes, G_N_ELEMENTS(dco_env_modes), PARAMETER_DCO_ENV_MODE);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(widgets->dco_env_mode));

    label = gtk_label_new("DCO Aftertouch Sensitivity:");
    widgets->dco_aftertouch_sensitivity = create_hscale(0, 127, PARAMETER_DCO_AFTERTOUCH_SENSITIVITY);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(widgets->dco_aftertouch_sensitivity));

    label = gtk_label_new("DCO Bender Range:");
    widgets->dco_bender_range = create_hscale(0, 12, PARAMETER_DCO_BENDER_RANGE);
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(widgets->dco_bender_range));

    label = gtk_label_new("DCO Pulse Waveform:");
    widgets->dco_pulse_waveform = create_combo_box_with_entries(dco_pulse_waveforms, G_N_ELEMENTS(dco_pulse_waveforms), PARAMETER_DCO_PULSE_WAVEFORM);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(widgets->dco_pulse_waveform));

    label = gtk_label_new("DCO Sawtooth Waveform:");
    widgets->dco_sawtooth_waveform = create_combo_box_with_entries(dco_sawtooth_waveforms, G_N_ELEMENTS(dco_sawtooth_waveforms), PARAMETER_DCO_SAWTOOTH_WAVEFORM);
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(widgets->dco_sawtooth_waveform));

    label = gtk_label_new("DCO Sub-Oscillator Waveform:");
    widgets->dco_sub_oscillator_waveform = create_combo_box_with_entries(dco_sub_oscillator_waveforms, G_N_ELEMENTS(dco_sub_oscillator_waveforms), PARAMETER_DCO_SUB_OSCILLATOR_WAVEFORM);
    create_grid_row(grid, 8, GTK_LABEL(label), GTK_WIDGET(widgets->dco_sub_oscillator_waveform));

    label = gtk_label_new("DCO Sub-Oscillator Level:");
    widgets->dco_sub_oscillator_level = create_hscale(0, 3, PARAMETER_DCO_SUB_OSCILLATOR_LEVEL);
    create_grid_row(grid, 9, GTK_LABEL(label), GTK_WIDGET(widgets->dco_sub_oscillator_level));

    label = gtk_label_new("DCO Noise Level:");
    widgets->dco_noise_level = create_hscale(0, 3, PARAMETER_DCO_NOISE_LEVEL);
    create_grid_row(grid, 10, GTK_LABEL(label), GTK_WIDGET(widgets->dco_noise_level));

    label = gtk_label_new("DCO PWM Depth:");
    widgets->dco_pwm_depth = create_hscale(0, 127, PARAMETER_DCO_PWM_DEPTH);
    create_grid_row(grid, 11, GTK_LABEL(label), GTK_WIDGET(widgets->dco_pwm_depth));

    label = gtk_label_new("DCO PWM Rate:");
    widgets->dco_pwm_rate = create_hscale(0, 127, PARAMETER_DCO_PWM_RATE);
    create_grid_row(grid, 12, GTK_LABEL(label), GTK_WIDGET(widgets->dco_pwm_rate));

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 6);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_grid_attach(grid, button_box, 0, 13, 2, 1);

    button = gtk_button_new_with_label("Close");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(close_window_callback), widgets->dialog);
    gtk_container_add(GTK_CONTAINER(button_box), button);

    return widgets;
}

void
show_oscillators_dialog(OscillatorsDialog *widgets)
{
    show_window(widgets->dialog);
}

void
set_oscillators_parameters(OscillatorsDialog *widgets, Patch *patch)
{
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->dco_range), patch->parameters[PARAMETER_DCO_RANGE]);
    gtk_range_set_value(GTK_RANGE(widgets->dco_lfo_depth), patch->parameters[PARAMETER_DCO_LFO_DEPTH]);
    gtk_range_set_value(GTK_RANGE(widgets->dco_env_depth), patch->parameters[PARAMETER_DCO_ENV_DEPTH]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->dco_env_mode), patch->parameters[PARAMETER_DCO_ENV_MODE]);
    gtk_range_set_value(GTK_RANGE(widgets->dco_aftertouch_sensitivity), patch->parameters[PARAMETER_DCO_AFTERTOUCH_SENSITIVITY]);
    gtk_range_set_value(GTK_RANGE(widgets->dco_bender_range), patch->parameters[PARAMETER_DCO_BENDER_RANGE]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->dco_pulse_waveform), patch->parameters[PARAMETER_DCO_PULSE_WAVEFORM]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->dco_sawtooth_waveform), patch->parameters[PARAMETER_DCO_SAWTOOTH_WAVEFORM]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->dco_sub_oscillator_waveform), patch->parameters[PARAMETER_DCO_SUB_OSCILLATOR_WAVEFORM]);
    gtk_range_set_value(GTK_RANGE(widgets->dco_sub_oscillator_level), patch->parameters[PARAMETER_DCO_SUB_OSCILLATOR_LEVEL]);
    gtk_range_set_value(GTK_RANGE(widgets->dco_noise_level), patch->parameters[PARAMETER_DCO_NOISE_LEVEL]);
    gtk_range_set_value(GTK_RANGE(widgets->dco_pwm_depth), patch->parameters[PARAMETER_DCO_PWM_DEPTH]);
    gtk_range_set_value(GTK_RANGE(widgets->dco_pwm_rate), patch->parameters[PARAMETER_DCO_PWM_RATE]);
}

void
clear_oscillators_parameters(OscillatorsDialog *widgets)
{
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->dco_range), 0);
    gtk_range_set_value(GTK_RANGE(widgets->dco_lfo_depth), 0);
    gtk_range_set_value(GTK_RANGE(widgets->dco_env_depth), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->dco_env_mode), 0);
    gtk_range_set_value(GTK_RANGE(widgets->dco_aftertouch_sensitivity), 0);
    gtk_range_set_value(GTK_RANGE(widgets->dco_bender_range), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->dco_pulse_waveform), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->dco_sawtooth_waveform), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->dco_sub_oscillator_waveform), 0);
    gtk_range_set_value(GTK_RANGE(widgets->dco_sub_oscillator_level), 0);
    gtk_range_set_value(GTK_RANGE(widgets->dco_noise_level), 0);
    gtk_range_set_value(GTK_RANGE(widgets->dco_pwm_depth), 0);
    gtk_range_set_value(GTK_RANGE(widgets->dco_pwm_rate), 0);
}
