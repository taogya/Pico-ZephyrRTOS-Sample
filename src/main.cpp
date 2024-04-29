
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <string.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/pwm.h>

static const struct adc_dt_spec vr_dt = ADC_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user), vr0);
static const struct pwm_dt_spec on_board_led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));
static const struct pwm_dt_spec add_led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led1));

void init_pwm()
{
	pwm_is_ready_dt(&on_board_led);
	pwm_is_ready_dt(&add_led);
}

void init_usb()
{
	const struct device *usb_device = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
	uint32_t dtr = 0;
	usb_enable(NULL);
	while (!dtr)
	{
		uart_line_ctrl_get(usb_device, UART_LINE_CTRL_DTR, &dtr);
		k_sleep(K_MSEC(100));
	}
	printk("USB ready\n");
}

void init_adc(void)
{
	adc_is_ready_dt(&vr_dt);
	adc_channel_setup_dt(&vr_dt);
	printk("ADC ready\n");
}

void update_adc_thread(void *p1, void *p2, void *p3)
{
	uint16_t buf;
	int32_t vref_mv = (int32_t)adc_ref_internal(vr_dt.dev);
	struct adc_sequence sequence = {
		.buffer = &buf,
		.buffer_size = sizeof(buf),
	};
	uint32_t max_period = PWM_MSEC(5U);
	int32_t pwm = 0;
	int32_t duty = 0;

	(void)adc_sequence_init_dt(&vr_dt, &sequence);

	while (1)
	{
		int32_t val_mv;
		adc_read_dt(&vr_dt, &sequence);
		val_mv = (int32_t)buf;
		adc_raw_to_millivolts_dt(&vr_dt, &val_mv);
		pwm = (val_mv * 100) / vref_mv;
		duty = max_period * pwm / 100;
		pwm_set_dt(&on_board_led, max_period, duty);
		pwm_set_dt(&add_led, max_period, duty);
		printk("%" PRId32 " %\n", pwm);

		k_sleep(K_MSEC(10));
	}
}

int main(void)
{
	init_usb();
	init_adc();
	init_pwm();
	update_adc_thread(NULL, NULL, NULL);

	return 0;
}

// K_THREAD_DEFINE(adc_tid, 50,
// 				update_adc_thread, NULL, NULL, NULL,
// 				5, 0, 0);