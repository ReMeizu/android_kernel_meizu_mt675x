#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-only
"""Generate private build headers from the matching owner's U10 resources."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

EXPECTED = {
    'dtb': (79423, '07e70460413b8be2bad6a0d0c3acb81017d7c2550cea9fed397cccd50df1d00f'),
    'goodix_config': (186, '5b18a65bd909dfba3eab28e3a0c888f39cb78c82c7c97470f839e9b7f8afcfe1'),
    'goodix_firmware': (90126, '52eb3e6d0985c4568677833d7bd09580d276650832e65bee064ec8196667f1f2'),
}

def checked(path, kind):
    payload = path.read_bytes()
    size, digest = EXPECTED[kind]
    if len(payload) != size or hashlib.sha256(payload).hexdigest() != digest:
        raise ValueError('resource does not match the supported U10 profile: ' + kind)
    return payload

def rows(payload, continuation=False):
    result = []
    for offset in range(0, len(payload), 16):
        line = '    ' + ','.join('0x%02x' % byte for byte in payload[offset:offset + 16])
        if offset + 16 < len(payload):
            line += ','
        if continuation:
            line += ' \\'
        result.append(line)
    return '\n'.join(result)

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--dtb', required=True, type=Path)
    parser.add_argument('--goodix-config', required=True, type=Path)
    parser.add_argument('--goodix-firmware', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--dtc', default='dtc')
    args = parser.parse_args()
    if args.output.exists():
        raise ValueError('output must be a new directory')
    data = {kind: checked(getattr(args, kind), kind) for kind in EXPECTED}
    # Run decompilation before creating any output; never substitute another board.
    dts = subprocess.check_output([args.dtc, '-I', 'dtb', '-O', 'dts', str(args.dtb)])
    if not dts.startswith(b'/dts-v1/;'):
        raise ValueError('invalid DTS output')
    config = ('/* Generated from owner-provided U10 configuration. */\n'
              '#ifndef GT9XX_U10_STOCK_CONFIG_H\n#define GT9XX_U10_STOCK_CONFIG_H\n'
              '#define GT9XX_U10_STOCK_CONFIG_BYTES 186\n#define CTP_CFG_GROUP0 { \\\n'
              + rows(data['goodix_config'], True) + '\n}\n'
              + ''.join('#define CTP_CFG_GROUP%d {}\n' % i for i in range(1, 6)) + '#endif\n')
    firmware = ('/* Generated from owner-provided U10 firmware; keep private. */\n'
                '#ifndef GT9XX_U10_STOCK_FIRMWARE_H\n#define GT9XX_U10_STOCK_FIRMWARE_H\n'
                '#define GT9XX_U10_STOCK_FIRMWARE_BYTES 90126\n'
                'const unsigned char gtp_default_FW[] = {\n'
                + rows(data['goodix_firmware']) + '\n};\n#endif\n')
    args.output.mkdir(parents=True)
    generated = {'u10.dts': dts, 'gt9xx_u10_stock_config.h': config.encode(),
                 'gt9xx_u10_stock_firmware.h': firmware.encode()}
    for name, payload in generated.items():
        (args.output / name).write_bytes(payload)
    receipt = {'profile': 'U10 international Flyme 6.3.0.0G',
               'inputs': {kind: {'bytes': len(payload), 'sha256': hashlib.sha256(payload).hexdigest()}
                          for kind, payload in data.items()},
               'outputs': {name: hashlib.sha256(payload).hexdigest() for name, payload in generated.items()}}
    (args.output / 'resources.json').write_text(json.dumps(receipt, indent=2) + '\n')

if __name__ == '__main__':
    main()
