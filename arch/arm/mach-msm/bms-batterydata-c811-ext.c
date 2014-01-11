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
	.y		= {2886, 2895, 2894, 2895, 2895, 2894, 2885},
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
				{4186, 4185, 4184, 4185, 4185, 4183, 4181},
				{4110, 4118, 4122, 4127, 4128, 4128, 4127},
				{4048, 4066, 4071, 4080, 4083, 4083, 4082},
				{3984, 4017, 4027, 4038, 4042, 4042, 4040},
				{3936, 3968, 3986, 3999, 4004, 4004, 4002},
				{3897, 3925, 3950, 3965, 3970, 3970, 3968},
				{3865, 3888, 3910, 3935, 3940, 3940, 3937},
				{3838, 3858, 3872, 3905, 3912, 3912, 3909},
				{3817, 3832, 3842, 3861, 3877, 3881, 3880},
				{3803, 3810, 3817, 3829, 3835, 3838, 3839},
				{3788, 3796, 3799, 3809, 3814, 3816, 3817},
				{3774, 3785, 3787, 3794, 3798, 3801, 3800},
				{3760, 3774, 3778, 3782, 3785, 3788, 3787},
				{3746, 3761, 3769, 3775, 3776, 3777, 3776},
				{3733, 3745, 3759, 3768, 3768, 3764, 3758},
				{3721, 3727, 3743, 3758, 3757, 3750, 3738},
				{3707, 3710, 3718, 3736, 3738, 3731, 3718},
				{3690, 3694, 3691, 3701, 3702, 3695, 3683},
				{3659, 3674, 3671, 3680, 3682, 3677, 3667},
				{3651, 3669, 3667, 3677, 3679, 3675, 3665},
				{3638, 3661, 3661, 3673, 3678, 3674, 3664},
				{3623, 3650, 3654, 3669, 3675, 3672, 3662},
				{3606, 3636, 3645, 3662, 3671, 3668, 3658},
				{3585, 3618, 3632, 3650, 3660, 3657, 3645},
				{3558, 3593, 3608, 3625, 3631, 3624, 3611},
				{3524, 3557, 3569, 3578, 3581, 3575, 3563},
				{3478, 3504, 3510, 3514, 3517, 3511, 3502},
				{3410, 3425, 3426, 3428, 3431, 3428, 3422},
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
				{551, 330, 209, 129, 100, 88, 85},
				{733, 452, 279, 140, 100, 87, 85},
				{761, 485, 311, 147, 103, 88, 86},
				{750, 480, 325, 156, 104, 89, 87},
				{745, 467, 330, 164, 108, 91, 89},
				{741, 456, 316, 176, 114, 95, 90},
				{739, 450, 300, 184, 124, 99, 93},
				{745, 448, 292, 154, 127, 110, 100},
				{763, 449, 288, 139, 101, 89, 88},
				{790, 460, 289, 138, 100, 88, 88},
				{824, 482, 297, 139, 101, 90, 88},
				{870, 514, 312, 140, 101, 91, 89},
				{926, 554, 337, 144, 104, 92, 92},
				{995, 598, 372, 150, 105, 91, 88},
				{1080, 657, 414, 158, 105, 89, 88},
				{1183, 750, 457, 159, 102, 88, 88},
				{1326, 915, 532, 158, 102, 88, 88},
				{1607, 1158, 732, 181, 104, 90, 90},
				{1832, 1262, 797, 184, 105, 91, 90},
				{1954, 1379, 898, 201, 107, 92, 91},
				{2064, 1481, 1011, 226, 111, 93, 91},
				{2292, 1583, 1127, 269, 114, 94, 91},
				{2716, 1686, 1231, 331, 115, 92, 91},
				{3355, 1800, 1320, 410, 117, 93, 91},
				{4566, 2003, 1409, 527, 125, 97, 94},
				{6832, 2693, 1522, 705, 141, 105, 99},
				{11586, 5045, 1830, 949, 167, 120, 105},
				{22760, 13234, 3489, 1377, 248, 171, 115},
	}
};

struct pm8921_bms_battery_data c811_2920_data = {
	.fcc			= 2920,
	.fcc_temp_lut		= &fcc_temp,
	.pc_temp_ocv_lut	= &pc_temp_ocv,
	.rbatt_sf_lut		= &rbatt_sf,
	.default_rbatt_mohm	= 112,
};

