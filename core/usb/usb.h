#ifndef H_USB
#define H_USB

#ifdef __cplusplus
extern "C" {
#endif

#include "../port.h"
#include "fotg210.h"

/* Standard USB state */
PACK(typedef struct usb_state {
    struct fotg210_regs regs;
}) usb_state_t;

/* Global GPT state */
extern usb_state_t usb;

/* Available Functions */
eZ80portrange_t init_usb(void);
void usb_reset(void);

/* Save/Restore */
typedef struct emu_image emu_image;
bool usb_restore(const emu_image*);
bool usb_save(emu_image*);

#ifdef __cplusplus
}
#endif

#endif
