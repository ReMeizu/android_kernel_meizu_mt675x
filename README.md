# Meizu U10 and U20 kernel

Linux 3.18 board-driver sources based on the Meizu M6 BSP. U10 has MT6750
and MT6353; U20 has MT6755 and MT6351. Both use the native `mt6755` build
layout. Each board has its own configuration and device tree.

| Component | U10 | U20 |
| --- | --- | --- |
| Board / power controller | Own stock DTS and MT6353 register definitions | Own stock DTS and MT6351 selection |
| Display | Five own panel wrappers | Own panel/bias code, including ILI9885A |
| Goodix touch | Own configuration; runtime firmware loader | Own regulator and normal/charger configuration |
| Alternative touch | FocalTech FT5X26 source | Use the board's Goodix selection |
| Cameras | Native-selected sensor sources, including S5K5E8YX | OV13853 / HI553 selection |
| Audio | MT6353 source integration | MT6351 source integration |
| Wi-Fi / Bluetooth / GPS / modem | Legacy MediaTek sources; device tests pending | Legacy MediaTek sources; device tests pending |
| GPU / suspend / charging | Device tests pending | Device tests pending |

The complete U20 kernel at `217f8cbf` passed the
[cloud build](https://github.com/ReMeizu/build-infra/actions/runs/36912871400):
its generated configuration, compiled stock-identical board DTB, linked ARM64
kernel and selected driver objects were verified. This is compilation evidence;
no new U20 boot image or device acceptance is included.

U10's `u10-3.18` branch adds the runtime firmware loader and its own native
build inputs. Its new full kernel build is pending. Early firmware availability,
boot/ramdisk integration and device testing remain separate requirements.

The U10 updater requests `goodix/u10.bin` through the kernel firmware loader.
Provide the exact owner image in `/lib/firmware/goodix/` in the early ramdisk
before touch probing. Missing or invalid firmware fails the update explicitly.
Executable firmware is not embedded in this source tree. The existing update,
checksum, controller compatibility and programming paths are preserved.

S5K5E8YX source comes from the official
[Meizu M681 release](https://github.com/meizuosc/m681/tree/ae87bdadf3dd0520aabeaccfa3c5498b91cef76c/drivers/misc/mediatek/imgsensor/src/mt6755/s5k5e8yx_mipi_raw),
with a MediaTek 3.18 API adapter in the U10 branch. Source availability does
not establish correct camera tuning, power sequencing or operation.

Build with the pinned AOSP ARM64 GCC 4.9 toolchain, an external output
directory and `TARGET_BUILD_VARIANT=userdebug`. Reviewed build profiles are
in [ReMeizu/build-infra](https://github.com/ReMeizu/build-infra/tree/kernel-components/recipes/components).
This is a native legacy kernel, not a mainline/GKI or Android 13 support claim.
M3s/Y15 needs a separate board port. Original copyright notices and `COPYING`
are preserved.
