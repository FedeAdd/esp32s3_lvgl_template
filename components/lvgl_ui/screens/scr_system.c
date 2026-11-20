#include "scr_system.h"
#include "screens.h"
#include "nav_bar.h"
#include "ui.h"     /* ui_scrSystem, ui_lblCpu, ui_lblHeap, ui_lblHeapMin,
                       ui_lblIdf, ui_lblChip, ui_lblFlash */
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_private/esp_clk.h"
#include "esp_log.h"

static const char *TAG = "scr_system";

#define SYSTEM_REFRESH_MS   1000

static lv_timer_t *s_refresh_timer = NULL;

/* Own minimum tracker: esp_get_minimum_free_heap_size() can't be reset,
 * so we keep our own value that the physical button can reset. */
static uint32_t s_heap_min = UINT32_MAX;

/* ------------------------------------------------------------------ */

static void refresh_dynamic(void)
{
    uint32_t heap = esp_get_free_heap_size();
    if (heap < s_heap_min) {
        s_heap_min = heap;
    }

    lv_label_set_text_fmt(ui_lblCpu, "CPU: %d MHz", (int)(esp_clk_cpu_freq() / 1000000));
    lv_label_set_text_fmt(ui_lblHeap, "Heap: %lu KB", (unsigned long)(heap / 1024));
    lv_label_set_text_fmt(ui_lblHeapMin, "Heap min: %lu KB", (unsigned long)(s_heap_min / 1024));
}

static void refresh_static(void)
{
    esp_chip_info_t info;
    esp_chip_info(&info);
    const char *model = (info.model == CHIP_ESP32S3) ? "ESP32-S3" : "ESP32";
    lv_label_set_text_fmt(ui_lblChip, "Chip: %s rev %d.%d, %d cores", model,
                          info.revision / 100, info.revision % 100, info.cores);

    lv_label_set_text_fmt(ui_lblIdf, "IDF: %s", esp_get_idf_version());

    uint32_t flash_size = 0;
    if (esp_flash_get_size(NULL, &flash_size) == ESP_OK) {
        lv_label_set_text_fmt(ui_lblFlash, "Flash: %lu MB",
                              (unsigned long)(flash_size / (1024 * 1024)));
    } else {
        lv_label_set_text(ui_lblFlash, "Flash: --");
    }
}

static void refresh_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    refresh_dynamic();
}

/* ------------------------------------------------------------------ */

void scr_system_prepare(void)
{
    nav_enable_swipe(ui_scrSystem);
}

void scr_system_init(void)
{
    nav_bar_set_active(SCREEN_SYSTEM);
    refresh_static();
    refresh_dynamic();
    s_refresh_timer = lv_timer_create(refresh_timer_cb, SYSTEM_REFRESH_MS, NULL);
}

void scr_system_uninit(void)
{
    if (s_refresh_timer) {
        lv_timer_del(s_refresh_timer);
        s_refresh_timer = NULL;
    }
}

void scr_system_step(void)
{
}

void scr_system_on_button(void)
{
    ESP_LOGI(TAG, "button: reset heap minimum");
    s_heap_min = UINT32_MAX;
    refresh_dynamic();
}
