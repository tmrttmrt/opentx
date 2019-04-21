# ESP-IDF OTA server component

A component for ESP-IDF that adds OTA capability for your project easily.

See also: https://github.com/yanbe/esp-idf-ota-template

## Usage

This repository is served as a "component" for ESP-IDF project so you can add OTA capability to your existing project easily.

```sh
$ cd path/to/your/esp-idf-project
$ git remote add esp32-ota-server https://github.com/yanbe/esp32-ota-server.git
$ git subtree add --prefix components/ota_server esp32-ota-server master --squash
$ make menuconfig
```

In `make menuconfig`,

* In Partition Table: select "Factory app, two OTA definitions"
* In Wifi Configuration section: set your Wifi access point's SSID / password

### Integrate OTA capability to your project

You have to add small portion of code to integrate OTA capability to your project.

OTA server have to started after ESP32 joined to WiFi network. So you have to
implement event handler (if not yet) and then emit `CONNECTED_BIT` event
which `ota_server_task` waits for.

In your main/main.c,

```c
#include "ota_server.h"

static EventGroupHandle_t wifi_event_group;

static const int CONNECTED_BIT = BIT0;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    /* ... */
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    /* ... */
    }
    return ESP_OK;
}

static void initialise_wifi(void)
{
/* snip. wifi initialization code... */
}


static void ota_server_task(void * param)
{
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
    ota_server_start();
    vTaskDelete(NULL);
}


void app_main() {
    initialise_wifi();
    xTaskCreate(&ota_server_task, "ota_server_task", 4096, NULL, 5, NULL);
}

```

### Inital flashing and OTA

To enable OTA capability, your have to configure partition table and flash your code via UART once.

```sh
$ make erase_flash flash
```

Then, via serial output, confirm which IP address is assigned to a ESP32. It should be something like below.

```sh
$ make monitor
...
I (4086) event: ip: 192.168.10.10, mask: 255.255.255.0, gw: 192.168.10.1
```

After that, you can use flashing capability via special make target.

```sh
$ cp components/ota-server/Makefile.example Makefile # For additional make target "ota"
$ make ota ESP32_IP=192.168.10.10
200 OK

Success. Next boot partition is ota_0
```

### Modifing and Updating OTA server component

As `components/ota_server` is introduced as via `git subtree` command, your can modify `components/ota_server`
and commit it as your project's seamlessly,

If you want to merge any changes have made to https://github.com/yanbe/esp32-ota-server ,
you can merge them into your project by following command.
```
$ git pull -s subtree --squash --allow-unrelated-histories esp32-ota-server master
```
