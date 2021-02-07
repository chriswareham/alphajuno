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
#include "envelope.h"

static gboolean envelope_event_callback(GtkWidget *, GdkEvent *, gpointer);
static void envelope_callback(GtkWidget *, gpointer);
static void envelope_draw(EnvelopeDialog *);

EnvelopeDialog *
new_envelope_dialog(GtkWindow *parent)
{
    EnvelopeDialog *widgets;
    GtkGrid *grid;
    GtkWidget *frame, *label, *button_box, *button;

    widgets = g_new(EnvelopeDialog, 1);

    widgets->dialog = create_window(parent, "Envelope", FALSE);

    grid = create_grid(GTK_CONTAINER(widgets->dialog));

    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
    gtk_grid_attach(grid, frame, 0, 0, 2, 1);

    widgets->envelope = gtk_drawing_area_new();
    gtk_widget_set_size_request(GTK_WIDGET(widgets->envelope), 317, 127);
    g_signal_connect(G_OBJECT(widgets->envelope), "draw", G_CALLBACK(envelope_event_callback), widgets);
    gtk_container_add(GTK_CONTAINER(frame), widgets->envelope);

    label = gtk_label_new("Attack Time:");
    widgets->env_t1 = create_hscale(0, 127, 26);
    g_signal_connect(G_OBJECT(widgets->env_t1), "value-changed", G_CALLBACK(envelope_callback), widgets);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(widgets->env_t1));

    label = gtk_label_new("Attack Level:");
    widgets->env_l1 = create_hscale(0, 127, 27);
    g_signal_connect(G_OBJECT(widgets->env_l1), "value-changed", G_CALLBACK(envelope_callback), widgets);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(widgets->env_l1));

    label = gtk_label_new("Break Time:");
    widgets->env_t2 = create_hscale(0, 127, 28);
    g_signal_connect(G_OBJECT(widgets->env_t2), "value-changed", G_CALLBACK(envelope_callback), widgets);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(widgets->env_t2));

    label = gtk_label_new("Break Level:");
    widgets->env_l2 = create_hscale(0, 127, 29);
    g_signal_connect(G_OBJECT(widgets->env_l2), "value-changed", G_CALLBACK(envelope_callback), widgets);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(widgets->env_l2));

    label = gtk_label_new("Decay Time:");
    widgets->env_t3 = create_hscale(0, 127, 30);
    g_signal_connect(G_OBJECT(widgets->env_t3), "value-changed", G_CALLBACK(envelope_callback), widgets);
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(widgets->env_t3));

    label = gtk_label_new("Sustain Level:");
    widgets->env_l3 = create_hscale(0, 127, 31);
    g_signal_connect(G_OBJECT(widgets->env_l3), "value-changed", G_CALLBACK(envelope_callback), widgets);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(widgets->env_l3));

    label = gtk_label_new("Release Time:");
    widgets->env_t4 = create_hscale(0, 127, 32);
    g_signal_connect(G_OBJECT(widgets->env_t4), "value-changed", G_CALLBACK(envelope_callback), widgets);
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(widgets->env_t4));

    label = gtk_label_new("Keyboard Follower:");
    widgets->env_keyboard_follower = create_hscale(0, 15, 33);
    create_grid_row(grid, 8, GTK_LABEL(label), GTK_WIDGET(widgets->env_keyboard_follower));

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 6);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_grid_attach(grid, button_box, 0, 9, 2, 1);

    button = gtk_button_new_with_label("Close");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(close_window_callback), widgets->dialog);
    gtk_container_add(GTK_CONTAINER(button_box), button);

    return widgets;
}

void
show_envelope_dialog(EnvelopeDialog *widgets)
{
    show_window(widgets->dialog);
}

void
set_envelope_parameters(EnvelopeDialog *widgets, Patch *patch)
{
    gtk_range_set_value(GTK_RANGE(widgets->env_t1), patch->parameters[PARAMETER_ENV_T1]);
    gtk_range_set_value(GTK_RANGE(widgets->env_l1), patch->parameters[PARAMETER_ENV_L1]);
    gtk_range_set_value(GTK_RANGE(widgets->env_t2), patch->parameters[PARAMETER_ENV_T2]);
    gtk_range_set_value(GTK_RANGE(widgets->env_l2), patch->parameters[PARAMETER_ENV_L2]);
    gtk_range_set_value(GTK_RANGE(widgets->env_t3), patch->parameters[PARAMETER_ENV_T3]);
    gtk_range_set_value(GTK_RANGE(widgets->env_l3), patch->parameters[PARAMETER_ENV_L3]);
    gtk_range_set_value(GTK_RANGE(widgets->env_t4), patch->parameters[PARAMETER_ENV_T4]);
    gtk_range_set_value(GTK_RANGE(widgets->env_keyboard_follower), patch->parameters[PARAMETER_ENV_KEYBOARD_FOLLOWER]);
}

void
clear_envelope_parameters(EnvelopeDialog *widgets)
{
    gtk_range_set_value(GTK_RANGE(widgets->env_t1), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env_l1), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env_t2), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env_l2), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env_t3), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env_l3), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env_t4), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env_keyboard_follower), 0);
}

static gboolean
envelope_event_callback(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    EnvelopeDialog *widgets = (EnvelopeDialog *) data;
    envelope_draw(widgets);
    return TRUE;
}

static void
envelope_callback(GtkWidget *widget, gpointer data)
{
    EnvelopeDialog *widgets = (EnvelopeDialog *) data;
    if (gtk_widget_get_visible(GTK_WIDGET(widgets->dialog))) {
        envelope_draw(widgets);
    }
}

static void
envelope_draw(EnvelopeDialog *widgets)
{
    GtkAllocation allocation;
    GdkWindow *window;
    GdkDrawingContext *drawing_context;
    cairo_region_t *cairo_region;
    cairo_t *cairo;
    gint t1, l1, t2, l2, t3, l3, t4;
    double dashes[2];

    gtk_widget_get_allocation(widgets->envelope, &allocation);

    t1 = gtk_range_get_value(GTK_RANGE(widgets->env_t1)) / 2;
    l1 = gtk_range_get_value(GTK_RANGE(widgets->env_l1));
    t2 = gtk_range_get_value(GTK_RANGE(widgets->env_t2)) / 2 + t1;
    l2 = gtk_range_get_value(GTK_RANGE(widgets->env_l2));
    t3 = gtk_range_get_value(GTK_RANGE(widgets->env_t3)) / 2 + t2;
    l3 = gtk_range_get_value(GTK_RANGE(widgets->env_l3));
    t4 = gtk_range_get_value(GTK_RANGE(widgets->env_t4)) / 2;

    dashes[0] = 2;
    dashes[1] = 2;

    /* create graphics context */

    window = gtk_widget_get_window(widgets->envelope);

    cairo_region = cairo_region_create();

    drawing_context = gdk_window_begin_draw_frame(window, cairo_region);

    cairo = gdk_drawing_context_get_cairo_context(drawing_context);

    /* draw background */

    cairo_set_source_rgb(cairo, 255, 255, 255);
    cairo_rectangle(cairo, 0, 0, allocation.width, allocation.height);
    cairo_fill(cairo);

    /* draw envelope */

    cairo_set_source_rgb(cairo, 0, 0, 0);

    cairo_set_line_width(cairo, 2);
    cairo_set_line_cap(cairo, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cairo, CAIRO_LINE_JOIN_ROUND);

    cairo_move_to(cairo, 0, allocation.height);
    cairo_line_to(cairo, t1, allocation.height - l1);
    cairo_line_to(cairo, t2, allocation.height - l2);
    cairo_line_to(cairo, t3, allocation.height - l3);
    cairo_line_to(cairo, allocation.width - t4, allocation.height - l3);
    cairo_line_to(cairo, allocation.width, allocation.height);
    cairo_stroke(cairo);

    /* draw dashed lines */

    cairo_set_line_width(cairo, 1);
    cairo_set_dash(cairo, dashes, 2, 0);
    cairo_set_line_cap(cairo, CAIRO_LINE_CAP_BUTT);
    cairo_set_line_join(cairo, CAIRO_LINE_JOIN_MITER);

    cairo_move_to(cairo, t1, allocation.height - 0);
    cairo_line_to(cairo, t1, allocation.height - l1);
    cairo_stroke(cairo);

    cairo_move_to(cairo, t2, allocation.height - 0);
    cairo_line_to(cairo, t2, allocation.height - l2);
    cairo_stroke(cairo);

    cairo_move_to(cairo, t3, allocation.height - 0);
    cairo_line_to(cairo, t3, allocation.height - l3);
    cairo_stroke(cairo);

    cairo_move_to(cairo, allocation.width - t4, allocation.height - 0);
    cairo_line_to(cairo, allocation.width - t4, allocation.height - l3);
    cairo_stroke(cairo);

    /* destroy graphics context */

    gdk_window_end_draw_frame(window, drawing_context);

    cairo_region_destroy(cairo_region);
}
