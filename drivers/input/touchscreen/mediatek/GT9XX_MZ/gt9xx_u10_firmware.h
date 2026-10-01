/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef GT9XX_U10_FIRMWARE_H
#define GT9XX_U10_FIRMWARE_H

#define GUP_U10_FIRMWARE_SIZE 90126U
#define GUP_U10_HEADER_SIZE 14U

static int gup_u10_validate_firmware(const u8 *data, size_t size)
{
    /* Hardware ID 0x00016000, PID 915L, version 0x3007: supported owner image. */
    static const u8 header[GUP_U10_HEADER_SIZE] = {
        0x00, 0x01, 0x60, 0x00, '9', '1', '5', 'L',
        0x00, 0x00, 0x00, 0x00, 0x30, 0x07
    };
    u32 checksum = 0;
    size_t i;

    if (!data || size != GUP_U10_FIRMWARE_SIZE)
        return -EINVAL;
    if (memcmp(data, header, sizeof(header)))
        return -EINVAL;
    for (i = GUP_U10_HEADER_SIZE; i < size; i += 2)
        checksum += ((u32)data[i] << 8) | data[i + 1];
    if (checksum & 0xffff)
        return -EBADMSG;
    return 0;
}

static int gup_u10_copy_section(u8 *buf, const u8 *data, size_t size,
                               u32 offset, u16 length, bool from_end)
{
    size_t start;

    if (!buf || !data || size != GUP_U10_FIRMWARE_SIZE ||
        offset > size - GUP_U10_HEADER_SIZE)
        return -EINVAL;
    start = from_end ? size - offset : GUP_U10_HEADER_SIZE + offset;
    if (start < GUP_U10_HEADER_SIZE || length > size - start)
        return -EINVAL;
    memcpy(buf, data + start, length);
    return 0;
}

#endif
