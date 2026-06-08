# Agent Instructions

## Primary work area

- The main user keymap is `qmk_firmware/keyboards/keyball/keyball39/keymaps/mykeymap/`.
- When asked to edit the Keyball39 keymap, prefer editing:
  - `qmk_firmware/keyboards/keyball/keyball39/keymaps/mykeymap/keymap.c`
  - `qmk_firmware/keyboards/keyball/keyball39/keymaps/mykeymap/config.h`
  - `qmk_firmware/keyboards/keyball/keyball39/keymaps/mykeymap/rules.mk`
- Do not change default, via, test, or other keyboard keymaps unless the user explicitly asks.

## Keymap behavior notes

- `mykeymap` contains custom keycodes for bracket auto-pair insertion.
- IME state is tracked in `keymap.c`; be careful when changing `KC_LNG1`, `KC_LNG2`, or `LT(..., KC_LNG*)` behavior.
- The scroll-mode custom key should preserve tap behavior by sending the original tap key and only enabling scroll mode on hold.
- Mouse button combos should be kept intentional and reviewed when moving base-layer keys.

## CI and build

- Regular push builds should target only `keyball39:mykeymap`.
- Release/tag builds may still build all firmware variants.
- Reuse `.github/workflows/build-firmware.yml` for firmware build jobs.
- Local QMK builds may require an MSYS2 MinGW 64-bit terminal on Windows:
  `qmk compile -kb keyball/keyball39 -km mykeymap`

## Git hygiene

- Check `git status --short --branch` before editing.
- Do not overwrite user changes.
- Do not commit unless the user explicitly asks for a commit.
- Keep changes scoped to the requested keymap or workflow.
