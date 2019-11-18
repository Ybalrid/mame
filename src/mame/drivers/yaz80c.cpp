// license:BSD-3-Clause
// copyright-holders:Arthur Brainville (Ybalrid)
/**************************************************************************

This driver is for a computer that doesn't exist yet, the yaz80c!

****************************************************************************/

#include "emu.h"
#include "includes/yaz80c.h"

void yaz80c_state::yaz80c_mem(address_map &map)
{
	map(0x0000, 0x7fff).rom();
	map(0x8000, 0xffff).ram();
}

void yaz80c_state::yaz80c_io(address_map &map)
{
	map.global_mask(0xff);
    map(0x00, 0x00).rw(m_dart, FUNC(z80dart_device::ca_r), FUNC(z80dart_device::ca_w));
    map(0x01, 0x01).rw(m_dart, FUNC(z80dart_device::da_r), FUNC(z80dart_device::da_w));
    map(0x10, 0x10).rw(m_dart, FUNC(z80dart_device::cb_r), FUNC(z80dart_device::cb_w));
    map(0x11, 0x11).rw(m_dart, FUNC(z80dart_device::db_r), FUNC(z80dart_device::db_w));
}

static void construct_ioport_yaz80c(device_t &owner, ioport_list &portlist, std::string &errorbuf)
{
	ioport_configurer configurer(owner, portlist, errorbuf);
}


void yaz80c_state::machine_start()
{
}

void yaz80c_state::machine_reset()
{
}

void yaz80c_state::serial_clock_dispatcher(int state)
{
    m_dart->rxca_w(state);
    m_dart->txca_w(state);
}
static const z80_daisy_config yaz80c_daisy_chain[] =
{
	{ DART_TAG },
	{ nullptr }
};

static DEVICE_INPUT_DEFAULTS_START(terminal)
    DEVICE_INPUT_DEFAULTS("RS232_TXBAUD", 0xff, RS232_BAUD_9600)
    DEVICE_INPUT_DEFAULTS("RS232_RXBAUD", 0xff, RS232_BAUD_9600)
    DEVICE_INPUT_DEFAULTS("RS232_STARTBITS", 0xff, RS232_STARTBITS_1)
    DEVICE_INPUT_DEFAULTS("RS232_DATABITS", 0xff, RS232_DATABITS_8)
    DEVICE_INPUT_DEFAULTS("RS232_PARITY", 0xff, RS232_PARITY_NONE)
    DEVICE_INPUT_DEFAULTS("RS232_STOPBITS", 0xff, RS232_STOPBITS_1)
DEVICE_INPUT_DEFAULTS_END


void yaz80c_state::yaz80c(machine_config& config)
{
    Z80(config, m_cpu, 4_MHz_XTAL); //*
    m_cpu->set_addrmap(AS_PROGRAM, &yaz80c_state::yaz80c_mem);
    m_cpu->set_addrmap(AS_IO, &yaz80c_state::yaz80c_io);
    m_cpu->set_daisy_config(yaz80c_daisy_chain);

    Z80DART(config, m_dart, 4_MHz_XTAL); //*
    m_dart->out_txda_callback().set(m_rs232, FUNC(rs232_port_device::write_txd)); //?
    m_dart->out_dtra_callback().set(m_rs232, FUNC(rs232_port_device::write_dtr)); //?
    m_dart->out_rtsa_callback().set(m_rs232, FUNC(rs232_port_device::write_rts)); //?

    //m_dart->out_synca_callback().set(FUNC(yaz80c_state::synca_w));
    m_dart->out_int_callback().set_inputline(m_cpu, INPUT_LINE_IRQ0);

    RS232_PORT(config, m_rs232, default_rs232_devices, "terminal");
    m_rs232->rxd_handler().set(m_dart, FUNC(z80dart_device::rxa_w));
    m_rs232->dcd_handler().set(m_dart, FUNC(z80dart_device::dcda_w));
    m_rs232->cts_handler().set(m_dart, FUNC(z80dart_device::ctsa_w));
    m_rs232->set_option_device_input_defaults("terminal", DEVICE_INPUT_DEFAULTS_NAME(terminal));

    CLOCK(config, m_serial_clock, 9'600 * 16);
    m_serial_clock->signal_handler().set(FUNC(yaz80c_state::serial_clock_dispatcher));
}

ROM_START(yaz80c)
ROM_REGION(0x8000, CPU_TAG, 0)
ROM_LOAD("yaz80c.bin", 0x0000, 0x8000, NO_DUMP) //NO_DUMP because that's not a board where we dumped some ROM and know the checksum, we are making it as we go!
ROM_END

//    YEAR  NAME      PARENT  COMPAT  MACHINE   INPUT     CLASS           INIT        COMPANY      FULLNAME        FLAGS
COMP(2019, yaz80c, 0, 0, yaz80c, yaz80c, yaz80c_state, empty_init, "Ybalrid", "yaz80c", MACHINE_CLICKABLE_ARTWORK)
