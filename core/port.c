#include "port.h"
#include "debug/debug.h"

/* Global APB state */
eZ80portrange_t port_map[0x10];

#define port_range(a) (((a)>>12)&0xF) /* converts an address to a port range 0x0-0xF */

static const uint32_t port_mirrors[0x10] = {0x7F,0xFF,0xFF,0x1FF,0xFFF,0xFF,0x1F,0xFF,0x7F,0xFFF,0x7F,0xFFF,0xFF,0x7F,0x7F,0xFFF};

static uint8_t port_read(uint16_t address, bool peek) {
    uint8_t port_loc = port_range(address);
    return port_map[port_loc].read_in(address & port_mirrors[port_loc], peek);
}
uint8_t port_peek_byte(uint16_t address) {
    return port_read(address, true);
}
uint8_t port_read_byte(uint16_t address) {
#ifdef DEBUG_SUPPORT
    if (debugger.data.ports[address] & DBG_PORT_READ) {
        open_debugger(HIT_PORT_READ_WATCHPOINT, address);
    }
#endif
    return port_read(address, false);
}

static void port_write(uint16_t address, uint8_t value, bool peek) {
    uint8_t port_loc = port_range(address);
    port_map[port_loc].write_out(address & port_mirrors[port_loc], value, peek);
}
void port_poke_byte(uint16_t address, uint8_t value) {
    port_write(address, value, true);
}
void port_write_byte(uint16_t address, uint8_t value) {
#ifdef DEBUG_SUPPORT
    if (debugger.data.ports[address] & DBG_PORT_FREEZE) {
        return;
    }
    if (debugger.data.ports[address] & DBG_PORT_WRITE) {
        open_debugger(HIT_PORT_WRITE_WATCHPOINT, address);
    }
#endif
    port_write(address, value, false);
}
