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
```
"Escape", "F1", ..., "F12",
"0", ..., "9", "A", ..., "Z",
"NumLock", "Num0", ..., "Num9",
"NumDivide", "NumMultiply", "NumSubtract",
"NumAdd", "NumEnter", "NumDecimal",
"PrintScreen", "ScrollLock", "Pause",
"Insert", "Home", "PageUp",
"Delete", "End", "PageDown",
"Up", "Left", "Down", "Right",
"Tab", "CapsLock", "Backspace", "Enter",
"LCtrl", "LWin", "LAlt", "Space", "RAlt", "Fn",
"RMenu", "RCtrl", "LShift", "RShift",
"~", "-", "=", "[", "]", "\", ";", "'", ",", ".", "/"
```
