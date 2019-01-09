#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/miscdevice.h>
#include <plat/gpio-cfg.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/io.h>
#include <mach/gpio.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <plat/clock.h>
#include <plat/gpio-cfg.h>
#include <plat/cpu.h>

#include "lcos.h"

//IIC MAX FRE----100 KHz

//MD_RSTN----GPC0_2    -----gpio_reset
//GPX0_0   ----GPX0_0      -----gpio_vdda
//CAM5M_RST ----GPL1_1 ----gpio_vddd
//XPWMTOUT1 ---GPD0_1 ----gpio_light

//#ifdef CONFIG_LCOS_HX7097N
//static struct lcos_i2c_platform_data hx7097n_pdata= {
//        .gpio_vdda        = EXYNOS4_GPX0(0),
//        .gpio_vddd        = EXYNOS4_GPL1(1),
//        .gpio_reset        = EXYNOS4_GPC0(2),
//        .gpio_light         = EXYNOS4_GPD0(1),
//};
//#endif
//
////mach-itop4412 --I2C_SDA7 / I2C_SCL7
//static struct i2c_board_info i2c_devs7[] __initdata = {
//
//#if defined(CONFIG_LCOS_HX7097N)
//        {
//                I2C_BOARD_INFO("lcos_hx7097", 0x90>>1),
//                .platform_data = &hx7097n_pdata,
//        },
//#endif
//
//};


#define LCOS_DRIVER				 "lcos_hx7097"
#define LCOS_I2C_ADDR			  0x90


#define LCOS_INTER_SET      0x00
#define LCOS_DISPLAY_DIR    0x01
#define LCOS_PANEL_POWER    0x02

#define LCOS_VCOM_SET_L     0x03
#define LCOS_VCOM_SET_H     0x04

#define LCOS_VCSTN_SET_L    0x05
#define LCOS_VCSTN_SET_H    0X06

#define LCOS_VRING_SET_L    0X07
#define LCOS_VRING_SET_H    0X08


#define LCOS_GMA_RGMAP1  0x09
#define LCOS_GMA_RGMAP2  0x0A
#define LCOS_GMA_RGMAP3  0x0B 
#define LCOS_GMA_RGMAP4  0x0C
#define LCOS_GMA_RGMAP5  0x0D
#define LCOS_GMA_RGMAP6  0x0E
#define LCOS_GMA_RGMAN1  0x0F
#define LCOS_GMA_RGMAN2  0x10
#define LCOS_GMA_RGMAN3  0x11
#define LCOS_GMA_RGMAN4  0x12
#define LCOS_GMA_RGMAN5  0x13
#define LCOS_GMA_RGMAN6  0x14
#define LCOS_GMA_GGMAP1  0x15
#define LCOS_GMA_GGMAP2  0x16
#define LCOS_GMA_GGMAP3  0x17 
#define LCOS_GMA_GGMAP4  0x18
#define LCOS_GMA_GGMAP5  0x19
#define LCOS_GMA_GGMAP6  0x1A  
#define LCOS_GMA_GGMAN1  0x1B
#define LCOS_GMA_GGMAN2  0x1C
#define LCOS_GMA_GGMAN3  0x1D
#define LCOS_GMA_GGMAN4  0x1E
#define LCOS_GMA_GGMAN5  0x1F
#define LCOS_GMA_GGMAN6  0x20
#define LCOS_GMA_BGMAP1  0x21
#define LCOS_GMA_BGMAP2  0x22
#define LCOS_GMA_BGMAP3  0x23
#define LCOS_GMA_BGMAP4  0x24
#define LCOS_GMA_BGMAP5  0x25
#define LCOS_GMA_BGMAP6  0x26
#define LCOS_GMA_BGMAN1  0x27
#define LCOS_GMA_BGMAN2  0x28
#define LCOS_GMA_BGMAN3  0x29
#define LCOS_GMA_BGMAN4  0x2A
#define LCOS_GMA_BGMAN5  0x2B
#define LCOS_GMA_BGMAN6  0x2C

#define LCOS_COMD_POWERON   0x100001
#define LCOS_COMD_POWEROFF  0x100002


struct lcos_data {
	struct i2c_client * lcos_client;
	struct miscdevice * lcos_miscdev;
  struct lcos_i2c_platform_data  gpio_platdata;
};
static struct lcos_data *g_lcos_data;


static int  lcos_reg_init(struct i2c_client *client)
{
	int result;

	result = i2c_smbus_write_byte_data(client, LCOS_INTER_SET, 0x05);
	if (result < 0)
		goto out;
	result = i2c_smbus_write_byte_data(client, LCOS_DISPLAY_DIR, 0x03);
	if (result < 0)
		goto out;
	result = i2c_smbus_write_byte_data(client, LCOS_VCOM_SET_L, 0x01);
	if (result < 0)
		goto out;
	result = i2c_smbus_write_byte_data(client, LCOS_VCOM_SET_H, 0x0A);
	if (result < 0)
		goto out;
	result = i2c_smbus_write_byte_data(client, LCOS_VCSTN_SET_L,0x01);
	if (result < 0)
		goto out;			
  result = i2c_smbus_write_byte_data(client, LCOS_VCSTN_SET_H,0x0A);
	if (result < 0)
		goto out;
		
	result = i2c_smbus_write_byte_data(client, LCOS_VRING_SET_L,0xFA);
	if (result < 0)
		goto out;
	result = i2c_smbus_write_byte_data(client, LCOS_VRING_SET_H,0x05);
	if (result < 0)
		goto out;

	
	result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAP1,0xFF);
	if (result < 0)
		goto out;
	result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAP2,0xB0);
	if (result < 0)
		goto out;
	result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAP3,0x70);
	if (result < 0)
		goto out;
		
	result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAP4,0x40);
	if (result < 0)
		goto out;	
		result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAP5,0x0C);
	if (result < 0)
		goto out;
  	result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAP6,0x0A);
	if (result < 0)
		goto out;
			
		result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAN1,0x00);
	if (result < 0)
		goto out;
		result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAN2,0x4E);
	if (result < 0)
		goto out;
			result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAN3,0x8F);
	if (result < 0)
		goto out;
			result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAN4,0XBF);
	if (result < 0)
		goto out;
			result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAN5,0XF3);
	if (result < 0)
		goto out;
			result = i2c_smbus_write_byte_data(client, LCOS_GMA_RGMAN6,0xFF);
	if (result < 0)
		goto out;
		
		
			result = i2c_smbus_write_byte_data(client, LCOS_GMA_GGMAP1,0xFF);
	if (result < 0)
		goto out;	
			result = i2c_smbus_write_byte_data(client, LCOS_GMA_GGMAP2,0xB0);
	if (result < 0)
		goto out;
			result = i2c_smbus_write_byte_data(client, LCOS_GMA_GGMAP3,0x70);
	if (result < 0)
		goto out;
		
		  result = i2c_smbus_write_byte_data(client, LCOS_GMA_GGMAP4,0x40);
	if (result < 0)
		goto out;
		  result = i2c_smbus_write_byte_data(client, LCOS_GMA_GGMAP5,0x0C);
	if (result < 0)
		goto out;
			  result = i2c_smbus_write_byte_data(client, LCOS_GMA_GGMAP6,0x0A);
	if (result < 0)
		goto out;
		
		
		  result = i2c_smbus_write_byte_data(client, LCOS_GMA_GGMAN1,0x00);
	if (result < 0)
		goto out;
			  result = i2c_smbus_write_byte_data(client, LCOS_GMA_GGMAN2,0x4E);
	if (result < 0)
		goto out;
		  result = i2c_smbus_write_byte_data(client, LCOS_GMA_GGMAN4,0x8F);
	if (result < 0)
		goto out;
	
	
		result = i2c_smbus_write_byte_data(client, LCOS_GMA_GGMAN5,0xBF);
	if (result < 0)
		goto out;
		  result = i2c_smbus_write_byte_data(client, LCOS_GMA_GGMAN6,0XF3);
	if (result < 0)
		goto out;
	
			  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAP6,0xFF);
	if (result < 0)
		goto out;
			
		  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAP1,0xFF);
	if (result < 0)
		goto out;
			  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAP2,0xB0);
	if (result < 0)
		goto out;
		  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAP3,0x70);
	if (result < 0)
		goto out;
		
			  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAP4,0x40);
	if (result < 0)
		goto out;
			  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAP5,0x0C);
	if (result < 0)
		goto out;
		  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAP6,0x0A);
	if (result < 0)
		goto out;
		
			  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAN1,0x00);
	if (result < 0)
		goto out;
			  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAN2,0x4E);
	if (result < 0)
		goto out;
		  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAN3,0x8F);
	if (result < 0)
		goto out;
		
	  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAN4,0xBF);
	if (result < 0)
		goto out;
			  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAN5,0XF3);
	if (result < 0)
		goto out;
		  result = i2c_smbus_write_byte_data(client, LCOS_GMA_BGMAN6,0xFF);
	if (result < 0)
		goto out;
			
	return 0;
out:
	dev_err(&client->dev, "Error when init lcos device:(%d)", result);
	return result;
}
static int lcos_sc_open(struct i2c_client *client)
{
	int result;
	 result = i2c_smbus_write_byte_data(client, LCOS_PANEL_POWER,0x0C);
	if (result < 0)
		goto out;
	return result;
	out:
		return -1;
}


static int lcos_sc_close(struct i2c_client *client)
{
	int result;
	 result = i2c_smbus_write_byte_data(client, LCOS_PANEL_POWER,0x04);
	if (result < 0)
		goto out;
	return result;
	out:
		return -1;
	
}

static int lcos_power_on(void)
{
	int result;
       gpio_set_value(g_lcos_data->gpio_platdata.gpio_vddd, 1);  
	 mdelay(5); 
	 gpio_set_value(g_lcos_data->gpio_platdata.gpio_reset, 1);
	 mdelay(5); 
   	result=lcos_reg_init(g_lcos_data->lcos_client);
	if(result<0)
       {
		goto err;
	}
	mdelay(5); 
       gpio_set_value(g_lcos_data->gpio_platdata.gpio_vdda, 1);  
	mdelay(5); 

	lcos_sc_open(g_lcos_data->lcos_client);
	mdelay(5); 
 	lcos_sc_close(g_lcos_data->lcos_client);
	mdelay(5); 
	gpio_set_value(g_lcos_data->gpio_platdata.gpio_light, 1);  
	return 0;
	err:
	    return -1;

}



static int lcos_power_off(struct i2c_client *client)
{
	gpio_set_value(g_lcos_data->gpio_platdata.gpio_light, 0);  
	lcos_sc_open(g_lcos_data->lcos_client);
	mdelay(5); 
	gpio_set_value(g_lcos_data->gpio_platdata.gpio_vdda, 0);  
	mdelay(5); 
	gpio_set_value(g_lcos_data->gpio_platdata.gpio_vddd, 0);  
	return 0;
}


static int lcos_gpio_init(void)
{
	int err = 0;
	// .gpio_vdda        = EXYNOS4_GPX0(0),
        //.gpio_vddd        = EXYNOS4_GPL1(1),
       // .gpio_reset        = EXYNOS4_GPC0(2),
       // .gpio_light         = EXYNOS4_GPD0(1),

	if(g_lcos_data->gpio_platdata.gpio_vdda&&g_lcos_data->gpio_platdata.gpio_vddd&&g_lcos_data->gpio_platdata.gpio_reset&&g_lcos_data->gpio_platdata.gpio_light)
	{
		err = gpio_request_one(g_lcos_data->gpio_platdata.gpio_vdda, GPIOF_OUT_INIT_LOW, "HX7097_VDDA");
		if (err)
			{
				printk("failed to request gpio_vdda[EXYNOS4_GPX0(0)] for LCOS reset control\n");
				goto gpio_err;
			}
			
		s3c_gpio_setpull(g_lcos_data->gpio_platdata.gpio_vdda, S3C_GPIO_PULL_UP);

		err = gpio_request_one(g_lcos_data->gpio_platdata.gpio_vddd, GPIOF_OUT_INIT_LOW, "HX7097_VDDD");
		if (err)
			{
				printk("failed to request gpio_vddd[ EXYNOS4_GPL1(1)] for LCOS reset control\n");
				goto gpio_err;
			}
			
		s3c_gpio_setpull(g_lcos_data->gpio_platdata.gpio_vddd, S3C_GPIO_PULL_UP);
		
		err = gpio_request_one(g_lcos_data->gpio_platdata.gpio_reset, GPIOF_OUT_INIT_LOW, "HX7097_RESET");
				if (err)
					{
						printk("failed to request gpio_reset[EXYNOS4_GPC0(2)] for LCOS reset control\n");
						goto gpio_err;
					}
					
		s3c_gpio_setpull(g_lcos_data->gpio_platdata.gpio_reset, S3C_GPIO_PULL_UP);

		
		err = gpio_request_one(g_lcos_data->gpio_platdata.gpio_light, GPIOF_OUT_INIT_LOW, "HX7097_LIGHT");
				if (err)
					{
						printk("failed to request gpio_light[EXYNOS4_GPD0(1)] for LCOS reset control\n");
						goto gpio_err;
					}
					
		s3c_gpio_setpull(g_lcos_data->gpio_platdata.gpio_light, S3C_GPIO_PULL_UP);

		
	}
	
	return 0;
	gpio_err:
		return -1;
		
}

static int lcos_gpio_realse(void)
{
	// .gpio_vdda        = EXYNOS4_GPX0(0),
        //.gpio_vddd        = EXYNOS4_GPL1(1),
       // .gpio_reset        = EXYNOS4_GPC0(2),
       // .gpio_light         = EXYNOS4_GPD0(1),

	if(g_lcos_data->gpio_platdata.gpio_vdda)
	{
		gpio_free(g_lcos_data->gpio_platdata.gpio_vdda);
		
	}
	if(g_lcos_data->gpio_platdata.gpio_vddd)
	{
		gpio_free(g_lcos_data->gpio_platdata.gpio_vddd);
		
	}
	if(g_lcos_data->gpio_platdata.gpio_reset)
	{
		gpio_free(g_lcos_data->gpio_platdata.gpio_reset);
		
	}
	if(g_lcos_data->gpio_platdata.gpio_light)
	{
		gpio_free(g_lcos_data->gpio_platdata.gpio_light);
		
	}
	return 0;
}

static int lcos_mode_open(struct inode *inode, struct file *file)
{

	//lcos_gpio_init();
	
	printk("--->lcos_mode_open is ok\n");
	return 0;

}


static long lcos_mode_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
       int ret=0;
   printk("--->lcos_mode_ioctl() is ok\n");

	switch (cmd) 
	{
		case LCOS_COMD_POWERON:
		{
			//lcos_power_on();
			printk("--->lcos_power_on() is ok\n");
			break;
		}		
		case LCOS_COMD_POWEROFF:
		{
			//lcos_power_off();
			printk("--->lcos_power_off() is ok\n");
			break;
		}
			
		default:
			ret = -1;
	}
	return ret;

}
static int lcos_mode_release(struct inode *inode, struct file *file)
{

	//lcos_gpio_realse();
	printk("--->lcos_mode_release is ok\n");
	return 0;
}

static const struct file_operations lcos_fops = {
	.open = lcos_mode_open,
	.release = lcos_mode_release,
	.unlocked_ioctl = lcos_mode_ioctl,
	.owner = THIS_MODULE,
};

static struct miscdevice lcos_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "HX7097N",
	.fops = &lcos_fops,
};

static int lcos_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	int result;
	struct lcos_data *pdata;
	struct i2c_adapter *adapter;
	struct lcos_i2c_platform_data *lcosdata = dev_get_platdata(&client->dev);
       printk("--->lcos_probe is ok\n");

	adapter = to_i2c_adapter(client->dev.parent);
	result = i2c_check_functionality(adapter,I2C_FUNC_SMBUS_BYTE |I2C_FUNC_SMBUS_BYTE_DATA);
	if (!result)
		goto err_out;
	
	pdata = kzalloc(sizeof(struct lcos_data), GFP_KERNEL);
	if (!pdata) {
			result = -ENOMEM;
			printk("lcos alloc data memory error!\n");
			goto err_out;
		}
		g_lcos_data = pdata;
		pdata->lcos_client= client;

		memcpy(&(pdata->gpio_platdata),lcosdata,sizeof(struct lcos_i2c_platform_data));
		printk("--->lcos memcpy   pdata->gpio_platdata!\n");

		result = misc_register(&lcos_device);
		if (result != 0) {
			printk(KERN_ERR "register lcos miscdevice error");
			goto err_regsiter_lcos_misc;
		}
		pdata->lcos_miscdev= &lcos_device;
			
		printk("--->lcos device driver probe successfully");
		return 0;

	err_regsiter_lcos_misc:
		misc_deregister(&lcos_device);
		pdata->lcos_miscdev= NULL;		
	err_out:
		return result;

}


static int lcos_remove(struct i2c_client *client)
{

	misc_deregister(&lcos_device);
	if(g_lcos_data!=NULL)
		{
			kfree(g_lcos_data);		
		}

	printk("--->lcos_remove is ok\n");
	return 0;
}

static const struct i2c_device_id lcos_id[] = {
	{"lcos_hx7097", 0},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(i2c, lcos_id);
static unsigned short normal_i2c[] = { I2C_CLIENT_END };

static struct i2c_driver lcos_driver = {
	.class = I2C_CLASS_HWMON,
	.probe = lcos_probe,
	.remove = lcos_remove,
	.id_table = lcos_id,
	.driver = {
		   .owner = THIS_MODULE,
		   .name = LCOS_DRIVER,
		   },
	.address_list = normal_i2c,

};
static int __init lcos_init(void)
{
	int res = i2c_add_driver(&lcos_driver);
	pr_info("%s: Probe name LCOS\n", __func__);
	if (res)
		pr_err("%s failed\n", __func__);
	return res;
}

static void __exit lcos_exit(void)
{
	pr_info("%s\n", __func__);
	i2c_del_driver(&lcos_driver);
}

module_init(lcos_init);
module_exit(lcos_exit);

MODULE_AUTHOR("vkaoke@gmail.com");
MODULE_DESCRIPTION("LOCS Driver--Optical Module of Smart Glasses");
MODULE_LICENSE("GPL");






