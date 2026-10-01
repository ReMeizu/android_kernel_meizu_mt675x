GT9XX_U20_PRIVATE_INCLUDE ?= $(srctree)/drivers/input/touchscreen/mediatek/GT9XX_MZ/config_u20

# Meizu GT9xx driver and board-owned resources.

ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/GT9XX_MZ/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include/

ifeq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX_U10_STOCK_RESOURCES),y)
# Matching board configuration and firmware are provided by the owner.
ifeq ($(strip $(GT9XX_U10_PRIVATE_INCLUDE)),)
$(error U10 requires GT9XX_U10_PRIVATE_INCLUDE; donor Goodix payloads are forbidden)
endif
ifeq ($(wildcard $(GT9XX_U10_PRIVATE_INCLUDE)/gt9xx_u10_stock_config.h),)
$(error Missing private U10 Goodix configuration header)
endif
ifeq ($(wildcard $(GT9XX_U10_PRIVATE_INCLUDE)/gt9xx_u10_stock_firmware.h),)
$(error Missing private U10 Goodix firmware header)
endif
ccflags-y += -I$(GT9XX_U10_PRIVATE_INCLUDE)
endif

ccflags-$(CONFIG_TOUCHSCREEN_MTK_GT9XX_U10_STOCK_RESOURCES) += -I$(srctree)/drivers/misc/mediatek/pmic/include/mt6353

obj-y	+=  gt9xx_driver.o
obj-y	+=  goodix_tool.o
obj-y	+=  gt9xx_update.o

ifeq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX_U20_STOCK_RESOURCES),y)
ifeq ($(wildcard $(GT9XX_U20_PRIVATE_INCLUDE)/gt9xx_u20_stock_config.h),)
$(error Missing owner-provided U20 Goodix configuration header)
endif
ccflags-y += -I$(GT9XX_U20_PRIVATE_INCLUDE)
endif

ifneq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX_U20_STOCK_RESOURCES),y)
ifneq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX_U10_STOCK_RESOURCES),y)
ifneq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX_M681_LEGACY_RESOURCES),y)
$(error GT9xx requires an implemented own-board resource profile; U20 resources are not integrated yet)
endif
ifeq ($(wildcard $(GT9XX_M681_PRIVATE_INCLUDE)/gt9xx_firmware.h),)
$(error Missing owner-provided M681 Goodix firmware header)
endif
ccflags-y += -I$(GT9XX_M681_PRIVATE_INCLUDE)
endif
endif
