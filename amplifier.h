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

#ifndef AMPLIFIER_H
#define AMPLIFIER_H

typedef struct {
    GtkWindow *dialog;
    GtkScale *vca_level;
    GtkComboBox *vca_env_mode;
    GtkScale *vca_aftertouch_sensitivity;
    GtkCheckButton *chorus;
    GtkScale *chorus_rate;
    GtkScale *lfo_rate;
    GtkScale *lfo_delay;
} AmplifierDialog;

AmplifierDialog *new_amplifier_dialog(GtkWindow *);
void show_amplifier_dialog(AmplifierDialog *);
void set_amplifier_parameters(AmplifierDialog *, Patch *);
void clear_amplifier_parameters(AmplifierDialog *);

#endif /* !AMPLIFIER_H */
