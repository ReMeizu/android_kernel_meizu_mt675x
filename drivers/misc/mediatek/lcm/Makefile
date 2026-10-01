#
# Copyright (C) 2015 MediaTek Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#

#
# Makefile for misc devices that really don't fit anywhere else.
#

obj-$(CONFIG_MTK_LCM) += mt65xx_lcm_list.o        \
                         lcm_common.o             \
                         lcm_gpio.o               \
                         lcm_i2c.o                \
                         lcm_pmic.o               \
                         lcm_util.o

LCM_LISTS := $(subst ",,$(CONFIG_CUSTOM_KERNEL_LCM))
obj-$(CONFIG_MTK_LCM) += $(foreach LCM,$(LCM_LISTS),$(LCM)/)

subdir-ccflags-$(CONFIG_MTK_LCM) += -I$(srctree)/drivers/misc/mediatek/lcm/inc
subdir-ccflags-$(CONFIG_MTK_FB) += -I$(srctree)/drivers/misc/mediatek/video/$(MTK_PLATFORM)/dispsys \
                                       -I$(srctree)/drivers/misc/mediatek/video/$(MTK_PLATFORM)/dispsys/mt6735 \
                                       -I$(srctree)/drivers/misc/mediatek/video/$(MTK_PLATFORM)/videox \
                                       -I$(srctree)/drivers/misc/mediatek/video/$(MTK_PLATFORM)/ \
                                       -I$(srctree)/drivers/misc/mediatek/video/$(MTK_PLATFORM)/mt6735/ \
                                       -I$(srctree)/drivers/misc/mediatek/video/include

LCM_DEFINES := $(shell echo $(CONFIG_CUSTOM_KERNEL_LCM) | tr a-z A-Z)
DEFINES += $(foreach LCM,$(LCM_DEFINES),$(LCM))
DEFINES += MTK_LCM_PHYSICAL_ROTATION=\"$(MTK_LCM_PHYSICAL_ROTATION)\"
ccflags-$(CONFIG_MTK_LCM) += $(addprefix -D, $(DEFINES))
ccflags-$(CONFIG_MTK_LCM_DEVICE_TREE_SUPPORT) += -DMTK_LCM_DEVICE_TREE_SUPPORT


ifneq ($(filter ili9885a_fhd_dsi_vdo_dj_asi_al1518 ili9885a_fhd_dsi_vdo_txd_asi_al1518 ili9885b_fhd_dsi_vdo_dj_asi_al1518 ili9885b_fhd_dsi_vdo_dj_asi_al1518otp ili9885b_fhd_dsi_vdo_txd_asi_al1518 ili9885b_fhd_dsi_vdo_txd_asi_al1518otp,$(LCM_LISTS)),)
obj-$(CONFIG_MTK_LCM) += u20_lcd_bias.o
endif
