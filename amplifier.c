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
#include "amplifier.h"

static ComboBoxEntry vca_env_modes[] = {
    { "Envelope",            "images/env_mode_normal.png" },
    { "Gate",                "images/env_mode_gate.png" },
    { "Envelope + Dynamics", "images/env_mode_normal_dynamics.png" },
    { "Gate + Dynamics",     "images/env_mode_gate_dynamics.png" }
};

AmplifierDialog *
new_amplifier_dialog(GtkWindow *parent)
{
    AmplifierDialog *widgets;
    GtkGrid *grid;
    GtkWidget *label, *button_box, *button;

    widgets = g_new(AmplifierDialog, 1);

    widgets->dialog = create_window(parent, "Amplifier", FALSE);

    grid = create_grid(GTK_CONTAINER(widgets->dialog));

    label = gtk_label_new("VCA Level:");
    widgets->vca_level = create_hscale(0, 127, 22);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(widgets->vca_level));

    label = gtk_label_new("VCA Env Mode:");
    widgets->vca_env_mode = create_combo_box(vca_env_modes, G_N_ELEMENTS(vca_env_modes), 2);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(widgets->vca_env_mode));

    label = gtk_label_new("VCA Aftertouch Sensitivity:");
    widgets->vca_aftertouch_sensitivity = create_hscale(0, 127, 23);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(widgets->vca_aftertouch_sensitivity));

    label = gtk_label_new("Chorus:");
    widgets->chorus = create_check_button(10);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(widgets->chorus));

    label = gtk_label_new("Chorus Rate:");
    widgets->chorus_rate = create_hscale(0, 127, 34);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(widgets->chorus_rate));

    label = gtk_label_new("LFO Rate:");
    widgets->lfo_rate = create_hscale(0, 127, 24);
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(widgets->lfo_rate));

    label = gtk_label_new("LFO Delay:");
    widgets->lfo_delay = create_hscale(0, 127, 25);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(widgets->lfo_delay));

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 6);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_grid_attach(grid, button_box, 0, 7, 2, 1);

    button = gtk_button_new_with_label("Close");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(close_window_callback), widgets->dialog);
    gtk_container_add(GTK_CONTAINER(button_box), button);

    return widgets;
}

void
show_amplifier_dialog(AmplifierDialog *widgets)
{
    show_window(widgets->dialog);
}

void
set_amplifier_parameters(AmplifierDialog *widgets, Patch *patch)
{
    gtk_range_set_value(GTK_RANGE(widgets->vca_level), patch->parameters[PARAMETER_VCA_LEVEL]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->vca_env_mode), patch->parameters[PARAMETER_VCA_ENV_MODE]);
    gtk_range_set_value(GTK_RANGE(widgets->vca_aftertouch_sensitivity), patch->parameters[PARAMETER_VCA_AFTERTOUCH_SENSITIVITY]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->chorus), patch->parameters[PARAMETER_CHORUS] ? TRUE : FALSE);
    gtk_range_set_value(GTK_RANGE(widgets->chorus_rate), patch->parameters[PARAMETER_CHORUS_RATE]);
    gtk_range_set_value(GTK_RANGE(widgets->lfo_rate), patch->parameters[PARAMETER_LFO_RATE]);
    gtk_range_set_value(GTK_RANGE(widgets->lfo_delay), patch->parameters[PARAMETER_LFO_DELAY]);
}

void
clear_amplifier_parameters(AmplifierDialog *widgets)
{
    gtk_range_set_value(GTK_RANGE(widgets->vca_level), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->vca_env_mode), 0);
    gtk_range_set_value(GTK_RANGE(widgets->vca_aftertouch_sensitivity), 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->chorus), FALSE);
    gtk_range_set_value(GTK_RANGE(widgets->chorus_rate), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lfo_rate), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lfo_delay), 0);
}
