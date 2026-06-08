/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"
#include "keymap_japanese.h"

enum custom_keycodes {
    MY_LPRN = SAFE_RANGE, // ( → 自動で () 入力してカーソルを中に移動
    MY_LBRC,              // [ → 自動で [] 入力してカーソルを中に移動
    MY_LCBR,              // { → 自動で {} 入力してカーソルを中に移動
    MY_LABK,              // < → 自動で <> 入力してカーソルを中に移動
    MY_SCRL_K,            // タップでK、ホールドでスクロールモード
};

static uint16_t scroll_k_timer = 0;
static bool     scroll_k_held  = false; // K が押されていて未確定の状態
static bool     scroll_k_sent  = false; // 別キー割り込みで K を先送り済み
static bool     scroll_k_fired = false; // スクロールモードが発動済み

// IME状態をエミュレートする変数
static bool is_ime_on = true; // デフォルトはIME ONと仮定

static void update_ime_state(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_LNG1:
                is_ime_on = true;
                break;
            case KC_LNG2:
                is_ime_on = false;
                break;
        }
        return;
    }

    if (record->tap.count) {
        switch (keycode) {
            case LT(2, KC_LNG1):
                is_ime_on = true;
                break;
            case LT(1, KC_LNG2):
                is_ime_on = false;
                break;
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_ime_state(keycode, record);

    // K が保留中に別キーが押されたら、K を先に送出して順番を保つ
    if (scroll_k_held && !scroll_k_fired && record->event.pressed && keycode != MY_SCRL_K) {
        scroll_k_held = false;
        scroll_k_sent = true;
        tap_code(KC_K);
    }

    switch (keycode) {
        case MY_LPRN:
            if (record->event.pressed) {
                tap_code16(S(KC_8));  // (
                tap_code16(S(KC_9));  // )
                if (is_ime_on) {
                    tap_code(KC_ENT);     // IME ON なら確定
                }
                tap_code(KC_LEFT);    // 確定したカッコの中にカーソルを戻す
            }
            return false;
        case MY_LBRC:
            if (record->event.pressed) {
                tap_code16(KC_RBRC);  // [
                tap_code16(KC_NUHS);  // ]
                if (is_ime_on) {
                    tap_code(KC_ENT);     // 確定
                }
                tap_code(KC_LEFT);    // 中に戻る
            }
            return false;
        case MY_LCBR:
            if (record->event.pressed) {
                tap_code16(S(KC_RBRC)); // {
                tap_code16(S(KC_NUHS)); // }
                if (is_ime_on) {
                    tap_code(KC_ENT);       // 確定
                }
                tap_code(KC_LEFT);      // 中に戻る
            }
            return false;
        case MY_LABK:
            if (record->event.pressed) {
                tap_code16(S(KC_COMM)); // <
                tap_code16(S(KC_DOT));  // >
                if (is_ime_on) {
                    tap_code(KC_ENT);       // 確定
                }
                tap_code(KC_LEFT);      // 中に戻る
            }
            return false;
        case MY_SCRL_K:
            if (record->event.pressed) {
                scroll_k_timer = timer_read();
                scroll_k_held  = true;
                scroll_k_fired = false;
                scroll_k_sent  = false;
            } else {
                scroll_k_held = false;
                if (scroll_k_fired) {
                    // ホールドだった → スクロール解除
                    if (get_highest_layer(layer_state) != 3) {
                        keyball_set_scroll_mode(false);
                    }
                } else if (!scroll_k_sent) {
                    // 単独タップ（他キー割り込みなし）→ K を送出
                    tap_code(KC_K);
                }
                // scroll_k_sent == true の場合は別キー処理時に既に送出済み
            }
            return false;
    }
    return true;
}

void matrix_scan_user(void) {
    // TAPPING_TERM 経過後もKが保留中ならスクロールモードを発動
    if (scroll_k_held && !scroll_k_fired && timer_elapsed(scroll_k_timer) >= TAPPING_TERM) {
        scroll_k_fired = true;
        scroll_k_held  = false;
        keyball_set_scroll_mode(true);
    }
}

#ifdef COMBO_ENABLE
const uint16_t PROGMEM combo_jk[] = {KC_J, MY_SCRL_K, COMBO_END};
const uint16_t PROGMEM combo_kl[] = {MY_SCRL_K, KC_L, COMBO_END};
combo_t key_combos[] = {
    COMBO(combo_jk, KC_BTN1),
    COMBO(combo_kl, KC_BTN2),
};
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    MT(MOD_LCTL,KC_A),MT(MOD_LSFT,KC_S), MT(MOD_LALT,KC_D), MT(MOD_LSFT,KC_F), KC_G,  KC_H     , KC_J     , MY_SCRL_K, KC_L     , MT(MOD_RCTL,KC_ENT),
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    KC_LCTL  , KC_LGUI  , KC_LALT  ,LT(1,KC_LNG2),MT(MOD_LSFT,KC_SPC),LT(3,KC_TAB),KC_BSPC,LT(2,KC_LNG1), _______  , _______  , _______  , KC_ESC
  ),

  [1] = LAYOUT_universal(
      KC_ESC    , S(KC_6)    , S(KC_7), S(KC_8)    , KC_SLSH  ,                         KC_BSLS, MY_LPRN  , S(KC_9)  , MY_LABK     , S(KC_DOT),
    S(KC_EQL)   , S(KC_4)  , S(KC_5)  ,KC_EQL, S(KC_MINS)  ,                           JP_UNDS, MY_LCBR    , JP_RBRC, KC_SCLN  ,KC_QUOT,
    KC_MINS,S(KC_1),KC_LBRC   , S(KC_3)  ,S(KC_SLSH),                            KC_NUHS   , MY_LBRC   , KC_BSLS  , S(KC_7)  , S(KC_2)   ,
    _______  , _______   , _______  , _______  , _______  , _______  ,      _______    , _______   , _______  , _______  , _______  , _______
  ),

  [2] = LAYOUT_universal(
   KC_MINS   , KC_7     , KC_8     , KC_9     , S(KC_SCLN)  ,                      KC_HOME  , KC_PGUP  , KC_PGDN  , KC_END  , KC_BSPC  ,
   KC_SLSH, KC_4     , KC_5     , KC_6     ,S(KC_QUOT),                            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , KC_DEL  ,
    KC_DOT  , KC_1     , KC_2     , KC_3     ,S(KC_MINS),                           LALT(KC_LEFT), KC_BTN1  , _______  , KC_BTN2  , _______  ,
    KC_QUOT   , KC_0     , KC_RALT   , _______   , _______   , _______ ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  [3] = LAYOUT_universal(
    KC_BRIU  , KC_F7   , KC_F8   , KC_F9  , KC_F10  ,                            S(G(KC_LEFT))  , S(G(KC_DOWN))  , S(G(KC_UP)) ,S(G(KC_RGHT))  , _______ , 
    KC_BRID  , KC_F4  , KC_F5  , KC_F6  , KC_F11  ,                            G(KC_LEFT)  , G(KC_DOWN)  , G(KC_UP)  , G(KC_RGHT)  , _______  ,
    KC_PSCR , KC_F1   , KC_F2  , KC_F3  , KC_F12  ,                            LALT(KC_LEFT)  , KC_BTN1  , _______  , KC_BTN2  , _______ ,
    KC_RCTL  , _______   , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3
    keyball_set_scroll_mode(get_highest_layer(state) == 3);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
