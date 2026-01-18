#include <keypadc.h>
#include <graphx.h>
#include <tice.h>
#include <usbdrvce.h>
#include "controller.h"
#include "emu.h"

int events[40];

bool started_transfer;

bool has_device;
usb_device_t current_device;

usb_endpoint_t control_endpoint;

uint16_t status;
size_t bytes_transfered;

int error;

usb_error_t configure(usb_device_t device) {

    usb_error_t err = USB_SUCCESS;

    current_device = device;

    // usb_GetDeviceDescriptor(device, &device_descriptor, 0x12, NULL);
    size_t length = usb_GetConfigurationDescriptorTotalLength(device, 0);

    if (length == 0)
        status = 69;

    usb_configuration_descriptor_t* conf_descriptor = static_cast<usb_configuration_descriptor_t*>(malloc(length));

    err = usb_GetConfigurationDescriptor(device, 0, conf_descriptor, length, &bytes_transfered);
    usb_SetConfiguration(device, conf_descriptor, 49);

    free(conf_descriptor);

    control_endpoint = usb_GetDeviceEndpoint(device, 0x81);
    has_device = true;

    if (err != USB_SUCCESS)
        return err;
    err = usb_Transfer(usb_GetDeviceEndpoint(device, 0x01), (uint8_t[]){1, 3, 2}, 3, 1, nullptr);

    return err;

}


usb_error_t event_handler(usb_event_t event, void* event_data, void* user_data) {

    usb_error_t err = USB_SUCCESS;
    
    if (event < 0 || event > 39) { // no event
        return err;
    }

    events[event] = 63 * 2;


    switch (event) {

        case USB_DEVICE_ENABLED_EVENT:

            err = configure(static_cast<usb_device_t>(event_data));        

            break;

        case USB_DEVICE_CONNECTED_EVENT:

            // not handling multiple devices

            has_device = true;

            if (!(usb_GetRole() & USB_ROLE_DEVICE))
                err = usb_ResetDevice(static_cast<usb_device_t>(event_data));

            break;

        case USB_DEVICE_DISABLED_EVENT:
        case USB_DEVICE_DISCONNECTED_EVENT:

            has_device = false;

            break;

        default:
            break;
    }

    return err;

}

usb_error_t transfer_handler(usb_endpoint_t _endpoint, usb_transfer_status_t _status, size_t _transferred, void *_data) {
    // gfx_SetTextXY(20, 20);
    // gfx_PrintString("Transfer handler: ");
    // gfx_PrintInt(transferred, 1);
    // gfx_Blit(gfx_screen);
    // gfx_SetTextFGColor(40);
    // gfx_SetTextBGColor(255);
    // gfx_SetTextXY(10, 100);
    // gfx_PrintString("xbc a");
    // gfx_SwapDraw();
    started_transfer = false;
    return USB_SUCCESS;
}

int main(void)
{
    usb_Init(&event_handler, nullptr, nullptr, USB_DEFAULT_INIT_FLAGS);
    started_transfer = false;
    xbc_packet = {};
    gfx_Begin();
    init();
    // timer_Enable(1, TIMER_32K, TIMER_NOINT, TIMER_UP);
    do {
        usb_HandleEvents();
        if (!has_device)
            continue;
        // error = usb_Transfer(control_endpoint, &xbc_packet, sizeof(xbc_control_packet_t), 0, nullptr);
        if (!started_transfer) {
            error = usb_ScheduleTransfer(control_endpoint, &xbc_packet, sizeof(xbc_control_packet_t), transfer_handler, nullptr);
            if (error == USB_SUCCESS)
                started_transfer = true;
        }
        update();
        if (false && xbc_packet.digital_buttons & XBC_A) {
            // gfx_Blit(gfx_screen);
            // gfx_SetTextXY(10, 100);
            // gfx_PrintString("xbc a");
            // gfx_SwapDraw();
            gfx_End();
            usb_Cleanup();
            return 0;
        }
    } while(!kb_IsDown(kb_KeyClear) && !kb_IsDown(kb_KeyDel));
    if(!kb_IsDown(kb_KeyDel)) {
        save_game();
    }
    gfx_End();
    usb_Cleanup();
    return 0;
}
