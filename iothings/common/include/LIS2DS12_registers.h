/** Based on ST MicroElectronics LIS2DS datasheet http://www.st.com/web/en/resource/technical/document/datasheet/DM00042751.pdf
* 18/06/2014 by Conor Forde <me@conorforde.com>
* Updates should be available at https://github.com/Snowda/LIS2DS
*

*/

#ifndef _LIS2DS12REG_H_
#define _LIS2DS12REG_H_

//Registers
#define LIS2DS_STATUS_REG_AUX 	0x07
#define LIS2DS_OUT_TEMP_L 		0x0C
#define LIS2DS_OUT_TEMP_H 		0x0D
#define LIS2DS_WHO_AM_I 		0x0F
#define LIS2DS_TEMP_CFG_REG 	0x1F /*rw */
#define LIS2DS_CTRL_REG1 		0x20 /*rw */
#define LIS2DS_CTRL_REG2 		0x21 /*rw */
#define LIS2DS_CTRL_REG3 		0x22 /*rw */
#define LIS2DS_CTRL_REG4 		0x23 /*rw */
#define LIS2DS_CTRL_REG5 		0x24 /*rw */
#define LIS2DS_CTRL_REG6 		0x25 /*rw */
#define LIS2DS_REFERENCE 		0x26 /*rw */
#define LIS2DS_STATUS_REG 		0x27
#define LIS2DS_OUT_X_L 			0x28
#define LIS2DS_OUT_X_H 			0x29
#define LIS2DS_OUT_Y_L 			0x2A
#define LIS2DS_OUT_Y_H 			0x2B
#define LIS2DS_OUT_Z_L 			0x2C
#define LIS2DS_OUT_Z_H 			0x2D
#define LIS2DS_FIFO_CTRL_REG 	0x2E /*rw */
#define LIS2DS_FIFO_SRC_REG 	0x2F
#define LIS2DS_INT1_CFG 		0x30 /*rw */
#define LIS2DS_INT1_SOURCE 		0x31
#define LIS2DS_INT1_THS 		0x32 /*rw */
#define LIS2DS_INT1_DURATION 	0x33 /*rw */
#define LIS2DS_INT_CFG 			0x34 /*rw */
#define LIS2DS_INT2_SOURCE 		0x35
#define LIS2DS_INT2_THS 		0x36 /*rw */
#define LIS2DS_INT2_DURATION 	0x37 /*rw */
#define LIS2DS_CLICK_CFG 		0x38 /*rw */
#define LIS2DS_CLICK_SRC 		0x39
#define LIS2DS_CLICK_THS 		0x3A /*rw */
#define LIS2DS_TIME_LIMIT 		0x3B /*rw */
#define LIS2DS_TIME_LATENCY 	0x3C /*rw */
#define LIS2DS_TIME_WINDOW 		0x3D /*rw */
#define LIS2DS_ACT_THS 			0x3E /*rw */
#define LIS2DS_ACT_DUR 			0x3F /*rw */

//Register Masks

//STATUS_AUX_REG masks
#define LIS2DS_TOR_MASK 		0x40
#define LIS2DS_TDA_MASK 		0x04

//WHO_AM_I masks
#define LIS2DS_I_AM_MASK 		0x33

// TEMP_CFG_REG masks
#define LIS2DS_TEMP_EN_MASK 	0xC0

// CTRL_REG1 masks
#define LIS2DS_ODR_MASK 			0xF0
#define LIS2DS_ODR_MASK_PWR_DWN 	0x00 /*HR / Normal / Low Power */
#define LIS2DS_ODR_MASK_1HZ     	0x10 /*HR / Normal / Low Power */
#define LIS2DS_ODR_MASK_10HZ    	0x20 /*HR / Normal / Low Power */
#define LIS2DS_ODR_MASK_25HZ    	0x30 /*HR / Normal / Low Power */
#define LIS2DS_ODR_MASK_50HZ    	0x40 /*HR / Normal / Low Power */
#define LIS2DS_ODR_MASK_100HZ   	0x50 /*HR / Normal / Low Power */
#define LIS2DS_ODR_MASK_200HZ   	0x60 /*HR / Normal / Low Power */
#define LIS2DS_ODR_MASK_400HZ   	0x70 /*HR / Normal / Low Power */
#define LIS2DS_ODR_MASK_1620HZ  	0x80 /* Low Power Mode only */
#define LIS2DS_ODR_MASK_HIGH_RES 	s0x90 /* in HR+Normal: 1344Hz, in Low Power 5376Hz */
#define LIS2DS_LPEN_MASK 			0x08
#define LIS2DS_Z_EN_MASK 			0x04
#define LIS2DS_Y_EN_MASK 			0x02
#define LIS2DS_X_EN_MASK			0x01
#define LIS2DS_XYZ_EN_MASK			0x07

// CTRL_REG2 masks
#define LIS2DS_HPM_MASK 		0xC0
#define LIS2DS_HPCF_MASK 		0x30
#define LIS2DS_FDS_MASK 		0x08
#define LIS2DS_HPCLICK_MASK 	0x04
#define LIS2DS_HPIS2_MASK 		0x02
#define LIS2DS_HPIS1_MASK 		0x01

// CTRL_REG3 masks
#define LIS2DS_I1_CLICK 		0x80
#define LIS2DS_I1_AOI 			0x60
#define LIS2DS_I1_DRDY 			0x18
#define LIS2DS_I1_DRDY1         0x08
#define LIS2DS_I1_DRDY2         0x10
#define LIS2DS_I1_WTM 			0x04
#define LIS2DS_I1_OVERRUN 		0x02

// CTRL_REG4 masks
#define LIS2DS_BDU_MASK 		0x80
#define LIS2DS_BLE_MASK 		0x40
#define LIS2DS_FS_MASK 			0x30
#define LIS2DS_HR_MASK 			0x08
#define LIS2DS_ST_MASK 			0x06
#define LIS2DS_SIM_MASK 		0x01

// CTRL_REG5 masks
#define LIS2DS_BOOT_MASK 		0x80
#define LIS2DS_FIFO_EN_MASK 	0x40
#define LIS2DS_LIR_INT1_MASK 	0x08
#define LIS2DS_D4D_INT1_MASK 	0x04
#define LIS2DS_LIR_INT2_MASK	0x02
#define LIS2DS_D4D_INT2_MASK	0x01

// CTRL_REG6 masks
#define LIS2DS_I2C_CCK_EN_MASK	0x80
#define LIS2DS_I2C_INT1_MASK 	0x40
#define LIS2DS_I2C_INT2_MASK 	0x20
#define LIS2DS_BOOT_I2_MASK 	0x10
#define LIS2DS_P2_ACT_MASK 		0x08
#define LIS2DS_H_LACTIVE_MASK 	0x02

// REF masks
// none

// STATUS_REG masks
#define LIS2DS_ZYXOR_MASK 		0x80
#define LIS2DS_ZOR_MASK 		0x40
#define LIS2DS_YOR_MASK 		0x20
#define LIS2DS_XOR_MASK 		0x10
#define LIS2DS_ZYXDA_MASK 		0x08
#define LIS2DS_ZDA_MASK 		0x04
#define LIS2DS_YDA_MASK 		0x02
#define LIS2DS_XDA_MASK 		0x01

// FIFO_CTRL_REG masks
#define LIS2DS_FM_MASK 			0xC0
#define LIS2DS_TR_MASK 			0x20
#define LIS2DS_FTH_MASK 		0x1F

// FIFO_SRC_REG masks
#define LIS2DS_WTM_MASK 		0x80
#define LIS2DS_OVRN_FIFO_MASK 	0x40
#define LIS2DS_EMPTY_MASK 		0x20
#define LIS2DS_FSS_MASK 		0x1F

// INT1/2_CFG masks
#define LIS2DS_AOI_MASK 		0x80
#define LIS2DS_6D_MASK 			0x40
#define LIS2DS_ZHIE_MASK 		0x20
#define LIS2DS_ZLIE_MASK 		0x10
#define LIS2DS_YHIE_MASK 		0x08
#define LIS2DS_YLIE_MASK 		0x04
#define LIS2DS_XHIE_MASK 		0x02
#define LIS2DS_XLIE_MASK 		0x01

// INT1/2_SRC masks
#define LIS2DS_INT_IA_MASK 		0x40
#define LIS2DS_ZH_MASK 			0x20
#define LIS2DS_ZL_MASK 			0x10
#define LIS2DS_YH_MASK 			0x08
#define LIS2DS_YL_MASK 			0x04
#define LIS2DS_XH_MASK 			0x02
#define LIS2DS_XL_MASK 			0x01

// INT1/2_THS masks
#define LIS2DS_THS_MASK 		0x4F

// INT1/2_DURATION masks
#define LIS2DS_D_MASK 			0x4F

// CLICK_CFG masks
#define LIS2DS_ZD_MASK 			0x20
#define LIS2DS_ZS_MASK 			0x10
#define LIS2DS_YD_MASK 			0x08
#define LIS2DS_YS_MASK 			0x04
#define LIS2DS_XD_MASK 			0x02
#define LIS2DS_XS_MASK 			0x01

// CLICK_SRC masks
#define LIS2DS_CLK_IA_MASK 		0x40
#define LIS2DS_DCLICK_MASK 		0x20
#define LIS2DS_SCLICK_MASK 		0x10
#define LIS2DS_SIGN_MASK 		0x08
#define LIS2DS_Z_CLICK_MASK 	0x04
#define LIS2DS_Y_CLICK_MASK 	0x02
#define LIS2DS_X_CLICK_MASK 	0x01

// CLICK_THS masks
#define LIS2DS_CLK_THS_MASK 	0x7F

// TIME_LIMIT masks
#define LIS2DS_TLI_MASK 		0x7F

// TIME_LATENCY masks
// none

// TIME_WINDOW masks
// none

// ACT_THS masks
#define LIS2DS_ACTH_MASK 		0x7F

// ACT_DUR masks
// None


#endif /* _LIS2DS12REG_H_ */
