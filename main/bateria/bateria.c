#include "bateria.h"

#include "esp_adc/adc_oneshot.h"

#define BAT_ADC ADC_CHANNEL_3

static adc_oneshot_unit_handle_t adc_handle;

void bateria_init(void) {
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };

    adc_oneshot_new_unit(
        &init_config,
        &adc_handle
    );

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_12,
    };

    adc_oneshot_config_channel(
        adc_handle,
        BAT_ADC,
        &config
    );
}

float bateria_leitura(void)
{
    int raw;

    adc_oneshot_read(adc_handle, BAT_ADC, &raw);

    float tensao = ((float)raw / 4095.0) * 3.3;
    tensao *= 2.0;

    return tensao;
}

int bateria_percentual(void)
{
    float tensao = bateria_leitura();

    float percentual = ((tensao - 6.0) / (8.4 - 6.0)) * 100.0;

    if(percentual > 100)
        percentual = 100;

    if(percentual < 0)
        percentual = 0;

    return (int)percentual;
}