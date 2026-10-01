# Meizu MT675x Linux 3.18

Board-driver sources for U10 (MT6750) and U20 (MT6755), based on the
Meizu M6 BSP. The U10 reference kernel previously linked with its own
MT6353, display, touch and board DT. The U20 reference linked panels,
MT6351 audio and cameras before the own-touch profile was completed.

| Module | U10 | U20 |
| --- | --- | --- |
| Own device tree and PMIC selection | Source and earlier full link | Source and earlier full link |
| Display / panel variants | Five variants, earlier full link | Six variants, earlier full link |
| Goodix touch | Own profile; external executable firmware required | Own regulator/configuration profile; new compile pending |
| Audio / camera / connectivity / modem / suspend | Device testing pending | Device testing pending |

These newly prepared source snapshots have not been compiled or tested on
phones. U10 requires its exact owner-provided Goodix firmware; do not use
another board's payload. U20 uses configuration tables and the existing
external-file updater, without embedding executable touch firmware.
M3s/Y15 is a separate board with a Linux 3.10 stock kernel; no source-built
M3s port is included here. Common chipset names do not establish compatibility.

Use Android ARM64 GCC 4.9 and an external output directory, userdebug.
Original copyright notices and GPL COPYING are preserved. Board validation,
charging, battery policy, peripheral and Android integration remain open.

This U20 export excludes an additional U10 S5K5E8YX camera source input
until its original source release is verified. The U20 profile selects
OV13853 and HI553. U10 firmware and camera inputs are separate prerequisites.
