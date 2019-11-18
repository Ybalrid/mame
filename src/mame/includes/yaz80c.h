// license:BSD-3-Clause
// copyright-holders:Arthur Brainville (Ybalrid)
// (this driver is a modified verdartn of the tranz330 from Ryan Holtz)
// copyright-holders:Ryan Holtz

#ifndef MAME_INCLUDES_yaz80c_H
#define MAME_INCLUDES_yaz80c_H

#pragma once

#include "cpu/z80/z80.h"
#include "machine/z80daisy.h"
#include "machine/z80dart.h"
#include "bus/rs232/rs232.h"
#include "machine/clock.h"

#define CPU_TAG     "cpu"
#define DART_TAG    "dart"
#define CTC_TAG     "ctc"
#define PIO_TAG     "pio"
#define RTC_TAG     "rtc"
#define VFD_TAG     "vfd"
#define RS232_TAG   "rs232"
#define SPEAKER_TAG "speaker"
#define SCLK_TAG "sclk"
class yaz80c_state : public driver_device
{
public:
    yaz80c_state(const machine_config& mconfig, device_type type, const char* tag)
        : driver_device(mconfig, type, tag)
        , m_cpu(*this, CPU_TAG)
        , m_dart(*this, DART_TAG)
        , m_rs232(*this, RS232_TAG)
        , m_serial_clock(*this, SCLK_TAG)

	{ }

	void yaz80c(machine_config &config);

private:
	virtual void machine_start() override;
	virtual void machine_reset() override;

	DECLARE_WRITE_LINE_MEMBER(serial_clock_dispatcher);

	void yaz80c_mem(address_map &map);
	void yaz80c_io(address_map &map);

	required_device<z80_device>             m_cpu;
	required_device<z80dart_device>         m_dart;
	required_device<rs232_port_device>      m_rs232;
    required_device<clock_device> m_serial_clock;

};

#endif // VERIFONE_yaz80c_H
