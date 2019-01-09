
#ifndef  __PLAT_HX7097_LCOS_H__
#define __PLAT_HX7097_LCOS_H__


struct lcos_i2c_platform_data {
	uint32_t gpio_vdda;		// vdda support
	uint32_t gpio_vddd;      	// vdda support
	uint32_t gpio_reset;		// Reset support
	uint32_t gpio_light;      	// vdda support
};

#endif	// __PLAT_HX7097_LCOS_H__

