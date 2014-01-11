/* Copyright (C) 2013, NEC CASIO Mobile Communications. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include <linux/mfd/pm8xxx/pm8921-bms.h>

static struct single_row_lut fcc_temp = {
	.x		= {-20, -10, 0, 10, 25, 40, 60},
	.y		= {1756, 1769, 1770, 1769, 1769, 1767, 1760},
	.cols	= 7
};

static struct pc_temp_ocv_lut pc_temp_ocv = {
	.rows		= 29,
	.cols		= 7,
	.temp		= {-20, -10, 0, 10, 25, 40, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40,
					35, 30, 25, 20, 15, 10, 9, 8, 7, 6, 5,
					4, 3, 2, 1, 0},
	.ocv		= {
				{4177, 4177, 4177, 4176, 4176, 4173, 4170},
				{4100, 4112, 4119, 4120, 4120, 4119, 4118},
				{4038, 4065, 4076, 4077, 4076, 4076, 4074},
				{3975, 4012, 4029, 4035, 4036, 4035, 4034},
				{3933, 3963, 3984, 3994, 3997, 3998, 3997},
				{3898, 3928, 3946, 3962, 3965, 3966, 3963},
				{3866, 3895, 3911, 3930, 3936, 3936, 3933},
				{3840, 3865, 3880, 3889, 3907, 3908, 3905},
				{3819, 3840, 3852, 3857, 3865, 3871, 3872},
				{3803, 3817, 3828, 3832, 3834, 3837, 3837},
				{3790, 3798, 3808, 3812, 3814, 3816, 3815},
				{3779, 3784, 3791, 3796, 3798, 3800, 3800},
				{3768, 3772, 3778, 3783, 3786, 3787, 3786},
				{3757, 3761, 3769, 3773, 3776, 3775, 3775},
				{3744, 3750, 3759, 3767, 3768, 3765, 3756},
				{3728, 3738, 3743, 3756, 3756, 3750, 3737},
				{3709, 3724, 3723, 3733, 3735, 3729, 3716},
				{3682, 3706, 3706, 3705, 3701, 3694, 3683},
				{3640, 3679, 3688, 3688, 3684, 3678, 3666},
				{3630, 3673, 3685, 3685, 3682, 3676, 3665},
				{3620, 3664, 3680, 3682, 3681, 3675, 3664},
				{3606, 3653, 3672, 3678, 3679, 3674, 3663},
				{3590, 3639, 3661, 3671, 3675, 3670, 3658},
				{3571, 3620, 3645, 3657, 3661, 3655, 3640},
				{3547, 3595, 3618, 3629, 3628, 3619, 3604},
				{3515, 3559, 3577, 3582, 3578, 3570, 3556},
				{3470, 3506, 3516, 3516, 3514, 3508, 3498},
				{3404, 3426, 3428, 3427, 3429, 3426, 3419},
				{3300, 3300, 3300, 3300, 3300, 3300, 3300}
	}
};

static struct sf_lut rbatt_sf = {
	.rows		= 28,
	.cols		= 7,
	
	.row_entries	= {-20,-10, 0, 10, 25, 40, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40,
					35, 30, 25, 20, 15, 10, 9, 8, 7, 6, 5,
					4, 3, 2, 1},
	.sf		= {
				{480, 262, 159, 131, 100, 88, 85},
				{502, 309, 193, 139, 101, 88, 87},
				{504, 323, 207, 145, 102, 89, 89},
				{497, 323, 209, 148, 105, 90, 91},
				{491, 321, 210, 151, 108, 93, 92},
				{489, 318, 207, 151, 110, 95, 94},
				{488, 316, 204, 144, 112, 98, 97},
				{488, 315, 202, 140, 102, 93, 95},
				{490, 316, 201, 138, 99, 88, 89},
				{497, 316, 201, 138, 99, 88, 90},
				{508, 318, 202, 139, 100, 90, 92},
				{526, 325, 204, 141, 101, 91, 93},
				{548, 336, 208, 144, 103, 91, 95},
				{576, 354, 215, 149, 105, 92, 91},
				{610, 382, 223, 153, 106, 90, 91},
				{650, 422, 234, 153, 105, 91, 92},
				{713, 469, 262, 154, 100, 88, 90},
				{975, 530, 323, 183, 109, 94, 96},
				{1363, 544, 304, 171, 105, 92, 94},
				{1550, 579, 318, 178, 107, 93, 94},
				{1748, 609, 331, 184, 109, 93, 93},
				{1963, 643, 345, 189, 109, 92, 92},
				{2237, 699, 363, 192, 107, 90, 92},
				{2622, 803, 384, 195, 107, 91, 92},
				{3130, 953, 425, 206, 109, 93, 94},
				{3836, 1188, 487, 226, 114, 97, 98},
				{5165, 1627, 613, 260, 124, 103, 99},
				{8031, 2919, 1027, 405, 164, 136, 103},
	}
};

struct pm8921_bms_battery_data c811_1800_data = {
	.fcc			= 1800,
	.fcc_temp_lut		= &fcc_temp,
	.pc_temp_ocv_lut	= &pc_temp_ocv,
	.rbatt_sf_lut		= &rbatt_sf,
	.default_rbatt_mohm	= 181,
};

