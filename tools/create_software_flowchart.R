out_dir <- "defense_assets"
if (!dir.exists(out_dir)) {
  dir.create(out_dir, recursive = TRUE)
}

font_family <- "Microsoft YaHei"

draw_flowchart <- function() {
  grid::grid.newpage()
  grid::pushViewport(grid::viewport(width = 0.96, height = 0.96))

  text_gp <- function(size = 7.2, bold = FALSE) {
    grid::gpar(
      fontsize = size,
      fontfamily = font_family,
      fontface = if (bold) "bold" else "plain",
      col = "black",
      lineheight = 0.95
    )
  }

  draw_text <- function(label, x, y, size = 7.2, bold = FALSE) {
    grid::grid.text(label, x = x, y = y, gp = text_gp(size, bold))
  }

  process_node <- function(x, y, w, h, label, size = 7.0) {
    grid::grid.roundrect(
      x = x, y = y, width = w, height = h,
      r = grid::unit(0.010, "npc"),
      gp = grid::gpar(col = "black", fill = "white", lwd = 0.85)
    )
    draw_text(label, x, y, size = size)
  }

  decision_node <- function(x, y, w, h, label, size = 7.0) {
    grid::grid.polygon(
      x = c(x, x + w / 2, x, x - w / 2),
      y = c(y + h / 2, y, y - h / 2, y),
      gp = grid::gpar(col = "black", fill = "white", lwd = 0.85)
    )
    draw_text(label, x, y, size = size)
  }

  arrow_line <- function(x, y) {
    grid::grid.lines(
      x = x, y = y,
      arrow = grid::arrow(type = "closed", length = grid::unit(0.055, "inches")),
      gp = grid::gpar(col = "black", lwd = 0.75)
    )
  }

  edge_label <- function(label, x, y) {
    draw_text(label, x, y, size = 6.0)
  }

  draw_text("ESP32-P4 物联网智能饮水机软件流程图", 0.5, 0.982, size = 9.8, bold = TRUE)

  process_node(0.50, 0.930, 0.24, 0.050, "系统上电")
  process_node(0.50, 0.852, 0.42, 0.064, "外设与任务初始化\n摄像头、OLED、水泵、AI、语音、身份、无线通信")
  process_node(0.50, 0.760, 0.34, 0.058, "主循环\n采集图像并执行 AI 推理")
  decision_node(0.50, 0.660, 0.28, 0.082, "水杯稳定？")

  process_node(0.20, 0.660, 0.25, 0.060, "待机显示\n继续采集与身份轮询")
  process_node(0.50, 0.550, 0.35, 0.064, "出水等待窗口\n语音指令 / 远程控水 / 超时自动 / 按键覆盖")
  process_node(0.50, 0.450, 0.39, 0.064, "确定出水模式\n自动满杯、语音半杯/满杯、机械按键/远程控水")
  process_node(0.50, 0.345, 0.34, 0.060, "启动水泵\n持续视觉监测与出水计量")

  decision_node(0.50, 0.235, 0.30, 0.082, "异常或\n水杯移走？")
  decision_node(0.50, 0.128, 0.30, 0.082, "达到目标\n水位？")

  process_node(0.20, 0.235, 0.27, 0.060, "立即停泵\n异常状态提示")
  process_node(0.80, 0.128, 0.27, 0.060, "停止水泵\n出水完成提示")
  process_node(0.50, 0.032, 0.38, 0.048, "上传本次记录\n用户 ID 与出水量上报云端")

  arrow_line(c(0.50, 0.50), c(0.905, 0.884))
  arrow_line(c(0.50, 0.50), c(0.820, 0.789))
  arrow_line(c(0.50, 0.50), c(0.731, 0.701))

  arrow_line(c(0.36, 0.325), c(0.660, 0.660))
  edge_label("否", 0.342, 0.684)
  arrow_line(c(0.20, 0.20, 0.30, 0.30, 0.33), c(0.690, 0.795, 0.795, 0.760, 0.760))

  arrow_line(c(0.50, 0.50), c(0.619, 0.582))
  edge_label("是", 0.535, 0.603)
  arrow_line(c(0.50, 0.50), c(0.518, 0.482))
  arrow_line(c(0.50, 0.50), c(0.418, 0.375))
  arrow_line(c(0.50, 0.50), c(0.315, 0.276))

  arrow_line(c(0.35, 0.335), c(0.235, 0.235))
  edge_label("是", 0.365, 0.259)
  arrow_line(c(0.50, 0.50), c(0.194, 0.169))
  edge_label("否", 0.535, 0.183)
  arrow_line(c(0.65, 0.665), c(0.128, 0.128))
  edge_label("是", 0.637, 0.151)

  arrow_line(c(0.50, 0.955, 0.955, 0.67), c(0.087, 0.087, 0.345, 0.345))
  edge_label("否", 0.932, 0.222)

  arrow_line(c(0.20, 0.20, 0.29, 0.29, 0.31), c(0.205, 0.085, 0.085, 0.032, 0.032))
  arrow_line(c(0.80, 0.80, 0.71, 0.71, 0.69), c(0.098, 0.075, 0.075, 0.032, 0.032))

  grid::popViewport()
}

pdf_file <- file.path(out_dir, "software_flowchart_esp32p4_iot.pdf")
svg_file <- file.path(out_dir, "software_flowchart_esp32p4_iot.svg")
png_file <- file.path(out_dir, "software_flowchart_esp32p4_iot.png")

grDevices::cairo_pdf(pdf_file, width = 7.2, height = 9.0, family = font_family)
draw_flowchart()
grDevices::dev.off()

grDevices::svg(svg_file, width = 7.2, height = 9.0, family = font_family)
draw_flowchart()
grDevices::dev.off()

grDevices::png(png_file, width = 2400, height = 3000, res = 300, type = "cairo")
draw_flowchart()
grDevices::dev.off()

message("Wrote: ", normalizePath(pdf_file, winslash = "\\", mustWork = FALSE))
message("Wrote: ", normalizePath(svg_file, winslash = "\\", mustWork = FALSE))
message("Wrote: ", normalizePath(png_file, winslash = "\\", mustWork = FALSE))
