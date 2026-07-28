from pathlib import Path
from PIL import Image, ImageDraw, ImageFont


W, H = 1920, 1080
OUT = Path(r"C:\Users\heshizhe\Desktop\STM32\H7serial\defense_assets\defense_overview_board.png")

FONT_REG = r"C:\Windows\Fonts\msyh.ttc"
FONT_BOLD = r"C:\Windows\Fonts\msyhbd.ttc"


def font(size, bold=False):
    return ImageFont.truetype(FONT_BOLD if bold else FONT_REG, size)


F_TITLE = font(58, True)
F_SUB = font(28)
F_SEC = font(34, True)
F_CARD = font(30, True)
F_TEXT = font(22)
F_SMALL = font(20)
F_NODE = font(23, True)
F_NODE_SMALL = font(18)
F_NUM = font(37, True)

BG = (246, 249, 253)
TEXT = (26, 39, 56)
MUTED = (91, 108, 130)
LINE = (198, 210, 224)
PANEL = (255, 255, 255)
PANEL_BLUE = (237, 247, 255)
PANEL_GREEN = (237, 252, 245)
PANEL_ORANGE = (255, 246, 236)
PANEL_RED = (255, 241, 241)
ORANGE = (255, 139, 35)
BLUE = (45, 142, 230)
GREEN = (50, 170, 106)
RED = (225, 72, 72)
PURPLE = (122, 99, 224)
DARK = (20, 32, 48)


def rounded(draw, box, r, fill, outline=LINE, width=2):
    draw.rounded_rectangle(box, radius=r, fill=fill, outline=outline, width=width)


def t(draw, xy, s, f, fill=TEXT, anchor=None, align="left"):
    draw.text(xy, s, font=f, fill=fill, anchor=anchor, align=align)


def center(draw, box, s, f, fill=TEXT, spacing=4):
    x1, y1, x2, y2 = box
    lines = s.split("\n")
    heights = [draw.textbbox((0, 0), line, font=f)[3] for line in lines]
    total_h = sum(heights) + spacing * (len(lines) - 1)
    y = y1 + (y2 - y1 - total_h) / 2
    for line, h in zip(lines, heights):
        t(draw, ((x1 + x2) / 2, y), line, f, fill, anchor="ma")
        y += h + spacing


def arrow(draw, start, end, color=ORANGE, width=4):
    x1, y1 = start
    x2, y2 = end
    draw.line((x1, y1, x2, y2), fill=color, width=width)
    if x2 >= x1:
        pts = [(x2, y2), (x2 - 16, y2 - 9), (x2 - 16, y2 + 9)]
    else:
        pts = [(x2, y2), (x2 + 16, y2 - 9), (x2 + 16, y2 + 9)]
    draw.polygon(pts, fill=color)


def bullet(draw, x, y, text, highlight=None, f=F_TEXT):
    draw.ellipse((x, y + 10, x + 9, y + 19), fill=ORANGE)
    if highlight and highlight in text:
        pre, post = text.split(highlight, 1)
        cx = x + 22
        t(draw, (cx, y), pre, f, MUTED)
        cx += draw.textlength(pre, font=f)
        t(draw, (cx, y), highlight, f, ORANGE)
        cx += draw.textlength(highlight, font=f)
        t(draw, (cx, y), post, f, MUTED)
    else:
        t(draw, (x + 22, y), text, f, MUTED)


def feature_card(draw, box, idx, title, subtitle, bullets, color, fill):
    x1, y1, x2, y2 = box
    rounded(draw, box, 18, PANEL, outline=LINE, width=2)
    draw.ellipse((x1 + 24, y1 + 24, x1 + 78, y1 + 78), fill=color)
    center(draw, (x1 + 24, y1 + 24, x1 + 78, y1 + 78), str(idx), F_CARD, fill=(255, 255, 255))
    t(draw, (x1 + 94, y1 + 24), title, F_CARD, TEXT)
    t(draw, (x1 + 94, y1 + 64), subtitle, F_SMALL, MUTED)
    y = y1 + 96
    for line, hl in bullets:
        bullet(draw, x1 + 28, y, line, hl)
        y += 33


def node(draw, box, label, fill=PANEL, outline=LINE, f=F_NODE):
    rounded(draw, box, 14, fill, outline=outline, width=2)
    center(draw, box, label, f, fill=TEXT)


def stat_card(draw, box, big, small, color=ORANGE):
    x1, y1, x2, y2 = box
    rounded(draw, box, 16, PANEL, outline=LINE, width=2)
    center(draw, (x1, y1 + 10, x2, y1 + 58), big, F_NUM, fill=color)
    center(draw, (x1 + 8, y1 + 61, x2 - 8, y2 - 8), small, F_SMALL, fill=MUTED)


def main():
    img = Image.new("RGB", (W, H), BG)
    draw = ImageDraw.Draw(img, "RGBA")

    # Header
    t(draw, (60, 42), "智能饮水机现场答辩总览", F_TITLE, DARK)
    draw.rounded_rectangle((60, 116, 580, 124), radius=4, fill=ORANGE)
    t(draw, (60, 145), "养老照护场景：安全取水、异常停泵、护工饮水管理", F_SUB, MUTED)
    rounded(draw, (1295, 48, 1848, 132), 22, PANEL_ORANGE, outline=ORANGE, width=2)
    center(draw, (1295, 48, 1848, 132), "重点创新：STM32H7 本地边缘视觉 AI\n驱动杯满自停与异常保护", F_TEXT, fill=TEXT)

    # Layout columns
    left = (60, 205, 530, 1016)
    mid = (560, 205, 1310, 1016)
    right = (1340, 205, 1860, 1016)

    rounded(draw, left, 22, (252, 254, 255), outline=LINE, width=2)
    rounded(draw, mid, 22, (252, 254, 255), outline=LINE, width=2)
    rounded(draw, right, 22, (252, 254, 255), outline=LINE, width=2)

    t(draw, (88, 232), "三大功能", F_SEC, TEXT)
    t(draw, (590, 232), "系统框图", F_SEC, TEXT)
    t(draw, (1370, 232), "AI识别能力与成果", F_SEC, TEXT)

    feature_card(
        draw,
        (88, 292, 502, 505),
        1,
        "智能出水",
        "三种入口，共用安全后端",
        [("自动：放杯后自动接水", "自动"), ("语音：手部不便可用", "语音"), ("按键：传统可靠兜底", "兜底")],
        BLUE,
        PANEL_BLUE,
    )
    feature_card(
        draw,
        (88, 532, 502, 745),
        2,
        "异常保护",
        "出水中实时视觉监测",
        [("杯移 / 无杯：立即停泵", "立即停泵"), ("遮挡 / 异常：安全优先", "安全"), ("降低溢水、烫伤风险", "烫伤")],
        RED,
        PANEL_RED,
    )
    feature_card(
        draw,
        (88, 772, 502, 985),
        3,
        "云端饮水管家",
        "护工管理与个性建议",
        [("指纹：区分老人身份", "指纹"), ("记录：云端同步查看", "同步"), ("DeepSeek：饮水建议", "DeepSeek")],
        GREEN,
        PANEL_GREEN,
    )

    # System block diagram
    m_x1, m_y1, m_x2, m_y2 = mid
    stm = (832, 380, 1038, 735)
    node(draw, stm, "主控\nSTM32H743IIT6", fill=PANEL_ORANGE, outline=ORANGE, f=F_CARD)
    node(draw, (840, 292, 1030, 342), "电源模块", fill=PANEL, outline=LINE, f=F_NODE)
    arrow(draw, (935, 342), (935, 376), color=MUTED, width=3)

    inputs = [
        ((590, 315, 760, 372), "摄像头\nAI视觉识别", BLUE),
        ((590, 412, 760, 469), "语音模块\n麦克风", LINE),
        ((590, 509, 760, 566), "按键模块", LINE),
        ((590, 606, 760, 663), "温湿度\n水位/指纹", LINE),
        ((590, 703, 760, 760), "传感器组", LINE),
    ]
    for box, label, color in inputs:
        outline = color if color != LINE else LINE
        fill = PANEL_BLUE if color == BLUE else PANEL
        node(draw, box, label, fill=fill, outline=outline, f=F_NODE_SMALL)
        arrow(draw, (box[2], (box[1] + box[3]) / 2), (stm[0] - 12, (box[1] + box[3]) / 2), color=MUTED, width=3)

    outputs = [
        ((1110, 315, 1280, 372), "OLED显示", LINE),
        ((1110, 412, 1280, 469), "语音播报\n扬声器", LINE),
        ((1110, 509, 1280, 566), "驱动模块\n水泵", ORANGE),
        ((1110, 606, 1280, 663), "继电器\n加热管", LINE),
        ((1070, 740, 1280, 840), "ESP32-C6\nWiFi / MQTT\nRainMaker云端", GREEN),
    ]
    for box, label, color in outputs:
        outline = color if color != LINE else LINE
        fill = PANEL_GREEN if color == GREEN else (PANEL_ORANGE if color == ORANGE else PANEL)
        node(draw, box, label, fill=fill, outline=outline, f=F_NODE_SMALL)
        arrow(draw, (stm[2] + 12, (box[1] + box[3]) / 2), (box[0], (box[1] + box[3]) / 2), color=MUTED, width=3)

    rounded(draw, (590, 885, 1280, 970), 20, PANEL_ORANGE, outline=ORANGE, width=2)
    center(draw, (590, 885, 1280, 970), "本地闭环：摄像头采集 → AI推理 → 判断水位/异常 → 停泵", F_TEXT, fill=TEXT)

    # AI stats
    stat_card(draw, (1370, 292, 1532, 402), "175K", "模型参数量", ORANGE)
    stat_card(draw, (1554, 292, 1830, 402), "72.9 ms", "STM32H7端单次推理", ORANGE)
    stat_card(draw, (1370, 426, 1532, 536), "13M", "MACC计算量", BLUE)
    stat_card(draw, (1554, 426, 1830, 536), "98.6%", "验证集准确率", BLUE)
    stat_card(draw, (1370, 560, 1532, 670), "INT8", "Cube.AI量化部署", GREEN)
    stat_card(draw, (1554, 560, 1830, 670), "5000张", "自建真实数据集", GREEN)

    rounded(draw, (1370, 704, 1830, 846), 18, PANEL, outline=LINE, width=2)
    t(draw, (1396, 728), "最终模型", F_CARD, TEXT)
    bullet(draw, 1398, 780, "MobileNetV3-Small 0.5×", "MobileNetV3")
    bullet(draw, 1398, 818, "分类 + 回归双头", "双头")

    rounded(draw, (1370, 874, 1830, 970), 18, PANEL, outline=LINE, width=2)
    t(draw, (1396, 894), "识别状态", F_CARD, TEXT)
    tags = [("无杯", RED), ("低水", BLUE), ("半杯", PURPLE), ("满杯", ORANGE), ("异常", RED)]
    x = 1398
    for label, color in tags:
        rounded(draw, (x, 932, x + 76, 962), 13, (255, 255, 255), outline=color, width=2)
        center(draw, (x, 932, x + 76, 962), label, F_SMALL, fill=color)
        x += 84

    OUT.parent.mkdir(parents=True, exist_ok=True)
    img.save(OUT, quality=95)
    print(OUT)


if __name__ == "__main__":
    main()
