-- ============================================
-- 饮水机管理系统 - 建库建表脚本
-- ============================================

CREATE DATABASE IF NOT EXISTS water_dispenser
  DEFAULT CHARACTER SET utf8mb4
  DEFAULT COLLATE utf8mb4_unicode_ci;

USE water_dispenser;

-- 用户表
-- role: 0=管理员, 1=护工, 2=饮水者
CREATE TABLE IF NOT EXISTS users (
  id INT AUTO_INCREMENT PRIMARY KEY,
  username VARCHAR(32) NOT NULL UNIQUE COMMENT '登录编号',
  password VARCHAR(256) NOT NULL COMMENT '密码',
  name VARCHAR(32) NOT NULL COMMENT '姓名',
  role TINYINT NOT NULL DEFAULT 2 COMMENT '角色:0=管理员 1=护工 2=饮水者',
  section VARCHAR(32) NOT NULL DEFAULT 'elderly' COMMENT '所属板块',
  gender TINYINT DEFAULT 0 COMMENT '性别:0=未知 1=男 2=女',
  age INT DEFAULT 0,
  height_cm FLOAT DEFAULT 0 COMMENT '身高(cm)',
  weight_kg FLOAT DEFAULT 0 COMMENT '体重(kg)',
  fp_id INT DEFAULT NULL COMMENT '预留:指纹ID',
  phone VARCHAR(64) DEFAULT NULL COMMENT 'PushPlus用户Token',
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 饮水记录表
CREATE TABLE IF NOT EXISTS records (
  id INT AUTO_INCREMENT PRIMARY KEY,
  user_id INT NOT NULL,
  volume_ml INT NOT NULL COMMENT '饮水量(ml)',
  recorded_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 每日汇总表
CREATE TABLE IF NOT EXISTS daily_summary (
  id INT AUTO_INCREMENT PRIMARY KEY,
  user_id INT NOT NULL,
  date DATE NOT NULL COMMENT '日期',
  total_ml INT DEFAULT 0 COMMENT '今日饮水总量',
  use_count INT DEFAULT 0 COMMENT '今日饮水次数',
  target_ml INT DEFAULT 0 COMMENT '建议饮水量(根据体重计算)',
  advice VARCHAR(512) DEFAULT NULL COMMENT '个性化建议',
  updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  UNIQUE KEY uk_user_date (user_id, date),
  FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 用户登录日志表
CREATE TABLE IF NOT EXISTS login_logs (
  id INT AUTO_INCREMENT PRIMARY KEY,
  user_id INT NOT NULL,
  login_ip VARCHAR(64) DEFAULT NULL,
  login_time DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 初始化默认管理员账号
-- 编号: admin  密码: admin123
INSERT INTO users (username, password, name, role, section) VALUES
('admin', 'admin123', '系统管理员', 0, '*');
