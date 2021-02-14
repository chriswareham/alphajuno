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
#include "filters.h"

static ComboBoxEntry lpf_env_modes[] = {
    { "Normal",            "images/env_mode_normal.png" },
    { "Invert",            "images/env_mode_invert.png" },
    { "Normal + Dynamics", "images/env_mode_normal_dynamics.png" },
    { "Dynamics",          "images/env_mode_dynamics.png" }
};

FiltersDialog *
new_filters_dialog(GtkWindow *parent)
{
    FiltersDialog *widgets;
    GtkGrid *grid;
    GtkWidget *label, *button_box, *button;

    widgets = g_new(FiltersDialog, 1);

    widgets->dialog = create_window(parent, "Filters", FALSE);

    grid = create_grid(GTK_CONTAINER(widgets->dialog));

    label = gtk_label_new("HPF Cutoff Frequency:");
    widgets->hpf_cutoff_frequency = create_hscale(0, 3, PARAMETER_HPF_CUTOFF_FREQUENCY);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(widgets->hpf_cutoff_frequency));

    label = gtk_label_new("LPF Cutoff Frequency:");
    widgets->lpf_cutoff_frequency = create_hscale(0, 127, PARAMETER_LPF_CUTOFF_FREQUENCY);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(widgets->lpf_cutoff_frequency));

    label = gtk_label_new("LPF Resonance:");
    widgets->lpf_resonance = create_hscale(0, 127, PARAMETER_LPF_RESONANCE);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(widgets->lpf_resonance));

    label = gtk_label_new("LPF Env Depth:");
    widgets->lpf_env_depth = create_hscale(0, 127, PARAMETER_LPF_ENV_DEPTH);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(widgets->lpf_env_depth));

    label = gtk_label_new("LPF Env Mode:");
    widgets->lpf_env_mode = create_combo_box_with_entries(lpf_env_modes, G_N_ELEMENTS(lpf_env_modes), PARAMETER_LPF_ENV_MODE);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(widgets->lpf_env_mode));

    label = gtk_label_new("LPF LFO Depth:");
    widgets->lpf_lfo_depth = create_hscale(0, 127, PARAMETER_LPF_LFO_DEPTH);
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(widgets->lpf_lfo_depth));

    label = gtk_label_new("LPF Keyboard Follower:");
    widgets->lpf_keyboard_follower = create_hscale(0, 15, PARAMETER_LPF_KEYBOARD_FOLLOWER);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(widgets->lpf_keyboard_follower));

    label = gtk_label_new("LPF Aftertouch Sensitivity:");
    widgets->lpf_aftertouch_sensitivity = create_hscale(0, 15, PARAMETER_LPF_AFTERTOUCH_SENSITIVITY);
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(widgets->lpf_aftertouch_sensitivity));

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 6);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_grid_attach(grid, button_box, 0, 8, 2, 1);

    button = gtk_button_new_with_label("Close");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(close_window_callback), widgets->dialog);
    gtk_container_add(GTK_CONTAINER(button_box), button);

    return widgets;
}

void
show_filters_dialog(FiltersDialog *widgets)
{
    show_window(widgets->dialog);
}

void
set_filters_parameters(FiltersDialog *widgets, Patch *patch)
{
    gtk_range_set_value(GTK_RANGE(widgets->hpf_cutoff_frequency), patch->parameters[PARAMETER_HPF_CUTOFF_FREQUENCY]);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_cutoff_frequency), patch->parameters[PARAMETER_LPF_CUTOFF_FREQUENCY]);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_resonance), patch->parameters[PARAMETER_LPF_RESONANCE]);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_env_depth), patch->parameters[PARAMETER_LPF_ENV_DEPTH]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lpf_env_mode), patch->parameters[PARAMETER_LPF_ENV_MODE]);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_lfo_depth), patch->parameters[PARAMETER_LPF_LFO_DEPTH]);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_keyboard_follower), patch->parameters[PARAMETER_LPF_KEYBOARD_FOLLOWER]);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_aftertouch_sensitivity), patch->parameters[PARAMETER_LPF_AFTERTOUCH_SENSITIVITY]);
}

void
clear_filters_parameters(FiltersDialog *widgets)
{
    gtk_range_set_value(GTK_RANGE(widgets->hpf_cutoff_frequency), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_cutoff_frequency), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_resonance), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_env_depth), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lpf_env_mode), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_lfo_depth), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_keyboard_follower), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lpf_aftertouch_sensitivity), 0);
}
