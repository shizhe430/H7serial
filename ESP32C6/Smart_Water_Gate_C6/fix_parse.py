with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Replace the UART parsing section
old = """    printf("[UART1] RX: %s\\n", s);  // debug

            if (strncmp(s, "STM_VOL:", 8) == 0) {
                water_volume = atoi(s + 8);
            } else if (strncmp(s, "STM_DONE:", 9) == 0) {
                water_valve_open = false;
                int done_vol = atoi(s + 9);
                water_volume = done_vol;
                last_dispensed_volume = done_vol;
                if (power_param) esp_rmaker_param_update_and_report(power_param, esp_rmaker_bool(false));
                if (last_vol_param) esp_rmaker_param_update_and_report(last_vol_param, esp_rmaker_int(done_vol));
                user_record_t *done_u = find_user_by_id(current_user_id);
                if (done_u) done_u->use_count++;
                if (count_param) esp_rmaker_param_update_and_report(count_param, esp_rmaker_int(done_u ? done_u->use_count : 0));
                ESP_LOGI(TAG, "Dispense complete: %dml", water_volume);
                notify_daily_water(done_vol);
            } else if (strncmp(s, STM_FP_PREFIX, strlen(STM_FP_PREFIX)) == 0) {
                int fp_id = atoi(s + strlen(STM_FP_PREFIX));
                current_user_id = fp_id;
                user_record_t *u = find_user_by_id(fp_id);
                if (u) {
                    char msg[96];
                    if (strlen(u->name) > 0)
                        snprintf(msg, sizeof(msg), "用户%s，请取水", u->name);
                    else
                        snprintf(msg, sizeof(msg), "用户%d号，请取水", fp_id);
                    voice_broadcast(msg);
                    update_all_params();
                    ESP_LOGI(TAG, "Fingerprint: user '%s' (ID %d)", u->name ? u->name : "", fp_id);
                } else {
                    ESP_LOGI(TAG, "Fingerprint: unknown ID %d", fp_id);
                }
            } else if (strncmp(s, STM_FP_NEW_PREFIX, strlen(STM_FP_NEW_PREFIX)) == 0) {
                int fp_id = atoi(s + strlen(STM_FP_NEW_PREFIX));
                user_record_t *u = find_user_by_id(fp_id);
                if (!u) {
                    u = add_user(fp_id, NULL);
                }
                if (u) {
                    current_user_id = fp_id;
                    char msg[96];
                    if (strlen(u->name) > 0)
                        snprintf(msg, sizeof(msg), "欢迎%s，请取水", u->name);
                    else
                        snprintf(msg, sizeof(msg), "欢迎新用户，指纹已注册为编号%d，请在网页设置姓名", fp_id);
                    voice_broadcast(msg);
                    update_all_params();
                    ESP_LOGI(TAG, "New fingerprint user ID %d", fp_id);
                }
            } else if (strncmp(s, STM_VOICE_PREFIX, strlen(STM_VOICE_PREFIX)) == 0) {
                const char *vname = s + strlen(STM_VOICE_PREFIX);
                char clean[32];
                strncpy(clean, vname, sizeof(clean) - 1);
                clean[sizeof(clean) - 1] = '\\0';
                int slen = strlen(clean);
                while (slen > 0 && (clean[slen-1]=='\\n'||clean[slen-1]=='\\r'||clean[slen-1]==' '))
                    clean[--slen] = '\\0';
                user_record_t *u = find_user_by_name(clean);
                if (u) {
                    current_user_id = u->id;
                    char msg[96];
                    snprintf(msg, sizeof(msg), "欢迎%s，请取水", u->name);
                    voice_broadcast(msg);
                    update_all_params();
                    ESP_LOGI(TAG, "Voice ID: '%s'", u->name);
                } else {
                    static int temp_id_counter = -1;
                    int tid = temp_id_counter--;
                    u = add_user(tid, clean);
                    if (u) {
                        current_user_id = tid;
                        char msg[96];
                        snprintf(msg, sizeof(msg), "已识别新用户%s，请取水", clean);
                        voice_broadcast(msg);
                    }
                }
            }"""

new = """    // 逐行解析（支持一次收到多条命令）
            char *line = s;
            char *next_line;
            while (line && *line) {
                // 找行尾
                next_line = strstr(line, "\\n");
                if (next_line) {
                    *next_line = '\\0';
                    next_line++;
                }
                // 去掉行尾\\r
                int linelen = strlen(line);
                while (linelen > 0 && (line[linelen-1]=='\\r'||line[linelen-1]=='\\n'||line[linelen-1]==' '))
                    line[--linelen] = '\\0';
                if (linelen == 0) { line = next_line; continue; }

                printf("[UART1] LINE: %s\\n", line);

                if (strncmp(line, "STM_VOL:", 8) == 0) {
                    water_volume = atoi(line + 8);
                } else if (strncmp(line, "STM_DONE:", 9) == 0) {
                    water_valve_open = false;
                    int done_vol = atoi(line + 9);
                    water_volume = done_vol;
                    last_dispensed_volume = done_vol;
                    user_record_t *done_u = find_user_by_id(current_user_id);
                    if (done_u) done_u->use_count++;
                    ESP_LOGI(TAG, "Dispense complete: %dml", water_volume);
                    notify_daily_water(done_vol);
                } else if (strncmp(line, STM_FP_PREFIX, strlen(STM_FP_PREFIX)) == 0) {
                    int fp_id = atoi(line + strlen(STM_FP_PREFIX));
                    current_user_id = fp_id;
                    user_record_t *u = find_user_by_id(fp_id);
                    if (u) {
                        voice_broadcast("用户，请取水");
                        update_all_params();
                    }
                } else if (strncmp(line, STM_FP_NEW_PREFIX, strlen(STM_FP_NEW_PREFIX)) == 0) {
                    int fp_id = atoi(line + strlen(STM_FP_NEW_PREFIX));
                    user_record_t *u = find_user_by_id(fp_id);
                    if (!u) u = add_user(fp_id, NULL);
                    if (u) {
                        current_user_id = fp_id;
                        voice_broadcast("欢迎新用户");
                        update_all_params();
                    }
                } else if (strncmp(line, STM_VOICE_PREFIX, strlen(STM_VOICE_PREFIX)) == 0) {
                    const char *vname = line + strlen(STM_VOICE_PREFIX);
                    char clean[32] = {0};
                    strncpy(clean, vname, sizeof(clean) - 1);
                    user_record_t *u = find_user_by_name(clean);
                    if (u) {
                        current_user_id = u->id;
                        voice_broadcast("欢迎，请取水");
                        update_all_params();
                    } else {
                        static int temp_id_counter = -1;
                        u = add_user(temp_id_counter--, clean);
                        if (u) current_user_id = u->id;
                    }
                }
                line = next_line;
            }"""

if old in content:
    content = content.replace(old, new)
    with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
        f.write(content)
    print("REPLACED - 逐行解析")
else:
    print("NOT FOUND")
    # debug - show what's around that area
    idx = content.find('printf("[UART1] RX: %s')
    if idx >= 0:
        print(content[idx:idx+200])
