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

#include<sys/ioctl.h>
#ifdef __NetBSD__
#include <sys/midiio.h>
#else
#include <linux/soundcard.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "midi.h"

static int midi_fd = -1;
static int midi_device_count = 0;
static MIDIDevice **midi_devices = NULL;
static int nchars(int);

/**
   \brief Intialises MIDI support by building a list of available MIDI devices.

   \return one on success, zero on error or if no MIDI devices can be found.
 */
int
midi_initialise(void)
{
    int fd, cnt, n;
#ifdef __NetBSD__
    char *dev = "/dev/music";
    struct synth_info info;
#else
    char *dev = "/dev/sequencer";
    struct midi_info info;
#endif
    MIDIDevice *device;

    if ((fd = open(dev, O_WRONLY)) < 0) {
        fprintf(stderr, "midi_devices(): unable to open %s: %s\n", dev, strerror(errno));
        return 0;
    }

#ifdef __NetBSD__
    if (ioctl(fd, SEQUENCER_NRMIDIS, &midi_device_count) < 0) {
#else
    if (ioctl(fd, SNDCTL_SEQ_NRMIDIS, &midi_device_count) < 0) {
#endif
        fprintf(stderr, "midi_devices(): unable to get number of devices: %s\n", strerror(errno));
        close(fd);
        return 0;
    }

    if (midi_device_count < 1) {
        fprintf(stderr, "midi_devices(): no devices found\n");
        close(fd);
        return 0;
    }

    midi_devices = calloc(midi_device_count + 1, sizeof(MIDIDevice *));

    for (cnt = info.device = 0; info.device < midi_device_count; info.device++) {
#ifdef __NetBSD__
        if (ioctl(fd, SEQUENCER_INFO, &info) < 0) {
#else
        if (ioctl(fd, SNDCTL_MIDI_INFO, &info) < 0) {
#endif
            printf("midi_devices(): unable to get info for device %d\n", info.device);
            continue;
        }
        n = nchars(info.device);
        device = malloc(sizeof(MIDIDevice));
        device->name = malloc(strlen(info.name) + n + 4);
        device->device = malloc(n + 11);
        sprintf(device->name, "%d : %s", info.device, info.name);
        sprintf(device->device, "/dev/rmidi%d", info.device);
        midi_devices[cnt] = device;
        ++cnt;
    }

    midi_device_count = cnt;

    close(fd);

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
    if (midi_fd != -1) {
        fprintf(stderr, "midi_open(): a device is already open\n");
        return 0;
    }

    midi_fd = open(device, O_RDWR|O_NONBLOCK|O_SYNC);
    if (midi_fd == -1) {
        fprintf(stderr, "midi_open(): failed to open: %s\n", strerror(errno));
        return 0;
    }

    return 1;
}

/**
   \brief Close a MIDI interface device.

   \return one on success, zero on error.
 */
int
midi_close()
{
    if (midi_fd == -1) {
        fprintf(stderr, "midi_close(): failed to close: no device open\n");
        return 0;
    }

    if (close(midi_fd) == -1) {
        fprintf(stderr, "midi_close(): failed to close: %s\n", strerror(errno));
        return 0;
    }

    midi_fd = -1;

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
    unsigned char data[3];

    if (midi_fd == -1) {
        fprintf(stderr, "midi_note_on(): failed: no device open\n");
        return 0;
    }

    data[0] = 0x90 | channel;
    data[1] = note & 0x7f;
    data[2] = velocity & 0x7f;

    if (write(midi_fd, data, 3) != 3) {
        fprintf(stderr, "midi_note_on(): failed: %s\n", strerror(errno));
        return 0;
    }

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
    unsigned char data[3];

    if (midi_fd == -1) {
        fprintf(stderr, "midi_note_off(): failed: no device open\n");
        return 0;
    }

    data[0] = 0x80 | channel;
    data[1] = note & 0x7f;
    data[2] = velocity & 0x7f;

    if (write(midi_fd, data, 3) != 3) {
        fprintf(stderr, "midi_note_off(): failed: %s\n", strerror(errno));
        return 0;
    }

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
    unsigned char data[2];

    if (midi_fd == -1) {
        fprintf(stderr, "midi_program_change(): failed: no device open\n");
        return 0;
    }

    data[0] = 0xc0 | channel;
    data[1] = program & 0x7f;

    if (write(midi_fd, data, 2) != 2) {
        fprintf(stderr, "midi_program_change(): failed: %s\n", strerror(errno));
        return 0;
    }

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
    int n, nread = 0;
    unsigned char c;

    if (midi_fd == -1) {
        fprintf(stderr, "sysex_read(): failed: no device open\n");
        return 0;
    }

    while ((n = read(midi_fd, &c, 1)) == 1 && c != 0xf0) {
        usleep(40);
    }

    if (n != 1) {
        return 0;
    }

    while (nread < length) {
        n = read(midi_fd, &c, 1);
        if (n == 1) {
            buffer[nread++] = c;
            if (autostop && buffer[nread - 1] == 0xf7) {
                break;
            }
        }
    }

    return nread;
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

    if (midi_fd == -1) {
        fprintf(stderr, "sysex_write(): failed: no device open\n");
        return 0;
    }

    data = calloc(length + 2, sizeof(unsigned char));

    data[0] = 0xf0;
    memcpy(data + 1, buffer, length);
    data[length + 1] = 0xf7;
    length += 2;

    if (write(midi_fd, data, length) != length) {
        fprintf(stderr, "sysex_write(): failed: %s\n", strerror(errno));
        free(data);
        return 0;
    }

    free(data);

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

    for (length -= 2; length > 3; length--) {
        checksum += buffer[length];
    }

    checksum %= 128;
    checksum = 128 - checksum;
    if (checksum == 128) {
        checksum = 0;
    }

    return checksum;
}

static int
nchars(int d)
{
    int i;
    for (i = d < 1 ? 1 : 0; d; ++i) {
        d /= 10;
    }
    return i;
}
