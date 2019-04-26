# OBSRazerChroma

An OBS Studio frontend plugin that implements recording indicator by blinking a key on Razer Chroma keyboard.

## Bulding

* Setup OBS Studio build (refer to [OBS Studio install instructions](https://github.com/obsproject/obs-studio/wiki/install-instructions)).
* Put this repo into `UI/frontend-plugins` directory.
* Add it to the `UI/frontend-plugins/CMakeLists.txt`.
* Build the project.

## Config

Config file is automatically created on first start in `plugin_config/obs-razer-chroma` directory.

Default config:
```ini
[general]
background_color=#C8C8C8
blink_color=#FF0000
blink_interval_msec=750
blink=true
key=F12
```

Key names:

|  Escape  |  F1  |  F2  | F3 | F4 |   F5  | F6 | F7 | F8 | F9 |  F10 | F11 |  F12  | PrintScreen | ScrollLock | Pause |          |         |           |             |             |
|:--------:|:----:|:----:|:--:|:--:|:-----:|:--:|:--:|:--:|:--:|:----:|:---:|:-----:|:-----------:|:----------:|:-----:|:--------:|:-------:|:---------:|:-----------:|:-----------:|
|   Tilde  |   1  |   2  |  3 |  4 |   5   |  6 |  7 |  8 |  9 |   0  |  -  |   =   |  Backspace  |   Insert   |  Home |  PageUp  | NumLock | NumDivide | NumMultiply | NumSubtract |
|    Tab   |   Q  |   W  |  E |  R |   T   |  Y |  U |  I |  O |   P  |  [  |   ]   |      \      |   Delete   |  End  | PageDown |   Num7  |    Num8   |     Num9    |    NumAdd   |
| CapsLock |   A  |   S  |  D |  F |   G   |  H |  J |  K |  L |   ;  |  '  |       |    Enter    |            |       |          |   Num4  |    Num5   |     Num6    |             |
|  LShift  |   Z  |   X  |  C |  V |   B   |  N |  M |  , |  . |   /  |     |       |    RShift   |            |   Up  |          |   Num1  |    Num2   |     Num3    |   NumEnter  |
|   LCtrl  | LWin | LAlt |    |    | Space |    |    |    |    | RAlt |  Fn | RMenu |    RCtrl    |    Left    |  Down |   Right  |   Num0  |           |  NumDecimal |             |
