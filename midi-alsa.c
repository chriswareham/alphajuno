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

#include <alsa/asoundlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "midi.h"

static int midi_device_count = 0;
static MIDIDevice **midi_devices = NULL;

static snd_seq_t *alsa_client = NULL;
static int alsa_port = -1;

snd_seq_t *
create_alsa_client(void)
{
    snd_seq_t *handle;
    int err;
    err = snd_seq_open(&handle, "default", SND_SEQ_OPEN_DUPLEX, 0);
    if (err < 0) {
        return NULL;
    }
    snd_seq_set_client_name(handle, "Alpha Juno Editor");
    return handle;
}

int
create_alsa_port(snd_seq_t *handle)
{
    return snd_seq_create_simple_port(
        handle,
        "Alpha Juno Editor",
        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC
    );
}

/**
   \brief Intialises MIDI support by building a list of available MIDI devices.

   \return one on success, zero on error or if no MIDI devices can be found.
 */
int
midi_initialise(void)
{
    alsa_client = create_alsa_client();
    if (!alsa_client) {
        fprintf(stderr, "unable to create alsa client\n");
        return 0;
    }

    alsa_port = create_alsa_port(alsa_client);

    midi_device_count = 1;
    midi_devices = calloc(2, sizeof(MIDIDevice *));
    MIDIDevice *device;
    device = malloc(sizeof(MIDIDevice));
    device->name = "Alpha Juno Editor MIDI OUT";
    device->device = "/dev/null";
    midi_devices[0] = device;

    return 1;
}

/**
   \brief Returns the number of available MIDI interface devices.

   \return the number of available MIDI interface devices.
 */
int
midi_get_device_count(void)
{
    if (midi_device_count < 1) {
        fprintf(stderr, "midi_get_devices(): MIDI support not initialised\n");
    }

    return midi_device_count;
}

/**
   \brief Returns a list of available MIDI interface devices.

   \return the NULL terminated list of available MIDI interface devices.
 */
MIDIDevice **
midi_get_devices(void)
{
    if (midi_device_count < 1) {
        fprintf(stderr, "midi_get_devices(): MIDI support not initialised\n");
    }

    return midi_devices;
}

/**
   \brief Open a MIDI interface device.

   \param device - filename of the MIDI interface device to open.
   \return one on success, zero on error.
 */
int
midi_open(const char *device)
{
    return 1;
}

/**
   \brief Close a MIDI interface device.

   \return one on success, zero on error.
 */
int
midi_close()
{
    return 1;
}

/**
   \brief Transmit a MIDI Note On message.

   \param channel - MIDI channel.
   \param note - note number.
   \param velocity - note velocity.
   \return one on success, zero on error.
 */
int
midi_note_on(unsigned char channel, unsigned char note, unsigned char velocity)
{
    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, alsa_port);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    snd_seq_ev_set_noteon(&ev, channel, note, velocity);
    snd_seq_event_output(alsa_client, &ev);
    snd_seq_drain_output(alsa_client);

    return 1;
}

/**
   \brief Transmit a MIDI Note Off message.

   \param channel - MIDI channel.
   \param note - note number.
   \param velocity - note velocity.
   \return one on success, zero on error.
 */
int
midi_note_off(unsigned char channel, unsigned char note, unsigned char velocity)
{
    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, alsa_port);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    snd_seq_ev_set_noteoff(&ev, channel, note, velocity);
    snd_seq_event_output(alsa_client, &ev);
    snd_seq_drain_output(alsa_client);

    return 1;
}

/**
   \brief Transmit a MIDI Program Change message.

   \param channel - MIDI channel.
   \param program - program number.
   \return one on success, zero on error.
 */
int
midi_program_change(unsigned char channel, unsigned char program)
{
    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, alsa_port);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    snd_seq_ev_set_pgmchange(&ev, channel, program);
    snd_seq_event_output(alsa_client, &ev);
    snd_seq_drain_output(alsa_client);

    return 1;
}

/**
   \brief Read a SysEx message from a MIDI interface device.

   \param buffer - SysEx buffer to read into.
   \param length - SysEx buffer length in bytes.
   \param autostop - whether to read short if a boundary byte is read.
   \return the number of bytes read.
 */
int
sysex_read(unsigned char *buffer, unsigned length, int autostop)
{
    return 0;
}

/**
   \brief Write a SysEx message to a MIDI interface device.

   \param message - SysEx buffer to write (minus boundary bytes).
   \param length - SysEx buffer length in bytes.
   \return one on success, zero on error.
 */
int
sysex_write(unsigned char *buffer, unsigned length)
{
    unsigned char *data;
    data = calloc(length + 2, sizeof(unsigned char));

    data[0] = 0xf0;
    memcpy(data + 1, buffer, length);
    data[length + 1] = 0xf7;
    length += 2;

    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, alsa_port);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    snd_seq_ev_set_sysex(&ev, length, data);
    snd_seq_event_output(alsa_client, &ev);
    snd_seq_drain_output(alsa_client);

    free(data);

    printf("sent sysex\n");

    return 1;
}

/**
   \brief Calculates a Roland checksum for a SysEx message.

   \param message - SysEx buffer (minus boundary bytes).
   \param length - SysEx buffer length in bytes.
   \return the checksum byte, or zero on error.
 */
unsigned char
roland_checksum(unsigned char *buffer, unsigned length)
{
    unsigned checksum = 0;

    if (length < 9) {
        fprintf(stderr, "roland_checksum() failed: buffer too short\n");
        return 0;
    }

    for (length -= 2; length > 3; --length) {
        checksum += buffer[length];
    }

    checksum %= 128;
    checksum = 128 - checksum;
    if (checksum == 128) {
        checksum = 0;
    }

    return checksum;
}
