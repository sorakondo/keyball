# Keyball Series

![Keyball61](./keyball61/doc/rev1/images/kb61_001.jpg)

Keyball series is keyboard family which have 100% track ball.

Keyboards in the family are:

* Available
    * Keyball39: split + 39 keys + a track ball
    * Keyball44: split + 44 keys + a track ball
    * Keyball61: split + 61 keys + a track ball
* Unavailable
    * Keyball46 (first one!)
    * One47

## Where to Buy

|Keyboard   |Shirogane Lab / 白銀ラボ                                   |Yushakobo / 遊舎工房                       |
|-----------|-------------------------------------------|-----------------------------------------------------------|
|Keyball39  |<https://shiroganelab.com/products/keyball39> |<https://shop.yushakobo.jp/products/5357>  |
|Keyball44  |<https://shiroganelab.com/products/keyball44> |<https://shop.yushakobo.jp/products/8337>  |
|Keyball61  |<https://shiroganelab.com/products/keyball61> |<https://shop.yushakobo.jp/products/5358>  |

## Build Guide

*   Keyball39:
    [English/英語](/keyball39/doc/rev1/buildguide_en.md),
    [日本語/Japanese](./keyball39/doc/rev1/buildguide_jp.md)
*   Keyball44:
    [English/英語](./keyball44/doc/rev1/buildguide_en.md),
    [日本語/Japanese](./keyball44/doc/rev1/buildguide_jp.md)
*   Keyball61:
    [English/英語](./keyball61/doc/rev1/buildguide_en.md),
    [日本語/Japanese](./keyball61/doc/rev1/buildguide_jp.md)

## Firmware

See [document for firmware source code](./qmk_firmware/keyboards/keyball/readme.md).

### Pre-compiled Firmwares

#### GitHub Actionsでビルドして書き込む

Keyball39の `mykeymap` を使用する場合は、次の手順でGitHub Actionsからファームウェアを作成できます。

1. キーマップの変更をGitHubへpushします。ファームウェア関連ファイルの変更を検知すると、GitHub Actionsの `Build keyball39 mykeymap` が自動的に実行されます。
2. GitHubリポジトリの **Actions** タブから該当する実行結果を開き、処理が完了するまで待ちます。
3. 実行結果の **Artifacts** から `keyball39-mykeymap-firmware` をダウンロードし、ZIPファイルを展開します。
4. 展開した `.hex` ファイルを、[Pro Micro Web Updater](https://sekigon-gonnoc.github.io/promicro-web-updater/)で書き込みます。
5. USBケーブルで片側のPro Microを接続し、Web UpdaterでHEXファイルを選択して書き込みます。認識されない場合は、Pro MicroのRESETとGNDを短く2回接触させてブートローダーを起動してください。
6. 左右それぞれのPro Microに、同じ `.hex` ファイルを書き込んでください。

書き込みには、WebUSBに対応したブラウザ（Chrome、Edgeなど）を使用してください。

## Keyball39のキーマップ編集

Keyball39を使用する場合、キー配置やキーマップの動作は次のファイルを中心に編集します。

```text
qmk_firmware/keyboards/keyball/keyball39/keymaps/mykeymap/keymap.c
```

このファイルには、キー配置、レイヤー、カスタムキーコード、コンボなどを記述します。
