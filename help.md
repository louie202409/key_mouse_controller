# Windows 虚拟键码（VK）常量表

下面是 `<windows.h>`（实际定义在 `winuser.h`）中常用的虚拟键码常量。所有常量都是 `#define` 的整数，可直接在 `SendInput` / `keybd_event` / `GetAsyncKeyState` 等 API 中使用。

---

## 一、鼠标按键（0x01 – 0x06）

| 常量 | 值 | 说明 |
|---|---|---|
| `VK_LBUTTON` | 0x01 | 鼠标左键 |
| `VK_RBUTTON` | 0x02 | 鼠标右键 |
| `VK_CANCEL` | 0x03 | Ctrl+Break |
| `VK_MBUTTON` | 0x04 | 鼠标中键 |
| `VK_XBUTTON1` | 0x05 | 鼠标侧键 1（后退） |
| `VK_XBUTTON2` | 0x06 | 鼠标侧键 2（前进） |

---

## 二、控制键与编辑键（0x08 – 0x2F）

| 常量 | 值 | 说明 |
|---|---|---|
| `VK_BACK` | 0x08 | Backspace |
| `VK_TAB` | 0x09 | Tab |
| `VK_CLEAR` | 0x0C | Clear（Num5 无 NumLock） |
| `VK_RETURN` | 0x0D | Enter |
| `VK_SHIFT` | 0x10 | Shift（通用，不分左右） |
| `VK_CONTROL` | 0x11 | Ctrl（通用，不分左右） |
| `VK_MENU` | 0x12 | Alt（通用，不分左右） |
| `VK_PAUSE` | 0x13 | Pause / Break |
| `VK_CAPITAL` | 0x14 | Caps Lock |
| `VK_ESCAPE` | 0x1B | Esc |
| `VK_SPACE` | 0x20 | 空格 |
| `VK_PRIOR` | 0x21 | Page Up |
| `VK_NEXT` | 0x22 | Page Down |
| `VK_END` | 0x23 | End |
| `VK_HOME` | 0x24 | Home |
| `VK_LEFT` | 0x25 | ← |
| `VK_UP` | 0x26 | ↑ |
| `VK_RIGHT` | 0x27 | → |
| `VK_DOWN` | 0x28 | ↓ |
| `VK_SELECT` | 0x29 | Select |
| `VK_PRINT` | 0x2A | Print |
| `VK_EXECUTE` | 0x2B | Execute |
| `VK_SNAPSHOT` | 0x2C | Print Screen |
| `VK_INSERT` | 0x2D | Insert |
| `VK_DELETE` | 0x2E | Delete |
| `VK_HELP` | 0x2F | Help |

---

## 三、字母与数字（0x30 – 0x5A）

字母键的 VK 值**等于其大写 ASCII 码**，数字键的 VK 值**等于其 ASCII 码**。

| 常量 | 值 | 说明 |
|---|---|---|
| `VK_0` … `VK_9` | 0x30 – 0x39 | 数字 0–9（主键盘） |
| `VK_A` … `VK_Z` | 0x41 – 0x5A | 字母 A–Z |

> 注意：`VK_A` 与 `'A'` 是同一个值（0x41），可以直接写 `'A'`。小写 `'a'` 不是有效的 VK，需要先转成大写。

---

## 四、功能键（0x70 – 0x87）

| 常量 | 值 | 说明 |
|---|---|---|
| `VK_F1` … `VK_F24` | 0x70 – 0x87 | F1 – F24 |

---

## 五、左右修饰键（0xA0 – 0xA5）

| 常量 | 值 | 说明 |
|---|---|---|
| `VK_LSHIFT` | 0xA0 | 左 Shift |
| `VK_RSHIFT` | 0xA1 | 右 Shift |
| `VK_LCONTROL` | 0xA2 | 左 Ctrl |
| `VK_RCONTROL` | 0xA3 | 右 Ctrl |
| `VK_LMENU` | 0xA4 | 左 Alt |
| `VK_RMENU` | 0xA5 | 右 Alt |

> `SendInput` 中若要区分左右，直接用这些常量即可；`VK_SHIFT` 等通用常量会由系统映射到当前激活的一侧。

---

## 六、OEM 与浏览器/多媒体键（0xA6 – 0xFE）

| 常量 | 值 | 说明 |
|---|---|---|
| `VK_BROWSER_BACK` | 0xA6 | 浏览器后退 |
| `VK_BROWSER_FORWARD` | 0xA7 | 浏览器前进 |
| `VK_BROWSER_REFRESH` | 0xA8 | 刷新 |
| `VK_BROWSER_STOP` | 0xA9 | 停止 |
| `VK_BROWSER_SEARCH` | 0xAA | 搜索 |
| `VK_BROWSER_FAVORITES` | 0xAB | 收藏夹 |
| `VK_BROWSER_HOME` | 0xAC | 主页 |
| `VK_VOLUME_MUTE` | 0xAD | 静音 |
| `VK_VOLUME_DOWN` | 0xAE | 音量减 |
| `VK_VOLUME_UP` | 0xAF | 音量加 |
| `VK_MEDIA_NEXT_TRACK` | 0xB0 | 下一曲 |
| `VK_MEDIA_PREV_TRACK` | 0xB1 | 上一曲 |
| `VK_MEDIA_STOP` | 0xB2 | 停止播放 |
| `VK_MEDIA_PLAY_PAUSE` | 0xB3 | 播放/暂停 |
| `VK_LAUNCH_MAIL` | 0xB4 | 打开邮件 |
| `VK_LAUNCH_MEDIA_SELECT` | 0xB5 | 选择媒体 |
| `VK_LAUNCH_APP1` | 0xB6 | 启动应用 1 |
| `VK_LAUNCH_APP2` | 0xB7 | 启动应用 2 |

---

## 七、OEM 符号键（0xBA – 0xE2）

这些键的**具体字符随键盘布局（美式/中文/欧洲等）而变**，但 VK 值固定。

| 常量 | 值 | 美式键盘上对应字符 |
|---|---|---|
| `VK_OEM_1` | 0xBA | `;:` |
| `VK_OEM_PLUS` | 0xBB | `=+` |
| `VK_OEM_COMMA` | 0xBC | `,<` |
| `VK_OEM_MINUS` | 0xBD | `-_` |
| `VK_OEM_PERIOD` | 0xBE | `.>` |
| `VK_OEM_2` | 0xBF | `/?` |
| `VK_OEM_3` | 0xC0 | `` `~ `` |
| `VK_OEM_4` | 0xDB | `[{` |
| `VK_OEM_5` | 0xDC | `\|` |
| `VK_OEM_6` | 0xDD | `]}` |
| `VK_OEM_7` | 0xDE | `'"` |
| `VK_OEM_8` | 0xDF | （依键盘而定） |
| `VK_OEM_102` | 0xE2 | 欧洲键盘 `<>` |

---

## 八、小键盘（0x60 – 0x6F）

| 常量 | 值 | 说明 |
|---|---|---|
| `VK_NUMPAD0` … `VK_NUMPAD9` | 0x60 – 0x69 | 小键盘 0–9 |
| `VK_MULTIPLY` | 0x6A | `*` |
| `VK_ADD` | 0x6B | `+` |
| `VK_SEPARATOR` | 0x6C | 分隔符 |
| `VK_SUBTRACT` | 0x6D | `-` |
| `VK_DECIMAL` | 0x6E | `.` |
| `VK_DIVIDE` | 0x6F | `/` |

---

## 九、其他锁定/切换键（0x90 – 0x92）

| 常量 | 值 | 说明 |
|---|---|---|
| `VK_NUMLOCK` | 0x90 | Num Lock |
| `VK_SCROLL` | 0x91 | Scroll Lock |

---

## 十、输入法相关（0x15、0x19、0x1C – 0x1F、0xE5、0xE7 等）

| 常量 | 值 | 说明 |
|---|---|---|
| `VK_KANA` / `VK_HANGUL` | 0x15 | 假名 / 韩文 |
| `VK_JUNJA` | 0x17 | Junja |
| `VK_FINAL` | 0x18 | Final |
| `VK_HANJA` / `VK_KANJI` | 0x19 | 汉字 / 汉字 |
| `VK_CONVERT` | 0x1C | 转换（IME） |
| `VK_NONCONVERT` | 0x1D | 不转换 |
| `VK_ACCEPT` | 0x1E | 接受 |
| `VK_MODECHANGE` | 0x1F | 模式切换 |
| `VK_PROCESSKEY` | 0xE5 | IME 处理 |
| `VK_PACKET` | 0xE7 | 用于 `SendInput` 发送 Unicode 字符 |
| `VK_ATTN` | 0xF6 | Attn |
| `VK_CRSEL` | 0xF7 | CrSel |
| `VK_EXSEL` | 0xF8 | ExSel |
| `VK_EREOF` | 0xF9 | Erase EOF |
| `VK_PLAY` | 0xFA | Play |
| `VK_ZOOM` | 0xFB | Zoom |
| `VK_NONAME` | 0xFC | 保留 |
| `VK_PA1` | 0xFD | PA1 |
| `VK_OEM_CLEAR` | 0xFE | Clear |

---

## 使用示例

```cpp
// 按下并释放 Enter
api->key_button_down(VK_RETURN);
api->key_button_up(VK_RETURN);

// Ctrl + S 保存
api->key_button_down(VK_CONTROL);
api->key_button_down('S');
api->key_button_up('S');
api->key_button_up(VK_CONTROL);

// 用左 Shift 而不是通用 Shift
api->key_button_down(VK_LSHIFT);
api->key_button_down('A');   // 注意 'A' 本身已是 VK
api->key_button_up('A');
api->key_button_up(VK_LSHIFT);

// 小键盘加号
api->key_button_down(VK_ADD);
api->key_button_up(VK_ADD);
```

---

## 两点提醒

1. **不要用 `VK_PACKET` 之外的方式发送 Unicode**：`SendInput` 的 `KEYBDINPUT` 里，若 `wVk = 0` 且设置 `KEYEVENTF_UNICODE`，则 `wScan` 填字符的 UTF-16 码元，可发送任意字符（不受键盘布局限制）。这是输入中文/emoji 的正确方式。
2. **`VK_SHIFT` / `VK_CONTROL` / `VK_MENU` 是"通用"键**，系统会映射到当前激活的一侧；需要明确左/右时请用 `VK_L*` / `VK_R*` 常量。

以上常量在 MinGW / MSVC 的 `winuser.h` 中均有定义，无需自己 `#define`。